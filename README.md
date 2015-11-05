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
3. start server on the machine you like `./tafs_server`, your filesystem real path is `/tmp/tafs`
4. edit the file `toy_afs/tafs/server` to specify the host:port of the server you ran
5. start client-side program, which is integrated into Fuse, and set mount point you like. e.g., `/tmp/afs`
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


## Writeup

- AFS-like protocol
  - Implementation: grpc + FUSE
  - Cache data types: file, attribute, cache_reply
    - File and cache_replay are stored in disk.
    - Attribute is stored in both memory and disk.
    - ex: a file in server path "/tmp/server/file1" corresponds to client cache:
      - Client cache file: "/tmp/cache/file1"
      - Client cache attribute: "/tmp/cache/file1.attr"
      - Client cache cache_reply: "/tmp/cache/file1.rele"
  - Support multiple clients consistency
    - Last writer wins
  - Support Crash recover
    - Once the file update is flushed, the client system will syncronize with server after reboot.
    - If crash happened before flush(), then the updates in file will disappear. 

- File system functionality support:
  - getattr(), access(), readdir(), mknod(), mkdir(), unlink(), rmdir(), rename(), truncate(), open(), read(), write(), flush(), release().
  - Each function corresponds a specific funciton in FUSE.
  - Most funcitons in FUSE have corresponding grpc fucnctions.
    - read() and rddir() use grpc streaming type, otherwise the data would be corrupted. 

- Cache Protocol:
  - A system call (read/write) usually involves serveral operations, so we properly modified FUSE functions to implement cache feature.
    - ex: a cat operation triggers: getattr() -> open() -> read() -> getattr() -> read() -> flush() -> release()
  - Read a file
    0. getattr():  
      - Check if there is any cache updates needed to write back to server; if so, then write back.
      - Retrieve file attribute from server, and cache in memory.
    1. open():
      - Retrieve file attribute from client disk, if there is any.
      - Compare the attribute between server and client
        - If the same, then return.
        - Otherwise, retreive file from server and store both attribute and file to the client cache in disk.
    2. flush():
      - make sure the cache is saved in the disk, using fsync().
  - Write a file

- Mechanism to implement protocol (Key funcitons) 
  - FUSE
    - getattr() : check if anything needs to write back to server .
    - open() : compare attribute between server and local. If attributes are not the same, then fetch from server and save in the cache.
    - flush() : make sure the cache is saved in the disk, using fsync().
    - release() : write back to server if file is updated. 

- Crash recover
  - Before the operation reaches flush(), i.e. save to disk, the updates to the file is in the memroy. If crash happens before flush(), we will loss the updates. Once the updates is flushed to cache in disk, and the client crashs before write back to server, we can recover the updates to server when client is back.
  - Mechanism:

- Improve performance
  - attribute: put attribute in memory cache for fast access




