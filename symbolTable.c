/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */


#include <string.h>
#include <stdlib.h>
#include "symbolTable.h"

#define TABLE_SIZE 10000
int GLOBAL_OFFSET ;

//function to add at the end of linked list of record type
record* insertAtEnd(record* ptr,record *temp)
{
	if(ptr==NULL)
	{
		ptr=temp;
		return temp;
	}
	record *iter=ptr,*prev=ptr;
	while(iter != NULL)
	{
		if(!strcmp(iter->rname,temp->rname))
		{
			printf("Line %d : %s field declared already in this record\n",temp->lineNo,temp->rname);
			return ptr;
		}
		prev = iter;
		iter = iter->next;
	}
	prev->next=temp;
	return ptr;
}

// function to check whether variable already exists or not in localTable
int checkExistence(TableLoc* presentList,char* newEntry)
{
	//TableLoc* temp = presentList;
	while(presentList)
	{
		if(strcmp(presentList->varname,newEntry)==0)
			return 1;		//variable entry already exists
		presentList = presentList->next;		
	}
	return 0;		//variable entry does not exist already
}

//adding new entry with same hash value in linked list at same hash position for same function (scope)
void addEntryAtEnd(TableLoc** presentList,TableLoc* newEntry)
{
	TableLoc *temp = *presentList,*prev = *presentList;
	if(temp == NULL)
	{
		*presentList = newEntry;
		return;
	}
	while(temp !=NULL)
	{
		prev = temp;
		temp = temp->next;
	}
	prev->next = newEntry;
	return;
}

//hash function
int hashVal(char* key, int size)
{
	long long int sum = 0;
	for(int i=0;i<strlen(key);i++){
		sum = sum*10 + (long long int)(key[i] - '0');
		if(sum<0)
			sum=0;
	}
	int hashvalue = sum % size;
	return hashvalue;
}
//creating the symbol table
symbolTable createSymbolTable(int size)
{
	symbolTable st = malloc(sizeof(symboltable));
	st->numFunc = 0;
	st->functions = NULL;
	st->fTable = malloc(size*sizeof(tablePointer*));

	for(int i=0;i<size;i++)
	st->fTable[i] = NULL;

	return st;
}
//creating local table entry for each function
TableLoc* createEntry(char* type,char* varname,int size,int offset,record* ptr,paramInfo* param)
{
//	printf("entry\n");
	TableLoc* entry = malloc(sizeof(Table));
	entry->type = type;
	entry->varname = varname;
	entry->size = size;
	entry->offset = offset;
	entry->ptr = ptr;
	entry->param = param;
	entry->next = NULL;
	return entry;
}
//creating a function entry for first layer hashing
tablePointer* createTablePointer(char* fname,int size)
{
	tablePointer* tp = malloc(sizeof(tablePointer));
	tp->localTable = malloc(size*sizeof(Table*));
	for(int i=0;i < size; i++)
	tp->localTable[i] = NULL;
	tp->fname = fname;
	tp->numVar = 0;
	tp->fSize = 0;
	tp->numInpPar = 0;
	tp->numOutPar = 0;
	tp->inParList = NULL;
	tp->outParList = NULL;
	tp->variables = NULL;
	return tp;
}

//---------------------------------------------------------------------------------------
// for global record defn to be taken from typeDefinitions
void fillGlobalUtils2(TableLoc* table, parseTree ast) //ast=fieldDefs or moreFields
{
	if(!table || !ast)
	return ;
	if(ast->nonTerminal == fieldDefinition)
	{
		record* ptr = malloc(sizeof(record));
		ptr->rname = ast->children[1].terminal->lexeme;
		ptr->type = ast->children[0].terminal->lexeme;
		ptr->lineNo = ast->children[1].terminal->lineNum;
		ptr->next = NULL;
		table->size += (ptr->type=="int")? 2:4;
		table->ptr = insertAtEnd(table->ptr,ptr);
		return;
	}
	for(int i=0;i< ast->numChildAST;i++)
	{
	fillGlobalUtils2(table,&(ast->children[i]));
	}
}

