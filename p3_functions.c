//
// Created by levixhu on 27/11/23.
//

#include "p3_functions.h"
#include "p1.h"
#define MAX_ARGS 256



void Cmd_fork (char *tr[], tListProc* L){
    pid_t pid;

    if(tr[1] == NULL) {
        if ((pid = fork()) == 0) {
            /*VaciarListaProcesos(&LP); Depende de la implementación de cada uno*/
            deleteListProc(L);
            printf("Ejecutando proceso %d\n", getpid());
        }else if (pid != -1) {
            waitpid(pid, NULL, 0);
        }else{
            perror("Cannot fork");
        }
    }else{
        perror("Fork does not need any arguments");
    }
}

int BuscarVariable (char * var, char *e[])  /*busca una variable en el entorno que se le pasa como parámetro*/
{
    int pos=0;
    char aux[MAXVAR];

    strcpy (aux,var);
    strcat (aux,"=");

    while (e[pos]!=NULL)
        if (!strncmp(e[pos],aux,strlen(aux)))
            return (pos);
        else
            pos++;
    errno=ENOENT;   /*no hay tal variable*/
    return(-1);
}


int CambiarVariable(char * var, char * valor, char *e[]) /*cambia una variable en el entorno que se le pasa como parámetro*/
{                                                        /*lo hace directamente, no usa putenv*/
    int pos;
    char *aux;

    if ((pos=BuscarVariable(var,e))==-1)
        return(-1);

    if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
        return -1;
    strcpy(aux,var);
    strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    strcpy(e[pos], aux);
    free(aux);
    return (pos);
}


/*las siguientes funciones nos permiten obtener el nombre de una senal a partir
del número y viceversa */


