// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------
void exec_func(int which){
    currentThread->space->InitRegisters();		// set the initial register values
    currentThread->space->RestoreState();
    printf("%s run user program\n",currentThread->getName());
    machine->Run();
    ASSERT(FALSE);
}
void fork_func(void (*func)){
	currentThread->space->InitRegisters();		// set the initial register values
    currentThread->space->RestoreState();
    machine->WriteRegister(PCReg,func);
    machine->WriteRegister(NextPCReg,func+4);
    printf("%s run user program\n",currentThread->getName());
    machine->Run();
    ASSERT(FALSE);
}
void
ExceptionHandler(ExceptionType which)
{
    int *registers = machine->registers;
	switch(which) {
	case SyscallException: {
    	int	type;
		type = machine->ReadRegister(2);
		DEBUG('a', "type = %d\n", type);
		switch(type) {
		case SC_Halt:
		{
			DEBUG('a', "Shutdown, initiated by user program.\n");
			printf("%s sys-halt\n",currentThread->getName());
		   	interrupt->Halt();
			break;
		}
		case SC_Create: 
		{
			printf("%s sys-create\n",currentThread->getName());
			int addr = machine->ReadRegister(4);
			int pos = 0;
			char name[10];
			while(1) {
				int data;
				machine->ReadMem(addr+pos, 1, &data);
				name[pos++] = data;
				if (data == 0)
					break;
			}
			fileSystem->Create(name, 128);
			machine->PCadvance();
			break;
		}
		case SC_Open: 
		{
			printf("%s sys-open\n",currentThread->getName());
			int addr = machine->ReadRegister(4);
			int pos = 0;
			char name[10];
			while(1) {
				int data;
				machine->ReadMem(addr+pos, 1, &data);
				name[pos++] = data;
				if (data == 0)
					break;
			}
			OpenFile* openfile = fileSystem->Open(name);
			machine->WriteRegister(2,int(openfile));
			machine->PCadvance();
			break;
		}
		case SC_Close: 
		{
			printf("%s sys-close\n",currentThread->getName());
			OpenFile* openFile = (OpenFile*)machine->ReadRegister(4);
			delete openFile;
			machine->PCadvance();
			break;
		}
		case SC_Write: 
		{
			printf("%s sys-write\n",currentThread->getName());
			int pos = machine->ReadRegister(4);
			int size = machine->ReadRegister(5);
			OpenFile* openFile = (OpenFile*)machine->ReadRegister(6);
			char buffer[size];
			int data;
			for(int i = 0; i < size; i++) {
				machine->ReadMem(pos+i, 1, &data);
				buffer[i] = data;
			}
			if (machine->ReadRegister(6) == ConsoleOutput)
				printf("%s\n", buffer);
			else
				openFile->Write(buffer, size);
			machine->PCadvance();	
			break;
		}
		case SC_Read:
		{
			printf("%s sys-read\n",currentThread->getName());
			int pos = machine->ReadRegister(4);
			int size = machine->ReadRegister(5);
			OpenFile* openFile = (OpenFile*)machine->ReadRegister(6);
			char buffer[size];
			int byte = openFile->Read(buffer, size);
			for(int i = 0; i < byte; i++)
				machine->WriteMem(pos+i, 1, (int)buffer[i]);
			machine->WriteRegister(2,byte);
			machine->PCadvance();
			break;
		}
		case SC_Exec:
		{
			printf("%s sys-exec\n",currentThread->getName());
			int addr = machine->ReadRegister(4);
			char name[20];
			int pos = 0;
			int data;
			while(1) {
				machine->ReadMem(addr+pos, 1, &data);
				name[pos++] = data;
				if (data == 0) break;
			}
			Thread* t = new Thread("exec-thread");
			AddrSpace* space = new AddrSpace(name);
 			t->space = space;
			t->Fork(exec_func, 0);
			machine->WriteRegister(2,t->getThreadID());
			machine->PCadvance();
			 break;
		}
		case SC_Fork:
		{
			printf("%s sys-fork\n",currentThread->getName());
			int func = machine->ReadRegister(4);
			AddrSpace *space=new AddrSpace(currentThread->space->programName);
			Thread *t=new Thread("fork-thread");
			t->space=space;
			t->Fork(fork_func,func);
			machine->PCadvance();
			break;
		}
		case SC_Yield:
		{
			printf("%s sys-yield\n",currentThread->getName());
			machine->PCadvance();
			currentThread->Yield();
			break;
		}
		case SC_Join:
		{
			printf("%s sys-join\n",currentThread->getName());
			int tid=machine->ReadRegister(4);
			while(threadIDs[tid]){
				currentThread->Yield();
			}
			machine->WriteRegister(2,threadIDs[tid]);
			machine->PCadvance();
			break;
		}
		case SC_Exit: 
		{
			printf("%s sys-exit\n",currentThread->getName());
			currentThread->Finish();
			break;
		}
		default:
		{
			break;
		}
		}
		break;
	}
	case PageFaultException: {
		int vpn =(unsigned) machine->ReadRegister(BadVAddrReg) / PageSize;
		int num=0;
		int pos;
		for(int i=0;i<machine->pageTableSize;i++){
			if(machine->pageTable[i].valid==TRUE)
				num++;
		}
		if(num>=MaxNumPhysPagesPerThread)
			pos=-1;
		else
			pos=machine->RequestPage();
		// printf("pos=%d\n", pos);
		if(num==0&&pos==-1){
			printf("there are no AddrSpace to allocate for pending threads!\n\n\n");
			currentThread->Finish();
		}
		if(pos==-1){
			int min;
			for(int i=0;i<machine->pageTableSize;i++){
				if(machine->pageTable[i].valid==TRUE){
					pos=i;
					min=machine->pageTable[i].time;
					break;
				}
			}
			for(int i=pos+1;i<machine->pageTableSize;i++){
				if(machine->pageTable[i].valid==TRUE&&machine->pageTable[i].time<min){
					pos=i;
					min=machine->pageTable[i].time;
				}
			}
			if(machine->pageTable[pos].dirty==TRUE){
				currentThread->space->getSwapSpace()->WriteAt(&(machine->mainMemory[machine->pageTable[pos].physicalPage*PageSize]),
				PageSize,pos*PageSize);
			}
			machine->pageTable[pos].valid=FALSE;
			pos=machine->pageTable[pos].physicalPage;
		}
		currentThread->space->getSwapSpace()->ReadAt(&(machine->mainMemory[pos*PageSize]),PageSize,vpn*PageSize);
		machine->pageTable[vpn].physicalPage = pos;
		machine->pageTable[vpn].valid = TRUE;
		machine->pageTable[vpn].use = FALSE;
		machine->pageTable[vpn].dirty = FALSE;
		machine->pageTable[vpn].readOnly = FALSE; 
		machine->pageTable[vpn].time=stats->userTicks;
		break;
	}
	default: {
		printf("Unexpected user mode exception %d\n", which);
		ASSERT(FALSE);
	}
	}
}
