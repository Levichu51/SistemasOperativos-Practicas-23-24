//
// Created by levixhu on 27/11/23.
//

#include "lista_procesos.h"

void createEmptyListProc(tListProc *L){
    *L = LNULL;
}

bool createNodeProc(tPosProc* p){
    *p = malloc(sizeof (struct tNodeProc));
    return *p != LNULL;
}

bool insertItemProc(tItemProc d, tListProc* L) {
    tPosProc q, r;

    if (!createNodeProc(&q))
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

tPosProc firstProc(tListProc L){
    return L;
}

tPosProc nextProc(tPosProc p, tListProc L){
    return p->next;
}

tPosProc lastProc(tListProc L){
    tPosProc p;

    for (p = L; p->next != LNULL; p = p->next);
    return p;
}

tPosProc previousProc(tPosProc p, tListProc L){
    tPosProc q;

    if (p == L)
        return LNULL;
    else {
        for (q = L; q->next != p; q = q->next);
        return q;
    }
}

bool isEmptyListProc(tListProc L ){
    return L == LNULL;
}

void updateItemProc(tItemProc d, tPosProc p, tListProc* L){
    p->data = d;
}

void deleteListProc(tListProc* L){
    tPosProc p;

    while (*L != LNULL){
        p = *L;
        *L = (*L)->next;
        free(p);
    }
}

void printListProc(tListProc L){
    tPosProc p = firstProc(L);
    tItemProc item;
    int i = 0;

    while(p != LNULL){
        item = getItemProc(p, L);
        printf("%d ", i);
        printf("Pid: %d User: %s State: %s Command: %s Date: %s", item.pid, item.usuario, item.estado, item.comando, item.date);

        i++;
        p = nextProc(p, L);
    }
}

tItemProc getItemProc(tPosProc p, tListProc L){
    return p->data;
}

void deleteAtPositionProc(tPosProc p, tListProc* L){
    tPosProc q;

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
