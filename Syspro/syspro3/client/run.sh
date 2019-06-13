#!/bin/bash
make clean
make
PORT=9003
PORT+=$1
./dropbox_client -d files -p $PORT -w 5 -b 100 -sp 9002 -sip 192.168.1.19
