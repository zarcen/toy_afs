/*
FUSE: Filesystem in Userspace
Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
Copyright (C) 2011       Sebastian Pipping <sebastian@pipping.org>

This program can be distributed under the terms of the GNU GPL.
See the file COPYING.

gcc -Wall fusexmp.c `pkg-config fuse --cflags --libs` -o fusexmp
*/

#define FUSE_USE_VERSION 26

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite()/utimensat() */
#define _XOPEN_SOURCE 700
#endif

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

#include "tafs.h"

static GreeterClient* greeter = NULL;
static char* cache_prefix = "/tmp/cache/";

void InitRPC(const char* serverhost) {
    if (greeter == NULL) {
        greeter = new GreeterClient(grpc::CreateChannel(serverhost, grpc::InsecureCredentials()));
    }
}

static int xmp_getattr(const char *path, struct stat *stbuf) {
    printf("## START ## xmp_getattr\n");
    int res;
    std::string rpcbuf;
    std::string cpp_path = path;
    res = greeter->GetAttr(cpp_path, rpcbuf);
    if (res < 0) {
        return res;
    }
    // debug
    printf("rpcbuf size: %lu, stat size: %lu \n", rpcbuf.size(), sizeof(struct stat));
    assert(rpcbuf.size() == sizeof(struct stat));

    memset(stbuf, 0, sizeof(struct stat)); 
    memcpy(stbuf, &rpcbuf[0], rpcbuf.size());

    return 0;
}

static int xmp_access(const char *path, int mask)
{
    printf("## START ## xmp_access\n");
    return 0;
    /*
       int res;
       std::string cpp_path = path;
       res = greeter->Access(cpp_path, mask);
       if (res < 0)
       return -errno;

       return res;
       */
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
    int res;

    res = readlink(path, buf, size - 1);
    if (res == -1)
        return -errno;

    buf[res] = '\0';
    return 0;
}


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
        off_t offset, struct fuse_file_info *fi)
{
    printf("## START ## xmp_readdir\n");
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
        res = -errno;
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

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    printf("## START ## xmp_mknod\n");
    int res;
    std::string cpp_path = path;
    res = greeter->Mknod(cpp_path, mode, rdev);

    return res == -1 ? -errno : 0;    

    /* On Linux this could just be 'mknod(path, mode, rdev)' but this
       is more portable */
    /*
       if (S_ISREG(mode)) {
       res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
       if (res >= 0)
       res = close(res);
       } else if (S_ISFIFO(mode))
       res = mkfifo(path, mode);
       else
       res = mknod(path, mode, rdev);
       if (res == -1)
       return -errno;

       return 0;
       */
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    printf("## START ## xmp_mkdir\n");

    std::string cpp_path = path;
    int res = greeter->MkDir(cpp_path, mode);

    return res == -1 ? -errno : 0;    

    /*
       res = mkdir(path, mode);
       if (res == -1)
       return -errno;

       return 0;
       */
}

static int xmp_unlink(const char *path)
{
    printf("## START ## xmp_unlink\n");
    std::string cpp_path = path;
    int res = greeter->Unlink(cpp_path);

    return res == -1 ? -errno : 0;
    /*
       res = unlink(path);
       if (res == -1)
       return -errno;

       return 0;
       */
}

static int xmp_rmdir(const char *path)
{
    printf("## START ## xmp_rmdir\n");
    std::string cpp_path = path;
    int res = greeter->RmDir(cpp_path);

    return res == -1 ? -errno : 0;    
    /*
       res = rmdir(path);
       if (res == -1)
       return -errno;

       return 0;
       */
}

