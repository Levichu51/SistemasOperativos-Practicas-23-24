//
// Created by levixhu on 27/11/23.
//

#ifndef P3_P1_H
#define P3_P1_H

#include "lista_hist.h"
#include "lista_mem.h"
#include "lista_procesos.h"

///Funciones P0
void processCommand(tList *L, char input[], bool *loop, tListMem *M, tListProc *P, char *environ[], char *envP[]);
int TrocearCadena(char *cadena, char *trozos[]);
void authors(char *str);
void pid(char *str);
void chDir(char *str);
void date(char *str);
void currentTime(char *str);
void hist(char *str, tList *L);
void infosys(char *str);
void help(char *str);
void bye(bool *loop);
void commandN(char *str, tList *L, bool *loop, tListMem *M, tListProc *P);


///Prototipos de funciones relacionadas con archivos y directorios (P1)
void create(char *tr[]);
void listStat(char *tr[]);
void auxStat(char *file, int longFormat, int showAccessTime, int showLinkPath);
void listDir(char *tr[]);
void auxList(char *file, int longFormat, int showAccessTime, int showLinkPath, int showHid, int showReca, int showRecb);
void delete(char *tr[]);
void deltree(char *path);


///Prototipos de funciones relacionadas con archivos (P0)
void ListOpenFiles(int descriptor);
void AddToFileList(int descriptor, const char *nombre, const char *modo);
void Cmd_open(char *tr[]);
void Cmd_close(char *tr[]);
void Cmd_dup(char *tr[]);
char *NombreFicheroDescriptor(int descriptor);
char *ModoFicheroDescriptor(int descriptor);


#endif //P3_P1_H
