/*
This program can be distributed under the terms of the GNU GPL.
See the file COPYING.

Compile: see Makefile
Usage: ./fusetafs /tmp/afs -d

Issues: make some part more c++ style
*/

#define FUSE_USE_VERSION 26

#include <signal.h>
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <cassert>
#include <unordered_map>
#include <iostream>
#include <chrono>

#include "tafs_rpc.h"
#include "cache_util.h"

// rpc caller object                                                                                                                                                        
static GreeterClient* greeter = NULL;                                                                                                                                       
// memory cache for file stat; <path, stat>                                                                                                                                 
static std::unordered_map<std::string, std::string> stat_hash;                                                                                                              
// flag for release() to decide to contact server or not                                                                                                                    
static int writeback_flag = -1;


class Timer {
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
    std::string str_;
public:
  Timer(std::string str) {
    str_ = str;
    start = std::chrono::system_clock::now();
  }
  ~Timer() {
      end = std::chrono::system_clock::now();
      auto elapsed =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      std::cout<<str_<<":"<<elapsed.count()<<"\n";
  }
};


void InitRPC(const char* serverhost) {
    if (greeter == NULL) {
        greeter = new GreeterClient(grpc::CreateChannel(serverhost, grpc::InsecureCredentials()));
    }
}

int IsCrash(const char* filename) {
    FILE* fp = fopen(filename, "r");
    int crash = 0;
    if (fp != NULL) {                                                                                                                                                       
        fscanf(fp, "%d", &crash);                                                                                                                                  
    } else {                                                                                                                                                                
        printf("FAIL OPEN FILE for CRASH \n");                                                                                                                   
    }                                                                                                                                                                       
    fclose(fp);
    return crash;
}

static int tafs_getattr(const char *path, struct stat *stbuf) {
    printf("= = = =  START = = = =  tafs_getattr\n");
    int res;
    std::string rpcbuf;
    std::string cpp_path = path;
    CacheUtil cu;
    std::string localfile_path = cu.ToCacheFileName(path);
    std::string localattr_path = cu.ToCacheAttrName(path);

    // check if there's anything in cache to release to server
    if (cu.IsExisted(cu.ToCacheReleName(path))) {
        if (!cu.IsExisted(localfile_path)) {
            cu.Unlink(localfile_path);
        }
        else {
            int fd = open(localfile_path.c_str(), O_RDONLY);
            std::string local_buf;
            if (cu.ReadWholeFile(fd, local_buf) < 0) {
                return -1;
            }
            // Sync back to server
            res = greeter->WriteS(cpp_path, local_buf, local_buf.size(), 0 /*offset*/);
            if (res < 0) {
                printf("-- cache existed, fail write back--\n");
                return res;
            } else {
                printf(" -- cached existed, write back to server success\n");
            }
            res = close(fd);
        }
    }

    res = greeter->GetAttr(cpp_path, rpcbuf);
    if (res < 0) {
        return res;
    }
    stat_hash[cu.ToCacheAttrName(path)] = rpcbuf;
    assert(rpcbuf.size() == sizeof(struct stat));

    memset(stbuf, 0, sizeof(struct stat)); 
    memcpy(stbuf, &rpcbuf[0], rpcbuf.size());

    return res;
}

static int tafs_access(const char *path, int mask) {
    printf("= = = =  START = = = =  tafs_access\n");
    int res;
    std::string cpp_path = path;
    res = greeter->Access(cpp_path, mask);
    return res;
}

static int tafs_readlink(const char *path, char *buf, size_t size)
{
    int res;

    res = readlink(path, buf, size - 1);
    if (res == -1)
        return -errno;

    buf[res] = '\0';
    return 0;
}


static int tafs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
        off_t offset, struct fuse_file_info *fi)
{
    printf("= = = =  START = = = =  tafs_readdir\n");
    (void) offset;
    (void) fi;
    //
    int res;
    // ReadDir
    std::string rpcbuf;
    std::vector<std::string> bufs;

    std::string cpp_path = path;
    res = greeter->ReadDir(cpp_path, bufs);
    if (res < 0) {
        return res;
    }
    for (std::string& raw : bufs) {
        struct dirent de;
        memcpy(&de, &raw[0], raw.size());
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de.d_ino;
        st.st_mode = de.d_type << 12;
        if (filler(buf, de.d_name, &st, 0)) {
            break;
        }
    }
    return 0;
}

