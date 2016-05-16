/**
 * \file Common.c
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Common.h"

#include <stdlib.h>

/*void throwExc(char *module, char *msg) {
    fprintf(stderr, "/!\\ [%s] %s", module, msg);
    exit(1);
}*/



int _elComparePointer(Ptr v1, Ptr v2) {
    return (int)(*(Ptr*)v1-*(Ptr*)v2);
}
int _elCompareChar(Ptr v1, Ptr v2) {
    return (int)(*(char*)v1-*(char*)v2);
}
int _elCompareUchar(Ptr v1, Ptr v2) {
    return (int)(*(unsigned char*)v1-*(unsigned char*)v2);
}
int _elCompareShort(Ptr v1, Ptr v2) {
    return (int)(*(short*)v1-*(short*)v2);
}
int _elCompareUshort(Ptr v1, Ptr v2) {
    return (int)(*(unsigned short*)v1-*(unsigned short*)v2);
}
int _elCompareInt(Ptr v1, Ptr v2) {
    return (int)(*(int*)v1-*(int*)v2);
}
int _elCompareUint(Ptr v1, Ptr v2) {
    return (int)(*(unsigned int*)v1-*(unsigned int*)v2);
}
int _elCompareLong(Ptr v1, Ptr v2) {
    return (int)(*(long*)v1-*(long*)v2);
}
int _elCompareUlong(Ptr v1, Ptr v2) {
    return (int)(*(unsigned long*)v1-*(unsigned long*)v2);
}
int _elCompareLonglong(Ptr v1, Ptr v2) {
    return (int)(*(long long*)v1-*(long long*)v2);
}
int _elCompareUlonglong(Ptr v1, Ptr v2) {
    return (int)(*(unsigned long long*)v1-*(unsigned long long*)v2);
}
int _elCompareFloat(Ptr v1, Ptr v2) {
    if(*(float*)v1<*(float*)v2) return -1;
    if(*(float*)v1>*(float*)v2) return 1;
    return 0;
}
int _elCompareDouble(Ptr v1, Ptr v2) {
    if(*(double*)v1<*(double*)v2) return -1;
    if(*(double*)v1>*(double*)v2) return 1;
    return 0;
}
int _elCompareLongdouble(Ptr v1, Ptr v2) {
    if(*(long double*)v1<*(long double*)v2) return -1;
    if(*(long double*)v1>*(long double*)v2) return 1;
    return 0;
}
int _elCompareBool(Ptr v1, Ptr v2) {
    return (int)(*(bool*)v1-*(bool*)v2);
}



ElCmpFct _elCompareFct(int type) {
    switch (type) {
        case EL_POINTER   : return _elComparePointer;
        case EL_CHAR      : return _elCompareChar;
        case EL_UCHAR     : return _elCompareUchar;
        case EL_SHORT     : return _elCompareShort;
        case EL_USHORT    : return _elCompareUshort;
        case EL_INT       : return _elCompareInt;
        case EL_UINT      : return _elCompareUint;
        case EL_LONG      : return _elCompareLong;
        case EL_ULONG     : return _elCompareUlong;
        case EL_LONGLONG  : return _elCompareLonglong;
        case EL_ULONGLONG : return _elCompareUlonglong;
        case EL_FLOAT     : return _elCompareFloat;
        case EL_DOUBLE    : return _elCompareDouble;
        case EL_LONGDOUBLE: return _elCompareLongdouble;
        case EL_BOOL      : return _elCompareBool;
        default           : return NULL;
    }
}

int _elSizeFct(int type) {
    switch (type) {
        case EL_POINTER   : return sizeof(Ptr);
        case EL_CHAR      : return sizeof(char);
        case EL_UCHAR     : return sizeof(unsigned char);
        case EL_SHORT     : return sizeof(short);
        case EL_USHORT    : return sizeof(unsigned short);
        case EL_INT       : return sizeof(int);
        case EL_UINT      : return sizeof(unsigned int);
        case EL_LONG      : return sizeof(long);
        case EL_ULONG     : return sizeof(unsigned long);
        case EL_LONGLONG  : return sizeof(long long);
        case EL_ULONGLONG : return sizeof(unsigned long long);
        case EL_FLOAT     : return sizeof(float);
        case EL_DOUBLE    : return sizeof(double);
        case EL_LONGDOUBLE: return sizeof(long double);
        case EL_BOOL      : return sizeof(bool);
        default           : return type;
    }
}
