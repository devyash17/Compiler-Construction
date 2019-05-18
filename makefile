edit : driver.o lexer.o parser.o utils.o parserHashTable.o  Stack.o symbolTable.o AST.o typechecker.o semanticAnalyzer.o
	gcc -o compiler driver.o lexer.o parser.o utils.o parserHashTable.o Stack.o symbolTable.o AST.o typechecker.o semanticAnalyzer.o

driver.o : driver.c 
	gcc -c driver.c
lexer.o : lexer.c lexer.h lexerDef.h
	gcc -c lexer.c
parser.o : parser.c parser.h parserDef.h
	gcc -c parser.c
Stack.o : Stack.c Stack.h
	gcc -c Stack.c
utils.o : utils.c utils.h 
	gcc -c utils.c
parserHashTable.o : parserHashTable.c parserHashTable.h
	gcc -c parserHashTable.c
symbolTable.o : symbolTable.c symbolTable.h symbolTableDef.h
	gcc -c symbolTable.c
AST.o :	AST.c AST.h
	gcc -c AST.c
typechecker.o : typechecker.c
	gcc -c typechecker.c
semanticAnalyzer.o : semanticAnalyzer.c
	gcc -c semanticAnalyzer.c

clean :
	rm compiler driver.o lexer.o parser.o utils.o parserHashTable.o Stack.o symbolTable.o AST.o typechecker.o semanticAnalyzer.o
