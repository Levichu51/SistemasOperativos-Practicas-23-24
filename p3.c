#include <stdio.h>
#include "p1.h"
#include "lista_hist.h"
#include "lista_mem.h"
#include "p2_functions.h"
#include "p3_functions.h"
#include "lista_procesos.h"
#define MAX_FILES 100
#define MAXNAME 255

///Authors : Levi Barros García @levi.barrosg || Raúl Meijide Couto @r.meijide

extern char **environ;

void authors(char *str){
    if(str == NULL){
        printf("Levi Barros García @levi.barrosg || Raúl Meijide Couto @r.meijide\n");

    }else if(strcmp(str, "-n") == 0){
        printf("Levi Barros García || Raúl Meijide Couto\n");

    }else if(strcmp(str, "-l") == 0){
        printf("@levi.barrosg || @r.meijide\n");

    }else{
        perror("Command not valid\n");
    }
}

void pid(char *str){
    if(str == NULL) {
        printf("Pid of the process: %d\n", getpid());

    }else if(strcmp(str, "-p") == 0){
        printf("Shell's parent process: %d\n", getppid());
    }
    else{
        perror("Command not valid\n");
    }
}

void chDir(char *str) {
    char dir[MAX];

    if (str == NULL) {
        if (getcwd(dir, sizeof(dir)) != NULL) //comprueba si la llamada fue exitosa
            printf("Current working directory: %s\n", dir);
        else {
            perror("Cannot get directory\n");
        }
    }else{
        if (chdir(str) == 0) {
            printf("Changed directory to %s\n", getcwd(dir, sizeof(dir)));
        } else {
            perror("chdir not possible\n");
        }
    }
}

void date(char *str){
    time_t t = time(NULL);
    struct tm *time = localtime(&t);

    if(str == NULL) {
        printf("%d/%d/%d\n", time->tm_mday, time->tm_mon + 1, (1900+ time->tm_year));

    }else{
        perror("Command not valid\n");
    }
}

void currentTime(char *str){
    time_t t = time(NULL);
    struct tm *time = localtime(&t);

    if(str == NULL) {
        printf("%d:%d:%d\n", time->tm_hour, time->tm_min, time->tm_sec);

    }else{
        perror("Command not valid\n");
    }
}

void hist(char *str, tList *L) {
    if (str == NULL) {
        printList(*L);

    } else if (strcmp(str, "-c") == 0) {
        deleteList(L);

    } else if (str[0] == '-') {
        int n = atoi(str + 1); // Convierte el número después del guion en un entero omite el primer carácter de la cadena o sea el -.

        if (n > 0) {
            tPosL p = first(*L);
            int i = 0;

            while (p != LNULL && i < n) {
                tItem item = getItem(p, *L);
                printf("%s", item.command);
                p = next(p, *L);
                i++;
            }
        } else {
            printf("Invalid option: %s\n", str);
        }
    }
}


void infosys(char *str){
    struct utsname system;

    if (uname(&system) != 0) {
        perror("Infosys not possible ERROR\n");

    }else if(str == NULL){
        printf("System name: %s\n", system.sysname);
        printf("System's version: %s\n", system.release);
        printf("Kernel: %s\n", system.version);
        printf("Hardware architecture: %s\n", system.machine);
        printf("Release ID: %s\n", system.release);
    }else{
        perror("Command not valid\n");
    }
}