void fillGlobalUtils1(tablePointer* tp, parseTree ast)  //ast=typedef
{
	if(!tp || !ast)
	return ;
	TableLoc* table = tp->localTable[hashVal(ast->children[1].terminal->lexeme,TABLE_SIZE)];
	if(checkExistence(table,ast->children[1].terminal->lexeme))
	{
		printf("line %lld : record %s being global, cannot be declared more than once.\n",ast->children[1].terminal->lineNum,ast->children[1].terminal->lexeme);
		return ;
	}
	table = createEntry("record",ast->children[1].terminal->lexeme,0,GLOBAL_OFFSET,NULL,NULL);
	addEntryAtEnd(&(tp->localTable[hashVal(ast->children[1].terminal->lexeme,TABLE_SIZE)]),table);

	record* varPtr = malloc(sizeof(record));	
	varPtr->rname = ast->children[1].terminal->lexeme;
	varPtr->type = NULL;
	varPtr->next = NULL;
	
	tp->variables = insertAtEnd(tp->variables,varPtr);
	
	tp->numVar++;
	fillGlobalUtils2(table,&(ast->children[2]));
	GLOBAL_OFFSET += table->size;
}

void fillGlobalRecords(tablePointer* tp,parseTree ast) //ast=typeDefs
{
	if(!ast)
	return;
	fillGlobalUtils1(tp,&(ast->children[0]));
	if(ast->numChildAST==2)
	fillGlobalRecords(tp,&(ast->children[1]));
}

//------------------------------------------------------------------------------------
void fillGlobalIds(tablePointer* tp,parseTree ast) //ast=declarations
{
	if(!tp || !ast)
	return ;
	parsetree decl = ast->children[0];
	if(decl.numChildAST == 3)
	{
		TableLoc* table;
		if(decl.children[0].nonTerminal == -1)  //INT or REAL
		{
			table = tp->localTable[hashVal(decl.children[1].terminal->lexeme,TABLE_SIZE)];
			if(checkExistence(table,decl.children[1].terminal->lexeme))
			{
				printf("line %lld : Variable %s being a global variable, cannot be declared more than once.\n",decl.children[1].terminal->lineNum,decl.children[1].terminal->lexeme);
				return ;
			}
			char* type = (decl.children[0].terminal->tokenType == TK_INT)? "int":"real";
			int size = (type == "int")? 2:4;
			table = createEntry(type,decl.children[1].terminal->lexeme,size,GLOBAL_OFFSET,NULL,NULL);
			addEntryAtEnd(&(tp->localTable[hashVal(decl.children[1].terminal->lexeme,TABLE_SIZE)]),table);
			GLOBAL_OFFSET += size;

			record* varPtr = malloc(sizeof(record));	
			varPtr->rname = decl.children[1].terminal->lexeme;
			varPtr->type = NULL;
			varPtr->next = NULL;
			
			tp->variables = insertAtEnd(tp->variables,varPtr);
			
			tp->numVar++;
		}
		else  // RECORD INSTANCE
		{
			table = tp->localTable[hashVal(decl.children[0].children[0].children[1].terminal->lexeme,TABLE_SIZE)];
			if(!checkExistence(table,decl.children[0].children[0].children[1].terminal->lexeme))
			{
				printf("line %lld : constructed datatype %s is not defined.\n",decl.children[0].children[0].children[1].terminal->lineNum, decl.children[0].children[0].children[1].terminal->lexeme);
				return;
			}
			char* type = decl.children[0].children[0].children[1].terminal->lexeme;
			int size = table->size;
			record* ptr = table->ptr;

			TableLoc* temp = createEntry(type,decl.children[1].terminal->lexeme,size,GLOBAL_OFFSET,ptr,NULL);
			addEntryAtEnd(&(tp->localTable[hashVal(decl.children[1].terminal->lexeme,TABLE_SIZE)]),temp);
			GLOBAL_OFFSET += size;
			
			record* varPtr = malloc(sizeof(record));	
			varPtr->rname = decl.children[1].terminal->lexeme;
			varPtr->type = NULL;
			varPtr->next = NULL;
			
			tp->variables = insertAtEnd(tp->variables,varPtr);
			
		}
	}
	if(ast->numChildAST == 2)
	fillGlobalIds(tp,&(ast->children[1]));
}

