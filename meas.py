#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, sys, argparse, time
import subprocess
import errno

def readonly(fs_prefix, min_mb, max_mb):
    # first read access & sub read access
    cleancache()
    first_read_map = []
    sub_read_map = []
    for i in range(min_mb, max_mb + 1):
        start_time = time.time()
        with open(fs_prefix + "f" + str(i), 'rb') as f:
            f.read()
            f.close()
        first_read_map.append((i, time.time() - start_time))
        time.sleep(2)   # add a little delay to avoid rpc call failing
        start_time = time.time()
        with open(fs_prefix + "f" + str(i), 'rb') as f:
            f.read()
            f.close()
        sub_read_map.append((i, time.time() - start_time))
        time.sleep(2)   # add a little delay to avoid rpc call failing
    for i in range(len(first_read_map)):
        print "%d,%f,%f" % (first_read_map[i][0], first_read_map[i][1], sub_read_map[i][1]) 

def writeonly(fs_prefix, min_mb, max_mb):
    # create & write
    cleancache()
    if fs_prefix[-1] != '/':
        fs_prefix += '/'
    first_write_map = []
    towrite = 'x'
    for i in range(min_mb, max_mb + 1):
        nbytes = towrite * i * 1024 * 1024
        filepath = fs_prefix + "f" + str(i)
        cnt = 0
        while True:
            start_time = time.time()
            with open(filepath, 'wb+') as f:
                f.write(nbytes)
                f.close()
            first_write_map.append((i, time.time() - start_time))
            filesize = os.path.getsize(filepath)
            if filesize == len(nbytes):
                break
            else:
                del first_write_map[-1]
        time.sleep(2)   # add a little delay to avoid rpc call failing
    for i in range(len(first_write_map)):
        print "%d,%f" % (first_write_map[i][0], first_write_map[i][1])

def readwrite(fs_prefix, min_mb, max_mb):
    # create & write
    cleancache()
    if fs_prefix[-1] != '/':
        fs_prefix += '/'
    first_write_map = []
    towrite = 'x'
    for i in range(min_mb, max_mb + 1):
        nbytes = towrite * i * 1024 * 1024
        filepath = fs_prefix + "f" + str(i)
        cnt = 0
        while True:
            start_time = time.time()
            with open(filepath, 'wb+') as f:
                f.write(nbytes)
                f.close()
            first_write_map.append((i, time.time() - start_time))
            filesize = os.path.getsize(filepath)
            if filesize == len(nbytes):
                break
            else:
                del first_write_map[-1]
        time.sleep(2)   # add a little delay to avoid rpc call failing
    
    # first read access & sub read access
    cleancache()
    first_read_map = []
    sub_read_map = []
    for i in range(min_mb, max_mb + 1):
        start_time = time.time()
        with open(fs_prefix + "f" + str(i), 'rb') as f:
            f.read()
            f.close()
        first_read_map.append((i, time.time() - start_time))
        time.sleep(2)   # add a little delay to avoid rpc call failing
        start_time = time.time()
        with open(fs_prefix + "f" + str(i), 'rb') as f:
            f.read()
            f.close()
        sub_read_map.append((i, time.time() - start_time))
        time.sleep(2)   # add a little delay to avoid rpc call failing
    for i in range(len(first_read_map)):
        print "%d,%f,%f,%f" % (first_write_map[i][0], first_write_map[i][1], first_read_map[i][1], sub_read_map[i][1]) 

def cleancache():
    cache_prefix = "/tmp/cache/"
    for the_file in os.listdir(cache_prefix):
        file_path = os.path.join(cache_prefix, the_file)
        try:
            if os.path.isfile(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                shutil.rmtree(file_path)
        except Exception, e:
            print e

def main():
    parser = argparse.ArgumentParser(description="""
        Use this tool to do the measurements for cs739-p2
        """)
    parser.add_argument("-o", "--options", 
            help="Test options ['rdonly', 'wronly', 'readwrite']", default='readwrite')
    parser.add_argument("-f", "--fs_prefix", 
            help="The mount point of fuse filesystem. Default=\"/tmp/afs\"",
            default='/tmp/afs')
    parser.add_argument("-m", "--minsize", help="The min size(Mb) to run testing (default: 1)",
            default=1)
    parser.add_argument("-M", "--maxsize", help="The max size(Mb) to run testing (default: 50)",
            default=50)
    args = parser.parse_args()

    min_mb = args.minsize   # the min filesize in Mb
    max_mb = args.maxsize   # the max filesize in Mb

    fs_prefix = args.fs_prefix
    if args.options is not None:
        # start test
        if args.options == 'readwrite':
            readwrite(fs_prefix, min_mb, max_mb)
        elif args.options == 'rdonly':
            readonly(fs_prefix, min_mb, max_mb)
        elif args.options == 'wronly':
            writeonly(fs_prefix, min_mb, max_mb)
        else:
            parser.print_help()
    else:
        parser.print_help()


if __name__ == '__main__':
    main()