void help(char *str) {
    if (str != NULL) {
        if (strcmp(str, "authors") == 0) {
            printf("Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names\n");
        } else if (strcmp(str, "pid") == 0) {
            printf("Prints the pid of the process executing the shell. pid -p prints the pid of the shell’s parent process\n");
        } else if (strcmp(str, "chdir") == 0) {
            printf("Changes the current working directory of the shell to dir. When invoked without arguments it prints the current working directory\n");
        } else if (strcmp(str, "date") == 0) {
            printf("Prints the current date in the format DD/MM/YYYY\n");
        } else if (strcmp(str, "time") == 0) {
            printf("Prints the current time in the format hh:mm:ss.\n");
        } else if (strcmp(str, "hist") == 0) {
            printf("Shows the historic of commands executed by this shell. hist -c clears the list of historic commands. hist -N prints the first N commands.\n");
        } else if (strcmp(str, "commandN") == 0) {
            printf("Repeats command number N\n");
        } else if (strcmp(str, "open") == 0) {
            printf("Opens a file and adds it to the list of shell open files.\n");
        } else if (strcmp(str, "close") == 0) {
            printf("Closes the df file descriptor and eliminates the corresponding item from the list\n");
        } else if (strcmp(str, "dup") == 0) {
            printf("Duplicates the df file descriptor creating the corresponding new entry on the file list\n");
        } else if (strcmp(str, "listopen") == 0) {
            printf(" Lists the shell open files. For each file it lists its descriptor, the file name and the opening mode.\n");
        } else if (strcmp(str, "infosys") == 0) {
            printf("Prints information on the machine running the shell.\n");
        } else if (strcmp(str, "quit") == 0 || strcmp(str, "exit") == 0 || strcmp(str, "bye") == 0) {
            printf("Ends the shell\n");
        }else if (strcmp(str, "delete") == 0){
            printf("Deletes files or/and empty directories\n");
        }else if(strcmp(str, "create") == 0){
            printf("Creates files (-f) or directories\n");
        }else if(strcmp(str, "deltree") == 0){
            printf("Deletes files and/or non empty directories recursively\n");
        }else if(strcmp(str, "list") == 0){
            printf("Prints the content of a directory (-long) (-acc) (-link) (-reca) (-recb) (-hid)\n");
        }else if (strcmp(str, "stat") == 0){
            printf("Gives information on files or directories (-long) (-acc) (-link)\n");
        }else if(strcmp(str, "malloc") == 0){
            printf("malloc [-free] [tam]\tasigna un bloque memoria de tamano tam con malloc\n"
                   "\t-free: desasigna un bloque de memoria de tamano tam asignado con malloc\n");
        }else if(strcmp(str, "shared") == 0){
            printf("shared [-free|-create|-delkey] cl [tam]\tasigna memoria compartida con clave cl en el programa\n"
                   "\t-create cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n"
                   "\t-free cl: desmapea el bloque de memoria compartida de clave cl\n"
                   "\t-delkey clelimina del sistema (sin desmapear) la clave de memoria cl\n"
                   "-> \n");
        }else if(strcmp(str, "mmap") == 0){
            printf("mmap [-free] fich prm\tmapea el fichero fich con permisos prm\n"
                   "\t-free fich: desmapea el ficherofich\n");
        }else if(strcmp(str, "read") == 0){
            printf("read fiche addr cont \tLee cont bytes desde fich a la direccion addr\n");
        }else if(strcmp(str, "write") == 0){
            printf("write [-o] fiche addr cont \tEscribe cont bytes desde la direccion addr a fich (-o sobreescribe)n\n");
        }else if(strcmp(str, "recurse") == 0){
            printf("recurse [n]\tInvoca a la funcion recursiva n veces\n");
        }else if(strcmp(str, "memdump") == 0){
            printf("memdump addr cont \tVuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr\n");
        }else if(strcmp(str, "memfill") == 0){
            printf("memfill addr cont byte \tLlena la memoria a partir de addr con byte\n");
        }else if(strcmp(str, "mem") == 0){
            printf("mem [-blocks|-funcs|-vars|-all|-pmap] ..\tMuestra muestra detalles de la memoria del proceso\n"
                   "\t\t-blocks: los bloques de memoria asignados\n"
                   "\t\t-funcs: las direcciones de las funciones\n"
                   "\t\t-vars: las direcciones de las variables\n"
                   "\t\t:-all: todo\n"
                   "\t\t-pmap: muestra la salida del comando pmap(o similar)\n");
        }
        else {
            perror("Argument not valid");
        }
    }
    else {
        printf("* authors [-l] [-n]\n * pid [-p]\n * chdir [dir]\n * date\n * time\n * hist [-c] [-N]\n * command N\n * open [file]\n * close [df]\n * dup [df]\n * listopen\n * infosys\n * help [cmd]\n * quit\n * exit\n * bye\n * create\n * delete\n * deltree\n * list\n * stat\n");
    }
}

