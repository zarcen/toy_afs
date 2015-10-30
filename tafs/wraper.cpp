#include "tafs2.h"

extern "C" {
    int CRead(char* path, char** buf, int* size) {
        GreeterClient greeter(
                grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()));
        std::string str = path;
        std::string strbuf;
        greeter.Read(str, strbuf);
        *size = strbuf.size();
        *buf = new char[*size];
        memcpy(*buf, &strbuf[0], *size);
    }
}
