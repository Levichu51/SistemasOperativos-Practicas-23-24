//
// Created by levixhu on 27/11/23.
//

#ifndef P3_P3_FUNCTIONS_H
#define P3_P3_FUNCTIONS_H

#include "lista_procesos.h"

void Cmd_fork (char *tr[], tListProc* L);
void cmd_showvar(char *tr[], char *environ[], char *arg3[]);
void Cmd_Uid(char *tr[], tListProc *L);
void Cmd_showEnviron(char *tr[], char *arg3[], char *environ[]);
void Cmd_changevar(char *tr[], char *arg3[], char *environ[]);
void Cmd_subsVar(char *tr[], char *arg3[], char *environ[]);
void Cmd_delJobs(char *tr[], tListProc *P);
void Cmd_Jobs(char *tr[], tListProc *P);
void Cmd_job(char *tr[], tListProc *P);
void Cmd_Exec(char *tr[], tListProc *P);
void externCommand(char *trozos[], char *comando, tListProc *list_proc, char *environ[]);
void insert(char *tr[], tListProc *P);
void execute(char *command, char *tr[], tListProc *P);


#endif //P3_P3_FUNCTIONS_H
