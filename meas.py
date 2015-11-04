#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, sys, argparse, time
import subprocess
import errno

cache_prefix = "/tmp/cache/"
min_mb = 1   # the min filesize in Mb
max_mb = 75  # the max filesize in Mb

def createfiles(fs_prefix):
    print('Start creating files in mount point "%s" from %d kB to %d kB' % 
            (fs_prefix, min_mb, max_mb))
    if fs_prefix[-1] != '/':
        fs_prefix += '/'
    for i in range(min_mb, max_mb + 1):
        while True:
            try:
                with open(fs_prefix + "f" + str(i), 'wb') as f:
                    f.seek(i*1024*1024 - 1)
                    f.write('\0')
                    f.close()
            except:
                print 'failed to create %s, try again' % (fs_prefix+"f"+str(i))
                continue
            break

def readtest(fs_prefix):
    print('Doing the read test')
    cleancache()
    if fs_prefix[-1] != '/':
        fs_prefix += '/'
    first_read_map = []
    sub_read_map = []
    for i in range(min_mb, max_mb + 1):
        start_time = time.time()
        with open(fs_prefix + "f" + str(i), 'rb') as f:
            f.read()
            f.close()
        first_read_map.append((i, time.time() - start_time))
        time.sleep(1)   # add a little delay to avoid rpc call failing
        start_time = time.time()
        with open(fs_prefix + "f" + str(i), 'rb') as f:
            f.read()
            f.close()
        sub_read_map.append((i, time.time() - start_time))
        time.sleep(1)   # add a little delay to avoid rpc call failing
    for i in range(len(first_read_map)):
        print "%d,%f,%f" % (first_read_map[i][0], first_read_map[i][1], sub_read_map[i][1]) 


def writetest(fs_prefix):
    print('Doing the write test')
    cleancache()
    if fs_prefix[-1] != '/':
        fs_prefix += '/'
    first_write_map = []
    sub_write_map = []
    towrite = 'x'
    for i in range(min_mb, max_mb + 1):
        start_time = time.time()
        nbytes = towrite * i * 1024 * 1024
        with open(fs_prefix + "f" + str(i), 'wb') as f:
            f.write(nbytes)
            f.close()
        first_write_map.append((i, time.time() - start_time))
        time.sleep(1)   # add a little delay to avoid rpc call failing
        start_time = time.time()
        with open(fs_prefix + "f" + str(i), 'wb') as f:
            f.write(nbytes)
            f.close()
        sub_write_map.append((i, time.time() - start_time))
        time.sleep(1)   # add a little delay to avoid rpc call failing
    for i in range(len(first_write_map)):
        print "%d,%f,%f" % (first_write_map[i][0], first_write_map[i][1], sub_write_map[i][1]) 

def cleancache():
    print('Cleaning the cache...')
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
            help="Test options ['read', 'write', 'create']", default=None)
    parser.add_argument("-f", "--fs_prefix", 
            help="The mount point of fuse filesystem. Default=\"/tmp/afs\"",
            default='/tmp/afs')
    args = parser.parse_args()

    fs_prefix = args.fs_prefix
    if args.options is not None:
        # start test
        if args.options == 'read':
            readtest(fs_prefix)
        elif args.options == 'write':
            writetest(fs_prefix)
        elif args.options == 'create':
            createfiles(fs_prefix)
        else:
            parser.print_help()
    else:
        parser.print_help()


if __name__ == '__main__':
    main()
