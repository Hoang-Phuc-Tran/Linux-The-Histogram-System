#   FILE			: makefile
#	PROJECT			: PROG1970 - Assignment #5
#	PROGRAMMER(S)		: Caine Phung - ID: 6983324  - Hoang Phuc Tran - ID: 8789102
#	FIRST VERSION		: 2023-05-04
#	DESCRIPTION		: This makefile will compile all the source files.
#
./bin/DC: ./obj/DC.o 
	cc ./obj/DC.o   -o ./bin/DC

#Dependencies                     
./obj/DC.o : ./src/DC.c ../Common/inc/Common.h ../DC/inc/DC.h
	cc -c ./src/DC.c -o ./obj/DC.o

#Other targets
all : ./bin/DC

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	




