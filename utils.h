/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */

/*
 * utils.h - definition of helper function
 * 
 */

#ifndef UTILS
#define UTILS

#include <stdbool.h>
#include "lexerDef.h"


int parseIdStr(char *idStr);
char *idRepr(int id);
char *tokenRepr(int id);


#endif