void bye(bool *loop){
    printf("Cmd closed, goodbye\n");
    *loop = false;
}
/*
void commandN(char *str, tList *L, bool *loop, tListMem *M, tListProc *P){
    if (str == NULL) {
        perror("Missing argument\n");

    }else{
        int n = atoi(str);
        int cnt = 0;
        tPosL pos = first(*L);

        while (pos != LNULL){
            cnt++;
            pos = next(pos, *L);
        }

        if(cnt == 1 && n == 0){
            perror("There are no commands\n");
            return;
        }
        if(cnt == n + 1){
            perror("Command not found\n");
            return;
        }

        else if (n >= 0 && !isEmptyList(*L)){
            tPosL p = first(*L);
            int i = 0;

            while (p != LNULL && i < n){
                p = next(p, *L);
                i++;
            }

            if (i == n  && p != LNULL){
                tItem item = getItem(p, *L);
                processCommand(L, item.command, loop, M, P, environ, envP);

            } else{
                printf("Command not found\n");
            }

        }else{
            perror("Invalid argument\n");
        }

    }
}
 */


//FICHEROS
struct OpenFile {
    int descriptor;
    char nombre[MAXNAME];
    char modo[4];
};

struct OpenFile archivosAbiertos[MAX_FILES];
int numArchivos = 0;

void initializeDefaultFiles() {
    char nombre0[] = "stdin";
    char nombre1[] = "stdout";
    char nombre2[] = "stderr";
    char modo[] = "rw";

    AddToFileList(0, nombre0, modo);
    AddToFileList(1, nombre1, modo);
    AddToFileList(2, nombre2, modo);
}

void ListOpenFiles(int descriptor){
    printf("Opened files:\n");

    for (int i = 0; i < numArchivos; i++){
        printf("Descriptor: %d, Name: %s, Mode: %s\n",archivosAbiertos[i].descriptor, archivosAbiertos[i].nombre, archivosAbiertos[i].modo);
    }
    if (descriptor != -1){
        printf("Duplicate: Descriptor %d\n", descriptor);
    }
}

void AddToFileList(int descriptor, const char *nombre, const char *modo) {
    if (numArchivos < MAX_FILES){

        archivosAbiertos[numArchivos].descriptor = descriptor;
        strncpy(archivosAbiertos[numArchivos].nombre, nombre, sizeof(archivosAbiertos[numArchivos].nombre));
        strncpy(archivosAbiertos[numArchivos].modo, "rw", sizeof(archivosAbiertos[numArchivos].modo));

        numArchivos++;

    } else{
        printf("Cannot open more files, limit reached\n");
    }
}

void Cmd_open(char *tr[]) {
    int df, mode = 0;

    if (tr[1] == NULL){ // si no hay parámetros
        ListOpenFiles(-1);
        return;
    }

    for (int i = 2; tr[i] != NULL; i++){
        if (!strcmp(tr[i], "cr")) mode |= O_CREAT;
        else if (!strcmp(tr[i], "ex")) mode |= O_EXCL;
        else if (!strcmp(tr[i], "ro")) mode |= O_RDONLY;
        else if (!strcmp(tr[i], "wo")) mode |= O_WRONLY;
        else if (!strcmp(tr[i], "rw")) mode |= O_RDWR;
        else if (!strcmp(tr[i], "ap")) mode |= O_APPEND;
        else if (!strcmp(tr[i], "tr")) mode |= O_TRUNC;
        else break;
    }

    if ((df = open(tr[1], mode, 0777)) == -1){
        perror("Cannot open file\n");
    } else if(tr[2] != NULL){
        AddToFileList(df, tr[1], tr[2]);
        printf("Opened file. Descriptor: %d, Name: %s, Mode: %s\n", df, tr[1], tr[2]);
    }else{
        AddToFileList(df, tr[1], "rw");
        printf("Opened file. Descriptor: %d, Name: %s, Mode: %s\n", df, tr[1], "rw");
    }
}