static int tafs_mknod(const char *path, mode_t mode, dev_t rdev)
{
    printf("= = = =  START = = = =  tafs_mknod\n");
    int res;
    std::string cpp_path = path;
    res = greeter->Mknod(cpp_path, mode, rdev);

    return res;
}

static int tafs_mkdir(const char *path, mode_t mode) {
    printf("= = = = START = = = = tafs_mkdir\n");
    CacheUtil cu;
    std::string cpp_path = path;
    int res = greeter->MkDir(cpp_path, mode);
    if (res != -1) {                                                                                                                                                        
        res = mkdir(cu.ToCacheFileName(path).c_str(), mode);
    }
    return res == -1 ? -errno : 0;    
}

static int tafs_unlink(const char *path) {
    printf("= = = =  START = = = =  tafs_unlink\n");
    CacheUtil cu;
    std::string cpp_path = path;
    int res = greeter->Unlink(cpp_path);
    if (res != -1) {                                                                                                                                                        
        if (cu.IsExisted(cu.ToCacheFileName(path))) {
            res = unlink(cu.ToCacheFileName(path).c_str());                                                                                                                     
        }
        if (cu.IsExisted(cu.ToCacheAttrName(path))) {
            res = unlink(cu.ToCacheAttrName(path).c_str());                                                                                                                     
        }
        if (cu.IsExisted(cu.ToCacheReleName(path))) {
            res = unlink(cu.ToCacheReleName(path).c_str());                                                                                                                     
        }
    }                                                                                                                                                                       
    return res == -1 ? -errno : 0;
}

static int tafs_rmdir(const char *path) {
    printf("= = = =  START = = = =  tafs_rmdir\n");
    CacheUtil cu;
    std::string cpp_path = path;
    int res = greeter->RmDir(cpp_path);
    if (res != -1) {                                                                                                                                                        
        res = rmdir(cu.ToCacheFileName(path).c_str());                                                                                                                      
    }
    return res == -1 ? -errno : 0;    
}

static int tafs_symlink(const char *from, const char *to)
{
    int res;

    res = symlink(from, to);
    if (res == -1)
        return -errno;

    return 0;
}

static int tafs_rename(const char *from, const char *to)
{
    printf("= = = = START = = = = tafs_rename\n");
    CacheUtil cu;
    std::string cpp_from = from;
    std::string cpp_to = to;
    int res = greeter->Rename(cpp_from, cpp_to);
    if (res != -1) {                                                                                                                                                        
        if (CacheUtil().IsExisted(cu.ToCacheFileName(from))) {
            res = rename(cu.ToCacheFileName(from).c_str(), 
                    cu.ToCacheFileName(to).c_str() );
        }
        if (CacheUtil().IsExisted(cu.ToCacheAttrName(from))) {
            res = rename(cu.ToCacheAttrName(from).c_str(), 
                    cu.ToCacheAttrName(to).c_str() );
        }
        if (CacheUtil().IsExisted(cu.ToCacheReleName(from))) {
            res = rename(cu.ToCacheReleName(from).c_str(), 
                    cu.ToCacheReleName(to).c_str() );
        }
    }
    return res == -1 ? -errno : 0;
}

static int tafs_link(const char *from, const char *to)
{
    printf("= = = =  START = = = =  tafs_link\n");
    int res;

    res = link(from, to);
    if (res == -1)
        return -errno;

    return 0;
}

static int tafs_chmod(const char *path, mode_t mode)
{
    printf("= = = =  START = = = =  tafs_chmod\n");
    int res;

    res = chmod(path, mode);
    if (res == -1)
        return -errno;

    return 0;
}

static int tafs_chown(const char *path, uid_t uid, gid_t gid)
{
    printf("= = = =  START = = = =  tafs_chown\n");
    int res;

    res = lchown(path, uid, gid);
    if (res == -1)
        return -errno;

    return 0;
}

static int tafs_truncate(const char *path, off_t size) {
    printf("= = = =  START = = = =  tafs_truncate\n");

    CacheUtil cu;
    std::string cpp_path = path;

    int res = truncate(cu.ToCacheFileName(path).c_str(), size);
    if (res < 0) {
        return -errno;
    } else {
        res = greeter->Truncate(cpp_path, size);
        writeback_flag = 1;
    }
    return res == -1 ? -errno : 0;    
}