//---------------------------------------------------------------------------
char* extractDatatype(parseTree datatype)
{
	if(!datatype)
	return NULL;
	return datatype->children[0].children[1].terminal->lexeme;
}

void addStmtVarUtils2(symbolTable st,tablePointer* tp,parseTree ast,int pos)  //ast = declaration
{
	if(!ast || !st || !tp)
	{
		return;
	}
	if(ast->numChildAST == 3)
	return ;
	TableLoc* temp = st->fTable[hashVal("global",TABLE_SIZE)]->localTable[hashVal(ast->children[1].terminal->lexeme,TABLE_SIZE)];
	if(checkExistence(temp,ast->children[1].terminal->lexeme))
	{
		printf("line %lld : %s record definition is already declared globally.\n",ast->children[1].terminal->lineNum,ast->children[1].terminal->lexeme);
		return ;
	}
	TableLoc* table = tp->localTable[hashVal(ast->children[1].terminal->lexeme,TABLE_SIZE)];
	if(checkExistence(table,ast->children[1].terminal->lexeme))
	{
		printf("line %lld : %s already declared in the function.\n",ast->children[1].terminal->lineNum,ast->children[1].terminal->lexeme);
		return;
	}
	char* type;
	int size;
	if(ast->children[0].nonTerminal == dataType)
	{
		type = extractDatatype(&(ast->children[0]));
		temp = st->fTable[hashVal("global",TABLE_SIZE)]->localTable[hashVal(type,TABLE_SIZE)];
		if(!checkExistence(temp,type))
		{
			printf("line %lld : constructed datatype %s is not defined.\n",ast->children[0].children[0].children[1].terminal->lineNum,type);
			return ;
		}
		
		size = temp->size;
		table = createEntry(type,ast->children[1].terminal->lexeme,size,tp->fSize,temp->ptr,NULL);
	}
	else{
		type = ast->children[0].terminal->lexeme ;
		size = (ast->children[0].terminal->tokenType == TK_INT)? 2:4;
		table = createEntry(type,ast->children[1].terminal->lexeme,size,tp->fSize,NULL,NULL);
	}
	addEntryAtEnd(&(tp->localTable[hashVal(ast->children[1].terminal->lexeme,TABLE_SIZE)]),table) ;
	
	record* varPtr = malloc(sizeof(record));	
	varPtr->rname = ast->children[1].terminal->lexeme;
	varPtr->type = NULL;
	varPtr->next = NULL;
	
	tp->variables = insertAtEnd(tp->variables,varPtr);
	
	tp->numVar++;
	tp->fSize += size;
}

void addStmtVarUtils1(symbolTable st,tablePointer* tp,parseTree ast,int pos) //ast = declarations
{
	addStmtVarUtils2(st,tp,&(ast->children[0]),pos);
	if(ast->numChildAST == 2)
	addStmtVarUtils1(st,tp,&(ast->children[1]),pos);
}

void addStmtVar(symbolTable st,tablePointer* tp,parseTree ast,int pos)  //ast = stmts
{
	if(!ast || !tp)
	return ;
	for(int i = 0;i < ast->numChildAST; i++)
	if(ast->children[i].nonTerminal == declarations)
	addStmtVarUtils1(st,tp,&(ast->children[i]),pos);
}

