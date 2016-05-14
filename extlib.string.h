/**
 * \file extlib.string.h
 * \brief Primitives functions for strings
 * \author Jason Pindat
 * \version 1.0
 * \date 09/13/2015
 *
 * All the basic functions to manage dynamic strings.
 *
 */

#ifndef EXTLIB_STRING_H
#define EXTLIB_STRING_H

#include "extlib.def.h"

/** String : type for a string. */
typedef struct _String *String;


/** \brief Creates a new string.
 *
 * \return New empty string.
 *
 */
String stringNew();

/** \brief Destroys a string.
 *
 * \param str : string to destroy.
 * \return void
 *
 */
void stringDel(String str);

/** \brief Destroys a string and returns the allocated C string.
 *
 * \param str : string to destroy.
 * \return void
 *
 */
char *stringDelKeepCStr(String str);

/** \brief Returns a new string representing the substring of str from start to end.
 *
 * \param str : string to destroy.
 * \param start : start position.
 * \param end : end position.
 * \return New string.
 *
 */
String stringSubString(String str, int start, int end);

/** \brief Duplicates a string.
 *
 * \param str : string to duplicate.
 * \return New string.
 *
 */
String stringCopy(String str);

String stringTrim(String str, int start, int end);

String stringClear(String str);

int stringLength(String str);

const char *stringCStr(String str);

String stringAppend(String str, const char *cStr);
String stringAppendString(String str, const String str2);
String stringAppendChar(String str, const char c);

void stringHeap(String str);


#endif
