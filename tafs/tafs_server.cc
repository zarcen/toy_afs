#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>

#include <grpc++/grpc++.h>

#include "tafs.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using tafs::OpenReq;
using tafs::OpenReply;
using tafs::AccessReq;
using tafs::AccessReply;
using tafs::ReadReq;
using tafs::ReadReply;
using tafs::WriteReq;
using tafs::WriteReply;
using tafs::TruncateReq;
using tafs::TruncateReply;
using tafs::RenameReq;
using tafs::RenameReply;
using tafs::MknodReq;
using tafs::MknodReply;
using tafs::UnlinkReq;
using tafs::UnlinkReply;
using tafs::ReadDirReq;
using tafs::ReadDirReply;
using tafs::MkDirReq;
using tafs::MkDirReply;
using tafs::RmDirReq;
using tafs::RmDirReply;
using tafs::GetAttrReq;
using tafs::GetAttrReply;
using tafs::ToyAFS;

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public ToyAFS::Service {
    public:
        const std::string path_prefix; 
        GreeterServiceImpl(): path_prefix("/tmp/tafs") {
        }

        /**
         * Access
         */
        Status Access(ServerContext* context, const AccessReq* request,
                AccessReply* reply) override {
            // default errno = 0
            reply->set_err(0);

            std::string path = path_prefix + request->path();

            int res;

            res = access(path.c_str(), request->mode());
            if (res == -1) {
                reply->set_err(-errno);
                return Status::OK;
            }
            reply->set_err(res);
            return Status::OK;
        }

        /**
         * GetAttr
         */
        Status GetAttr(ServerContext* context, const GetAttrReq* request,
                GetAttrReply* reply) override {
            // default errno = 0
            reply->set_err(0);
            int res;
            struct stat stbuf;
            std::string path = path_prefix + request->path();

            printf("GetAttr: %s \n", path.c_str());

            res = lstat(path.c_str(), &stbuf);
            if (res == -1) {
                reply->set_err(-errno);
            } 
            else {
                std::string buf;

                int stat_size = sizeof(struct stat);
                buf.resize(stat_size);

                assert(buf.size() == sizeof(struct stat));
                memcpy(&buf[0], &stbuf, buf.size());
                reply->set_buf(buf);    
            }
            return Status::OK;
        }

        /**
         * Open
         */
        Status Open(ServerContext* context, const OpenReq* request,
                OpenReply* reply) override {
            // default errno = 0
            reply->set_err(0);

            std::string path = path_prefix + request->path();

            int res;

            res = open(path.c_str(), request->flag()); 
            if (res == -1) {
                reply->set_err(-errno);
                return Status::OK;
            }
            close(res);
            reply->set_err(res);
            return Status::OK;
        }

        /**
         * Read
         */
        Status Read(ServerContext* context, const ReadReq* request,
                ReadReply* reply) override {
            // default errno = 0
            reply->set_num_bytes(0);
            int res;
            std::string path = path_prefix + request->path();
            int size = request->size();
            int offset = request->offset();

            int fd = open(path.c_str(), O_RDONLY);
            if (fd == -1) {
                reply->set_num_bytes(-1);
                return Status::OK;
            }

            std::string buf;
            buf.resize(size);
            res = pread(fd, &buf[0], size, offset);
            if (res == -1) {
                reply->set_num_bytes(-errno);
            }
            close(fd);
            reply->set_buf(buf);
            reply->set_num_bytes(res);

            return Status::OK;
        }

        /**
         * Write
         */
        Status Write(ServerContext* context, const WriteReq* request,
                WriteReply* reply) override {
            // default errno = 0
            reply->set_num_bytes(-errno);
            std::string path = path_prefix + request->path();
            int fd;
            int res;
            int size = request->size();
            int offset = request->offset();

            fd = open(path.c_str(), O_WRONLY);
            if (fd == -1) {
                reply->set_num_bytes(-errno);
                return Status::OK;
            }

            std::string buf = request->buf();
            res = pwrite(fd, &buf[0], size, offset);

            if (res == -1) {
                reply->set_num_bytes(-errno);
                return Status::OK;
            }

            close(fd);
            reply->set_num_bytes(res);
            return Status::OK;
        }

        /**
         * Truncate
         */
        Status Truncate(ServerContext* context, const TruncateReq* request,
                TruncateReply* reply) override {
            // default errno = 0
            reply->set_err(-errno);
            std::string path = path_prefix + request->path();
            int res;

            res = truncate(path.c_str(), request->size());
            if (res == -1) {
                reply->set_err(-errno);
                return Status::OK;
            }
            reply->set_err(res);
            return Status::OK;
        }
        
        /**
         * Rename
         */
        Status Rename(ServerContext* context, const RenameReq* request,
                RenameReply* reply) override {
            // default errno = 0
            reply->set_err(0);
            std::string oldpath = path_prefix + request->oldpath();
            std::string newpath = path_prefix + request->newpath();
            int res = rename(oldpath.c_str(), newpath.c_str());
            if (res == -1) {
                reply->set_err(-1);
            }
            reply->set_err(res);
            return Status::OK;
        }

        /**
         * Mknod
         */
        Status Mknod(ServerContext* context, const MknodReq* request,
                MknodReply* reply) override {
            // default errno = 0
            reply->set_err(-errno);
            std::string path = path_prefix + request->path();
            int mode = request->mode();
            int rdev = request->rdev();
            int res;

            /* On Linux this could just be 'mknod(path, mode, rdev)' but this
               is more portable */
            if (S_ISREG(mode)) {
                res = open(path.c_str(), O_CREAT | O_EXCL | O_WRONLY, mode);
                if (res >= 0) {
                    res = close(res);
                }
            } else if (S_ISFIFO(mode)) {
                res = mkfifo(path.c_str(), mode);
            } else {
                res = mknod(path.c_str(), mode, rdev);
            }

            if (res == -1) {
                reply->set_err(-errno);
                return Status::OK;
            }
            reply->set_err(res);
            return Status::OK;
        }

        /**
         * ReadDir
         */
        Status ReadDir(ServerContext* context, const ReadDirReq* request,
                ServerWriter<ReadDirReply>* writer) override {
            ReadDirReply* reply = new ReadDirReply();
            // default errno = 0
            reply->set_err(0);

            std::string path = path_prefix + request->path();

            DIR *dp;
            struct dirent *de;

            dp = opendir(path.c_str());
            if (dp == NULL) {
                reply->set_err(-errno);
                return Status::OK;
            }

            while ((de = readdir(dp)) != NULL) {
                std::string buf;
                buf.resize(sizeof(struct dirent));
                memcpy(&buf[0], de, buf.size());
                reply->set_buf(buf);
                writer->Write(*reply);
            }

            closedir(dp);
            return Status::OK;
        }

        /**
         * MkDir
         */
        Status MkDir(ServerContext* context, const MkDirReq* request,
                MkDirReply* reply) override {
            // default errno = 0
            reply->set_err(0);
            std::string path = path_prefix + request->path();
            int res;

            res = mkdir(path.c_str(), request->mode());
            if (res == -1) {
                reply->set_err(-errno);
                return Status::OK;
            }
            reply->set_err(res);
            return Status::OK;
        }

        /**
         * RmDir
         */
        Status RmDir(ServerContext* context, const RmDirReq* request,
                RmDirReply* reply) override {
            // default errno = 0
            reply->set_err(0);
            std::string path = path_prefix + request->path();
            int res;

            res = rmdir(path.c_str());
            if (res == -1) {
                reply->set_err(-errno);
                return Status::OK;
            }

            reply->set_err(res);
            return Status::OK;
        }

        /**
         * Unlink
         */
        Status Unlink(ServerContext* context, const UnlinkReq* request,
                UnlinkReply* reply) override {
            // default errno = 0
            reply->set_err(0);
            std::string path = path_prefix + request->path();
            int res;
            res = unlink(path.c_str());
            if (res == -1) {
                reply->set_err(-errno);
                return Status::OK;
            }
            reply->set_err(res);
            return Status::OK;
        }

};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    GreeterServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();

    return 0;
}