//-----------------------------------------------------------------------------------------------------------------------------
//adding input and output parameters of a function in localTable of function(scope)
void addParameters(symbolTable st, parseTree ast, int pos, paramInfo* param) //ast = parameter_list
{
		int hashPos = hashVal(ast->children[1].terminal->lexeme,TABLE_SIZE);
		//considering if any collisions is there
		int flag = 0;
		if(checkExistence(st->fTable[hashVal("global",TABLE_SIZE)]->localTable[hashPos],ast->children[1].terminal->lexeme))
		{
			printf("line %lld : variable %s being declared is already declared as a global variable.\n",ast->children[1].terminal->lineNum,ast->children[1].terminal->lexeme);
			flag = 1;
			//return;
		}
		else if(checkExistence(st->fTable[pos]->localTable[hashPos],ast->children[1].terminal->lexeme))
		{
			printf("line %lld : variable %s being declared is already in this scope.\n",ast->children[1].terminal->lineNum,ast->children[1].terminal->lexeme);
			flag = 1;
			//return;
		}
		//if(flag==1)
			//return;
		
		if(!flag)
		{
			if(param->isInputPar)
				param->parIndex = st->fTable[pos]->numInpPar;
			else if(param->isOutputPar)
				param->parIndex = st->fTable[pos]->numOutPar;
			
			st->fTable[pos]->numVar++;
			
			TableLoc* t1;
			char str1[] = "int"; char str2[] = "real";
			record *parListPtr = malloc(sizeof(record));
			
			if(ast->children[0].nonTerminal == dataType)
			{
				char *c = extractDatatype(&(ast->children[0]));	
				int rhashValue = hashVal(c,TABLE_SIZE);
				TableLoc* tl = st->fTable[hashVal("global",TABLE_SIZE)]->localTable[rhashValue] ;
				
				if(!checkExistence(tl,c))
				{
					printf("line %lld : constructed datatype %s is not defined.\n",ast->children[0].children[0].children[1].terminal->lineNum,c);
					return;
				}		
				/*if(tl == NULL)     //to check whether datatype declared already or not
				{
					printf("line %lld : constructed datatype %s is not defined.\n",ast->children[0].children[0].children[1].terminal->lineNum,c);
					return;
				}*/
				int rSize = tl->size;
				t1 = createEntry(c, ast->children[1].terminal->lexeme, rSize, st->fTable[pos]->fSize, tl->ptr, param);
				//st->fTable[hashVal("global",TABLE_SIZE)]->localTable[rhashValue] = t1;
				st->fTable[pos]->fSize += rSize;
				if(param->isInputPar)
				{
					parListPtr->rname = ast->children[1].terminal->lexeme;
					parListPtr->type = c;
					parListPtr->lineNo = ast->children[1].terminal->lineNum;
					st->fTable[pos]->inParList = insertAtEnd(st->fTable[pos]->inParList,parListPtr);
					st->fTable[pos]->numInpPar += 1;
				}
				else if(param->isOutputPar)
				{
					parListPtr->rname = ast->children[1].terminal->lexeme;
					parListPtr->type = c;
					parListPtr->lineNo = ast->children[1].terminal->lineNum;
					st->fTable[pos]->outParList = insertAtEnd(st->fTable[pos]->outParList,parListPtr);
					st->fTable[pos]->numOutPar += 1;
				}				
			}
			else if(strcmp(ast->children[0].terminal->lexeme,"int") == 0)
			{
				t1 = createEntry("int", ast->children[1].terminal->lexeme, 2, st->fTable[pos]->fSize, NULL, param);
				st->fTable[pos]->fSize += 2;
				if(param->isInputPar)
				{
					parListPtr->rname = ast->children[1].terminal->lexeme;
					parListPtr->type = str1;
					parListPtr->lineNo = ast->children[1].terminal->lineNum;
					st->fTable[pos]->inParList=insertAtEnd(st->fTable[pos]->inParList,parListPtr);
					st->fTable[pos]->numInpPar += 1;
				}
				else if(param->isOutputPar)
				{
					parListPtr->rname = ast->children[1].terminal->lexeme;
					parListPtr->type = str1;
					parListPtr->lineNo = ast->children[1].terminal->lineNum;
					st->fTable[pos]->outParList=insertAtEnd(st->fTable[pos]->outParList,parListPtr);
					st->fTable[pos]->numOutPar += 1;
				}
			}
			else
			{
				t1 = createEntry("real", ast->children[1].terminal->lexeme, 4, st->fTable[pos]->fSize, NULL, param);
				st->fTable[pos]->fSize += 4;
				if(param->isInputPar)
				{
					parListPtr->rname = ast->children[1].terminal->lexeme;
					parListPtr->type = str2;
					parListPtr->lineNo = ast->children[1].terminal->lineNum;
					st->fTable[pos]->inParList=insertAtEnd(st->fTable[pos]->inParList,parListPtr);
					st->fTable[pos]->numInpPar += 1;
				}
				else if(param->isOutputPar)
				{
					parListPtr->rname = ast->children[1].terminal->lexeme;
					parListPtr->type = str2;
					parListPtr->lineNo = ast->children[1].terminal->lineNum;
					st->fTable[pos]->outParList=insertAtEnd(st->fTable[pos]->outParList,parListPtr);
					st->fTable[pos]->numOutPar += 1;
				}
			}
			//considering if any collisions are there
			addEntryAtEnd(&(st->fTable[pos]->localTable[hashPos]),t1);
		}		
		if(ast->numChildAST == 3)
		{
			paramInfo *param1;
			if(param->isInputPar)
			{
				param1 = malloc(sizeof(paramInfo));
				param1->isInputPar = true;
				param1->isOutputPar = false;
				param1->parIndex = 0;
			}
			else if(param->isOutputPar)
			{
				param1 = malloc(sizeof(paramInfo));
				param1->isInputPar = false;
				param1->isOutputPar = true;
				param1->parIndex = 0;
			}
			addParameters(st, &(ast->children[2].children[0]), pos, param1);
		}
		return;
}

