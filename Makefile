#
# C++ Smart Poiner
#
# Hang Yuan <yuanhang3260@gmail.com>
#
CC=g++ -std=c++11
CFLAGS=-Wall -Werror -O2
LFLAGS=-lssl -lcrypto

default: main.cpp shared_ptr.h weak_ptr.h unique_ptr.h
	$(CC) main.cpp -o a.out

clean:
	rm -rf a.out