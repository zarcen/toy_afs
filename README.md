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
