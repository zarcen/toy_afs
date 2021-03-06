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
using grpc::ClientReader;
using grpc::ClientWriter;
using grpc::Status;

using tafs::ToyAFS;
using tafs::GetAttrReq;
using tafs::GetAttrReply;
using tafs::OpenReq;
using tafs::OpenReply;
using tafs::ReadDirReq;
using tafs::ReadDirReply;
using tafs::MkDirReq;
using tafs::MkDirReply;
using tafs::RmDirReq;
using tafs::RmDirReply;
using tafs::AccessReq;
using tafs::AccessReply;
using tafs::WriteSReq;
using tafs::WriteSReply;
using tafs::MknodReq;
using tafs::MknodReply;
using tafs::UnlinkReq;
using tafs::UnlinkReply;
using tafs::TruncateReq;
using tafs::TruncateReply;
using tafs::RenameReq;
using tafs::RenameReply;
using tafs::ReadSReq;
using tafs::ReadSReply;


class GreeterClient {
    enum ErrorCode {
        LoginError = -1
    };

    public:
    GreeterClient(std::shared_ptr<Channel> channel)
        : stub_(ToyAFS::NewStub(channel)) {}

    // put stat in buf
    int GetAttr(const std::string& path, std::string& buf) {
        GetAttrReq request;
        request.set_path(path);

        GetAttrReply reply;
        ClientContext context;
        Status status = stub_->GetAttr(&context, request, &reply);
        if (status.ok()) {
            buf = reply.buf();
            return reply.err();
        } else {
            return -1;
        }
    }

    /*
    int Read(const std::string& path, std::string& buf, int size, int offset) {
        ReadReq request;
        request.set_path(path);
        request.set_size(size);
        request.set_offset(offset);

        ReadReply reply;
        ClientContext context;
        Status status = stub_->Read(&context, request, &reply);
        if (status.ok()) {
            buf = reply.buf();
            return reply.num_bytes();
        }
        else {
            return -1;
        }
    }
    */

    // put file in buf
    int ReadS(const std::string& path, std::string& buf, int size, int offset) {
        ReadSReq request;
        request.set_path(path);
        request.set_size(size);
        request.set_offset(offset);

        ReadSReply reply;
        ClientContext context;

        std::unique_ptr<ClientReader<ReadSReply> > reader(stub_->ReadS(&context, request));
        buf = std::string();
        buf.reserve(size);

        while (reader->Read(&reply)) {
            buf += reply.buf();
            if (reply.num_bytes() < 0) {
                break;
            }
        }
        Status status = reader->Finish();
        return status.ok() ? buf.size() : -1;
    }

    // read directory
    int ReadDir(const std::string& path, std::vector<std::string>& buf) {
        ReadDirReq request;
        request.set_path(path);

        ReadDirReply reply;
        ClientContext context;

        std::unique_ptr<ClientReader<ReadDirReply> > reader(stub_->ReadDir(&context, request));

        while (reader->Read(&reply)) {
            buf.push_back(reply.buf());
            if (reply.err() < 0) {
                break;
            }
        }

        Status status = reader->Finish();

        return status.ok() ? reply.err() : -1;
    }

    int MkDir(const std::string& path, int mode) {
        MkDirReq request;
        request.set_path(path);
        request.set_mode(mode);

        MkDirReply reply;
        ClientContext context;
        Status status = stub_->MkDir(&context, request, &reply);

        return status.ok() ? reply.err() : -1;
    }

    int Open(const std::string& path, int flag) {
        OpenReq request;
        request.set_path(path);
        request.set_flag(flag);

        OpenReply reply;
        ClientContext context;
        Status status = stub_->Open(&context, request, &reply);

        return status.ok() ? reply.err() : -1;
    }

    int RmDir(const std::string& path) {
        RmDirReq request;
        request.set_path(path);

        RmDirReply reply;
        ClientContext context;
        Status status = stub_->RmDir(&context, request, &reply);

        return status.ok() ? reply.err() : -1;
    }

    int Access(const std::string& path, int mode) {
        AccessReq request;
        request.set_path(path);
        request.set_mode(mode);

        AccessReply reply;
        ClientContext context;
        Status status = stub_->Access(&context, request, &reply);

        return status.ok() ? reply.err() : -1;
    }

    /*
    int Write(const std::string& path, std::string& data, int size, int offset) {
        WriteReq request;
        request.set_path(path);
        request.set_buf(data);
        request.set_size(size);
        request.set_offset(offset);

        WriteReply reply;
        ClientContext context;
        Status status = stub_->Write(&context, request, &reply);

        return status.ok() ? reply.num_bytes() : -1;
    }
    */

    int WriteS(const std::string& path, std::string& data, int size, int offset) {
        WriteSReq request;
        WriteSReply reply;
        ClientContext context;
        std::unique_ptr<ClientWriter<WriteSReq> > writer(
                stub_->WriteS(&context, &reply));
        int remain = size;
        int stump = 1048576; // 1Mb
        int curr = offset;
        while (remain >= 0) {
            request.set_path(path); 
            request.set_buf(data.substr(curr, std::min(stump, remain)));
            request.set_size(std::min(stump, remain));
            request.set_offset(curr);
            curr += stump;
            remain -= stump;
            if (!writer->Write(request)) {
                // Broken stream.
                break;
            }
        }
        writer->WritesDone();
        Status status = writer->Finish();

        return status.ok() ? reply.num_bytes() : -1;
    }

    int Mknod(const std::string& path, int mode, int rdev) {
        MknodReq request;
        request.set_path(path);
        request.set_mode(mode);
        request.set_rdev(rdev);

        MknodReply reply;
        ClientContext context;
        Status status = stub_->Mknod(&context, request, &reply);

        return status.ok() ? reply.err() : -1;
    }

    int Unlink(const std::string& path) {
        UnlinkReq request;
        request.set_path(path);

        UnlinkReply reply;
        ClientContext context;
        Status status = stub_->Unlink(&context, request, &reply);
        return reply.err();
    } 

    int Truncate(const std::string& path, int size) {
        TruncateReq request;
        request.set_path(path);
        request.set_size(size);

        TruncateReply reply;
        ClientContext context;
        Status status = stub_->Truncate(&context, request, &reply);

        return status.ok() ? reply.err() : -1;
    }

    int Rename(const std::string& oldpath, const std::string& newpath) {
        RenameReq request;
        request.set_oldpath(oldpath);
        request.set_newpath(newpath);
        RenameReply reply;
        ClientContext context;
        Status status = stub_->Rename(&context, request, &reply);
        return status.ok() ? reply.err() : -1;
    }

    private:
    std::unique_ptr<ToyAFS::Stub> stub_;
};


