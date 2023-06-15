#   FILE			: makefile
#	PROJECT			: PROG1970 - Assignment #5
#	PROGRAMMER(S)	: Caine Phung - ID: 6983324
#                     Hoang Phuc Tran - ID: 8789102
#	FIRST VERSION	: 2023-05-04
#	DESCRIPTION		: This makefile will compile all the source files for chat-server.
#
./bin/DP-1: ./obj/DP-1.o 
	cc ./obj/DP-1.o   -o ./bin/DP-1

#Dependencies                     
./obj/DP-1.o : ./src/DP-1.c ../Common/inc/Common.h ../DP-1/inc/DP-1.h
	cc -c ./src/DP-1.c -o ./obj/DP-1.o

#Other targets
all : ./bin/DP-1

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	



