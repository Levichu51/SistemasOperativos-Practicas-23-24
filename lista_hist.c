//
// Created by levixhu on 27/11/23.
//

#include "lista_hist.h"

void createEmptyList(tList* L){
    *L = LNULL;
}

bool createNode(tPosL* p){
    *p = malloc(sizeof (struct tNode));
    return *p != LNULL;
}

bool insertItem(tItem d, tList* L) {
    tPosL q, r;

    if (!createNode(&q))
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

tPosL first(tList L){
    return L;
}

tPosL next(tPosL p, tList L){
    return p->next;
}

tPosL last(tList L){
    tPosL p;

    for (p = L; p->next != LNULL; p = p->next);
    return p;
}

tPosL previous (tPosL p, tList L){
    tPosL q;

    if (p == L)
        return LNULL;
    else {
        for (q = L; q->next != p; q = q->next);
        return q;
    }
}

bool isEmptyList(tList L ){
    return L == LNULL;
}

tPosL findItem (tItem d, tList L){
    tPosL p;

    for (p = L; (p != LNULL) && (strcmp(p->data.command, d.command) != 0); p = p->next);
    return p;
}

void updateItem(tItem d, tPosL p, tList* L){
    p->data = d;
}

void deleteList(tList* L){
    tPosL p;

    while (*L != LNULL){
        p = *L;
        *L = (*L)->next;
        free(p);
    }
}

void printList(tList L){
    tPosL p = first(L);
    tItem item;
    int i = 0;

    while(p != LNULL){
        item = getItem(p, L);
        printf("%d ", i);
        printf("%s", item.command);

        i++;
        p = next(p, L);
    }
}

tItem getItem(tPosL p, tList L){
    return p->data;
}

void deleteAtPosition(tPosL p, tList* L){
    tPosL q;

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