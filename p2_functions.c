//
// Created by levixhu on 27/11/23.
//

#include "p2_functions.h"
#include "p1.h"
#include "p2_functions.h"
#include "lista_mem.h"
#define TAMANO 2048

int globalInt = 3;
char globalChar = 'C';
double globalDouble = 333.3;

int globalInt2;
char globalChar2;
double globalDouble2;



void *cadtop(char *str) {
    void *p;
    sscanf(str, "%p", &p);
    return p;
}

void Recursiva (int n)
{
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        Recursiva(n-1);
}
char *RecursivaChar(int n) {
    char *dinamico = (char *)malloc(TAMANO);

    if (n > 0)
        Recursiva(n - 1);

    return dinamico;
}

void CmdRecursiva(char *tr[]){
    int n;

    if(tr[1] == NULL){
        printf("Recursive needs an argument (int) to be executed\n");
    }else{
        n = atoi(tr[1]);
        Recursiva(n);
    }

}

void LlenarMemoria (void *p, size_t cont, unsigned char byte)
{
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    for (i=0; i<cont;i++)
        arr[i]=byte;
}

void * ObtenerMemoriaShmget (key_t clave, size_t tam, tListMem *L)
{
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;
    tItemMem item;
    //time_t t = time(NULL);
    //struct tm *time = localtime(&t);

    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE)  /*no nos vale*/
    {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);
    item.key = clave;
    item.size = s.shm_segsz;
    strcpy(item.type, "SHARED");
    //strftime(item.date, 512, "%Y-%m-%d %H:%M:%S", time);
    item.dir = p;

    if(p != NULL){
        if(insertItemMem(item, L))
            printf("File allocated %d in %p %lu bytes ", item.key, p, item.size);
        else
            perror("Cannot allocate");
    }else{
        perror("Cannot insert");
    }
    /** Guardar en la lista, p.e.  InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    return (p);
}

void SharedCreate (char *tr[], tListMem *L)
{
    key_t cl;
    size_t tam;
    void *p;
    tPosM mp;
    tItemMem item;

    if (tr[0]==NULL || tr[1]==NULL) {
        printListShared(*L);
        /** ImprimirListaShared(&L); **/
        return;
    }
    if(strcmp("-create", tr[1]) == 0) {
        cl = (key_t) strtoul(tr[2], NULL, 10);
        tam = (size_t) strtoul(tr[3], NULL, 10);

        if (tam == 0) {
            printf("No se asignan bloques de 0 bytes\n");
            return;
        }
        if ((p = ObtenerMemoriaShmget(cl, tam, L)) != NULL)
            printf("Asignados %lu bytes en %p\n", (unsigned long) tam, p);
        else
            printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long) cl, strerror(errno));

    }

    if(strcmp("-free", tr[1]) == 0){
        cl = (key_t) strtoul(tr[2], NULL, 10);
        mp = firstMem(*L);

        while (mp != NULL) {
            item = getItemMem(mp, *L);

            if (cl == item.key && strcmp("SHARED", item.type) == 0) {
                if (shmget(item.key, 0, 0) == 0 && shmdt(item.dir) == -1) {
                    perror("Cannot deallocate");
                } else {
                    int shmid = shmget(item.key, 0, 0);
                    shmctl(shmid, IPC_RMID, NULL);
                    printf("Shared type block %s key: %d deallocated %p\n", item.type, item.key, item.dir);
                    deleteAtPositionMem(mp, L);
                }
                break;
            }

            mp = nextMem(mp, *L);
        }
    }

    if(strcmp("-delkey", tr[1]) == 0){
        if(SharedDelkey(&tr[2]) == 1){
            cl = (key_t) strtoul(tr[2], NULL, 10);
            mp = firstMem(*L);
            printf("Key %d deleted\n", cl);

            while(mp != NULL){
                item = getItemMem(p, *L);

                if(item.key == cl && strcmp("SHARED", item.type) == 0){
                    shmdt(item.dir); //detach el seg memoria de shmat
                }

                mp = nextMem(mp, *L);
            }
        }
        else{
            perror("Cannot delete the key");
        }
    }

}

void * MapearFichero (char * fichero, int protection)
{
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
        return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
        return NULL;

/* Guardar en la lista    InsertarNodoMmap (&L,p, s.st_size,df,fichero); */
    return p;
}

