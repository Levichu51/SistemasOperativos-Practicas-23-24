//
// Created by levixhu on 27/11/23.
//

#include "lista_mem.h"

void createEmptyListMem(tListMem * L){
    *L = LNULL;
}

bool createNodeMem(tPosM * p){
    *p = malloc(sizeof (struct tNodeMem));
    return *p != LNULL;
}

bool insertItemMem(tItemMem d, tListMem * L) {
    tPosM q, r;

    if (!createNodeMem(&q))
        return false;

    else {
        q->data = d;
        q->next = LNULL;

        if (*L == LNULL) {
            *L = q;
        } else {
            for (r = *L; r->next != LNULL; r = r->next);
            r->next = q;
        }

        return true;
    }
}

tPosM firstMem(tListMem L){
    return L;
}

tPosM nextMem(tPosM p, tListMem L){
    return p->next;
}

tPosM lastMem(tListMem L){
    tPosM p;

    for (p = L; p->next != LNULL; p = p->next);
    return p;
}

tPosM previousMem (tPosM p, tListMem L){
    tPosM q;

    if (p == L)
        return LNULL;
    else {
        for (q = L; q->next != p; q = q->next);
        return q;
    }
}

bool isEmptyListMem(tListMem L ){
    return L == LNULL;
}

tPosM findItemMem (tItemMem d, tListMem L){
    tPosM p;

    for (p = L; (p != LNULL) && (strcmp(p->data.name, d.name) != 0); p = p->next);
    return p;
}

void updateItemMem(tItemMem d, tPosM p, tListMem * L){
    p->data = d;
}

void deleteListMem(tListMem * L){
    tPosM p;

    while (*L != LNULL){
        p = *L;
        *L = (*L)->next;
        free(p);
    }
}

void printListMem(tListMem L){
    tPosM p = firstMem(L);
    tItemMem item;
    int i = 0;

    if(isEmptyListMem(L)){
        printf("Lista vacia\n");
    }

    while(p != LNULL){
        item = getItemMem(p, L);

        printf("%d ", i);
        printf("%s %zu %p\n", item.type, item.size, item.dir);

        i++;
        p = nextMem(p, L);
    }
}

void printListMalloc(tListMem L){
    tPosM p = firstMem(L);
    tItemMem item;
    int i = 0;

    if(isEmptyListMem(L)){
        printf("Lista vacia\n");
    }

    while(p != LNULL){
        item = getItemMem(p, L);
        if(strcmp(item.type, "MALLOC") == 0){
            printf("%d ", i);
            printf("%zu bytes in %p type %s\n", item.size, item.dir, item.type);
        }

        i++;
        p = nextMem(p, L);
    }
}

void printListShared(tListMem L){
    tPosM p = firstMem(L);
    tItemMem item;
    int i = 0;

    if(isEmptyListMem(L)){
        printf("Lista vacia\n");
    }

    while(p != LNULL){
        item = getItemMem(p, L);
        if(strcmp(item.type, "SHARED") == 0){
            printf("%d ", i);
            printf("%d %p %zu %s\n",item.key, item.dir, item.size, item.type);
        }

        i++;
        p = nextMem(p, L);
    }
}

void printListMmap(tListMem L){
    tPosM p = firstMem(L);
    tItemMem item;
    int i = 0;

    if(isEmptyListMem(L)){
        printf("Lista vacia\n");
    }

    while(p != LNULL){
        item = getItemMem(p, L);
        if(strcmp(item.type, "MMAP") == 0){
            printf("%d ", i);
            printf("%d %s size %zu %s\n", item.descriptor, item.name, item.size, item.type);
        }

        i++;
        p = nextMem(p, L);
    }
}

tItemMem getItemMem(tPosM p, tListMem L){
    return p->data;
}

void deleteAtPositionMem(tPosM p, tListMem * L){
    tPosM q;

    if (p == *L)
        *L = (*L)->next;

    else if (p->next == LNULL) {
        for (q = *L; q->next != p; q = q->next);
        q->next = LNULL;
    }

    else{
        q = p->next;
        p->data = q->data;
        p->next = q->next;
        p = q;
    }

    free(p);
}

