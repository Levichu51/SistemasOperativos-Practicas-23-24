//
// Created by levixhu on 27/11/23.
//

#ifndef P3_LISTA_PROCESOS_H
#define P3_LISTA_PROCESOS_H

#define LNULL NULL
#define MAXVAR 1000
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <errno.h>
#include <mcheck.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>

typedef struct p{
    pid_t pid;
    char *usuario;
    char *estado;
    char *comando;
    char *date;   //time_t
    int estado2;
    int prioridad;
    char *senal;
    int fin;
}tItemProc;

typedef struct tNodeProc* tPosProc;

struct tNodeProc{
    tItemProc data;
    tPosProc next;
};

typedef tPosProc tListProc;

void createEmptyListProc(tListProc *L);
bool insertItemProc(tItemProc d, tListProc *L);
tPosProc firstProc(tListProc L);
tPosProc nextProc(tPosProc p, tListProc L);
tPosProc lastProc(tListProc L);
tPosProc previousProc(tPosProc p, tListProc L);
bool isEmptyListProc(tListProc L );
tItemProc getItemProc(tPosProc p, tListProc L);
tPosProc findItemProc(tItemProc d, tListProc L);
void updateItemProc(tItemProc d, tPosProc p, tListProc* L);
void deleteListProc(tListProc *L);
bool createNodeProc(tPosProc *p);
void printListProc(tListProc L);
void deleteAtPositionProc(tPosProc p, tListProc *L);



#endif //P3_LISTA_PROCESOS_H