static int xmp_symlink(const char *from, const char *to)
{
    int res;

    res = symlink(from, to);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    printf("## START ## xmp_rename\n");
    int res;

    res = rename(from, to);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_link(const char *from, const char *to)
{
    printf("## START ## xmp_link\n");
    int res;

    res = link(from, to);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
    printf("## START ## xmp_chmod\n");
    int res;

    res = chmod(path, mode);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
    printf("## START ## xmp_chown\n");
    int res;

    res = lchown(path, uid, gid);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
    printf("## START ## xmp_truncate\n");

    std::string cpp_path = path;
    int res = greeter->Truncate(cpp_path, size);

    return res == -1 ? -errno : 0;    

    /*
       int res;

       res = truncate(path, size);
       if (res == -1)
       return -errno;

       return 0;
       */
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    printf("## START ## xmp_open\n");
    std::string cpp_path = path;
    std::string cache_path = cache_prefix + cpp_path;
    printf("XMP_OPEN: cache_path -> %s\n", cache_path.c_str());
    // F_OK: check file exist
    // W_OK: check write permission
    // R_OK: check read permission
    // X_OK: check execute permission
    int local_res;
    struct stat local_stbuf;
    memset(&local_stbuf, 1, sizeof(struct stat)); 
    // get remote-stat from remote server via rpc call
    struct stat stbuf;
    std::string rpcbuf;
    int res = greeter->GetAttr(cpp_path, rpcbuf);
    if (res < 0) {
        return res;
    }
    memset(&stbuf, 0, sizeof(struct stat)); 
    memcpy(&stbuf, &rpcbuf[0], rpcbuf.size());
    // end get remote-stat

    // if local disk cache exists, compare difference; if the same, then return
    if ( (local_res = lstat(cache_path.c_str(), &local_stbuf)) == -1 
            || memcmp (&stbuf, &local_stbuf, sizeof(struct stat)) != 0) {  
        // Sync file from remote
        off_t size = stbuf.st_size; // the server-side file size
        res = greeter->Read(cpp_path, rpcbuf, (int)size, 0 /*offset*/);
        if (res < 0) {
            return res;
        }
        int fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0644);
        if ((fd == -1) && (EEXIST == errno)) {
            /* open the existing file with truncate flag */
            fd = open(path, O_TRUNC);
            if (res == -1) {
                return -errno;
            } 
        }
        res = pwrite(fd, &rpcbuf[0], size, 0 /*offset*/);
        if (res == -1) {
            return -errno;
        } 
    } 
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
        struct fuse_file_info *fi)
{
    printf("## START ## xmp_read\n");
    int res;
    // Read
    std::string rpcbuf;
    std::string cpp_path = path;

    res = greeter->Read(cpp_path, rpcbuf, size, offset);
    if (res < 0) {
        res = -errno;
        return res;
    }
    memcpy(buf, &rpcbuf[0]+offset, size);
    return size;
}

static int xmp_write(const char *path, const char *buf, size_t size,
        off_t offset, struct fuse_file_info *fi)
{
    printf("## START ## xmp_write\n");
    std::string cpp_path = path;
    std::string cache_path = cache_prefix + cpp_path;
    printf("XMP_WRITE: cache_path -> %s\n", cache_path.c_str());
    int fd = open(cache_path.c_str(), O_WRONLY);
    if (fd == -1) {
        return -errno;
    }
    int res = pwrite(fd, buf, size, offset);
    if (res == -1) {
        return -errno;
    }
    // Sync back to server
    std::string rpcbuf;
    rpcbuf.resize(size);
    memcpy(&rpcbuf[0], buf, size);
    res = greeter->Write(cpp_path, rpcbuf, size, offset);
    return res;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
    printf("## START ## xmp_statfs\n");
    int res;

    res = statvfs(path, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
    return 0;
}

static int xmp_release(const char *path, struct fuse_file_info *fi)
{
    printf("## START ## xmp_release\n");
    /* Just a stub.	 This method is optional and can safely be left
       unimplemented */

    (void) path;
    (void) fi;
    return 0;
}

static int xmp_flush(const char *path, struct fuse_file_info *fi)
{
    printf("## START ## xmp_flush\n");
    return 0;
}

static int xmp_fsync(const char *path, int isdatasync,
        struct fuse_file_info *fi)
{
    printf("## START ## xmp_fsync\n");
    /* Just a stub.	 This method is optional and can safely be left
       unimplemented */

    (void) path;
    (void) isdatasync;
    (void) fi;
    return 0;
}


struct tafs_fuse_operations:fuse_operations {
    tafs_fuse_operations () {
        getattr	= xmp_getattr;
        access		= xmp_access;
        readlink	= xmp_readlink;
        readdir	    = xmp_readdir;
        mknod		= xmp_mknod;
        mkdir		= xmp_mkdir;
        symlink	    = xmp_symlink;
        unlink		= xmp_unlink;
        rmdir		= xmp_rmdir;
        rename		= xmp_rename;
        link		= xmp_link;
        chmod		= xmp_chmod;
        chown		= xmp_chown;
        truncate	= xmp_truncate;
        utimens	    = xmp_utimens,
        open		= xmp_open;
        read		= xmp_read;
        write		= xmp_write;
        statfs		= xmp_statfs;
        release	    = xmp_release;
        flush       = xmp_flush;
        fsync		= xmp_fsync;
    }
};

static struct tafs_fuse_operations xmp_oper;

int main(int argc, char** argv) {                                                                                                                                            
    /*if (argc < 2) {
      printf("Please provide the serverhost to connect\n");
      }
      char* serverhost = argv[1];
      */
    //InitRPC(serverhost);
    InitRPC("node1:50051");

    // Login                                                                                                                                                                   
    int login_reply = greeter->Login(739);                                                                                                                                      
    std::cout << "Login Replied: " << login_reply << std::endl;                                                                                                                

    /*
    // Read
    std::string readbuf;
    greeter->Read("1.txt", readbuf);
    printf("%s\n", readbuf.c_str()); 
    */

    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);

}