static int tafs_open(const char *path, struct fuse_file_info *fi) {
    printf("= = = =  START = = = =  tafs_open\n");

    CacheUtil cu;

    std::string cpp_path = path;
    std::string localfile_path = cu.ToCacheFileName(path);
    std::string localattr_path = cu.ToCacheAttrName(path);

    // get server attr
    std::string server_stat;
    int res = greeter->GetAttr(cpp_path, server_stat);
    if (res < 0) {
        return res;
    }

    // read from local disk
    fi->fh = (int)-1;
    bool local_existed = false;
    if (cu.IsExisted(localfile_path) 
     && cu.IsExisted(localattr_path)) {
        printf("== local file and attr exist\n");

        std::string local_stat;
        if (cu.GetLocalAttr(stat_hash, localattr_path, local_stat) >= 0
                && server_stat.compare(local_stat) == 0) {
            local_existed  = true;
            fi->fh = open(localfile_path.c_str(), O_RDWR);
            printf("== local attr is same as server, fh: %d ==\n", (int)fi->fh);
        }
        else {
            printf("== local attr is NOT same as server");
        }
    }
    else {
        printf("== local file and attr not exist \n");
    }

    // read from server
    if (!local_existed) {
        std::string rpcbuf;
        struct stat st;
        memcpy(&st, &server_stat[0], sizeof(struct stat));
        int size = st.st_size;
        printf("--- st size: %d\n", size);
        res = greeter->ReadS(cpp_path, rpcbuf, size, 0);
        if (res < 0) {
            return res;
        }

        // save to local disk
        cu.mkfolder(localfile_path);
        if (cu.SaveFile(localfile_path, rpcbuf, fi->fh) < 0) {
            return -1;
        }

        uint64_t dummy;
        cu.mkfolder(localattr_path);
        if (cu.SaveFile(localattr_path, server_stat, dummy) < 0) {
            close(dummy);
            return -1;
        }

        close(dummy);
        printf("== Read from server and save ==\n");
    }

    return 0;
}

static int tafs_read(const char *path, char *buf, size_t size, off_t offset,
        struct fuse_file_info *fi) {
    printf("= = = =  START = = = =  tafs_read\n");
    CacheUtil cu;
    int res = cu.ReadFile(fi->fh, buf, size, offset);
    if (res < 0) {
        printf("-- readfile fail when fd = %d\n", (int)fi->fh);
    } else {
        printf("-- readfile success\n");
    }
    return res;
}

static int tafs_write(const char *path, const char *buf, size_t size,
        off_t offset, struct fuse_file_info *fi) {
    printf("= = = =  START = = = =  tafs_write\n");
    CacheUtil cu;
    std::string cpp_path = path;
    std::string cache_path = cu.ToCacheFileName(path);

    // validate fh
    if (-1 == cu.ValidateCacheFh(fi->fh, path)) {
        printf("--!-- Fail to validate %s\n", path); return -1;
    }

    int fd = fi->fh;
    int res = pwrite(fd, buf, size, offset);

    if (res < 0) {
        writeback_flag = -1;
        return res;
    }
    writeback_flag = 1;                                                                                                 
    printf("-- writefile success\n");
    return res;
}

