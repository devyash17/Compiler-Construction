/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */


#include <string.h>
#include<stdlib.h>
#include "lexerDef.h"
#include "parserDef.h"

#include "parserHashTable.h"


char* strRepToken[] = {"TK_ASSIGNOP","TK_COMMENT","TK_FIELDID","TK_ID","TK_NUM","TK_RNUM","TK_FUNID",
				  "TK_RECORDID","TK_WITH","TK_PARAMETERS","TK_END","TK_WHILE","TK_INT","TK_REAL",
				  "TK_TYPE","TK_MAIN","TK_GLOBAL","TK_PARAMETER","TK_LIST","TK_SQL","TK_SQR",
				  "TK_INPUT","TK_OUTPUT","TK_SEM","TK_COLON","TK_DOT","TK_COMMA","TK_ENDWHILE",
				  "TK_OP","TK_CL","TK_IF","TK_THEN","TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN",
				  "TK_PLUS","TK_MINUS","TK_MUL","TK_DIV","TK_CALL","TK_RECORD","TK_ENDRECORD",
				  "TK_ELSE","TK_AND","TK_OR","TK_NOT","TK_LT","TK_LE","TK_EQ","TK_GT","TK_GE",
				  "TK_NE","TK_EOF","eps","TK_ERROR" };
				  
char* strRepId[] = {"'<---'","TK_COMMENT","field identifier","identifier","numeric constant","real constant","function id","record id","'with'",
					"'parameters'","'end'","'while'","'int'","'real'","'type'","'_main'","'global'","'parameter'","'list'","'['","']'","'input'","'output'",
					"';'" , "':'" , "'.'" , "','", "'endwhile'", "'('" ,"')'" , "'if'" , "'then'" , "'endif'" , "'read'" , "'write'" , 
					"'return'" , "'+'" , "'-'" , "'*'" , "'/'" , "'call'" , "'record'" , "'endrecord'" , "'else'" , "'&&&'" , "'@@@'" , "'~'" , "'<'", 
					"'<='" , "'=='" , "'>'" , "'>='" , "'!='" , "end of file" , "eps","error" , "program" , "mainFunction" , "otherFunctions" , "function" , 
					"input_par" , "output_par" , "parameter_list" , "dataType" , "primitiveDatatype" , "constructedDatatype" , "remaining_list" , "stmts" , 
					"typeDefinitions" ,"typeDefinition" , "fieldDefinitions" , "fieldDefinition" , "moreFields" , "declarations" , "declaration" , 
					"global_or_not" , "otherStmts" , "stmt" , "assignmentStmt" , "singleOrRecId" , "singleOrRecIdPrime" , "funCallStmt" , "outputParameters" , 
					"inputParameters" , "iterartiveStmt" , "conditionalStmt" , "elsePart" , "ioStmt" , "allVar", "arithmeticExpression" , "expPrime" , "term" ,
					"termPrime" , "factor" , "highPrecedenceOperators" , "lowPrecedenceOperators" , "all" , "temp" , "booleanExpression" , "var" ,
					"logicalOp" , "relationalOp" , "returnStmt" , "optionalReturn" , "idList" , "more_ids"};
				  

				  
				  
int parseIdStr(char *idStr) 
{
	//return the id of string stored in hashtable
    parserTable* lookuptable = createParserHashTable(220);
    return getTokenTypePHT(idStr,lookuptable);	
}



char *idRepr(int id) 
{
		if(id>=12345 && id<=12394)
		return strRepId[id-12345+56];
		else if(id>=0 && id<=55)
		return strRepId[id];
		else
		return "";
}



char *tokenRepr(int id) 
{
	if(id >= 0 && id <=55)
		return strRepToken[id];
	else
		return "";
}


