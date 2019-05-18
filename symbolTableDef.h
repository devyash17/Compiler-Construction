/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AST.h"
#include"parser.h"

typedef struct paramInfo
{
	bool isInputPar;
	bool isOutputPar;
	int parIndex;
}paramInfo;

typedef struct record
{
	char* rname;
	char* type;
	int lineNo;
	struct record* next;
}record;

typedef struct TableLoc
{
	char* type;
	char* varname;
	int size;
	int offset;
	record* ptr;
	paramInfo* param;
	struct TableLoc *next;
}TableLoc;

typedef struct hashtable
{
	TableLoc* entry;
}hashtable;

typedef struct tablePointer
{
	TableLoc** localTable;
	char* fname;
	int numVar;
	int fSize;
	int numInpPar;
	int numOutPar;
	record* inParList;
	record* outParList;
	record* variables;
}tablePointer;

typedef struct symboltable
{
	tablePointer** fTable;
	int numFunc;
	record* functions;
}symboltable;

typedef symboltable* symbolTable;


