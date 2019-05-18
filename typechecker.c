/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "parserDef.h"
#include "lexer.h"
#include "symbolTable.h"
char *idRepr(int id) ;

typedef enum{
integer,
real,
rec,
boolean,
error
}status;

struct tablePointer* tp;
int TypeChecker(parsetree root, symbolTable s){
	//printf("idahr\n");
	//if(root.numChild == 0)
	//printf("%s\n",root.terminal->lexeme);
	//else printf("%s\n",idRepr(root.nonTerminal));
	if(root.numChildAST != 0 && (root.nonTerminal == function || root.nonTerminal == mainFunction))
	{
		//printf("%s\n",idRepr(root.nonTerminal)) ;
		tp = root.tp;
		//if(!tp)
		//printf("%s\n",idRepr(root.nonTerminal));
	} 
	int type1, type2;
	type1 = 9;
	type2 = 8;
	if(root.terminal == NULL){
		switch(root.nonTerminal){
			
			case assignmentStmt:
				//printf("idahr333\n");
				type1 = TypeChecker(root.children[0], s);
				
				type2 = TypeChecker(root.children[1], s);
				//printf("%d\t%d", type1,type2);
				if(type1 == error || type2 == error) break;
				else if(type2 != type1){
					if(root.children[0].terminal == NULL){
						printf(" line %llu:Type of left variable %s.%s is not equal to type of right expression \n",root.children[0].children[0].terminal->lineNum,root.children[0].children[0].terminal->lexeme, root.children[0].children[1].terminal->lexeme);
						//return error;
						break;
					}
					else{
						printf("line %llu:Type of left variable %s is not equal to type of right expression \n",root.children[0].terminal->lineNum ,root.children[0].terminal->lexeme);
						//return error;
						break;
					}
				}
					else{
						return type1;
					}
			case singleOrRecId:
			;
				//if(root.terminal != NULL) return root.stEntry->type;
					//if(root.children[0].stEntry == NULL) return error;
					
						// we have to access the record typedefinitions here. Fill in the required detais
				//record* rtype = tp->localTable[hashVal(root.children[0].terminal->lexeme, 10000)]->ptr;
				record* rtype;
				if(s->fTable[hashVal("global",10000)]->localTable[hashVal(root.children[0].terminal->lexeme,10000)])
				rtype = s->fTable[hashVal("global",10000)]->localTable[hashVal(root.children[0].terminal->lexeme, 10000)]->ptr;
				else if(tp->localTable[hashVal(root.children[0].terminal->lexeme, 10000)])
				rtype = tp->localTable[hashVal(root.children[0].terminal->lexeme, 10000)]->ptr;
				else break ;

				if(rtype == NULL) break;
				while(rtype != NULL){
					if(!strcmp(rtype->rname , root.children[1].terminal->lexeme)) {
						if(strcmp(rtype->type,"int")==0)
							return integer;
						else if(strcmp(rtype->type,"real")==0)
							return real;
						else 
							return rec;
						//return rtype->type;
					}
					rtype = rtype->next;
				}
			case input_par:
				break;
			case output_par:
				break;
			case declarations:
				break;
			case arithmeticExpression:
				//printf("idhar444\n");
				type1 = TypeChecker(root.children[0], s);
				type2 = TypeChecker(root.children[1], s);
				//printf("%d\t%d\n", type1, type2);
				if(type1 == error || type2 == error) break;
				else if(type2 != type1){
					parsetree last = root;
					parsetree curr = root;
					while(curr.numChildAST != 0){
						last = curr;
						curr = curr.children[0];
					}
					printf("line %llu:Type mismatch\n", curr.terminal->lineNum);
					//printf("Type mismatch at line number");
					//return error;	
					break;
				}
				else{
					return type1;
				}
			case term:
				type1 = TypeChecker(root.children[0], s);
				type2 = TypeChecker(root.children[1], s);
				if(type1 == error || type2 == error) break;//return error;
				else if(type2 != type1){
					parsetree last = root;
					parsetree curr = root;
					while(curr.numChildAST != 0){
						last = curr;
						curr = curr.children[0];
					}
					printf("line %llu:Type mismatch\n", curr.terminal->lineNum);
					//return error;
					break;
				}
				else{
					return type1;
				}
			case all:
				return TypeChecker(root.children[1], s);
			case termPrime:
			case expPrime:
				//printf("idhar555\n");
				if(root.numChildAST == 3){
					type1 = TypeChecker(root.children[1], s);
					type2 = TypeChecker(root.children[2], s);
					if(type1 == error || type2 == error) break;
					else if(type2 != type1){
						parsetree last = root;
						parsetree curr = root;
						while(curr.terminal == NULL && curr.numChildAST != 0){
							last = curr;
							curr = curr.children[0];
						}
						printf("line %llu:Type mismatch", curr.terminal->lineNum);
						break;
					}
					else{
						return type1;
					}	
				}
				else{
					type1 = TypeChecker(root.children[1], s);
					//printf("%d\n", type1);
					return type1;
				}	
			case booleanExpression:
				if(root.numChildAST == 3){
					type1 = TypeChecker(root.children[0], s);
					type2 = TypeChecker(root.children[2], s);
					if((root.children[1].terminal->tokenType == TK_AND) || (root.children[1].terminal->tokenType == TK_OR)){
						if(type1 != boolean || type2 != boolean){
							parsetree last = root;
							parsetree curr = root;
							while(curr.terminal == NULL && curr.numChildAST != 0){
								last = curr;
								curr = curr.children[0];
							}
							printf("line %llu:Logical operators can only be applied to boolean types", curr.terminal->lineNum);
						}	
					}
					else{
						if((type1!= integer &&type1!= real)||(type2!=integer&&type2!=real))
						{
							printf("line %llu: Invalid expression. Relational operations possible only on int and real\n", root.children[0].terminal->lineNum);
							//return error;
							break;
						}
						if(type1!=type2)
						{
							printf("line %llu:types of expressions should be same while comparing\n", root.children[0].terminal->lineNum);
							//return error;
							break;
						}
						return boolean;	
					}
				}
				else{
					if((type1=TypeChecker(root.children[0],s))== error)
						break;//return error;
					if(type1!= boolean)
					{
						printf("line %llu: NOT operator can only be applied to boolean types\n", root.children[0].terminal->lineNum);
					}
					return boolean;
				}	
			default:
				for(int i = 0; i < root.numChildAST; i++){
					//if(TypeChecker(root.children[i] , s) == error) break;
					TypeChecker(root.children[i] , s);
				}
		}
	}
	else{
		switch(root.terminal->tokenType){
		
			case TK_ID:
				//printf("idahr\n");
				if(tp == NULL) break;
				//printf("idahr777\n");
				
				char* temp;
				if(s->fTable[hashVal("global",10000)]->localTable[hashVal(root.terminal->lexeme, 10000)])
				temp = s->fTable[hashVal("global",10000)]->localTable[hashVal(root.terminal->lexeme, 10000)]->type;
				else if(tp->localTable[hashVal(root.terminal->lexeme, 10000)])
				temp = tp->localTable[hashVal(root.terminal->lexeme, 10000)]->type;
				else
				 {
				 	printf("Line %llu: variable %s not declared\n",root.terminal->lineNum,root.terminal->lexeme);	
				 	break ;
				 }
				
				if(strcmp(temp,"int")==0)
					return integer;
				else if(strcmp(temp,"real")==0)
					return real;
				else 
					return rec;
			case TK_NUM:
				//printf("idhar666\n");
				return integer;
			case TK_RNUM:
				//printf("idhar666\n");
				return real;
			default:
				break;

		}
	}
	
}