static int tafs_statfs(const char *path, struct statvfs *stbuf) {
    printf("= = = =  START = = = =  tafs_statfs\n");
    int res;

    res = statvfs(path, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}

static int tafs_utimens(const char *path, const struct timespec ts[2]) {
    printf("= = = =  START = = = =  tafs_utimens\n");
    return 0;
}

static int tafs_flush(const char *path, struct fuse_file_info *fi) {
    printf("= = = =  START = = = =  tafs_flush\n");
    CacheUtil cu;

    // validate fh
    if (-1 == cu.ValidateCacheFh(fi->fh, path)) {
        printf("--!-- flush: Fail to validate %s\n", path); return -1;
    }

    int ret = fsync(fi->fh);

    close(fi->fh);

    // with fsync inside
    std::string rele = CacheUtil().ToCacheReleName(path);
    if (CacheUtil().IsExisted(rele)) {
        printf("--!--!-- %s already exists\n", rele.c_str());
    }
    else if (CacheUtil().Touch(rele) < 0) {
        printf("--!--!-- Fail to cache_repaly file: %s \n", path);
    }
    return ret;
}


static int tafs_release(const char *path, struct fuse_file_info *fi) {
    printf("= = = =  START = = = =  tafs_release\n");
    std::string cpp_path = path;
#ifdef CRASH_TEST
    if (IsCrash("crash_config")) {
        printf("Simulating CRASH Case...\n");                                                                                                                   
        kill(getpid(), SIGINT);
        return 0;
    }
    else {
#endif

        CacheUtil cu;                                                                                                                                                   
        if (-1 == cu.ValidateCacheFh(fi->fh, path)) {
            printf("--!-- Release: Fail to validate %s\n", path); return -1;
        }

        int res = 0;
        if ((int)fi->fh != -1) {
            if (writeback_flag == 1) {
                printf("ACTION(tafs_release) - write %s back to server\n", path);                                                                                                
                std::string local_buf;
                if (cu.ReadWholeFile(fi->fh, local_buf) < 0) {                                                                                                                       
                    fprintf(stderr,                                                                                                                                             
                            "ERROR(tafs_release) - failed at fetching cache from disk: %s\n",                                                                                    
                            strerror(errno));                                                                                                                                   
                    return -1;                                                                                                                                                  
                }                                                                                                                                                               
                // Sync back to server                                                                                                                                          
                res = greeter->WriteS(cpp_path, local_buf, local_buf.size(), 0 /*offset*/);                                                                                      
                if (res < 0) {                                                                                                                                                  
                    fprintf(stderr,                                                                                                                                             
                            "ERROR(tafs_release) - failed at writing to server RPC call: %s\n",                                                                                  
                            strerror(errno));                                                                                                                                   
                    return res;                                                                                                                                                 
                }
            }

            if (cu.Unlink(cu.ToCacheReleName(path))){ 
                printf("--!--!-- Fail to unlink file: %s \n", path);
            }
            else {
                printf("--!--!-- Success to unlink file: %s \n", path);
            }

            writeback_flag = -1;
            res = close(fi->fh);
        }
        return res;
#ifdef CRASH_TEST
    }
#endif
}

static int tafs_fsync(const char *path, int isdatasync,
        struct fuse_file_info *fi) {
    printf("= = = =  START = = = =  tafs_fsync\n");
    /* Just a stub.	 This method is optional and can safely be left
       unimplemented */

    (void) path;
    (void) isdatasync;
    (void) fi;
    return 0;
}


struct tafs_fuse_operations:fuse_operations {
    tafs_fuse_operations () {
        getattr	= tafs_getattr;
        access		= tafs_access;
        readlink	= tafs_readlink;
        readdir	    = tafs_readdir;
        mknod		= tafs_mknod;
        mkdir		= tafs_mkdir;
        symlink	    = tafs_symlink;
        unlink		= tafs_unlink;
        rmdir		= tafs_rmdir;
        rename		= tafs_rename;
        link		= tafs_link;
        chmod		= tafs_chmod;
        chown		= tafs_chown;
        truncate	= tafs_truncate;
        utimens	    = tafs_utimens,
        open		= tafs_open;
        read		= tafs_read;
        write		= tafs_write;
        statfs		= tafs_statfs;
        release	    = tafs_release;
        flush       = tafs_flush;
        fsync		= tafs_fsync;
    }
};

static struct tafs_fuse_operations tafs_oper;

int main(int argc, char** argv) {                                                                                                                                          
    const char* default_server = "node1:50051";                                                                                                                             
    FILE* fp = fopen("./server", "r");                                                                                                                                      
    if (fp != NULL) {                                                                                                                                                       
        char host_to_connect[200];                                                                                                                                          
        fscanf(fp, "%s", host_to_connect);                                                                                                                                  
        printf("connecting to... %s\n", host_to_connect);                                                                                                                   
        InitRPC(host_to_connect);                                                                                                                                           
    } else {                                                                                                                                                                
        printf("connecting to... %s\n", default_server);                                                                                                                    
        InitRPC(default_server);                                                                                                                                            
    }                                                                                                                                                                       

    umask(0);                                                                                                                                                               
    return fuse_main(argc, argv, &tafs_oper, NULL);
}