void Cmd_close(char *tr[]) {
    int df;

    if (tr[1] == NULL || (df = atoi(tr[1])) < 0){
        ListOpenFiles(-1);
        return;
    }

    if (close(df) == -1){
        perror("Cannot close descriptor\n");
    }
    else{
        for (int i = 0; i < numArchivos; i++){ //borrado del archivo
            if (archivosAbiertos[i].descriptor == df) {
                printf("Descriptor %d closed and deleted\n", df);

                for (int j = i; j < numArchivos - 1; j++){
                    archivosAbiertos[j] = archivosAbiertos[j + 1];
                }
                numArchivos--;
            }
        }
    }
}

char *NombreFicheroDescriptor(int descriptor) {

    for (int i = 0; i < numArchivos; i++){
        if (archivosAbiertos[i].descriptor == descriptor){
            return archivosAbiertos[i].nombre;
        }
    }

    return NULL;
}

char *ModoFicheroDescriptor(int descriptor) {

    for (int i = 0; i < numArchivos; i++){
        if (archivosAbiertos[i].descriptor == descriptor){
            return archivosAbiertos[i].modo;
        }
    }

    return NULL;
}


void Cmd_dup(char *tr[]) {
    int df, duplicado;
    char *p, *m;

    if (tr[1] == NULL || (df = atoi(tr[1])) < 0){
        ListOpenFiles(-1);
        return;
    }

    p = NombreFicheroDescriptor(df);
    m = ModoFicheroDescriptor(df);

    if (p == NULL) {
        printf("Descriptor not found %d.\n", df);
    }

    if ((duplicado = dup(df)) == -1){
        perror("Cannot duplicate\n");
    }

    AddToFileList(duplicado, p, m);

    int flags = fcntl(duplicado, F_GETFL);

    if (flags == -1){
        perror("Cannot obtain flags\n");
    }

    printf("Descriptor %d duplicated to %d. Name: %s, Mode: %s\n", df, duplicado, p, m);
}




///Funciones P1

char LetraTF (mode_t m)
{
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}

