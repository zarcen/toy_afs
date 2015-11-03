# toy_afs
A toy version of AFS-like user-space filesystem implemented by [FUSE](http://fuse.sourceforge.net/) and [gRPC](https://github.com/grpc/grpc) (Google RPC)


## Get Started

### Env Requirement
- Build grpc (https://github.com/grpc/grpc)
- Build protobuf (https://github.com/google/protobuf/releases)
- Build Fuse (http://fuse.sourceforge.net)
- Install `pkg-config` via `sudo apt-get install pkg-config` (needed when compiling fuse programs)

### Build the server and client side programs
1. `cd toy_afs/tafs`
2. `make`
3. start server on the machine you like `./tafs_server`
4. edit the file `toy_afs/tafs/server` to specify the host:port of the server you ran
5. start client-side program, which is integrated into Fuse, and set mount point you like. e.g., `./fusetafs /tmp/afs/ -d`
   - verbose mode(for debug/demo): `./fusetafs /tmp/afs/ -d`
   - silent mode: `./fusetafs /tmp/afs/ -d`


## Important References:
- fuse_operations Struct Reference
  - http://fuse.sourceforge.net/doxygen/structfuse__operations.html

- Protocol Buffer Language Guide (proto3)
  - https://developers.google.com/protocol-buffers/docs/proto3?hl=en
  
- How to initialize a struct using the c style while using the g++ compiler? 

  This is to solve the `static struct fuse_operations` problem, which cause a error message of 'non-trivial designated initializers not supported' when compiled with g++.

  - http://stackoverflow.com/questions/12122234/how-to-initialize-a-struct-using-the-c-style-while-using-the-g-compiler
  - http://stackoverflow.com/questions/16622434/clarification-for-implementing-a-fuse-filesystem-in-c

- Tutorial: HowTo integrate a C++ library/class into a C program

  How to use extern "C" {} to integrate c++ codes into c
  - http://www.teddy.ch/c++_library_in_c/

- An IBM example of using FUSE+RPC to realize a AFS
  - https://www.ibm.com/developerworks/library/l-fuse/


## Known Bugs
- When calling gRPC func too fast, transferred data would sometimes be corrupt
