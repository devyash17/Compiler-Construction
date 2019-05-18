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
#include"symbolTableDef.h"


record* insertAtEnd(record* ptr,record *temp);
int hashVal(char* key, int size);
symbolTable createSymbolTable(int size);
TableLoc* createEntry(char* type,char* varname,int size,int offset,record* ptr,paramInfo* param);
tablePointer* createTablePointer(char* fname,int size);
void fillGlobalUtils2(TableLoc* table, parseTree ast);
void fillGlobalUtils1(tablePointer* tp, parseTree ast);
void fillGlobalRecords(tablePointer* tp, parseTree ast);
void fillGlobalIds(tablePointer* tp,parseTree ast);
char* extractDatatype(parseTree datatype);
void addStmtVarUtils2(symbolTable st,tablePointer* tp,parseTree ast,int pos);
void addStmtVarUtils1(symbolTable st,tablePointer* tp,parseTree ast,int pos) ;
void addStmtVar(symbolTable st,tablePointer* tp,parseTree ast,int pos);
void addParameters(symbolTable st, parseTree ast, int pos, paramInfo* param);
void fillFuncIdsUtils2(symbolTable st,parseTree ast,char* key);
void fillFuncIdsUtils1(symbolTable st,parseTree ast) ;
void fillFuncVar(symbolTable st,parseTree ast) ;
void printSymbolTable(symbolTable st);
int checkExistence(TableLoc* presentList,char* newEntry);
void addEntryAtEnd(TableLoc** presentList,TableLoc* newEntry);
void globalRecordsUpdate(parseTree ast,symbolTable st,tablePointer* tp);
void globalIdsUpdate(parseTree ast,symbolTable st,tablePointer* tp);
symbolTable fillSymbolTable(parseTree ast);
void printTable(tablePointer* tp);
void printFuncVariables(record* temp,tablePointer* tp);
void printGlobalVariables(symbolTable st);
void printFnameAndSizes(symbolTable st);
void printRecDefAndSize(symbolTable st);
//void printRecord(tablePointer* tp)







