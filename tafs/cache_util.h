#ifndef CACHE_UTIL_H
#define CACHE_UTIL_H

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
//#include "sysstat.h"

//
// ref: http://stackoverflow.com/questions/675039/how-can-i-create-directory-tree-in-c-linux
//
class CacheUtil 
{
    typedef struct stat Stat;

public:
    int do_mkdir(const char *path, mode_t mode)
    {
        Stat            st;
        int             status = 0;

        if (stat(path, &st) != 0)
        {
            /* Directory does not exist. EEXIST for race condition */
            if (mkdir(path, mode) != 0 && errno != EEXIST)
                status = -1;
        }
        else if (!S_ISDIR(st.st_mode))
        {
            errno = ENOTDIR;
            status = -1;
        }

        return(status);
    }

    /**
     ** mkpath - ensure all directories in path exist
     ** Algorithm takes the pessimistic view and works top-down to ensure
     ** each directory in path exists, rather than optimistically creating
     ** the last element and working backwards.
     */
    int mkpath(const char *path, mode_t mode)
    {
        char           *pp;
        char           *sp;
        int             status;
        char           *copypath = strdup(path);

        status = 0;
        pp = copypath;
        while (status == 0 && (sp = strchr(pp, '/')) != 0)
        {
            if (sp != pp)
            {
                /* Neither root nor double slash in path */
                *sp = '\0';
                status = do_mkdir(copypath, mode);
                *sp = '/';
            }
            pp = sp + 1;
        }
        if (status == 0)
            status = do_mkdir(path, mode);
        free(copypath);
        return (status);
    }


    // 
    // exclude last filename
    //
    int mkfolder(std::string filepath) {
        std::string tt = filepath.substr(0, filepath.find_last_of("\\/"));
        return mkpath(tt.c_str(), 0700);
    }

    int IsExisted(std::string filepath) {
        return access( filepath.c_str(), F_OK ) != -1;
    }

    std::string GetFile(std::string filepath) {
        FILE* fp = fopen(filepath.c_str(), "rb");
        if (fp != NULL) {
            fseek(fp, 0L, SEEK_END);
            int sz = ftell(fp);
            fseek(fp, 0L, SEEK_SET);
            std::string buf;
            buf.resize(sz);
            fread(&buf[0], sz, 1, fp);
            fclose(fp);
            return buf;
        }
        else {
            printf("Warning: file %s is zero size \n", filepath.c_str());
            return std::string();
        }
    }

    bool SaveToDisk(std::string filepath, std::string& data) {
        FILE* fp = fopen(filepath.c_str(), "wb");
        if (fp != NULL) {
          fwrite(&data[0], 1, data.size(), fp);
          fclose(fp);
          printf("== Save file: %s \n", filepath.c_str());
        }
        return fp != NULL;
    }

    int GetLocalAttr(std::unordered_map<std::string, std::string>& stat_hash, 
        std::string& attrpath, std::string& stbuf) {
        const auto& it = stat_hash.find(attrpath);
        // check if it's in hash
        // if so, return from hash
        // otherwise
        //   check if it existd in local disk
        //   if so, read it and hash it
        //   otherwise, return -1
        if (it == stat_hash.end()) {
            stbuf = GetFile(attrpath);
            if (stbuf.size() == 0) {
                return -1;
            }
            stat_hash.insert({attrpath, stbuf});
        }
        else {
            stbuf = it->second;
        }
        return 0;
    }

    int ReadFile(uint64_t fd, std::string& buf) {
        off_t currentPos = lseek(fd, (size_t)0, SEEK_CUR);
        int size = lseek(fd, (size_t)0, SEEK_END);
        buf.resize(size);
        lseek(fd, 0, SEEK_SET);
        int res = read(fd, &buf[0], size);
        if (res < 0) {
            printf(" CACHE READ FAIL. ERRNO = %d\n", errno);
        }
        //printf("--- ReadFile size:%u, str:%s \n", buf.size(), buf.c_str());
        return res;
    }

    int SaveFile(const std::string& filepath, std::string& data, uint64_t& fh) {
        int fd = open(filepath.c_str(), O_RDWR | O_CREAT | O_EXCL, 0644);
        //int fd = open(filepath.c_str(), O_RDWR);
        printf("open with O_CREAT\n");
        if ((fd == -1) && (EEXIST == errno)) {
            /* open the existing file with truncate flag */
            fd = open(filepath.c_str(), O_TRUNC | O_RDWR);
            printf("open with O_TRUNC\n");
            if (fd == -1) {
                return -errno;
            } 
        }

        fh = fd;
        printf("In.... SaveFile filename = %s, fd = %d\n",filepath.c_str(), fd);

        int res = pwrite(fd, &data[0], data.size(), 0 /*offset*/);
        lseek(fd, (size_t)0, SEEK_CUR);
        if (res == -1) {
            close(fd);
            return -errno;
        } 
        return 0;
    }

};

#endif