void CmdMmap(char *arg[], tListMem *L)
{
    char *perm;
    void *p;
    int protection=0;
    tItemMem item;
    struct stat file_info;
    tPosM mp;
    lstat(arg[1], &file_info);

    if (arg[1]==NULL){
        printListMmap(*L);
        return;
    }

    if(strcmp(arg[1], "-free") != 0) {

        if ((perm = arg[2]) != NULL && strlen(perm) < 4) {
            if (strchr(perm, 'r') != NULL) protection |= PROT_READ;
            if (strchr(perm, 'w') != NULL) protection |= PROT_WRITE;
            if (strchr(perm, 'x') != NULL) protection |= PROT_EXEC;
        }
        if ((p = MapearFichero(arg[1], protection)) == NULL) {
            perror("Imposible mapear fichero");
        } else if (p != NULL) {
            item.dir = p;
            item.size = file_info.st_size;
            strcpy(item.type, "MMAP");
            item.descriptor = open(arg[1], O_CREAT);
            strcpy(item.name, arg[1]);

            if (insertItemMem(item, L)) {
                printf("fichero %s mapeado en %p\n", arg[1], p);
            } else {
                printf("No se pudo mapear\n");
            }
        }
    }
    else if(strcmp(arg[1], "-free") == 0){
        mp = firstMem(*L);

        while(mp != NULL){
            item = getItemMem(mp, *L);

            if(strcmp(item.name, arg[2]) == 0){

                if(munmap(item.dir, item.size) != -1){
                    close(item.descriptor);
                    printf("Unmapped file %s %zu size\n", item.name, item.size);
                    deleteAtPositionMem(mp, L);
                    return;
                }else{
                    perror("Cannot unmap");
                }
            }else{
                perror("File not mapped");
            }
            mp = nextMem(mp, *L);
        }
    }

}

int SharedDelkey (char *args[])
{
    key_t clave;
    int id;
    char *key=args[0];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey necesita clave_valida\n");
        return -1;
    }
    if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return -1;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1) {
        perror("shmctl: imposible eliminar id de memoria compartida\n");
        return -1;
    }

    return 1;

}