//adding entries for new functions not declared already and _mainF
void fillFuncIdsUtils2(symbolTable st,parseTree ast,char* key)  //ast = function or mainF
{
        int pos = hashVal(key,TABLE_SIZE);        
        if(st->fTable[pos] != NULL)
        {
        		if(ast->nonTerminal == function)
                	printf("line %lld : function %s already declared before.\n",ast->children[0].terminal->lineNum,key);
                else
                	("_main function declared twice");
                return;
        }
        
        record* varPtr = malloc(sizeof(record));	
		varPtr->rname = key;
		varPtr->type = NULL;
		varPtr->next = NULL;
	
	    st->functions = insertAtEnd(st->functions,varPtr);
        
		//printf("%s\n",key);
		tablePointer* tp = createTablePointer(key,TABLE_SIZE);
		ast->tp = tp;
		st->fTable[pos] = tp;   
		int i;
		if(ast->nonTerminal == function)
		{
			for(i=0; i < ast->numChildAST; i++)
			{
				if(ast->children[i].nonTerminal == input_par)
				{
					paramInfo *param = malloc(sizeof(paramInfo));
					param->isInputPar = true;
					param->isOutputPar = false;
					param->parIndex = 0;
					addParameters(st, &(ast->children[i].children[0]), pos, param);
				}
				else if( ast->children[i].nonTerminal == output_par)
				{
					paramInfo *param = malloc(sizeof(paramInfo));
					param->isInputPar = false;
					param->isOutputPar = true;
					param->parIndex = 0;
					addParameters(st, &(ast->children[i].children[0]), pos, param);
				}
			}
		}
		
		if(ast->children[ast->numChildAST-1].nonTerminal == stmts)
		{
			addStmtVar(st,tp, &(ast->children[ast->numChildAST-1]), pos);
		}
		return;
}