struct SEN{
    char* nombre;
    int senal;
};
static struct SEN sigstrnum[]={
        {"HUP", SIGHUP},
        {"INT", SIGINT},
        {"QUIT", SIGQUIT},
        {"ILL", SIGILL},
        {"TRAP", SIGTRAP},
        {"ABRT", SIGABRT},
        {"IOT", SIGIOT},
        {"BUS", SIGBUS},
        {"FPE", SIGFPE},
        {"KILL", SIGKILL},
        {"USR1", SIGUSR1},
        {"SEGV", SIGSEGV},
        {"USR2", SIGUSR2},
        {"PIPE", SIGPIPE},
        {"ALRM", SIGALRM},
        {"TERM", SIGTERM},
        {"CHLD", SIGCHLD},
        {"CONT", SIGCONT},
        {"STOP", SIGSTOP},
        {"TSTP", SIGTSTP},
        {"TTIN", SIGTTIN},
        {"TTOU", SIGTTOU},
        {"URG", SIGURG},
        {"XCPU", SIGXCPU},
        {"XFSZ", SIGXFSZ},
        {"VTALRM", SIGVTALRM},
        {"PROF", SIGPROF},
        {"WINCH", SIGWINCH},
        {"IO", SIGIO},
        {"SYS", SIGSYS},
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
        {"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
        {"PWR", SIGPWR},
#endif
#ifdef SIGEMT
        {"EMT", SIGEMT},
#endif
#ifdef SIGINFO
        {"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
        {"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
        {"CLD", SIGCLD},
#endif
#ifdef SIGLOST
        {"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
        {"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
        {"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
        {"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
        {"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
        {"WAITING", SIGWAITING},
#endif
        {NULL,-1},
};    /*fin array sigstrnum */


int ValorSenal(char * sen)  /*devuelve el numero de senial a partir del nombre*/
{
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (!strcmp(sen, sigstrnum[i].nombre))
            return sigstrnum[i].senal;
    return -1;
}


char *NombreSenal(int sen)  /*devuelve el nombre senal a partir de la senal*/
{			/* para sitios donde no hay sig2str*/
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (sen==sigstrnum[i].senal)
            return sigstrnum[i].nombre;
    return ("SIGUNKNOWN");
}

void MostrarEntorno(char *env[], char *nombre_entorno){
    int i = 0;

    while (env[i]!= NULL) {
        printf ("%p:%s[%d]= (%p) %s\n", &env[i],nombre_entorno, i,env[i],env[i]);
        i++;
    }
}

void cmd_showvar(char *tr[], char *arg3[], char *environ[]){
    char *main_name = "main arg3";
    int i, j;

    if(tr[1]==NULL) {
        MostrarEntorno(arg3, main_name);
    }
    else if(tr[2] == NULL){
        char *v = getenv(tr[1]);

        if(v == NULL){
            perror("Var does not exist");
        }
        else if((i = BuscarVariable(tr[1], arg3)) == -1){
            perror("Bad proc");
        }
        else if((j = BuscarVariable(tr[1], environ)) == -1){
            perror("Bad proc");
        }
        else{
            printf("With arg3 main: %s (%p) %p\n", arg3[i], arg3[i], &arg3[i]);
            printf("With environ: %s (%p) %p\n", environ[j], environ[j], &environ[j]);
            printf("With getenv: %s (%p)\n", v, &v);
        }
    }
}

void Cmd_Uid(char *tr[], tListProc *L) {
    if (tr[1] != NULL) {
        if (strcmp(tr[1], "-get") == 0) {
            uid_t real_uid = getuid();
            struct passwd *real_pwd = getpwuid(real_uid);
            printf("Real credential: %d, (%s)\n", real_uid, real_pwd->pw_name);

            uid_t effective_uid = geteuid();
            struct passwd *effective_pwd = getpwuid(effective_uid);
            printf("Effective credential: %d, (%s)\n", effective_uid, effective_pwd->pw_name);


        } else if (strcmp(tr[1], "-set") == 0) {
            if (tr[2] != NULL) {
                if (strcmp(tr[2], "-l") == 0 && tr[3] != NULL) {
                    struct passwd *pwd = getpwnam(tr[3]);

                    if (pwd != NULL) {
                        if (setuid(pwd->pw_uid) == 0) {
                            printf("UID changed to: %d\n", getuid());
                        } else {
                            perror("Error changing UID");
                        }
                    } else {
                        fprintf(stderr, "User not found: %s\n", tr[3]);
                    }
                } else {
                    uid_t new_uid = atoi(tr[2]);

                    if (setuid(new_uid) == 0) {
                        printf("UID changed to: %d\n", getuid());
                    } else {
                        perror("Error changing UID");
                    }
                }
            } else {
                fprintf(stderr, "Usage: %s -set [-l] id\n", tr[0]);
            }
        } else {
            fprintf(stderr, "Usage: %s [-get|-set] [-l] [id]\n", tr[0]);
        }
    } else {
        printf("Current UID: %d\n", getuid());
    }
}

void Cmd_showEnviron(char *tr[], char *arg3[], char *environ[]){
    if (tr[1] == NULL) {
        char *main_name = "main arg3";
        MostrarEntorno(arg3, main_name);

    } else if (strcmp(tr[1], "-addr") == 0) {
        printf("Address of environ: %p\n", (void *)environ);
        printf("Value of environ: %p\n", (void *)*environ);

        printf("Address of arg3 (third argument of main): %p\n", (void *)arg3);
        printf("Value of arg3: %p\n", (void *)*arg3);

    }
    else if(strcmp(tr[1], "-environ") == 0){
        char *main_name = "environ";
        MostrarEntorno(environ, main_name);
    }
    else {
        fprintf(stderr, "Usage: %s [-environ|-addr]\n", tr[0]);
    }
}

void Cmd_changevar(char *tr[], char *arg3[], char *environ[]) {
    if (tr[1] != NULL && tr[2] != NULL && tr[3] != NULL) {
        char *var = tr[2];
        char *valor = tr[3];

        if (strcmp(tr[1], "-a") == 0){
            int index = BuscarVariable(var, arg3);

            if (index != -1) {
                printf("Changing variable %s using arg3\n", var);
                CambiarVariable(var, valor, arg3);
                printf("New value in arg3: %s\n", arg3[index]);

            } else {
                fprintf(stderr, "Variable %s not found in arg3\n", var);
            }
        } else if (strcmp(tr[1], "-e") == 0) {
            int index = BuscarVariable(var, environ);

            if (index != -1) {
                printf("Changing variable %s using environ\n", var);
                CambiarVariable(var, valor, environ);
                printf("New value in environ: %s\n", environ[index]);

            } else {
                fprintf(stderr, "Variable %s not found in environ\n", var);
            }
        } else if (strcmp(tr[1], "-p") == 0) {
            char *env_str = (char *)malloc(strlen(var) + strlen(valor) + 2);

            if (env_str != NULL) {
                sprintf(env_str, "%s=%s", var, valor);

                if (putenv(env_str) == 0) {
                    printf("Changing variable %s using putenv\n", var);
                    printf("New value in environ: %s\n", getenv(var));

                } else {
                    perror("Error changing variable using putenv");
                }
            } else {
                perror("Memory allocation error");
            }
        } else {
            fprintf(stderr, "Usage: %s [-a|-e|-p] var valor\n", tr[0]);
        }
    } else {
        fprintf(stderr, "Usage: %s [-a|-e|-p] var valor\n", tr[0]);
    }
}

void Cmd_subsVar(char *tr[], char *arg3[], char *environ[]) {
    if (tr[1] != NULL && tr[2] != NULL && tr[3] != NULL) {
        char *var1 = tr[2];
        char *var2 = tr[3];
        char *valor = tr[4];

        if (strcmp(tr[1], "-a") == 0) {
            int index_var1 = BuscarVariable(var1, arg3);

            if (index_var1 != -1) {
                //free(arg3[index_var1]);
                CambiarVariable(var2, valor, arg3);

                printf("Replacing variable %s with %s=%s in arg3\n", var1, var2, valor);
                printf("New value in arg3 for %s: %s\n", var2, arg3[index_var1]);
            } else {
                fprintf(stderr, "Variable %s not found in arg3\n", var1);
            }
        } else if (strcmp(tr[1], "-e") == 0) {
            int index_var1 = BuscarVariable(var1, environ);

            if (index_var1 != -1) {
                free(environ[index_var1]);

                CambiarVariable(var2, valor, environ);

                printf("Replacing variable %s with %s=%s in environ\n", var1, var2, valor);
                printf("New value in environ for %s: %s\n", var2, environ[index_var1]);
            } else {
                fprintf(stderr, "Variable %s not found in environ\n", var1);
            }
        } else {
            fprintf(stderr, "Usage: %s [-a|-e] var1 var2 valor\n", tr[0]);
        }
    } else {
        fprintf(stderr, "Usage: %s [-a|-e] var1 var2 valor\n", tr[0]);
    }
}


///Procesos

void deleteSig(tListProc *P){
    tPosProc p;
    tItemProc item;

    if(isEmptyListProc(*P)){
        printf("Empty List\n");
        return;
    }else{
        p = firstProc(*P);

        while(p != NULL){
            item = getItemProc(p, *P);
            if(strcmp(item.estado, "SIGNALED") == 0){
                deleteAtPositionProc(p, P);
            }
            p = nextProc(p, *P);
        }
    }
}

void deleteTerm(tListProc *P){
    tPosProc p;
    tItemProc item;

    if(isEmptyListProc(*P)){
        printf("Empty List\n");
        return;
    }else{
        p = firstProc(*P);

        while(p != NULL){
            item = getItemProc(p, *P);
            if(strcmp(item.estado, "FINISHED") == 0){
                deleteAtPositionProc(p, P);
            }
            p = nextProc(p, *P);
        }
    }

}

void printJobs(tItemProc item){
    char *state;

    if(strcmp(item.estado, "STOPPED") == 0){
        state = "STOPPED";
        printf("%d %s p=%d %s %s %s\n", item.pid, item.usuario, getpriority(PRIO_PROCESS, item.pid), item.date, state, item.comando);
    }
    else if(strcmp(item.estado, "SIGNALED") == 0){
        state = "SIGNALED";
        printf("%d %s p=%d %s %s %s\n", item.pid, item.usuario, getpriority(PRIO_PROCESS, item.pid), item.date, state, item.comando);
    }
    else if(strcmp(item.estado, "FINISHED") == 0){
        state = "FINISHED";
        printf("%d %s p=%d %s %s %s\n", item.pid, item.usuario, getpriority(PRIO_PROCESS, item.pid), item.date, state, item.comando);
    }
    else if(strcmp(item.estado, "ACTIVE") == 0){
        state = "ACTIVE";
        printf("%d %s p=%d %s %s %s\n", item.pid, item.usuario, getpriority(PRIO_PROCESS, item.pid), item.date, state, item.comando);
    }
    else if(strcmp(item.estado, "UNKNOWN\n") == 0){
        state = "UNKNOWN";
    }
    else{
        perror("UNKNOWN");
    }
}

void printFinalState(tItemProc item){

    if(strcmp(item.estado, "ACTIVE") == 0){
        printf("Process %d %s %s\n", item.pid, item.estado, item.date);
    }
    else if(strcmp(item.estado, "SIGNALED") == 0){
        printf("Process %d %s signal %d %s\n", item.pid, item.estado, item.fin, item.date);
    }
    else if(strcmp(item.estado, "FINISHED") == 0){
        printf("Process %d %s terminated signal %d name %s %s\n", item.pid, item.estado, item.fin, NombreSenal(item.fin), item.date);
    }
    else if(strcmp(item.estado, "STOPPED") == 0){
        printf("Process %d %s stopped signal %d name %s %s\n", item.pid, item.estado, item.fin, NombreSenal(item.fin), item.date);
    }
    else{
        printf("Process %d, UNKNOWN STATE\n", item.pid);
    }
}

tItemProc updateState(tItemProc item, int choose){
    int status;

    if (choose != 0) {
        choose = WUNTRACED | WNOHANG | WCONTINUED; //opciones del proceso padre que se proporcionan a waitpid
    }

    pid_t result = waitpid(item.pid, &status, choose); //cogemos el pid para buscar el item y su estatus

    if (result == item.pid) {
        if (WIFEXITED(status)) {
            strcpy(item.estado, "FINISHED");
            item.fin = WEXITSTATUS(status);
        } else if (WIFSTOPPED(status)) {
            strcpy(item.estado, "STOPPED");
            item.fin = WSTOPSIG(status);
        } else if (WIFCONTINUED(status)) {
            strcpy(item.estado, "ACTIVE");
        } else if (WIFSIGNALED(status)) {
            strcpy(item.estado, "SIGNALED");
            item.fin = WTERMSIG(status);
        } else {
            strcpy(item.estado, "UNKNOWN");
        }
    }

    return item;
}

void printListProcJobs(tListProc *P){
    tPosProc p = firstProc(*P);
    tItemProc item;
    int i = 0;

    if(isEmptyListProc(*P)){
        printf("Empty List\n");
        return;
    }

    while(p != LNULL){
        item = getItemProc(p, *P);
        item = updateState(item, 1);
        printf("%d ", i);
        updateItemProc(item, p, P);
        //printJobs(item);
        printFinalState(item);

        i++;
        p = nextProc(p, *P);
    }
}

void Cmd_Jobs(char *tr[], tListProc *P){
    if(tr[1] == NULL){
        printListProcJobs(P);
    }
    else{
        perror("Command does not need any arguments");
    }
}

void Cmd_delJobs(char *tr[], tListProc *P){
    if(tr[1] == NULL){
        printListProcJobs(P);
    }
    else if(strcmp(tr[1], "-sig") == 0){
        deleteSig(P);
        return;
    }
    else if(strcmp(tr[1], "-term") == 0){
        deleteTerm(P);
        return;
    }
    else{
        perror("Cannot delete");
    }
}

void Cmd_job(char *tr[], tListProc *P){
    tPosProc p;
    tItemProc item_aux;
    pid_t pid;

    if (tr[1] == NULL) {
        printListProcJobs(P);
        return;
    } else if (tr[2] == NULL) {
        if (isEmptyListProc(*P)) {
            printf("Empty list\n");
            return;
        } else {
            pid = atoi(tr[1]);
            p = firstProc(*P);

            while (p != NULL) {
                item_aux = getItemProc(p, *P);

                if (item_aux.pid == pid) {
                    item_aux = updateState(item_aux, 1);
                    updateItemProc(item_aux, p, P);
                    printJobs(item_aux);
                    return;
                }

                p = nextProc(p, *P);
            }
        }
    } else if (strcmp(tr[1], "-fg") == 0) {
        pid = atoi(tr[2]);
        p = firstProc(*P);

        while (p != NULL) {
            item_aux = getItemProc(p, *P);

            if (item_aux.pid == pid) {
                item_aux = updateState(item_aux, 0);
                updateItemProc(item_aux, p, P);
                printJobs(item_aux);
                //free(item_aux.comando);
                //free(item_aux.usuario);
                //free(item_aux.date);
                deleteAtPositionProc(p, P);
                return;
            }

            p = nextProc(p, *P);
        }
        printf("Process not in the list\n");
    } else {
        fprintf(stderr, "Usage: job [-fg] pid\n");
    }
}

void Cmd_Exec(char *tr[], tListProc *P) {
    if (tr[1] != NULL) {
        if (execvp(tr[1], &tr[1]) == -1) {
            perror("Error executing program");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Usage: %s program [args...]\n", tr[0]);
    }
}

bool isBackgroundCommand(char *tr[]) {
    int i = 0;
    while (tr[i] != NULL) {
        i++;
    }

    return i > 0 && strcmp(tr[i - 1], "&") == 0;
}

void execute(char *command, char *tr[], tListProc *P) {
    if (tr[0] != NULL) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Error forking process");
            exit(EXIT_FAILURE);

        } else if (pid == 0) {

            if (isBackgroundCommand(tr)) {
                int i = 0;
                while (tr[i] != NULL) {
                    i++;
                }
                tr[i - 1] = NULL;

                if (setsid() == -1) {
                    perror("Error detaching from terminal");
                    exit(EXIT_FAILURE);
                }
            }

            execvp(command, tr);

            perror("Error executing program");
            exit(EXIT_FAILURE);

        } else {
            if (isBackgroundCommand(tr)) {
                printf("Background process created with PID: %d\n", pid);

                tItemProc item;
                item.pid = pid;

                uid_t uid = getuid();
                struct passwd *pwd = getpwuid(uid);
                if (pwd != NULL) {
                    item.usuario = strdup(pwd->pw_name);
                } else {
                    item.usuario = strdup("unknown");
                }
                item.fin = 0;
                //strcpy(item.comando, command);
                item.comando = command;
                //printf("%s\n", item.comando);
                time_t rawt; //obtener tiempo
                time(&rawt);
                char *date_str = ctime(&rawt);
                date_str[strlen(date_str) - 1] = '\0';
                item.date = date_str;

                item.estado = strdup("ACTIVE");

                insertItemProc(item, P);

            } else {
                int status;

                waitpid(pid, &status, 0);
            }
        }
    } else {
        fprintf(stderr, "Usage: program [args...]\n");
    }
}

void insert(char *tr[], tListProc *P){ //funcion de prueba para insertar
    tItemProc item;

    item.pid = 3;
    if(insertItemProc(item, P))
        printf("OK %d", item.pid);
    else
        printf("no");
}


