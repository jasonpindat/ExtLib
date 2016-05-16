/**
 * \file String.c
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Common.h"
#include "ExtLib/String.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFSIZE 8

struct _String {
    RealType type;

    int length;
    int capacity;
    char *ct;
};



static void stringResize(String str, int minimumNeeded) {
    do {
         str->capacity*=2;
    } while(str->capacity < minimumNeeded);

    str->ct = realloc(str->ct, str->capacity*sizeof(char));
}



String stringNew() {
    String str=malloc(sizeof(struct _String));

    str->type = STRING;

    str->length = 0;
    str->capacity = DEFSIZE;
    str->ct = malloc(DEFSIZE*sizeof(char));

    return str;
}

void stringDel(String str) {
    free(str->ct);
    free(str);
}

char *stringDelKeepCStr(String str) {
    char *cStr = (char *)stringCStr(str);
    free(str);
    return cStr;
}



String stringClone(String str) {
    String str2=malloc(sizeof(struct _String));

    str2->type = STRING;

    str2->length = str->length;
    str2->capacity = str->capacity;
    str2->ct=malloc(str2->capacity*sizeof(char));
    memcpy(str2->ct, str->ct, str2->length*sizeof(char));

    return str2;
}

String stringSubString(String str, int start, int end) {
    String str2 = malloc(sizeof(struct _String));

    str2->type = STRING;

    str2->length = end-start;

    str2->capacity = DEFSIZE;
    while(str2->capacity < end-start)
         str2->capacity *= 2;

    str2->ct = malloc(str2->capacity*sizeof(char));
    memcpy(str2->ct, str->ct+start, str2->length*sizeof(char));

    return str2;
}



void stringTrim(String str, int start, int end) {
    if(start != 0) {
        char *ct = str->ct;
        for(int i=start, j=0; i<end; i++, j++)
            ct[j] = ct[i];
    }

    str->length = end-start;
}



void stringClear(String str) {
    str->length = 0;
}



bool stringIsEmpty(String str) {
    return str->length == 0;
}

int stringLength(String str) {
    return str->length;
}



const char *stringCStr(String str) {
    if(str->capacity < str->length+1)
        stringResize(str, str->length+1);

    str->ct[str->length] = '\0';

    return str->ct;
}

char stringGet(String str, int pos) {
    return str->ct[pos];
}



void stringAppend(String str, const char *cStr) {
    int cStrLen = strlen(cStr);

    if(str->capacity < str->length+cStrLen)
        stringResize(str, str->length+cStrLen);

    memcpy(str->ct+str->length*sizeof(char), cStr, cStrLen);
    str->length += cStrLen;
}

void stringAppendString(String str, const String str2) {
    if(str->capacity < str->length+str2->length)
        stringResize(str, str->length+str2->length);

    memcpy(str->ct+str->length*sizeof(char), str2->ct, str2->length);
    str->length += str2->length;
}

void stringAppendChar(String str, const char c) {
    if(str->capacity < str->length+1)
        stringResize(str, str->length+1);

    str->ct[str->length++] = c;
}



void stringDump(String str) {
    int elts=stringLength(str);
    int effcost=elts*sizeof(char);
    int opcost=sizeof(struct _String);
    int preallcost=(str->capacity-elts)*sizeof(char);

    printf("String at %p\n", str);
    printf("\t%d characters\n", elts);
    printf("\t%d bytes used for elemets\n", effcost);
    printf("\t%d bytes used as operating cost\n", opcost);
    printf("\t%d bytes used as preallocated\n", preallcost);
    printf("\t%d bytes total used\n", effcost+opcost+preallcost);
}



// Iteration

void stringForEach(String str, ElActFct actFct, Ptr infos) {
    for(int i=0; i<str->length; i++)
        actFct(str->ct + i, infos);
}