char * ConvierteModo2 (mode_t m)
{
    static char permisos[12];
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

void create(char *tr[]){
    struct stat file_info;
    FILE *file;
    int estado = 0;

    if (stat(tr[1], &file_info) == 0 && strcmp(tr[1], "-f") != 0) { //si es directorio obtuvo ya un estado de 1, ya exite y no se vuelve a crear
        if (S_ISDIR(file_info.st_mode)) {
            printf("Directory %s already exists\n", tr[1]);
            estado = 1;

        }
    }
    else if(stat(tr[2], &file_info) == 0){ //lo mismo con archivos
        if(S_ISREG(file_info.st_mode)) {
            printf("File %s already exists\n", tr[2]);
            estado = 1;
        }
    }

    if(strcmp(tr[1], "-f") == 0 && estado == 0){ //si le entra un -f y su estado es 0, no esta creado, crea un archivo con todos los permisos
        file = fopen(tr[2], "w");

        if(stat(tr[2], &file_info) == 0){
            chmod(tr[2], S_IRWXU | S_IRWXG | S_IRWXO);

        }else{
            perror("Cannot create the file");
        }

        if(file != NULL){
            printf("File %s created successfully\n", tr[2]);
            fclose(file);

        }else{
            perror("Cannot create the file\n");
        }

    }else if(strcmp(tr[1], "-f") != 0 && estado == 0){ //si no le entra un -f y su estado es 0, o sea no esta ya creado, hace un directorio

        if(mkdir(tr[1], 0777) == 0){
            printf("New directory %s created\n", tr[1]);

        }else{
            perror("Cannot create the directory\n");
        }
    }
}

void auxStat(char *file, int longFormat, int flagAccessTime, int flagLinkPath){
    struct stat file_info;
    struct tm time_info;
    time_t modificationTime;
    struct passwd *user_info;
    struct group *group_info;
    char path[MAX];
    ssize_t length;

    if(stat(file, &file_info) == 0){

        if(longFormat) { //el formato largo t muestra el owner, el group, los permisos y el archivo

            if (flagAccessTime) { //si se detecto -acc muestra su ultima fecha de modificacion
                modificationTime = file_info.st_mtime;
                time_info = *localtime(&modificationTime);

                printf("Last Modification Time: %d-%02d-%02d %02d:%02d:%02d\n",time_info.tm_year + 1900, time_info.tm_mon + 1, time_info.tm_mday,time_info.tm_hour, time_info.tm_min, time_info.tm_sec);
            }

            user_info = getpwuid(file_info.st_uid);
            group_info = getgrgid(file_info.st_gid);

            printf("Owner: %s (UID: %d) ", user_info != NULL ? user_info->pw_name : "NULL", file_info.st_uid); //comprueba si el owner es o no NULL, si lo es imprime NULL, si no el owner
            printf(" Group: %s (GID: %d) ", group_info != NULL ? group_info->gr_name : "NULL", file_info.st_gid);
            printf(" Permissions: %s ", ConvierteModo2(file_info.st_mode)); //se enseñan los permisos con el formate de ConvierteModo2
            printf(" File: %s\n", file);

            printf("\n");

        }else{
            printf("File: %s\n", file);
        }

        if (flagLinkPath && S_ISLNK(file_info.st_mode)) {
            length = readlink(file, path, sizeof(path) - 1); //si es un link y se detecto el flag -link lo lee y enseña su path

            if (length != -1) {
                path[length] = '\0';
                printf("Path: %s\n", path);
            }
        }

    }else{
        perror("Cannot Stat the file");
    }

}

void listStat(char *tr[]){
    int longFormat = 0;
    int flagAccessTime = 0;
    int flagLinkPath = 0;
    int cnt = 1;
    char dir[MAX];
    int i = 1;

    while(tr[i] != NULL && i < 4){ //i itera el bucle y cnt te dice en donde estas, la posicion en la que te encuentras

        if (strcmp(tr[i], "-long") == 0 && !longFormat) {
            longFormat = 1;
        }
        else if (strcmp(tr[i], "-acc") == 0 && !flagAccessTime) {
            flagAccessTime = 1;
        }
        else if (strcmp(tr[i], "-link") == 0 && !flagLinkPath) {
            flagLinkPath = 1;

        }else{
            break; //rompe al detectar algo que no es una flag
        }

        i++;
        cnt++;
    }

    if(tr[cnt] == NULL){
        printf("Current dir: %s\n", getcwd(dir, sizeof(dir))); //si no se detectan flags imprime el path

    }else if(!longFormat && (flagLinkPath || flagAccessTime)){
        printf("Cannot print -acc and -link in a proper manner without -long\n"); //se necesita un -long para -link y -acc
        i = cnt;

        while(tr[i] != NULL){
            printf("File: %s\n", tr[i]); //imprime los archivos sin mas y se detecta un -acc o -link sin -long

            i++;
        }

    }
    else{
        i = cnt;
        while(tr[i] != NULL){ //va a la funcion auxiliar enviando los archivos detectados que maneja los flags
            auxStat(tr[i], longFormat, flagAccessTime, flagLinkPath);

            i++;
        }
    }
}


void listDir(char *tr[]){
    int longFormat = 0;
    int flagAccessTime = 0;
    int flagLinkPath = 0;
    int flagReca = 0;
    int flagRecb = 0;
    int flagHid = 0;
    int cnt = 1;
    char dir[MAX];
    int i = 1;

    while(tr[i] != NULL && i < 7){

        if (strcmp(tr[i], "-long") == 0 && !longFormat) {
            longFormat = 1;
        }
        else if (strcmp(tr[i], "-acc") == 0 && !flagAccessTime) {
            flagAccessTime = 1;
        }
        else if (strcmp(tr[i], "-link") == 0 && !flagLinkPath) {
            flagLinkPath = 1;

        }else if(strcmp(tr[i], "-hid") == 0 && !flagHid){
            flagHid = 1;

        }else if(strcmp(tr[i], "-reca") == 0 && !flagReca){
            flagReca = 1;

        }else if(strcmp(tr[i], "-recb") == 0 && !flagRecb){
            flagRecb = 1;
        }
        else{
            break;
        }

        i++;
        cnt++;
    }

    if(tr[cnt] == NULL){
        printf("Current dir: %s\n", getcwd(dir, sizeof(dir))); //si no se detectan flags imprime el path

    }else if(flagRecb && flagReca){
        printf("-recb and -reca are incompatible\n");
    }
    else if(!longFormat && (flagLinkPath || flagAccessTime)){ //se necesita un -long para -link y -acc
        printf("Cannot print -acc and -link in a proper manner without -long\n");

        while(tr[i] != NULL){ //imprime los archivos sin mas y se detecta un -acc o -link sin -long
            auxList(tr[i], longFormat, flagAccessTime, flagLinkPath, flagHid, flagReca, flagRecb);

            i++;
        }

    }
    else{
        i = cnt; //tr[i] seran los archivos detectados despues de los flags
        while(tr[i] != NULL){ //llama a la funcion auxiliar enviando los archivos detectados que maneja los flags
            auxList(tr[i], longFormat, flagAccessTime, flagLinkPath, flagHid, flagReca, flagRecb);

            i++;
        }
    }
}

void auxList(char *file, int longFormat, int flagAccessTime, int flagLinkPath, int flagHid, int flagReca, int flagRecb){
    struct stat file_info;
    char path[MAX];
    DIR *directory;
    struct dirent *input;
    struct stat file_info2;
    struct dirent *input2;
    DIR *directory2;
    char subDir[256];
    char auxDir[256];


    if(stat(file, &file_info) == 0 && S_ISDIR(file_info.st_mode)){
        directory = opendir(file);

        if(directory != NULL){
            printf("Files in %s\n", file);

            if(flagRecb){ //flag de -recb
                strcpy(subDir, getcwd(auxDir, sizeof auxDir));

                if(stat(file, &file_info2) == 0){
                    if(S_ISDIR(file_info2.st_mode)){
                        directory2 = opendir(file);

                        chdir(file); //se cambia al subDir del archivo

                        if(directory2 != NULL){

                            while ((input2 = readdir(directory2)) != NULL){

                                if(stat(input2->d_name, &file_info2) && S_ISDIR(file_info2.st_mode)) { //se mueve por los subdirectorios a, si detecta el flag de -hid se muestran los archivos ocultos

                                    if (strcmp(input2->d_name, ".") == 0 || strcmp(input2->d_name, "..") == 0 || (!flagHid && input2->d_name[0] != *".") ) {
                                        auxList(input2->d_name, longFormat, flagAccessTime, flagLinkPath, flagHid, flagReca, flagRecb); // si detecta un -hid imprime los archivos ocultos
                                    }

                                }
                            }
                            closedir(directory2);

                        }else{
                            printf("No files at directory %s\n", file);
                        }

                    }else{
                        perror("Not a directory\n");
                    }
                    chdir(auxDir); //se vuelve al original

                }else{
                    perror("Cannot find any file or directory\n");
                }

            }

            while ((input = readdir(directory)) != NULL){ //recorre el directorio mostrando sus archivos en la carpeta
                snprintf(path, sizeof(path), "%s/%s", file, input->d_name);

                if(flagHid || (input->d_name[0] != '.' && !flagHid)){ //si el elemento no es oculto y detecta si econtro un -hid
                    auxStat(path, longFormat, flagAccessTime, flagLinkPath); //llamas a Stat para ver el estado de los archivos del directorio
                }

            }

            if(flagReca){ //flag de -reca
                strcpy(subDir, getcwd(auxDir, sizeof auxDir));

                if(stat(file, &file_info2) == 0){
                    if(S_ISDIR(file_info2.st_mode)){
                        directory2 = opendir(file);

                        chdir(file); //se cambia al subDir del archivo

                        if(directory2 != NULL){

                            while ((input2 = readdir(directory2)) != NULL){

                                if(stat(input2->d_name, &file_info2) && S_ISDIR(file_info2.st_mode)) { //se mueve por los subdirectorios a, si detecta el flag de -hid se muestran los archivos ocultos

                                    if (strcmp(input2->d_name, ".") == 0 || strcmp(input2->d_name, "..") == 0 || (!flagHid && input2->d_name[0] != *".") ) {
                                        auxList(input2->d_name, longFormat, flagAccessTime, flagLinkPath, flagHid, flagReca, flagRecb); // si detecta un -hid imprime los archivos ocultos
                                    }

                                }
                            }
                            closedir(directory2);

                        }else{
                            printf("No files at directory %s\n", file);
                        }

                    }else{
                        perror("Not a directory\n");
                    }
                    chdir(auxDir); //se vuelve al original

                }else{
                    perror("Cannot find any file or directory\n");
                }

            }
        }
        closedir(directory);

    }else{
        perror("Cannot List the directory\n");
    }
}

void delete(char *tr[]) {
    int i = 1;
    const char *files;
    struct stat file_info;
    DIR *directory;
    int is_empty;
    struct dirent *input;

    while (tr[i] != NULL) {
        files = tr[i];

        if (stat(files, &file_info) != 0) {
            perror("Cannot find the file or directory");

        } else {
            if (S_ISDIR(file_info.st_mode)) {
                directory = opendir(files);

                if (directory != NULL) {
                    is_empty = 1;

                    while ((input = readdir(directory))) {
                        if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0) { //si detecta un fichero vacio o no para poder borrar, si lo detecta se acaba el bucle
                            is_empty = 0;
                            break;
                        }
                    }

                    closedir(directory);

                    if (is_empty) { // si no detecto archivos borra
                        if (rmdir(files) == 0) {
                            printf("Deleted directory %s\n", files);
                        } else {
                            perror("Cannot delete directory");
                        }
                    } else {
                        printf("Cannot delete a non-empty directory: %s\n", files);
                    }
                } else {
                    perror("Cannot open directory");
                }
            } else {
                if (unlink(files) == 0) {
                    printf("Deleted file %s\n", files);
                } else {
                    perror("Cannot delete file");
                }
            }
        }
        i++;
    }
}

