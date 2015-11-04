#!/usr/bin/bash
protoc -I ../protos --cpp_out=. $1
