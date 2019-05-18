/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */
 
#ifndef PARSERDEF
#define PARSERDEF

#include <stdbool.h>
#include "lexerDef.h"
//#include "symbolTable.h" 
struct tablePointer ; 
 
#define NONTERMINAL_OFFSET 12345
#define NO_OF_TERMINALS 56
#define NO_OF_NONTERMINALS 50
#define MAX_RULE_LEN 10
#define NO_OF_PRODUCTIONS 90
#define MAX_ID_SIZE 30
#define RECORD_DATATYPE 50000

typedef struct
{
	int numRules;
	int** rules;
}nonTerminal;

typedef nonTerminal* Grammar;
typedef struct{
	int* first;
} first;

typedef struct tableEntry
{
	int nonTerm;
	int productionNum;
	int syn;
}tableEntry;

typedef first* FirstSet;
typedef first* FollowSet;
typedef tableEntry Table[NO_OF_NONTERMINALS][NO_OF_TERMINALS];

typedef enum { 
	program = NONTERMINAL_OFFSET,
	mainFunction,
	otherFunctions,
	function,
	input_par,
	output_par,
	parameter_list,
	dataType,
	primitiveDatatype,
	constructedDatatype,
	remaining_list,
	stmts,
	typeDefinitions,
	typeDefinition,
	fieldDefinitions,
	fieldDefinition,
	moreFields,
	declarations,
	declaration,
	global_or_not,
	otherStmts,
	stmt,
	assignmentStmt,
	singleOrRecId,
    singleOrRecIdPrime,
	funCallStmt,
	outputParameters,
	inputParameters,
	iterativeStmt,
	conditionalStmt,
	elsePart,
	ioStmt,
	allVar,
	arithmeticExpression,
	expPrime,
	term,
	termPrime,
	factor,
	highPrecedenceOperators,
	lowPrecedenceOperators,
	all,
	temp,
	booleanExpression,
	var,
	logicalOp,
	relationalOp,
	returnStmt,
	optionalReturn,
	idList,
	more_ids
}nonTermIds;

typedef struct parsetree
{
	int numChild;
	int numChildAST;
	tokenInfo* terminal;
	int ruleNo;
	int nonTerminal;
	struct parsetree* children;
	struct tablePointer* tp;
} parsetree;
typedef parsetree* parseTree;

#endif