void deltree(char *path) {
    struct dirent *input;
    struct stat file_info;
    DIR *directory;
    char otherPath[512];

    if(path == NULL){
        printf("Current dir: %s\n", getcwd(otherPath, sizeof(otherPath)));
    }else {

        if (stat(path, &file_info) == 0) {
            if (S_ISDIR(file_info.st_mode)) {

                if ((directory = opendir(path)) == NULL) {
                    perror("Cannot open directory");
                    return;
                }

                while ((input = readdir(directory)) != NULL) {
                    if (strcmp(input->d_name, ".") == 0 || strcmp(input->d_name, "..") == 0) {
                        continue;
                    }
                    char subpath[PATH_MAX];
                    snprintf(subpath, sizeof(subpath), "%s/%s", path, input->d_name);

                    if(S_ISDIR(file_info.st_mode)) {
                        deltree(subpath);
                    }//llamada recursiva al subdirectorio
                }
                closedir(directory);

                if (rmdir(path) == 0) {
                    printf("Deleted directory: %s\n", path);//se borra el directorio
                } else {
                    perror("Cannot delete directory");
                }
            } else {
                if (unlink(path) == 0) {
                    printf("Deleted file: %s\n", path);//se borra el archivo
                } else {
                    perror("Cannot delete file");
                }
            }
        } else {
            perror("Cannot access file/directory");
        }
    }
}


