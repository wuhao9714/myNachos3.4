#include "syscall.h"

int f(){
	Exit(0);
}

int main(){
	Fork(f);
	Yield();
	Exit(Join(Exec("./test/filesc")));
}
