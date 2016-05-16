/**
 * \file String.h
 * \brief Primitives functions for strings
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * All the basic functions to manage dynamic strings.
 *
 * Copyright 2014-2016
 *
 */

#ifndef EXTLIB_STRING_H
#define EXTLIB_STRING_H

#include "Common.h"

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
 * \return nothing.
 *
 */
void stringDel(String str);

/** \brief Destroys a string and returns the allocated C string.
 *
 * \param str : string to destroy.
 * \return c string
 *
 */
char *stringDelKeepCStr(String str);



/** \brief Duplicates a string.
 *
 * \param str : string to duplicate.
 * \return New string.
 *
 */
String stringClone(String str);

/** \brief Returns a new string representing the substring of str from start to end.
 *
 * \param str : string to duplicate.
 * \param start : start position (included).
 * \param end : end position (excluded).
 * \return New string.
 *
 */
String stringSubString(String str, int start, int end);



/** \brief Trims a string to a substring of str from start to end.
 *
 * \param str : string to trim.
 * \param start : start position (included).
 * \param end : end position (excluded).
 * \return nothing.
 *
 */
void stringTrim(String str, int start, int end);



/** \brief Empties a string.
 *
 * \param str : string to clear.
 * \return nothing.
 *
 */
void stringClear(String str);



/** \brief Determinates whether a string is empty or not.
 *
 * \param str : a string.
 * \return true if str is empty, false otherwise.
 *
 */
bool stringIsEmpty(String str);

/** \brief Returns the length of a string.
 *
 * \param str : a string.
 * \return length of str.
 *
 */
int stringLength(String str);



/** \brief Returns the internal C string of a string.
 *
 * \param str : a string.
 * \return the internal C string of str.
 *
 */
const char *stringCStr(String str);

/** \brief Returns the character at given position in a string.
 *
 * \param str : a string.
 * \param pos : position in the string.
 * \return the character in str at pos.
 *
 */
char stringGet(String str, int pos);



/** \brief Appends a C string to a string.
 *
 * \param str : a string.
 * \param cStr : a C string.
 * \return nothing.
 *
 */
void stringAppend(String str, const char *cStr);

/** \brief Appends a string to another string.
 *
 * \param str : a string.
 * \param str2 : a string.
 * \return nothing.
 *
 */
void stringAppendString(String str, const String str2);

/** \brief Appends a character to a string.
 *
 * \param str : a string.
 * \param c : a character.
 * \return nothing.
 *
 */
void stringAppendChar(String str, const char c);



/** \brief Details the heap usage of a given string
 *
 * \param str : String to dump
 * \return void
 *
 */
void stringDump(String str);



// Iteration

/** \brief makes an action for every character of the string
 *
 * \param str : String to iterate.
 * \param actFct : Pointer to a function called for each element of the string.
 * \param infos : pointer broadcasted to actFct. Useful to share additional informations to the function.
 * \return nothing.
 *
 */
void stringForEach(String str, ElActFct actFct, Ptr infos);

#endif
