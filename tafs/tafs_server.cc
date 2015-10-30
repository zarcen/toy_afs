/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include <grpc++/grpc++.h>

#include "tafs.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using tafs::HelloRequest;
using tafs::HelloReply;
using tafs::LoginRequest;
using tafs::LoginReply;
using tafs::ReadReq;
using tafs::ReadReply;
using tafs::GetAttrReq;
using tafs::GetAttrReply;
using tafs::ToyAFS;

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public ToyAFS::Service {
    public:
        const std::string path_prefix; 
        GreeterServiceImpl(): path_prefix("/tmp/tafs/") {
        }

        /** Sample
        */
        Status SayHello(ServerContext* context, const HelloRequest* request,
                HelloReply* reply) override {
            std::string prefix("Hello ");
            reply->set_message(prefix + request->name());

            std::string t;
            t.resize(40);
            for (int i = 0; i < 10; i++) {
                memcpy(&t[i*4], &i, sizeof(int));
            }

            for (int i = 0; i < 10; i++) {
                int k;
                memcpy(&k, &t[i*4], sizeof(int));
                printf("%d\n", k);
            }


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
            struct stat *stbuf;
            std::string path = path_prefix + request->path();
            res = lstat(path.c_str(), stbuf);
            if (res == -1) {
                reply->set_err(-errno);
            }
            std::string buf;
            buf.resize(sizeof(struct stat));


            memcpy(&buf[0], stbuf, sizeof(struct stat));
            reply->set_buf(buf);    
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

            int fd = open(path.c_str(), O_RDONLY);
            if (fd == -1) {
                reply->set_num_bytes(-1);
                return Status::OK;
            }

            std::string buf;
            int size;
            off_t currentPos = lseek(fd, (size_t)0, SEEK_CUR);
            size = lseek(fd, (size_t)0, SEEK_END);
            lseek(fd, currentPos, SEEK_SET);
            buf.resize(size);
            res = read(fd, &buf[0], size);
            if (res == -1) {
                reply->set_num_bytes(-1);
            }
            close(fd);
            reply->set_buf(buf);

            return Status::OK;
        }

        /**
         * login
         */
        Status Login(ServerContext* context, const LoginRequest* request,
                LoginReply* reply) override {
            // get input
            int uid = request->num();

            // set output
            reply->set_num(uid);

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
