// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: tafs.proto
#ifndef GRPC_tafs_2eproto__INCLUDED
#define GRPC_tafs_2eproto__INCLUDED

#include "tafs.pb.h"

#include <grpc++/support/async_stream.h>
#include <grpc++/impl/rpc_method.h>
#include <grpc++/impl/proto_utils.h>
#include <grpc++/impl/service_type.h>
#include <grpc++/support/async_unary_call.h>
#include <grpc++/support/status.h>
#include <grpc++/support/stub_options.h>
#include <grpc++/support/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace tafs {

class ToyAFS GRPC_FINAL {
 public:
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status SayHello(::grpc::ClientContext* context, const ::tafs::HelloRequest& request, ::tafs::HelloReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::HelloReply>> AsyncSayHello(::grpc::ClientContext* context, const ::tafs::HelloRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::HelloReply>>(AsyncSayHelloRaw(context, request, cq));
    }
    virtual ::grpc::Status Login(::grpc::ClientContext* context, const ::tafs::LoginRequest& request, ::tafs::LoginReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::LoginReply>> AsyncLogin(::grpc::ClientContext* context, const ::tafs::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::LoginReply>>(AsyncLoginRaw(context, request, cq));
    }
    virtual ::grpc::Status TestBytes(::grpc::ClientContext* context, const ::tafs::TestB& request, ::tafs::LoginReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::LoginReply>> AsyncTestBytes(::grpc::ClientContext* context, const ::tafs::TestB& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::LoginReply>>(AsyncTestBytesRaw(context, request, cq));
    }
    virtual ::grpc::Status GetAttr(::grpc::ClientContext* context, const ::tafs::GetAttrReq& request, ::tafs::GetAttrReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::GetAttrReply>> AsyncGetAttr(::grpc::ClientContext* context, const ::tafs::GetAttrReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::GetAttrReply>>(AsyncGetAttrRaw(context, request, cq));
    }
    virtual ::grpc::Status Open(::grpc::ClientContext* context, const ::tafs::OpenReq& request, ::tafs::OpenReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::OpenReply>> AsyncOpen(::grpc::ClientContext* context, const ::tafs::OpenReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::OpenReply>>(AsyncOpenRaw(context, request, cq));
    }
    virtual ::grpc::Status Read(::grpc::ClientContext* context, const ::tafs::ReadReq& request, ::tafs::ReadReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::ReadReply>> AsyncRead(::grpc::ClientContext* context, const ::tafs::ReadReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::ReadReply>>(AsyncReadRaw(context, request, cq));
    }
    virtual ::grpc::Status Write(::grpc::ClientContext* context, const ::tafs::WriteReq& request, ::tafs::WriteReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::WriteReply>> AsyncWrite(::grpc::ClientContext* context, const ::tafs::WriteReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::WriteReply>>(AsyncWriteRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReaderInterface< ::tafs::ReadDirReply>> ReadDir(::grpc::ClientContext* context, const ::tafs::ReadDirReq& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::tafs::ReadDirReply>>(ReadDirRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::tafs::ReadDirReply>> AsyncReadDir(::grpc::ClientContext* context, const ::tafs::ReadDirReq& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::tafs::ReadDirReply>>(AsyncReadDirRaw(context, request, cq, tag));
    }
    virtual ::grpc::Status MkDir(::grpc::ClientContext* context, const ::tafs::MkDirReq& request, ::tafs::MkDirReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::MkDirReply>> AsyncMkDir(::grpc::ClientContext* context, const ::tafs::MkDirReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::MkDirReply>>(AsyncMkDirRaw(context, request, cq));
    }
    virtual ::grpc::Status RmDir(::grpc::ClientContext* context, const ::tafs::RmDirReq& request, ::tafs::RmDirReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::RmDirReply>> AsyncRmDir(::grpc::ClientContext* context, const ::tafs::RmDirReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::RmDirReply>>(AsyncRmDirRaw(context, request, cq));
    }
    virtual ::grpc::Status Unlink(::grpc::ClientContext* context, const ::tafs::UnlinkReq& request, ::tafs::UnlinkReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::UnlinkReply>> AsyncUnlink(::grpc::ClientContext* context, const ::tafs::UnlinkReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::UnlinkReply>>(AsyncUnlinkRaw(context, request, cq));
    }
    virtual ::grpc::Status Access(::grpc::ClientContext* context, const ::tafs::AccessReq& request, ::tafs::AccessReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::AccessReply>> AsyncAccess(::grpc::ClientContext* context, const ::tafs::AccessReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::AccessReply>>(AsyncAccessRaw(context, request, cq));
    }
    virtual ::grpc::Status Flush(::grpc::ClientContext* context, const ::tafs::FlushReq& request, ::tafs::FlushReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::FlushReply>> AsyncFlush(::grpc::ClientContext* context, const ::tafs::FlushReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::FlushReply>>(AsyncFlushRaw(context, request, cq));
    }
    virtual ::grpc::Status Mknod(::grpc::ClientContext* context, const ::tafs::MknodReq& request, ::tafs::MknodReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::MknodReply>> AsyncMknod(::grpc::ClientContext* context, const ::tafs::MknodReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::MknodReply>>(AsyncMknodRaw(context, request, cq));
    }
    virtual ::grpc::Status Truncate(::grpc::ClientContext* context, const ::tafs::TruncateReq& request, ::tafs::TruncateReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::TruncateReply>> AsyncTruncate(::grpc::ClientContext* context, const ::tafs::TruncateReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::tafs::TruncateReply>>(AsyncTruncateRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::HelloReply>* AsyncSayHelloRaw(::grpc::ClientContext* context, const ::tafs::HelloRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::LoginReply>* AsyncLoginRaw(::grpc::ClientContext* context, const ::tafs::LoginRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::LoginReply>* AsyncTestBytesRaw(::grpc::ClientContext* context, const ::tafs::TestB& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::GetAttrReply>* AsyncGetAttrRaw(::grpc::ClientContext* context, const ::tafs::GetAttrReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::OpenReply>* AsyncOpenRaw(::grpc::ClientContext* context, const ::tafs::OpenReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::ReadReply>* AsyncReadRaw(::grpc::ClientContext* context, const ::tafs::ReadReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::WriteReply>* AsyncWriteRaw(::grpc::ClientContext* context, const ::tafs::WriteReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientReaderInterface< ::tafs::ReadDirReply>* ReadDirRaw(::grpc::ClientContext* context, const ::tafs::ReadDirReq& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::tafs::ReadDirReply>* AsyncReadDirRaw(::grpc::ClientContext* context, const ::tafs::ReadDirReq& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::MkDirReply>* AsyncMkDirRaw(::grpc::ClientContext* context, const ::tafs::MkDirReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::RmDirReply>* AsyncRmDirRaw(::grpc::ClientContext* context, const ::tafs::RmDirReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::UnlinkReply>* AsyncUnlinkRaw(::grpc::ClientContext* context, const ::tafs::UnlinkReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::AccessReply>* AsyncAccessRaw(::grpc::ClientContext* context, const ::tafs::AccessReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::FlushReply>* AsyncFlushRaw(::grpc::ClientContext* context, const ::tafs::FlushReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::MknodReply>* AsyncMknodRaw(::grpc::ClientContext* context, const ::tafs::MknodReq& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::tafs::TruncateReply>* AsyncTruncateRaw(::grpc::ClientContext* context, const ::tafs::TruncateReq& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub GRPC_FINAL : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::Channel>& channel);
    ::grpc::Status SayHello(::grpc::ClientContext* context, const ::tafs::HelloRequest& request, ::tafs::HelloReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::HelloReply>> AsyncSayHello(::grpc::ClientContext* context, const ::tafs::HelloRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::HelloReply>>(AsyncSayHelloRaw(context, request, cq));
    }
    ::grpc::Status Login(::grpc::ClientContext* context, const ::tafs::LoginRequest& request, ::tafs::LoginReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::LoginReply>> AsyncLogin(::grpc::ClientContext* context, const ::tafs::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::LoginReply>>(AsyncLoginRaw(context, request, cq));
    }
    ::grpc::Status TestBytes(::grpc::ClientContext* context, const ::tafs::TestB& request, ::tafs::LoginReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::LoginReply>> AsyncTestBytes(::grpc::ClientContext* context, const ::tafs::TestB& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::LoginReply>>(AsyncTestBytesRaw(context, request, cq));
    }
    ::grpc::Status GetAttr(::grpc::ClientContext* context, const ::tafs::GetAttrReq& request, ::tafs::GetAttrReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::GetAttrReply>> AsyncGetAttr(::grpc::ClientContext* context, const ::tafs::GetAttrReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::GetAttrReply>>(AsyncGetAttrRaw(context, request, cq));
    }
    ::grpc::Status Open(::grpc::ClientContext* context, const ::tafs::OpenReq& request, ::tafs::OpenReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::OpenReply>> AsyncOpen(::grpc::ClientContext* context, const ::tafs::OpenReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::OpenReply>>(AsyncOpenRaw(context, request, cq));
    }
    ::grpc::Status Read(::grpc::ClientContext* context, const ::tafs::ReadReq& request, ::tafs::ReadReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::ReadReply>> AsyncRead(::grpc::ClientContext* context, const ::tafs::ReadReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::ReadReply>>(AsyncReadRaw(context, request, cq));
    }
    ::grpc::Status Write(::grpc::ClientContext* context, const ::tafs::WriteReq& request, ::tafs::WriteReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::WriteReply>> AsyncWrite(::grpc::ClientContext* context, const ::tafs::WriteReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::WriteReply>>(AsyncWriteRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReader< ::tafs::ReadDirReply>> ReadDir(::grpc::ClientContext* context, const ::tafs::ReadDirReq& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::tafs::ReadDirReply>>(ReadDirRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::tafs::ReadDirReply>> AsyncReadDir(::grpc::ClientContext* context, const ::tafs::ReadDirReq& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::tafs::ReadDirReply>>(AsyncReadDirRaw(context, request, cq, tag));
    }
    ::grpc::Status MkDir(::grpc::ClientContext* context, const ::tafs::MkDirReq& request, ::tafs::MkDirReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::MkDirReply>> AsyncMkDir(::grpc::ClientContext* context, const ::tafs::MkDirReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::MkDirReply>>(AsyncMkDirRaw(context, request, cq));
    }
    ::grpc::Status RmDir(::grpc::ClientContext* context, const ::tafs::RmDirReq& request, ::tafs::RmDirReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::RmDirReply>> AsyncRmDir(::grpc::ClientContext* context, const ::tafs::RmDirReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::RmDirReply>>(AsyncRmDirRaw(context, request, cq));
    }
    ::grpc::Status Unlink(::grpc::ClientContext* context, const ::tafs::UnlinkReq& request, ::tafs::UnlinkReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::UnlinkReply>> AsyncUnlink(::grpc::ClientContext* context, const ::tafs::UnlinkReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::UnlinkReply>>(AsyncUnlinkRaw(context, request, cq));
    }
    ::grpc::Status Access(::grpc::ClientContext* context, const ::tafs::AccessReq& request, ::tafs::AccessReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::AccessReply>> AsyncAccess(::grpc::ClientContext* context, const ::tafs::AccessReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::AccessReply>>(AsyncAccessRaw(context, request, cq));
    }
    ::grpc::Status Flush(::grpc::ClientContext* context, const ::tafs::FlushReq& request, ::tafs::FlushReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::FlushReply>> AsyncFlush(::grpc::ClientContext* context, const ::tafs::FlushReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::FlushReply>>(AsyncFlushRaw(context, request, cq));
    }
    ::grpc::Status Mknod(::grpc::ClientContext* context, const ::tafs::MknodReq& request, ::tafs::MknodReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::MknodReply>> AsyncMknod(::grpc::ClientContext* context, const ::tafs::MknodReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::MknodReply>>(AsyncMknodRaw(context, request, cq));
    }
    ::grpc::Status Truncate(::grpc::ClientContext* context, const ::tafs::TruncateReq& request, ::tafs::TruncateReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::TruncateReply>> AsyncTruncate(::grpc::ClientContext* context, const ::tafs::TruncateReq& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::tafs::TruncateReply>>(AsyncTruncateRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::Channel> channel_;
    ::grpc::ClientAsyncResponseReader< ::tafs::HelloReply>* AsyncSayHelloRaw(::grpc::ClientContext* context, const ::tafs::HelloRequest& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::LoginReply>* AsyncLoginRaw(::grpc::ClientContext* context, const ::tafs::LoginRequest& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::LoginReply>* AsyncTestBytesRaw(::grpc::ClientContext* context, const ::tafs::TestB& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::GetAttrReply>* AsyncGetAttrRaw(::grpc::ClientContext* context, const ::tafs::GetAttrReq& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::OpenReply>* AsyncOpenRaw(::grpc::ClientContext* context, const ::tafs::OpenReq& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::ReadReply>* AsyncReadRaw(::grpc::ClientContext* context, const ::tafs::ReadReq& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::WriteReply>* AsyncWriteRaw(::grpc::ClientContext* context, const ::tafs::WriteReq& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientReader< ::tafs::ReadDirReply>* ReadDirRaw(::grpc::ClientContext* context, const ::tafs::ReadDirReq& request) GRPC_OVERRIDE;
    ::grpc::ClientAsyncReader< ::tafs::ReadDirReply>* AsyncReadDirRaw(::grpc::ClientContext* context, const ::tafs::ReadDirReq& request, ::grpc::CompletionQueue* cq, void* tag) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::MkDirReply>* AsyncMkDirRaw(::grpc::ClientContext* context, const ::tafs::MkDirReq& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::RmDirReply>* AsyncRmDirRaw(::grpc::ClientContext* context, const ::tafs::RmDirReq& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::UnlinkReply>* AsyncUnlinkRaw(::grpc::ClientContext* context, const ::tafs::UnlinkReq& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::AccessReply>* AsyncAccessRaw(::grpc::ClientContext* context, const ::tafs::AccessReq& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::FlushReply>* AsyncFlushRaw(::grpc::ClientContext* context, const ::tafs::FlushReq& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::MknodReply>* AsyncMknodRaw(::grpc::ClientContext* context, const ::tafs::MknodReq& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::tafs::TruncateReply>* AsyncTruncateRaw(::grpc::ClientContext* context, const ::tafs::TruncateReq& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    const ::grpc::RpcMethod rpcmethod_SayHello_;
    const ::grpc::RpcMethod rpcmethod_Login_;
    const ::grpc::RpcMethod rpcmethod_TestBytes_;
    const ::grpc::RpcMethod rpcmethod_GetAttr_;
    const ::grpc::RpcMethod rpcmethod_Open_;
    const ::grpc::RpcMethod rpcmethod_Read_;
    const ::grpc::RpcMethod rpcmethod_Write_;
    const ::grpc::RpcMethod rpcmethod_ReadDir_;
    const ::grpc::RpcMethod rpcmethod_MkDir_;
    const ::grpc::RpcMethod rpcmethod_RmDir_;
    const ::grpc::RpcMethod rpcmethod_Unlink_;
    const ::grpc::RpcMethod rpcmethod_Access_;
    const ::grpc::RpcMethod rpcmethod_Flush_;
    const ::grpc::RpcMethod rpcmethod_Mknod_;
    const ::grpc::RpcMethod rpcmethod_Truncate_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::Channel>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::SynchronousService {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status SayHello(::grpc::ServerContext* context, const ::tafs::HelloRequest* request, ::tafs::HelloReply* response);
    virtual ::grpc::Status Login(::grpc::ServerContext* context, const ::tafs::LoginRequest* request, ::tafs::LoginReply* response);
    virtual ::grpc::Status TestBytes(::grpc::ServerContext* context, const ::tafs::TestB* request, ::tafs::LoginReply* response);
    virtual ::grpc::Status GetAttr(::grpc::ServerContext* context, const ::tafs::GetAttrReq* request, ::tafs::GetAttrReply* response);
    virtual ::grpc::Status Open(::grpc::ServerContext* context, const ::tafs::OpenReq* request, ::tafs::OpenReply* response);
    virtual ::grpc::Status Read(::grpc::ServerContext* context, const ::tafs::ReadReq* request, ::tafs::ReadReply* response);
    virtual ::grpc::Status Write(::grpc::ServerContext* context, const ::tafs::WriteReq* request, ::tafs::WriteReply* response);
    virtual ::grpc::Status ReadDir(::grpc::ServerContext* context, const ::tafs::ReadDirReq* request, ::grpc::ServerWriter< ::tafs::ReadDirReply>* writer);
    virtual ::grpc::Status MkDir(::grpc::ServerContext* context, const ::tafs::MkDirReq* request, ::tafs::MkDirReply* response);
    virtual ::grpc::Status RmDir(::grpc::ServerContext* context, const ::tafs::RmDirReq* request, ::tafs::RmDirReply* response);
    virtual ::grpc::Status Unlink(::grpc::ServerContext* context, const ::tafs::UnlinkReq* request, ::tafs::UnlinkReply* response);
    virtual ::grpc::Status Access(::grpc::ServerContext* context, const ::tafs::AccessReq* request, ::tafs::AccessReply* response);
    virtual ::grpc::Status Flush(::grpc::ServerContext* context, const ::tafs::FlushReq* request, ::tafs::FlushReply* response);
    virtual ::grpc::Status Mknod(::grpc::ServerContext* context, const ::tafs::MknodReq* request, ::tafs::MknodReply* response);
    virtual ::grpc::Status Truncate(::grpc::ServerContext* context, const ::tafs::TruncateReq* request, ::tafs::TruncateReply* response);
    ::grpc::RpcService* service() GRPC_OVERRIDE GRPC_FINAL;
   private:
    std::unique_ptr< ::grpc::RpcService> service_;
  };
  class AsyncService GRPC_FINAL : public ::grpc::AsynchronousService {
   public:
    explicit AsyncService();
    ~AsyncService() {};
    void RequestSayHello(::grpc::ServerContext* context, ::tafs::HelloRequest* request, ::grpc::ServerAsyncResponseWriter< ::tafs::HelloReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestLogin(::grpc::ServerContext* context, ::tafs::LoginRequest* request, ::grpc::ServerAsyncResponseWriter< ::tafs::LoginReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestTestBytes(::grpc::ServerContext* context, ::tafs::TestB* request, ::grpc::ServerAsyncResponseWriter< ::tafs::LoginReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestGetAttr(::grpc::ServerContext* context, ::tafs::GetAttrReq* request, ::grpc::ServerAsyncResponseWriter< ::tafs::GetAttrReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestOpen(::grpc::ServerContext* context, ::tafs::OpenReq* request, ::grpc::ServerAsyncResponseWriter< ::tafs::OpenReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestRead(::grpc::ServerContext* context, ::tafs::ReadReq* request, ::grpc::ServerAsyncResponseWriter< ::tafs::ReadReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestWrite(::grpc::ServerContext* context, ::tafs::WriteReq* request, ::grpc::ServerAsyncResponseWriter< ::tafs::WriteReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestReadDir(::grpc::ServerContext* context, ::tafs::ReadDirReq* request, ::grpc::ServerAsyncWriter< ::tafs::ReadDirReply>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestMkDir(::grpc::ServerContext* context, ::tafs::MkDirReq* request, ::grpc::ServerAsyncResponseWriter< ::tafs::MkDirReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestRmDir(::grpc::ServerContext* context, ::tafs::RmDirReq* request, ::grpc::ServerAsyncResponseWriter< ::tafs::RmDirReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestUnlink(::grpc::ServerContext* context, ::tafs::UnlinkReq* request, ::grpc::ServerAsyncResponseWriter< ::tafs::UnlinkReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestAccess(::grpc::ServerContext* context, ::tafs::AccessReq* request, ::grpc::ServerAsyncResponseWriter< ::tafs::AccessReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestFlush(::grpc::ServerContext* context, ::tafs::FlushReq* request, ::grpc::ServerAsyncResponseWriter< ::tafs::FlushReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestMknod(::grpc::ServerContext* context, ::tafs::MknodReq* request, ::grpc::ServerAsyncResponseWriter< ::tafs::MknodReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestTruncate(::grpc::ServerContext* context, ::tafs::TruncateReq* request, ::grpc::ServerAsyncResponseWriter< ::tafs::TruncateReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
  };
};

}  // namespace tafs


#endif  // GRPC_tafs_2eproto__INCLUDED