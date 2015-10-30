# toy_afs
A toy filesystem which is AFS-like

## Todo
- (Done) build grpc (https://github.com/grpc/grpc)
- (Done) build protobuf (https://github.com/google/protobuf/releases/download/v3.0.0-alpha-1/protobuf-cpp-3.0.0-alpha-1.tar.gz)
- (Done) build Fuse (http://fuse.sourceforge.net)
- rewrite the sync_files.sh
- implement the user-level filesystem
- implement the local update protocol for crash consistency


## Func
- int open(const char *path, int oflags);
- int  read(  int  handle,  void  *buffer,  int  nbyte );
- int  write(  int  handle,  void  *buffer,  int  nbyte  );

## RPC Calls
- int tafs_read(const char *path, char *buf, size_t size)
- int tafs_open(const char *path, int flag)
- int tafs_getattr(const char *path, struct stat *stbuf)

## Important References:
- How to initialize a struct using the c style while using the g++ compiler?
  
  This is to solve the `static struct fuse_operations` problem, which cause a error message of 'non-trivial designated initializers not supported' when compiled with g++.

  - http://stackoverflow.com/questions/12122234/how-to-initialize-a-struct-using-the-c-style-while-using-the-g-compiler
  - http://stackoverflow.com/questions/16622434/clarification-for-implementing-a-fuse-filesystem-in-c

- Tutorial: HowTo integrate a C++ library/class into a C program

  How to use extern "C" {} to integrate c++ codes into c

  http://www.teddy.ch/c++_library_in_c/

- An IBM example of using FUSE+RPC to realize a AFS

  https://www.ibm.com/developerworks/library/l-fuse/
