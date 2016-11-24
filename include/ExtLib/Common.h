/**
 * \file Common.h
 * \brief Functions and types needed by ExtLib
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * Copyright 2014-2016
 *
 */

#ifndef EXTLIB_COMMON_H
#define EXTLIB_COMMON_H

#include <stdbool.h>

typedef enum {
    ARRAY,
    SIMPLELIST,
    LIST,
    STRING,
    HEAP
} RealType;

/** Ascendant sorting */
#define EL_ASC        1
/** Descendant sorting */
#define EL_DESC       -1

/** Pointer type representation */
#define EL_POINTER    0
/** char type representation */
#define EL_CHAR       -1
/** unsigned char type representation */
#define EL_UCHAR      -2
/** short type representation */
#define EL_SHORT      -3
/** unsigned short type representation */
#define EL_USHORT     -4
/** int type representation */
#define EL_INT        -5
/** unsigned int type representation */
#define EL_UINT       -6
/** unsigned long type representation */
#define EL_LONG       -7
/** unsigned long type representation */
#define EL_ULONG      -8
/** long long type representation */
#define EL_LONGLONG   -9
/** unsigned long long type representation */
#define EL_ULONGLONG  -10
/** float type representation */
#define EL_FLOAT      -11
/** double type representation */
#define EL_DOUBLE     -12
/** long double type representation */
#define EL_LONGDOUBLE -13
/** bool type representation */
#define EL_BOOL       -14

/** Ptr : type for a generic pointer. */
typedef void *Ptr;

/** ElCmpFct : A comparison function. must take 2 generic pointers (Ptr) to objects and return an int (<0 if elt1 < elt2, =0 if elt1 = elt2, >0 if elt1> elt2) */
typedef int(*ElCmpFct)(Ptr elt1, Ptr elt2);

/** ElCopyFct : A copy function. must take 2 generic pointers (Ptr) 1st to the destination, 2nd to source and return nothing */
typedef void(*ElCopyFct)(Ptr dest, Ptr src);

/** ElDelFct : A deletion function. must take 1 generic pointer (Ptr) to the object to delete and return nothing */
typedef void(*ElDelFct)(Ptr obj);

/** ElActFct : An action function. must take 2 generic pointers (Ptr) 1st to an object, 2nd to additional informations sent by the caller and return nothing */
typedef void(*ElActFct)(Ptr obj, Ptr infos);


/* \brief Throws an error.
 *
 * \param module : Name of the module and function that throws the error.
 * \param msg : Error message.
 * \return void
 *
 */
//void throwExc(char *module, char *msg);

/** \brief Returns the comparison function from a type
 *
 * \param type : A predefined type (EL_*).
 * \return ElCmpFct : the comparison function associated to type
 *
 */
ElCmpFct _elCompareFct(int type);

//! \{
int _elSizeFct(int type);
//! \}

#endif