//adding entries if otherFunctions are present
void fillFuncIdsUtils1(symbolTable st,parseTree ast)  // ast = otherFunctions
{
        if(!st || !ast)
                return;

        fillFuncIdsUtils2(st,&(ast->children[0]),ast->children[0].children[0].terminal->lexeme);
     
        if(ast->numChildAST == 2)
        fillFuncIdsUtils1(st,&(ast->children[1]));
        return;
}
//fill symbol table for different functions present
void fillFuncVar(symbolTable st,parseTree ast)  // ast=program
{
	if(!st || !ast)
		return ;

	for(int i=0;i < ast->numChildAST ;i++)
	{
		if(ast->children[i].nonTerminal == otherFunctions)
		{
			fillFuncIdsUtils1(st,&(ast->children[0]));
		}
		else
		{
			fillFuncIdsUtils2(st, &(ast->children[i]), "_main");
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
void globalRecordsUpdate(parseTree ast,symbolTable st,tablePointer* tp)
{
	if(!ast || !st)
	return ;
	if(ast->nonTerminal == typeDefinitions)
	{
		fillGlobalRecords(tp,ast);
		return;
	}
	for(int i = 0;i < ast->numChildAST;i++)
	globalRecordsUpdate(&(ast->children[i]),st,tp);
}

void globalIdsUpdate(parseTree ast,symbolTable st,tablePointer* tp)
{
	if(!ast || !st)
	return ;
	if(ast->nonTerminal == declarations)
	{
		fillGlobalIds(tp,ast);
		return;
	}
	for(int i = 0;i < ast->numChildAST;i++)
	globalIdsUpdate(&(ast->children[i]),st,tp);
}
/*
void printRecord(tablePointer* tp)
{
	record* in = tp->inParList;
	record* out = tp->outParList;
	printf("\n\n\nINPUT LIST:\n");
	while(in)
	{
		printf("%s %s\n",in->rname,in->type);
		in = in->next;
	}
	in = out;
	printf("\n\n\nOUTPUT LIST:\n");
	while(in)
	{
		printf("%s %s\n",in->rname,in->type);
		in = in->next;
	}
}*/

//filling symbol table
symbolTable fillSymbolTable(parseTree ast)
{
	symbolTable st = createSymbolTable(TABLE_SIZE);
	tablePointer* tp = createTablePointer("global",TABLE_SIZE);
	st->fTable[hashVal("global",TABLE_SIZE)] = tp;
	
    record* varPtr = malloc(sizeof(record));	
	varPtr->rname = "global";
	varPtr->type = NULL;
	varPtr->next = NULL;
    st->functions = insertAtEnd(st->functions,varPtr);
	
	globalRecordsUpdate(ast,st,tp);
	globalIdsUpdate(ast,st,tp);
	fillFuncVar(st,ast);
	
	return st;		
}

//printing only global variables and their offsets
void printGlobalVariables(symbolTable st)
{
	tablePointer* tp = st->fTable[hashVal("global",TABLE_SIZE)];
	if(!tp)
		return;
	char *lexeme,*type,*scope;
	int offset;
	scope = tp->fname;
	record *temp;
	temp = tp->variables;
	printf("\nlexeme		type		scope		offset\n\n");
	while(temp != NULL)
	{
		char *c = temp->rname;
		if(c[0] == '#')
		{
			temp = temp->next;
			continue;
		}
		TableLoc* table;
		table = tp->localTable[hashVal(c,TABLE_SIZE)];
		if(!table)
		{
			temp = temp->next;
			continue;
		}
		while(table)
		{
			lexeme = table->varname;
			type = table->type;
			offset = table->offset;
			printf("%s		",lexeme);
			if(table->ptr != NULL)
			{
				record *rPtr;
				rPtr = table->ptr;
				while(rPtr != NULL)
				{
					printf("%s",rPtr->type); //print int,int,real
					if(rPtr->next != NULL)
						printf(" x ");
					rPtr=rPtr->next;
				}							
				printf("		%s		%d\n",scope,offset);
			}
			else
			{
				printf("%s		%s		%d\n",type,scope,offset);
			}
			table = table->next;
		}
		temp = temp->next;
	}
}

//print function variables in order of their offset
void printFuncVariables(record* temp,tablePointer* tp)
{
	
	char *lexeme,*type,*scope;
	int offset;
	scope = tp->fname;
	while(temp != NULL)
	{
		char *c = temp->rname;
		TableLoc* table;
		table = tp->localTable[hashVal(c,TABLE_SIZE)];
		if(!table)
			continue;
		while(table)
		{
			lexeme = table->varname;
			offset = table->offset;
			type = table->type;
			printf("%s		%s		%s		%d\n",lexeme,type,scope,offset);
			table = table->next;
		}
		temp = temp->next;
	}
}

//printing functions in order of which they are present in the test file
void printTable(tablePointer* tp)
{
	if(!tp)
		return;
	int flag=0;
	if(strcmp(tp->fname,"global")==0)
		flag=1;

	record *temp;

	switch(flag)
	{
		case 0:	
				printFuncVariables(tp->inParList,tp);
				printFuncVariables(tp->outParList,tp);
				printFuncVariables(tp->variables,tp);
				break;
		case 1:
				;
				char *lexeme,*type,*scope;
				int offset;
				scope = tp->fname;
				temp = tp->variables;
				while(temp != NULL)
				{
					char *c = temp->rname;
					TableLoc* table;
					table = tp->localTable[hashVal(c,TABLE_SIZE)];
					if(!table)
						continue;
					while(table)
					{
						lexeme = table->varname;
						type = table->type;
						printf("%s		",lexeme);
						if(table->ptr != NULL)
						{
							record *rPtr;
							rPtr = table->ptr;
							while(rPtr != NULL)
							{
								printf("%s",rPtr->type); //print int,int,real
								if(rPtr->next != NULL)
									printf(" x ");
								rPtr=rPtr->next;
							}							
							printf("		%s		-\n",scope);
						}
						else
						{
							printf("%s		%s		-\n",type,scope);
						}
						table = table->next;
					}
					temp = temp->next;
				}
				break;
	}
	printf("\n");
}		


//print complete symbol table
void printSymbolTable(symbolTable st)
{
	if(!st)
	return ;
	printf("\nlexeme		type		scope		offset\n\n");
	record *temp;
	temp = st->functions;
	while(temp != NULL)
	{
		char *c = temp->rname;
		printTable(st->fTable[hashVal(c,TABLE_SIZE)]);
		temp = temp->next;
	}
}

//print function name and its size
void printFnameAndSizes(symbolTable st)
{
	printf("\nfunctionName		size\n\n");
	record *temp;
	temp = st->functions;
	tablePointer* tp;
	while(temp != NULL)
	{
		char *c = temp->rname;
		if(strcmp(c,"global")==0)
		{
			temp = temp->next;
			continue;
		}
		tp = st->fTable[hashVal(c,TABLE_SIZE)];
		printf("%s		%d\n",tp->fname,tp->fSize);
		temp = temp->next;
	}
}

//function to print record definitions and their size along with their type
void printRecDefAndSize(symbolTable st)
{
	tablePointer* tp = st->fTable[hashVal("global",TABLE_SIZE)];
	if(!tp)
		return;
	char *lexeme;
	int width;
	record *temp;
	temp = tp->variables;
	int i=0;
	printf("\nrecordName		typeExpression		width\n\n");
	while(temp != NULL)
	{
		char *c = temp->rname;
		if(c[0] != '#')
		{
			temp = temp->next;
			continue;
		}
		TableLoc* table;
		table = tp->localTable[hashVal(c,TABLE_SIZE)];
		if(table == NULL)
		{
			temp = temp->next;
			continue;
		}
		while(table != NULL)
		{
			lexeme = table->varname;
			width = table->size;
			
			printf("%s		",lexeme);
			if(table->ptr != NULL)
			{
				record *rPtr;
				rPtr = table->ptr;
				while(rPtr != NULL)
				{
					printf("%s",rPtr->type); //print int,int,real
					if(rPtr->next != NULL)
						printf(" x ");
					rPtr=rPtr->next;
				}							
				printf("		%d\n",width);
			}
			table = table->next;
		}
		temp = temp->next;
	}
}


