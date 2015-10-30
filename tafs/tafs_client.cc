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

#include <grpc++/grpc++.h>

#include "tafs.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using tafs::HelloRequest;
using tafs::HelloReply;
using tafs::LoginRequest;
using tafs::LoginReply;
using tafs::ToyAFS;
using tafs::GetAttrReq;
using tafs::GetAttrReply ;
using tafs::OpenReq;
using tafs::ReadReq;
using tafs::ReadReply;

class GreeterClient {
 enum ErrorCode {
   LoginError = -1
 };

 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(ToyAFS::NewStub(channel)) {}

  // Assambles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string& user) {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SayHello(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    }
  }

  int Login(int uid) {
    LoginRequest request;
    request.set_num(uid);

    LoginReply reply;
    ClientContext context;
    Status status = stub_->Login(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.num();
    } else {
      return LoginError;
    }
  }

  // put stat in buf
  int GetAttr(const std::string& path, std::string& buf) {
    GetAttrReq request;
    request.set_path(path);

    GetAttrReply reply;
    ClientContext context;
    Status status = stub_->GetAttr(&context, request, &reply);
    buf = reply.buf();
    return reply.err();
  }

  // put file in buf
  int Read(const std::string& path, std::string& buf) {
    ReadReq request;
    request.set_path(path);

    ReadReply reply;
    ClientContext context;
    Status status = stub_->Read(&context, request, &reply);
    buf = reply.buf();
    return reply.num_bytes();
  }

 private:
  std::unique_ptr<ToyAFS::Stub> stub_;
};

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureCredentials()).
  GreeterClient greeter(
      grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()));

  // SayHello
  std::string user("world");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;

  // Login
  int login_reply = greeter.Login(739);
  std::cout << "Login Replied: " << login_reply << std::endl;

  return 0;
}