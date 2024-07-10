//
// Created by levixhu on 27/11/23.
//

#ifndef P3_LISTA_MEM_H
#define P3_LISTA_MEM_H


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>


#define LNULL NULL

typedef struct m{
    char *date;
    key_t key;
    size_t size;
    char name[30];
    char type[10];
    int descriptor;
    void *dir;
}tItemMem;

typedef struct tNodeMem* tPosM;

struct tNodeMem{
    tItemMem data;
    tPosM next;
};

typedef tPosM tListMem;

void createEmptyListMem(tListMem* L);
bool insertItemMem(tItemMem d, tListMem* L);
tPosM firstMem(tListMem L);
tPosM nextMem(tPosM p, tListMem L);
tPosM lastMem(tListMem L);
tPosM previousMem (tPosM p, tListMem L);
bool isEmptyListMem(tListMem L );
tItemMem getItemMem (tPosM p, tListMem L);
tPosM findItemMem (tItemMem d, tListMem L);
void updateItemMem(tItemMem d, tPosM p, tListMem * L);
void deleteListMem(tListMem * L);
bool createNodeMem(tPosM * p);
void printListMem(tListMem L);
void printListMalloc(tListMem L);
void printListShared(tListMem L);
void printListMmap(tListMem L);
void deleteAtPositionMem(tPosM p, tListMem* L);




#endif //P3_LISTA_MEM_H
