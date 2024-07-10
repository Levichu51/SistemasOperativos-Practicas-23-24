//
// Created by levixhu on 27/11/23.
//

#ifndef P3_P2_FUNCTIONS_H
#define P3_P2_FUNCTIONS_H

#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <errno.h>
#include <mcheck.h>
#include "lista_mem.h"


///Recurse
void Recursiva(int n);
void CmdRecursiva(char *tr[]);
///Shared
void *ObtenerMemoriaShmget(key_t clave, size_t tam, tListMem *L);
void SharedCreate(char *tr[], tListMem *L);
int SharedDelkey(char *args[]);
///Mmap
void *MapearFichero(char *fichero, int protection);
void CmdMmap(char *arg[], tListMem *L);
///R & W
void *cadtop(char *str);
ssize_t EscribirFichero(char *f, void *p, size_t cont, int overwrite);
ssize_t LeerFichero(char *f, void *p, size_t cont);
void CmdRead(char *ar[]);
void CmdWrite(char *tr[]);
void Do_MemPmap(void);
///Malloc
void newMalloc(char *tr[], tListMem *L);
///Mem
void memProcess(char *tr[], tListMem M);
///Memfill
void LlenarMemoria(void *p, size_t cont, unsigned char byte);
void newMemFill(char *tr[]);
///MemDump
void MemDump(void *address, size_t numBytes);
void newMemDump(char *tr[]);



#endif //P3_P2_FUNCTIONS_H