int TrocearCadena(char *cadena, char *trozos[]){
    int i=1;

    if ((trozos[0] = strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i] = strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void processCommand(tList *L, char input[], bool *loop, tListMem *M, tListProc *P, char *environ[], char *envP[]){
    char *tokens[15];
    int valid = TrocearCadena(input, tokens);

    if(valid > 0){
        if(strcmp(tokens[0], "authors") == 0){
            authors(tokens[1]);
        }
        else if(strcmp(tokens[0], "date") == 0){
            date(tokens[1]);

        }else if(strcmp(tokens[0], "time") == 0){
            currentTime(tokens[1]);

        }else if(strcmp(tokens[0], "infosys") == 0){
            infosys(tokens[1]);

        }else if(strcmp(tokens[0], "pid") == 0){
            pid(tokens[1]);

        }else if(strcmp(tokens[0], "bye") == 0 || strcmp(tokens[0], "quit") == 0 || strcmp(tokens[0], "exit") == 0){
            bye(loop);

        }else if(strcmp(tokens[0], "chdir") == 0){
            chDir(tokens[1]);

        }else if(strcmp(tokens[0], "hist") == 0){
            hist(tokens[1], L);

        }else if(strcmp(tokens[0], "help") == 0){
            help(tokens[1]);

        }else if(strcmp(tokens[0], "command") == 0){
            //commandN(tokens[1], L, loop, M, P);
            printf("Arreglando commandN\n");

        }else if (strcmp(tokens[0], "open") == 0) {
            Cmd_open(tokens);

        } else if (strcmp(tokens[0], "close") == 0) {
            Cmd_close(tokens);

        } else if (strcmp(tokens[0], "dup") == 0) {
            Cmd_dup(tokens);

        }else if(strcmp(tokens[0], "create") == 0){
            create(tokens);

        }else if(strcmp(tokens[0], "delete") == 0){
            delete(tokens);

        }else if(strcmp(tokens[0], "stat") == 0){
            listStat(tokens);

        }else if(strcmp(tokens[0], "list") == 0){
            listDir(tokens);

        }else if(strcmp(tokens[0], "deltree") == 0){
            deltree(tokens[1]);
        }
        else if(strcmp(tokens[0], "malloc") == 0){
            newMalloc(tokens, M);
        }
        else if(strcmp(tokens[0], "shared") == 0){
            SharedCreate(tokens, M);
        }
        else if(strcmp(tokens[0], "mmap") == 0){
            CmdMmap(tokens, M);
        }
        else if(strcmp(tokens[0], "mem") == 0){
            memProcess(tokens, *M);
        }
        else if(strcmp(tokens[0], "read") == 0){
            CmdRead(tokens);
        }
        else if(strcmp(tokens[0], "write") == 0){
            CmdWrite(tokens);
        }
        else if (strcmp(tokens[0], "recurse") == 0){
            CmdRecursiva(tokens);
        }
        else if(strcmp(tokens[0], "memfill") == 0){
            newMemFill(tokens);
        }
        else if(strcmp(tokens[0], "memdump") == 0){
            newMemDump(tokens);
        }
        else if(strcmp(tokens[0], "fork") == 0){
            Cmd_fork(tokens, P);
        }
        else if(strcmp(tokens[0], "showvar") == 0){
            cmd_showvar(tokens, envP, environ);
        }
        else if(strcmp(tokens[0], "uid") == 0){
            Cmd_Uid(tokens, P);
        }
        else if(strcmp(tokens[0], "showenv") == 0){
            Cmd_showEnviron(tokens, envP, environ);
        }
        else if(strcmp(tokens[0], "changevar") == 0){
            Cmd_changevar(tokens, envP, environ);
        }
        else if(strcmp(tokens[0], "subsvar") == 0){
            Cmd_subsVar(tokens, envP, environ);
        }
        else if(strcmp(tokens[0], "job") == 0){
            Cmd_job(tokens, P);
        }
        else if(strcmp(tokens[0], "jobs") == 0){
            Cmd_Jobs(tokens, P);
        }
        else if(strcmp(tokens[0], "deljobs") == 0){
            Cmd_delJobs(tokens, P);
        }
        else if(strcmp(tokens[0], "exec") == 0){
            Cmd_Exec(tokens, P);
        }
        else if(strcmp(tokens[0], "prueba") == 0){
            insert(tokens, P);
        }
        else {
            //externCommand(tokens, input, P, environ);
            execute(input, tokens, P);
        }

    }
}

void imprimirPrompt(){
    printf(">> ");
}

void leerEntrada(tList *L, char str[]){
    tItem item;
    fgets(str, MAX, stdin);
    strcpy(item.command, str);
    insertItem(item, L);
}

int main(int argc, char *argv[], char *envP[]) {
    bool loop = true;
    char input[MAX];
    tList *L = malloc(sizeof (L));
    tListMem *M = malloc(sizeof (M));
    tListProc *P = malloc(sizeof (P));
    createEmptyList(L);
    createEmptyListMem(M);
    createEmptyListProc(P);
    initializeDefaultFiles();

    while(loop){
        imprimirPrompt();
        leerEntrada(L, input);
        processCommand(L, input, &loop, M, P, envP, environ);
    }
    deleteList(L);
    deleteListMem(M);
    deleteListProc(P);
    free(L);
    free(M);
    free(P);

    return 0;
}
