/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */
# include "parserDef.h"
#include "utils.h"
# include <stdlib.h>
# include <stdio.h>

void nodeCount(parseTree root,int* ans)
{
	if(!root)
	return ;
	*ans=*ans+root->numChild;
	for(int i=0;i<root->numChild;i++)
	nodeCount(&(root->children[i]),ans);
}

void nodeCountAST(parseTree root,int* ans)
{
	if(!root)
	return ;
	*ans=*ans+root->numChildAST;
	for(int i=0;i<root->numChildAST;i++)
	nodeCountAST(&(root->children[i]),ans);
}

int useful(int tokenClass) {
    switch(tokenClass) {
        case TK_MAIN:
        case TK_END:
        case TK_SEM:
        case TK_INPUT:
        case TK_PARAMETER:
        case TK_LIST:
        case TK_SQL:
        case TK_SQR:
        case TK_OUTPUT:
        case TK_COMMA:
        case TK_COLON:
        case TK_DOT:
        case TK_CALL:
        case TK_WITH:
        case TK_PARAMETERS:
        case TK_ASSIGNOP:
        case TK_WHILE:
        case TK_ENDWHILE:
        case TK_IF:
        case TK_ENDIF:
        case TK_OP:
        case TK_CL:
        case TK_RETURN:
        case TK_TYPE:
        case TK_ENDRECORD:
        case eps:
            return 0;
            break;
        default:
            return 1;
    }
}

void copy(parseTree dst,parseTree src)
{
	dst->numChild = src->numChild;
	dst->numChildAST = src->numChildAST;
	dst->ruleNo = src->ruleNo;
	dst->terminal = src->terminal;
	dst->nonTerminal = src->nonTerminal;
	dst->children = NULL;
	dst->tp = NULL;
}

void createASTUtils(parseTree curr, parseTree par)
{
	if(curr==NULL)
	return ; 
	if(curr -> numChildAST == 0)
	{
		if(!useful(curr->terminal->tokenType))
		par->numChildAST--;
		return;
	}
	if(curr->numChildAST == 1 && curr->children[0].numChildAST == 0)
	{
		if(useful(curr->children[0].terminal->tokenType))
		{
			copy(curr,&(curr->children[0]));
			return;
		}
	}
	
	int count = curr->numChild;
	for(int i = 0; i < count; i++)
	createASTUtils(&(curr->children[i]),curr);
	
	if(curr->numChildAST == 0)
		par->numChildAST--;
	if(curr->numChildAST == 1 && curr->children[0].numChildAST == 0)
	{
		for(int i = 0; i < curr->numChild ;i++)
		{
			if(curr->children[i].numChildAST==0 && curr->children[i].nonTerminal==-1)
			{
				if(useful(curr->children[i].terminal->tokenType))
					copy(curr,&(curr->children[i]));
			}
		}
	}
}

void buildAST(parseTree ast,parseTree root)
{
	if(!root)
	return;
	ast->children = malloc((root->numChildAST)*sizeof(parsetree));
	int m=0;
	for(int i = 0;i < root->numChild;i++) 
	{
		if(root->children[i].numChildAST!=0)
		{
			copy(&(ast->children[m]),&(root->children[i]));
			buildAST(&(ast->children[m]),&(root->children[i]));
			m++;
		}
		else
		{
			if(root->children[i].nonTerminal==-1 && useful(root->children[i].terminal->tokenType))
			{
				copy(&(ast->children[m]),&(root->children[i]));
				m++;
			}
		}
	}
}
	
parseTree createAST(parseTree root)
{
	createASTUtils(root,NULL);
	parseTree ast = malloc(sizeof(parsetree));
	copy(ast,root);
	buildAST(ast,root);
	return ast;
}

void printAST(parseTree root)
{
	parseTree current;
	int class;
	for(int i=0;i<root->numChildAST;i++)
	{
		current=&(root->children[i]);
		if(!current)
		printf("NULL\n");
		if(current->numChildAST==0 && current->terminal->tokenType==eps)
		continue;
		if(current->numChildAST>0)
		{
			printf("-------\t\t");
			printf("-------\t\t");
			printf("-------\t\t");
			printf("-------\t\t");
		}
		else{
			printf("%s\t\t",current->terminal->lexeme);
			printf("%lld\t\t",current->terminal->lineNum);
			printf("%s\t\t",tokenRepr(current->terminal->tokenType));
			if(current->terminal->tokenType==TK_NUM || current->terminal->tokenType==TK_RNUM)
				printf("%s\t\t",current->terminal->lexeme);
			else printf("-------\t\t");	
		}
		printf("%s\t\t",idRepr(root->nonTerminal));
		if(current->numChildAST==0)
		{
			printf("YES\t\t");
			printf("-------\t\t");
		}
		else{
			printf("NO\t\t");
			printf("%s\t\t",idRepr(current->nonTerminal));
		}
		printf("\n");
		printAST(current);
	}
}


