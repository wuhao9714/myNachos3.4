void func(){}

#include "syscall.h"

char buffer[17];
OpenFileId fd;
int count;

int
main(){
	count=17;
	Create("file.txt");
	fd=Open("file.txt");
	Write("I love nachos3.4",count,fd);
	Close(fd);
	fd=Open("file.txt");
	count=Read(buffer,count,fd);
	Write(buffer,count,ConsoleOutput);
}