ssize_t EscribirFichero (char *f, void *p, size_t cont,int overwrite)
{
    ssize_t  n;
    int df,aux, flags=O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
        flags=O_CREAT | O_WRONLY | O_TRUNC;

    if ((df=open(f,flags,0777))==-1)
        return -1;

    if ((n=write(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

void CmdWrite(char *tr[]){
    if (tr[1] == NULL || tr[2] == NULL || tr[3] == NULL) {
        printf("Missed arguments\n");
        return;
    }

    char *filename;
    size_t dataLength;
    char *data = NULL;
    void *p;
    int overwrite = 0;  // Por defecto, no sobrescribir

    // Verificar si tr[1] es igual a "-o"
    if (strcmp(tr[1], "-o") == 0) {
        overwrite = 1;
        filename = tr[2];
        data = tr[3];
        dataLength = atoi(tr[4]);
    } else {
        filename = tr[1];
        data = tr[2];
        dataLength = atoi(tr[3]);
    }

    p = cadtop(data);

    ssize_t result = EscribirFichero(filename, p, dataLength, overwrite);

    if (result == -1) {
        perror("Error al escribir en el fichero");
    } else {
        printf("Se han escrito %zd bytes en el fichero %s\n", result, filename);
    }
}

ssize_t LeerFichero (char *f, void *p, size_t cont)
{
    struct stat s;
    ssize_t  n;
    int df,aux;

    if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
        return -1;
    if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont=s.st_size;
    if ((n=read(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

void CmdRead (char *ar[])
{
    void *p;
    size_t cont=-1;  /* -1 indica leer todo el fichero*/
    ssize_t n;
    if (ar[1]==NULL || ar[2]==NULL){
        printf ("faltan parametros\n");
        return;
    }

    p=cadtop(ar[2]);  /*convertimos de cadena a puntero*/

    if (ar[2]!=NULL) {
        if (ar[3] == NULL) {
            cont = 100; //default
        }else {
            cont = (size_t) atoi(ar[3]);
        }
    }

    if ((n=LeerFichero(ar[1],p,cont))==-1)
        perror ("Imposible leer fichero");
    else
        printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[1],p);
}

void Do_MemPmap (void) /*sin argumentos*/
{ pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){ /*proceso hijo*/
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        exit(1);
    }
    waitpid (pid,NULL,0);
}

void newMalloc(char *tr[], tListMem *L){
    tItemMem item;
    //time_t t = time(NULL);
    //struct tm *time = localtime(&t);
    int tam;
    tPosM mp;

    if(tr[1] == NULL){
        printListMalloc(*L);
        return;
    }

    if(strcmp(tr[1], "-free") != 0) {
        tam = atof(tr[1]);

        if (tam == 0){
            perror("Cannot malloc with 0 bytes");
        }
        else {
            item.size = tam;
            item.dir = malloc(item.size);
            strcpy(item.type, "MALLOC");
            //strftime(item.date, 512, "%Y-%m-%d %H:%M:%S", time);

            if (item.dir == NULL) {
                perror("Error bad address");
            } else if (insertItemMem(item, L)) {
                printf("New data allocated (malloc) tam: %zu dir %p\n", item.size, item.dir);
            } else {
                perror("Cannot allocate");
            }
        }
    }
    else if(strcmp(tr[1], "-free") == 0){
        tam = atof(tr[2]);

        if(tam == 0){
            printf("Cannot deallocate");
        }else{
            mp = firstMem(*L);

            while(mp != NULL){
                item = getItemMem(mp, *L);
                if(item.size == tam){
                    if(item.dir == NULL){
                        perror("Bad address");
                    }else{
                        printf("Deleted block with size %zu in %p\n",item.size, item.dir);
                        free(item.dir);
                        item.dir = NULL;
                        deleteAtPositionMem(mp, L);
                    }
                }
                mp = nextMem(mp, *L);
            }

        }
    }
}

void MemDump(void *address, size_t numBytes) {
    unsigned char *ptr = (unsigned char *)address;

    for (size_t i = 0; i < numBytes; i++) {
        printf("%c - %02X       ", (ptr[i] >= 32 && ptr[i] <= 126) ? ptr[i] : '.', ptr[i]);

        if ((i + 1) % 6 == 0) {
            printf("\n");
        }
    }

    if (numBytes % 6 != 0) {
        printf("\n");
    }
}

void newMemDump(char *tr[]) {
    if (tr[1] == NULL || tr[2] == NULL) {
        printf("Missed arguments\n");
        return;
    }

    void *address = (void *)strtoul(tr[1], NULL, 16);
    size_t numBytes = (size_t)strtoul(tr[2], NULL, 10);

    MemDump(address, numBytes);
}

void newMemFill(char *tr[]) {
    void *address;
    size_t numBytes;
    unsigned char fillByte;

    // Parse the memory address or use the default address
    if (tr[1] == NULL || (address = (void *)strtol(tr[1], NULL, 16)) == NULL) {
        printf("Invalid memory address. Using default address: 0x %s\n", RecursivaChar(1));
        address = (void *) RecursivaChar(1);
    }

    // Parse the number of bytes or use the default value
    if (tr[2] == NULL) {
        printf("Number of bytes not provided. Using default value: 128\n");
        numBytes = 128;
        fillByte = 'A';
        LlenarMemoria(address, numBytes, fillByte);
        printf("Memory filled at address %p with %zu bytes of value 0x%02X\n", address, numBytes, fillByte);
        return;

    } else {
        numBytes = (size_t)atof(tr[2]);

        if(numBytes > 1000){
            printf("Too much bytes\n");
            return;
        }
    }

    // Parse the fill byte or use the default value
    if (tr[3] == NULL) {
        printf("Fill byte not provided. Using default value: A\n");
        fillByte = 'A';
    } else {
        fillByte = (unsigned char)strtol(tr[3], NULL, 0);
    }

    // Perform memory fill
    LlenarMemoria(address, numBytes, fillByte);

    printf("Memory filled at address %p with %zu bytes of value 0x%02X\n", address, numBytes, fillByte);
}

void memProcess(char *tr[], tListMem M){
    int showBlocks = 0;
    int showFuncs = 0;
    int showVars = 0;
    int showPmap = 0;

    int localInt = 1;
    char localChar = 'A';
    double localDouble = 3.3;

    static int staticlnt = 2;
    static char staticChar = 'B';
    static double staticDouble = 33.3;

    static int staticlnt2;
    static char staticChar2;
    static double staticDouble2;

    if(tr[1] == NULL || strcmp(tr[1], "-all") == 0){
        printf("V.L %p, %p, %p\n", &localInt, &localChar, &localDouble);
        printf("V.G %p, %p, %p\n", &globalInt, &globalChar, &globalDouble);
        printf("V.E %p, %p, %p\n\n", &staticlnt, &staticChar, &staticDouble);
        printf("Funciones programa %p, %p, %p\n", &chDir, &authors, &create); //funciones de la p0
        printf("Funciones libreria %p, %p, %p\n\n", &puts, &free, &perror);
        Do_MemPmap();
        printf("\n");
        printListMem(M);
        return;
    }
    else if(strcmp(tr[1], "-funcs") == 0){
        showFuncs = 1;
    }
    else if(strcmp(tr[1], "-blocks") == 0){
        showBlocks = 1;
    }
    else if(strcmp(tr[1], "-pmap") == 0){
        showPmap = 1;
    }
    else if(strcmp(tr[1], "-vars") == 0){
        showVars = 1;
    }else{
        perror("Not a valid option");
    }

    if(showBlocks){
        printListMem(M);
        return;
    }
    if(showPmap){
        Do_MemPmap();
    }
    if(showFuncs){
        printf("Funciones programa %p, %p, %p\n", &chDir, &authors, &create); //funciones P0 y P1
        printf("Funciones libreria %p, %p, %p\n", &puts, &free, &perror);
    }
    if(showVars){
        printf("V.L %p, %p, %p\n", &localInt, &localChar, &localDouble);
        printf("V.G %p, %p, %p\n", &globalInt, &globalChar, &globalDouble);
        printf("V.E %p, %p, %p\n", &staticlnt, &staticChar, &staticDouble);
        printf("V.G N.I %p %p %p\n", &globalInt2, &globalChar2, &globalDouble2);
        printf("V.E N.I %p %p %p\n", &staticlnt2, &staticChar2, &staticDouble2);
    }

}

