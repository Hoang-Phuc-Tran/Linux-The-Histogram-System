#   FILE			: makefile
#	PROJECT			: PROG1970 - Assignment #5
#	PROGRAMMER(S)	: Caine Phung - ID: 6983324
#                     Hoang Phuc Tran - ID: 8789102
#	FIRST VERSION	: 2023-05-04
#	DESCRIPTION		: This makefile will compile all the source files for chat-server.
#
./bin/DP-2: ./obj/DP-2.o 
	cc ./obj/DP-2.o   -o ./bin/DP-2

#Dependencies                     
./obj/DP-2.o : ./src/DP-2.c ../Common/inc/Common.h ../DP-2/inc/DP-2.h
	cc -c ./src/DP-2.c -o ./obj/DP-2.o

#Other targets
all : ./bin/DP-2

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	



