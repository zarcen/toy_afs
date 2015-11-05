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
    - ex: a file in server path "/tmp/tafs/file" corresponds to client cache:
      - Client cache file: "/tmp/cache/file"
      - Client cache attribute: "/tmp/cache/file.attr"
      - Client cache cache_reply: "/tmp/cache/file.rele"
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
    - ex: a **cat** operation triggers: getattr() -> open() -> read() -> getattr() -> read() -> flush() -> release()
  - Read / Write a file
    0. getattr():  
      0. Check if there is any cache updates needed to write back to server; if so, then write back.
      1. Retrieve file attribute from server, and cache in memory.
    1. open():
      0. Retrieve file attribute from client disk, if there is any.
      1. Compare the attribute between server and client
        - If the same, then return.
        - Otherwise, retreive file from server and store both attribute and file to the client cache in disk.
    2. flush():
      0. make sure the cache is saved in the disk, using fsync().
      1. apply cache reply mechanism to make sure the file updates are written back to server.
        - generate a empty file to indicate the file is not write back yet, e.g. "/tmp/cache/file.rele"
    3. release():
      0. check if there is updates in file, and write back if so.
      1. update cache reply
        - e.g. remove "/tmp/cache/file.rele"

- Consistency Protocol:
  - Read / Write a file
    0. getattr():  
      * Check if there is any cache updates needed to write back to server
        - e.g. check "/tmp/cache/file.rele"
    2. flush():
      * apply cache reply mechanism to make sure the file updates are written back to server.
        - generate a empty file to indicate the file is not write back yet, e.g. "/tmp/cache/file.rele"
    3. release():
      * write back to server
      * update cache reply
        - e.g. remove "/tmp/cache/file.rele"

- Crash recover
  - Before the operation reaches flush(), i.e. save to disk, the updates to the file is in the memroy. If crash happens before flush(), we will loss the updates. Once the updates is flushed to cache in disk, and the client crashs before write back to server, we can recover the updates to server when client is back.

- Improve performance
  - attribute: put attribute in memory cache for fast access
  - Pass file descriptor instead of open a file to reduce the overhead. Will close it in flush().


- Single client: write, 1st read, 2nd read
  * ![fig.1](https://github.com/zarcen/toy_afs/blob/writeup/data/single.png)

- Scale: Single v.s. Three clients
  * ![fig.2](https://github.com/zarcen/toy_afs/blob/writeup/data/scale.png)
