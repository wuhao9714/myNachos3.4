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
			printf("halt\n");
		   	interrupt->Halt();
			break;
		}
		case SC_Create: 
		{
			int addr = registers[4];
			int pos = 0;
			char name[10];
			while(1) {
				int data;
				machine->ReadMem(addr+pos, 1, &data);
				name[pos++] = data;
				if (data == 0)
					break;
			}
			fileSystem->Create(name, 256);
			registers[PrevPCReg] = registers[PCReg];
			registers[PCReg] = registers[NextPCReg];
			registers[NextPCReg] = registers[NextPCReg] + 4;
			break;
		}
		case SC_Open: 
		{
			int addr = registers[4];
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
			registers[2] = (int)openfile;
			registers[PrevPCReg] = registers[PCReg];
			registers[PCReg] = registers[NextPCReg];
			registers[NextPCReg] = registers[NextPCReg] + 4;
			break;
		}
		case SC_Close: 
		{
			OpenFile* openFile = (OpenFile*)registers[4];
			delete openFile;
			registers[PrevPCReg] = registers[PCReg];
			registers[PCReg] = registers[NextPCReg];
			registers[NextPCReg] = registers[NextPCReg] + 4;
			break;
		}
		case SC_Write: 
		{
			int pos = registers[4];
			int size = registers[5];
			OpenFile* openFile = (OpenFile*)registers[6];
			char buffer[size];
			int data;
			for(int i = 0; i < size; i++) {
				machine->ReadMem(pos+i, 1, &data);
				buffer[i] = data;
			}
			if (registers[6] == 1)
				printf("%s\n", buffer);
			else
				openFile->Write(buffer, size);
			registers[PrevPCReg] = registers[PCReg];
			registers[PCReg] = registers[NextPCReg];
			registers[NextPCReg] = registers[NextPCReg] + 4;	
			break;
		}
		case SC_Read:
		{
			int pos = registers[4];
			int size = registers[5];
			OpenFile* openFile = (OpenFile*)registers[6];
			char buffer[size];
			int rst = openFile->Read(buffer, size);
			for(int i = 0; i < rst; i++)
				machine->WriteMem(pos+i, 1, (int)buffer[i]);
			registers[2] = rst;
			registers[PrevPCReg] = registers[PCReg];
			registers[PCReg] = registers[NextPCReg];
			registers[NextPCReg] = registers[NextPCReg] + 4;
			break;
		}
		case SC_Exec:
		{
			// int add = registers[4];
			// char name[20];
			// int pos = 0;
			// while(1) {
			// 	int data;
			// 	machine->ReadMem(add+pos, 1, &data);
			// 	name[pos++] = data;
			// 	if (data == 0) break;
			// }
			// Thread* t = new Thread("Exec thread");
			// OpenFile* executable = fileSystem->Open(name);
		 //    if (executable == NULL) {
			// 	printf("Unable to open file %s\n", name);
			// 	return;
		 //    }
			// AddrSpace* space = new AddrSpace(executable);
 		// 	t->space = space;
		 //    delete executable;			// close file
   //  		space->InitRegisters();		// set the initial register values
   //  		space->RestoreState();		// load page table register
			// t->Fork(exec_func, 0);
			// registers[2] = (int)space;
			// registers[PrevPCReg] = registers[PCReg];
			// registers[PCReg] = registers[NextPCReg];
			// registers[NextPCReg] = registers[NextPCReg] + 4;
			printf("exec\n");
			 break;
		}
		case SC_Fork:
		{
			break;
		}
		case SC_Yield:
		{
			break;
		}
		case SC_Join:
		{
			break;
		}
		case SC_Exit: 
		{
			printf("sys-exit\n");
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
		printf("pos=%d\n", pos);
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
