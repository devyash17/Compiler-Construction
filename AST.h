/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */
#include"parserDef.h"

int useful(int node);
parseTree createAST(parseTree root);
void buildAST(parseTree ast,parseTree root);
void createASTUtils(parseTree curr, parseTree par);
void copy(parseTree dst,parseTree src);
void nodeCount(parseTree root,int* ans);
void nodeCountAST(parseTree root,int* ans);
void printAST(parseTree root);
