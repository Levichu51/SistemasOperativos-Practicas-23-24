//
// Created by levixhu on 27/11/23.
//

#ifndef P3_LISTA_HIST_H
#define P3_LISTA_HIST_H

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
#define MAX 4096


typedef struct r{
    char command[MAX];
}tItem;

typedef struct tNode* tPosL;

struct tNode{
    tItem data;
    tPosL next;
};

typedef tPosL tList;

void createEmptyList(tList* L);
bool insertItem(tItem d, tList* L);
tPosL first(tList L);
tPosL next(tPosL p, tList L);
tPosL last(tList L);
tPosL previous (tPosL p, tList L);
bool isEmptyList(tList L );
tItem getItem (tPosL p, tList L);
tPosL findItem (tItem d, tList L);
void updateItem(tItem d, tPosL p, tList* L);
void deleteList(tList* L);
bool createNode(tPosL* p);
void printList(tList L);
void deleteAtPosition(tPosL p, tList* L);



#endif //P3_LISTA_HIST_H
