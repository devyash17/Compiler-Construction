/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */

/*
 * lexer.c: implementation of functions of lexical analyzer
 * 
 * list of functions implemented:
 *
 *		void removeComments(char* testcaseFile,char* cleanFile)
 *		FILE* getStream(FILE *fp,char *c)
 *		void printToken(tokenInfo *t)
 *		void getNextToken(FILE *fp, tokenInfo *t)
 */




#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "lexer.h"
//#include "lookUpTable.h"
//#include "BufferInput.h"

#define MAX_BUFFER_SIZE 4096 		//max buffer size --Standard Size

unsigned long long lineNum = 1;		// the "current line" in the source file
char inputBuffer[MAX_BUFFER_SIZE];	// the input buffer
int bufIndex; 				
int bufSize = -1; 					

/*
 * function to remove the comments
 * and printing the result on console 
 */
 LookUpTable* createLookUpTable(int num_pos)
{
	LookUpTable* lookUpTable = (LookUpTable*)malloc(sizeof(LookUpTable));
	lookUpTable->num_pos = num_pos;
	lookUpTable->positions = (List**)malloc(num_pos*sizeof(List*));
	for(int i=0;i<num_pos;i++){
		lookUpTable->positions[i] = (List*)malloc(sizeof(List));
		lookUpTable->positions[i]->head = NULL;
	}
	fillLookUpTable(lookUpTable);
	return lookUpTable;
}

void insertNodeElement(LookUpTable* lookUpTable, char* key, terminalId tokentype)
{
	int hashvalue =  hashFuncLUT(key,lookUpTable->num_pos);
	NodeElement* entry = (NodeElement*)malloc(sizeof(NodeElement));
	entry->type = tokentype;
	entry->lexeme = (char*)malloc(sizeof(char)*(strlen(key)+1));
	strcpy(entry->lexeme,key);
	entry->next = lookUpTable->positions[hashvalue]->head;
	lookUpTable->positions[hashvalue]->head = entry;
}

int hashFuncLUT(char* key, int num_pos)
{
	int sum = 0;

	for(int i=0;i<strlen(key);i++){
		sum = sum*10 + (key[i] - '0');
		if(sum<0)
			sum = 0;
	}

	int hashvalue = sum % num_pos;

	return hashvalue;
}

//Filling keywords in the Lookup Table
void fillLookUpTable(LookUpTable* lookUpTable)
{
	insertNodeElement( lookUpTable, "with", TK_WITH);
	insertNodeElement( lookUpTable, "parameters", TK_PARAMETERS);
	insertNodeElement( lookUpTable, "end", TK_END);
	insertNodeElement( lookUpTable, "while", TK_WHILE);
	insertNodeElement( lookUpTable, "type", TK_TYPE);
	insertNodeElement( lookUpTable, "_main", TK_MAIN);
	insertNodeElement( lookUpTable, "global", TK_GLOBAL);
	insertNodeElement( lookUpTable, "parameter", TK_PARAMETER);
	insertNodeElement( lookUpTable, "list", TK_LIST);
	insertNodeElement( lookUpTable, "input", TK_INPUT);
	insertNodeElement( lookUpTable, "output", TK_OUTPUT);
	insertNodeElement( lookUpTable, "int", TK_INT);
	insertNodeElement( lookUpTable, "real", TK_REAL);
	insertNodeElement( lookUpTable, "endwhile", TK_ENDWHILE);
	insertNodeElement( lookUpTable, "if", TK_IF);
	insertNodeElement( lookUpTable, "then", TK_THEN);
	insertNodeElement( lookUpTable, "endif", TK_ENDIF);
	insertNodeElement( lookUpTable, "read", TK_READ);
	insertNodeElement( lookUpTable, "write", TK_WRITE);
	insertNodeElement( lookUpTable, "return", TK_RETURN);
	insertNodeElement( lookUpTable, "call", TK_CALL);
	insertNodeElement( lookUpTable, "record", TK_RECORD);
	insertNodeElement( lookUpTable, "endrecord", TK_ENDRECORD);
	insertNodeElement( lookUpTable, "else", TK_ELSE);
}



terminalId getTokenType(char* key, LookUpTable* lookUpTable)
{
	int hashvalue = hashFuncLUT(key, lookUpTable->num_pos);

	NodeElement* temp = lookUpTable->positions[hashvalue]->head;

	while(temp!=NULL)
	{
		if(strcmp(temp->lexeme,key)==0)
			return temp->type;
		temp = temp->next;
	}
	return 2;
}

void removeComments(char* testcaseFile,char* cleanFile)
{
	FILE* src,*dst,*read;
	src=fopen(testcaseFile,"r");
	if (src == NULL)	//check whether testcase file is opened or not
    { 
        printf("Cannot open the test case file \n"); 
        exit(0); 
    } 
	dst=fopen(cleanFile,"w");
	if (dst == NULL)	//check whether clean file is opened or not
    { 
        printf("Cannot open the test case file \n"); 
        exit(0); 
    }	
	char c;
	int m=0,size=1000;
	char* inputBuffer=(char*)malloc(1000);
	while(!feof(src))
	{
		c=getc(src);
		if(c=='%')
		{
			while(!feof(src))
			{
				if((c=getc(src))=='\n')
				{
					inputBuffer[m++]=c;
					if(m==size)
					{
						inputBuffer=(char*)realloc(inputBuffer,2*size);
						size=2*size;
					}
					break;
				}
			}
		}
		else
		{
			if(c!=EOF)
			inputBuffer[m++]=c;
			if(m==size)
			{
				inputBuffer=(char*)realloc(inputBuffer,2*size);
				size=2*size;
			}
		}
	}
	fprintf(dst,"%s",inputBuffer);
	free(inputBuffer);
	fclose(src);
	fclose(dst);
	
	//printing the comment free code on console from file
	read=fopen(cleanFile,"r");
	if (read == NULL)	//check whether clean file is opened or not
    { 
        printf("Cannot open the test case file \n"); 
        exit(0); 
    }
    char ch;
    while (ch != EOF) 
    { 
        printf ("%c", ch); 
        ch = fgetc(read); 
    }
    fclose(read);
}



//change this function//
FILE* getStream(FILE *fp,char* c) 
{
	/* getting the next character from the input file using buffer*/

	
    if(bufSize==bufIndex)
    {
    		bufSize=fread(inputBuffer,sizeof(char),(size_t)MAX_BUFFER_SIZE,fp);
    		bufIndex=1;
    		switch(bufSize)
    		{
    			case 0:
    				*c=26;
    				return fp;
    			default:
    				*c=inputBuffer[0];
    				return fp;
    		}
    }
    
    switch(bufSize)
    {
    	case -1:
    		bufSize=fread(inputBuffer,sizeof(char),(size_t)MAX_BUFFER_SIZE,fp);
    		bufIndex=1;
    		switch(bufSize)
    		{
    			case 0:
    				*c=26;
    				return fp;
    			default:
    				*c=inputBuffer[0];
    				return fp;
    		}
    		break;
    	
    	
    	case 0:
    		*c=26;
    		return fp;
    	default:
    		*c=inputBuffer[bufIndex];
    		bufIndex++;
    		return fp;
    }		
    
    
}


//function to print the tokens on console
void printToken(tokenInfo *t) 
{
    printf("Line %llu--> %s\t\t%s", t->lineNum, tokenRepr(t->tokenType),t->lexeme);
    printf("\n");
}

void getNextToken(FILE *fp, tokenInfo *t) 
{
    
    int count = 0;
    char c;
    LookUpTable* lookUpTable = createLookUpTable(50);
    int state = 0; //starting state
	while(true)
    {
        
        fp= getStream(fp,&c);
        t->lineNum = lineNum;	//storing the line number of lexeme
		t->lexeme[count] = c; // storing each character read in lexeme 
        count++;
         

		switch(state)
		{
			case 0:
				switch(c)
				{
						case '%':
								state=1;
								break;
						case '<':
								state=2;
								break;
						case '&':
								state=5;
								break;
						case '@':
								state=7;
								break;
						case '[': // TK_SQL found
								t->tokenType = TK_SQL;
								t->lexeme[count]='\0';
								return;
						case ']': // TK_SQR found
								t->tokenType = TK_SQR;
								t->lexeme[count]='\0';
								return;
		                		case ',': // TK_COMMA found
				                t->tokenType = TK_COMMA;
				                t->lexeme[count]='\0';
								return;						
						case ';': // TK_SEM found
								t->tokenType = TK_SEM;
								t->lexeme[count]='\0';
								return;
						case ':': // TK_COLON found
								t->tokenType = TK_COLON;
								t->lexeme[count]='\0';
								return;
						case '.': // TK_DOT found
								t->tokenType = TK_DOT;
								t->lexeme[count]='\0';
						        return;
		                case '(': // TK_OP found
				                t->tokenType = TK_OP;
				                t->lexeme[count]='\0';
				                return;
		                case ')': // TK_CL found
				                t->tokenType = TK_CL;
				                t->lexeme[count]='\0';
				                return;
		                case '+': // TK_PLUS found
				                t->tokenType = TK_PLUS;
				                t->lexeme[count]='\0';
				                return;
		                case '-': // TK_MINUS found
				                t->tokenType = TK_MINUS;
				                t->lexeme[count]='\0';
				                return;
		                case '*': // TK_MUL found
				                t->tokenType = TK_MUL;
				                t->lexeme[count]='\0';
				                return;
		                case '/': // TK_DIV found
				                t->tokenType = TK_DIV;
				                t->lexeme[count]='\0';
				                return;
						case '~': // TK_NOT found
								t->tokenType = TK_NOT;
								t->lexeme[count]='\0';
								return;
		                case '=':
				                state = 9;
				                break;			        
		                case '>':
				                state = 10;
				                break;	
		                case '!':
				                state = 11;
				                break;	
		                case 'b':
		                case 'c':
		                case 'd':
				                state = 12;
				                break;
		                case 'a':
		                case 'e':
		                case 'f':
		                case 'g':
		                case 'h':
		                case 'i':
		                case 'j':
		                case 'k':
		                case 'l':
		                case 'm':
		                case 'n':
		                case 'o':
		                case 'p':
		                case 'q':
		                case 'r':
		                case 's':
		                case 't':
		                case 'u':
		                case 'v':
		                case 'w':
		                case 'x':
		                case 'y':
		                case 'z':
				                state = 13;
				                break;
		                case '0':
		                case '1':
		                case '2':
		                case '3':
		                case '4':
		                case '5':
		                case '6':
		                case '7':
		                case '8':
		                case '9':
				                state = 16;
				                break;						
				case '_':
								state = 19;
								break;
				case '#':
								state = 22;
								break;
		                case '\n':
		                    	lineNum++;
		                case ' ':
		                case '\t':
		                case '\r':
				                state = 24;
				                break;
		                case 26: // End Of File (26 is ASCII value of EOF)
				                t->tokenType = TK_EOF;
				                sprintf(t->lexeme, "End of File");
				                //t->lexeme[count]='\0';
				                return;
		                default: // invalid starting character
				                sprintf(t->lexeme, "Unknown Symbol %c", c); // storing the error message in t->lexeme
				                t->tokenType = TK_ERROR;
				               // t->lexeme[count]='\0';//---------------------------                    
		                    return;
				}
				break;		                       
		                    
			case 1:	// comment starts and read till encounter a new line
				switch(c) 
				{
		                case '\n':
		                	bufIndex--;
		                	t->lexeme[count-1]='\0';
		                	state = 0; //after end of comment return to state 0
		                    count = 0;	
		                    //lineNum++;
		                    t->tokenType = TK_COMMENT;		                                        
		                    return;
		                
		                default:
		                    ;
				}
				break;

			case 2:
				switch(c) 
				{
						case '-':
		                    state = 3;
		                    break;
		                case '=': // token TK_LE found
		                    t->tokenType = TK_LE;
		                    t->lexeme[count]='\0';
		                    return;
		                default:
		                    t->tokenType = TK_LT;                  
		                    bufIndex--; // push back the extra read character in buffer(retract)
		                    t->lexeme[count-1]='\0';		                    
		                    return;
				}
				break;
				
			case 3:
				switch(c) 
				{
		                case '-':
		                    state = 4;
		                    break;
		                default:
		                    bufIndex--;
		                    t->tokenType = TK_ERROR;	
		                    strcpy(t->lexeme, "Unknown pattern <-");// storing the error message in t->lexeme
		                    return;
				}
				break;
				
			case 4:
				switch(c) 
				{
		                case '-': // TK_ASSIGNOP found
		                    t->tokenType = TK_ASSIGNOP;
		                    t->lexeme[count]='\0';
		                    return;
		                default:
		                    bufIndex--;
		                    t->tokenType = TK_ERROR;
		                    strcpy(t->lexeme, "Unknown pattern <--");// storing the error message in t->lexeme		                    	
		                    return;
				}
				break;
				
			case 5:
				switch(c) 
				{
		                case '&':
		                    state = 6;
		                    break;
		                default:    
		                    t->tokenType = TK_ERROR;
		                    if(c=='\n')
		                    {
		                    	lineNum++;
		                    	return;
		                    }
		                    if(c=='\t' || c=='\r' || c == ' ')
		                    	return;
		                    sprintf(t->lexeme, "Unknown pattern &");// storing the error message in t->lexeme
		                    bufIndex--;
		                    return;
				}
				break;					
						
			case 6:
				switch(c) 
				{
		                case '&': // TK_AND found
		                    t->tokenType = TK_AND;
		                    t->lexeme[count]='\0';
		                    return;
		                default:
		                    sprintf(t->lexeme, "Unknown pattern &&");// storing the error message in t->lexeme
		                    t->tokenType = TK_ERROR;
		                    if(c=='\n')
		                    {
		                    	lineNum++;
		                    	return;
		                    }
		                    if(c=='\t' || c=='\r' || c == ' ')
		                    	return;
		                    bufIndex--;
		                    return;
				}
				break;							
				
			case 7:
				switch(c) 
				{
		                case '@':
		                    state = 8;
		                    break;
		                default:
		                    sprintf(t->lexeme, "Unknown pattern @");// storing the error message in t->lexeme
		                    t->tokenType = TK_ERROR;
		                    if(c=='\n')
		                    {
		                    	lineNum++;
		                    	return;
		                    }
		                    if(c=='\t' || c=='\r' || c == ' ')
		                    	return;
		                    bufIndex--;
		                    return;
				}
				break;	
		
			case 8:
				switch(c) 
				{
		                case '@': // TK_OR found
		                    t->tokenType = TK_OR;
		                    t->lexeme[count]='\0';
		                    return;
		                default:
		                    sprintf(t->lexeme, "Unknown pattern @@");
		                    t->tokenType = TK_ERROR;
		                    if(c=='\n')
		                    {
		                    	lineNum++;
		                    	return;
		                    }
		                    if(c=='\t' || c=='\r' || c == ' ')
		                    	return;
		                    bufIndex--;
		                    return;
				}
				break;	
		
			case 9:
				switch(c) 
				{
		                case '=': // TK_EQ found
		                    t->tokenType = TK_EQ;
		                    t->lexeme[count]='\0';
		                    return;
		                default:
		                    sprintf(t->lexeme,"Unknown pattern =");
		                    t->tokenType = TK_ERROR;
		                    if(c=='\n')
		                    {
		                    	lineNum++;
		                    	return;
		                    }
		                    if(c=='\t' || c=='\r' || c == ' ')
		                    	return;
		                    bufIndex--;
		                    return;
				}
				break;	
				
			case 10:
				switch(c) 
				{
		                case '=': // TK_GE found
		                    t->tokenType = TK_GE;
		                    t->lexeme[count]='\0';
		                    return;
		                default: // TK_GT found
		                    bufIndex--;
		                    t->lexeme[count-1]='\0';
		                    t->tokenType = TK_GT;
		                    return;
				}
				break;			
		
			case 11:
				switch(c) 
				{
		                case '=': // TK_NE found
		                    t->tokenType = TK_NE;
		                    t->lexeme[count]='\0';
		                    return;
		                default:
		                    sprintf(t->lexeme, "Unknown pattern !");// storing the error message in t->lexeme
		                    t->tokenType = TK_ERROR;
		                    if(c=='\n')
		                    {
		                    	lineNum++;
		                    	return;
		                    }
		                    if(c=='\t' || c=='\r' || c == ' ')
		                    	return;
		                    bufIndex--;
		                    return;
				}
				break;	
		
			case 12:
				switch(c) 
				{
		                case 'a':
		                case 'b':
		                case 'c':
		                case 'd':
		                case 'e':
		                case 'f':
		                case 'g':
		                case 'h':
		                case 'i':
		                case 'j':
		                case 'k':
		                case 'l':
		                case 'm':
		                case 'n':
		                case 'o':
		                case 'p':
		                case 'q':
		                case 'r':
		                case 's':
		                case 't':
		                case 'u':
		                case 'v':
		                case 'w':
		                case 'x':
		                case 'y':
		                case 'z':
		                    state = 13;
		                    break;
		                case '2':
		                case '3':
		                case '4':
		                case '5':
		                case '6':
		                case '7':
		                    state = 14;
		                    break;
		                default:
		                    bufIndex--;
		                    t->tokenType = getTokenType(t->lexeme,lookUpTable);
		                    t->lexeme[count-1] = '\0';//-----------------
		                    if(c=='\n' || c=='\t' || c=='\r' || c == ' ')
		                    	return;
		                    return;
				}
				break;
				
			case 13:
				switch(c) 
				{
		                case 'a':
		                case 'b':
		                case 'c':
		                case 'd':
		                case 'e':
		                case 'f':
		                case 'g':
		                case 'h':
		                case 'i':
		                case 'j':
		                case 'k':
		                case 'l':
		                case 'm':
		                case 'n':
		                case 'o':
		                case 'p':
		                case 'q':
		                case 'r':
		                case 's':
		                case 't':
		                case 'u':
		                case 'v':
		                case 'w':
		                case 'x':
		                case 'y':
		                case 'z':
		                    if(count == 21)//to check that the length is 20 characters and if greater return error
		                    { 				
		                        while(c >= 97 && c <= 122)
		                        	fp= getStream(fp,&c);
		                        bufIndex--; 
		                        t->tokenType = TK_ERROR;
		                        t->lexeme[21] = '\0';
		                        sprintf(t->lexeme, "Identifier is longer than the prescribed length of 20 characters");// storing the error message in t->lexeme
		                        return;
		                    }
		                    break;
		                
		                default:		                	
		                	bufIndex--;
		                	t->lexeme[count - 1] = '\0';
		                	t->tokenType=getTokenType(t->lexeme,lookUpTable);
		                    if(c=='\n' || c=='\t' || c=='\r' || c == ' ')
		                    	return;
		                   return;
				}
				break;
					
			case 14:
				switch(c) 
				{
		                case 'b':
		                case 'c':
		                case 'd':
		                    if(count == 21) //to check that the length is 20 characters and if greater return error
		                    {
		                        while(c >= 98 && c <= 100)
		                        	fp= getStream(fp,&c);
		                        while(c >= 50 && c <= 55) 
		                        	fp= getStream(fp,&c);
		                        bufIndex--;
		                        t->tokenType = TK_ERROR;
		                        t->lexeme[21] = '\0';
		                        sprintf(t->lexeme, "Identifier is longer than the prescribed length of 20 characters");// storing the error message in t->lexeme
		                        return;
		                    }
		                    break;
		                case '2':
		                case '3':
		                case '4':
		                case '5':
		                case '6':
		                case '7':
		                    if(count == 21) //to check that the length is 20 characters and if greater return error
		                    {
		                        while(c >= 50 && c <= 55) 
		                        	fp= getStream(fp,&c);
		                        bufIndex--;
		                        t->tokenType = TK_ERROR;
		                        t->lexeme[21] = '\0';
		                        sprintf(t->lexeme, "Identifier is longer than the prescribed length of 20 characters");// storing the error message in t->lexeme
		                        return;
		                    }
		                    state = 15;
		                    break;
		                default: // TK_ID found
		                    bufIndex--;
		                    t->lexeme[count - 1] = '\0';
		                    t->tokenType = TK_ID;
		                    if(c=='\n' || c=='\t' || c=='\r' || c == ' ')
		                    	return;
		                    return;
				}
				break;
				
				
				
			case 15:
				switch(c) 
				{
		                case '2':
		                case '3':
		                case '4':
		                case '5':
		                case '6':
		                case '7':
		                    if(count == 21)//to check that the length is 20 characters and if greater return error
		                     {
		                        while(c >= 50 && c <= 55) 
		                        	fp= getStream(fp,&c);
		                        bufIndex--;
		                        t->tokenType = TK_ERROR;
		                        t->lexeme[21] = '\0';
		                        sprintf(t->lexeme, "Identifier is longer than the prescribed length of 20 characters");// storing the error message in t->lexeme
		                        return;
		                    }
		                    break;
		                default: // TK_ID found
		                    bufIndex--;
		                    t->lexeme[count-1]='\0';
		                    t->tokenType = TK_ID;
		                    if(c=='\n' || c=='\t' || c=='\r' || c == ' ')
		                    	return;		                    
		                    t->lexeme[count - 1] = '\0';
		                    return;
				}
				break;	
						
				
			case 16:
				switch(c) 
				{
		                case '0':
		                case '1':
		                case '2':
		                case '3':
		                case '4':
		                case '5':
		                case '6':
		                case '7':
		                case '8':
		                case '9':
		                    if(count == 21) 
		                    {
		                        while(c >= 48 && c <= 57)
		                        { 
		                        	fp= getStream(fp,&c);
		                        	t->lexeme[count++]=c;
		                        }
		                        switch(c) 
		                        {
		                        	case '.':
		                        	fp= getStream(fp,&c);
		                        	t->lexeme[count++]=c;
		                            while(c >= 48 && c <= 57)
		                            {
		                            	fp= getStream(fp,&c);
		                            	t->lexeme[count++]=c;
		                            }
		                            bufIndex--;
		                            break;
		                        
		                        	default:
		                            bufIndex--;
		                            break;
		                         }

		                        t->tokenType = TK_ERROR;
		                        t->lexeme[count-1] = '\0';
		                        sprintf(t->lexeme, "%s: number longer than prescribed length", t->lexeme);
		                        return;
		                    }
		                    break;
		                case '.':
		                    state = 17;
		                    break;
		                default: // TK_NUM found
		                    bufIndex--;
		                    t->lexeme[count-1]='\0';
		                    t->tokenType = TK_NUM;
		                    if(c=='\n' || c=='\t' || c=='\r' || c == ' ')
		                    	return;			                    
		                    t->lexeme[count - 1] = '\0';
		                    return;
		            }
		            break;
	 
	 
			case 17:
				switch(c) 
				{
		                case '0':
		                case '1':
		                case '2':
		                case '3':
		                case '4':
		                case '5':
		                case '6':
		                case '7':
		                case '8':
		                case '9':
		                    state = 18;
		                    break;
		                default:
		                    bufIndex--;
		                    t->lexeme[count - 1] = '\0';
		                    sprintf(t->lexeme, "unknown pattern %s", t->lexeme);
		                    t->tokenType = TK_ERROR;
		                    return;
				}
				break; 
	 
			case 18:
				switch(c) 
				{
		                case '0':
		                case '1':
		                case '2':
		                case '3':
		                case '4':
		                case '5':
		                case '6':
		                case '7':
		                case '8':
		                case '9': // TK_RNUM found
		                	fp= getStream(fp,&c);
		                	t->lexeme[count++]=c;
		                    if(c >=48 && c<= 57)
		                    {
		                    	while(c >=48 && c<= 57)
		                    	{
		                    		fp= getStream(fp,&c);
		                    		t->lexeme[count++]=c;
		                    	}
		                    	bufIndex--;
		                    	t->tokenType = TK_ERROR;
		                    	t->lexeme[count-1] = '\0';
		                    	strcat(t->lexeme,"	:  Unknown Pattern");
		                    	
		                    	return;
		                    }
		                    bufIndex--;
		                    t->tokenType = TK_RNUM;
		                    t->lexeme[count-1] = '\0';
		                    return;
		                default:
		                    bufIndex--;
		                    t->tokenType = TK_ERROR;
		                    t->lexeme[count-1] = '\0';
		                    strcat(t->lexeme,"	:  Unknown Pattern");
		                    return;
				}
				break; 
	 
	 

			case 19:
				switch(c) 
				{
		                case 'a':
		                case 'b':
		                case 'c':
		                case 'd':
		                case 'e':
		                case 'f':
		                case 'g':
		                case 'h':
		                case 'i':
		                case 'j':
		                case 'k':
		                case 'l':
		                case 'm':
		                case 'n':
		                case 'o':
		                case 'p':
		                case 'q':
		                case 'r':
		                case 's':
		                case 't':
		                case 'u':
		                case 'v':
		                case 'w':
		                case 'x':
		                case 'y':
		                case 'z':
		                case 'A':
		                case 'B':
		                case 'C':
		                case 'D':
		                case 'E':
		                case 'F':
		                case 'G':
		                case 'H':
		                case 'I':
		                case 'J':
		                case 'K':
		                case 'L':
		                case 'M':
		                case 'N':
		                case 'O':
		                case 'P':
		                case 'Q':
		                case 'R':
		                case 'S':
		                case 'T':
		                case 'U':
		                case 'V':
		                case 'W':
		                case 'X':
		                case 'Y':
		                case 'Z':
		                    state = 20;
		                    break;
		                default:
		                    sprintf(t->lexeme, "Unknown pattern _");
		                    t->tokenType = TK_ERROR;		                    
		                    bufIndex--;
		                    return;
				}
				break; 
				
				
	 
			case 20:
				switch(c) 
				{
		                case 'a':
		                case 'b':
		                case 'c':
		                case 'd':
		                case 'e':
		                case 'f':
		                case 'g':
		                case 'h':
		                case 'i':
		                case 'j':
		                case 'k':
		                case 'l':
		                case 'm':
		                case 'n':
		                case 'o':
		                case 'p':
		                case 'q':
		                case 'r':
		                case 's':
		                case 't':
		                case 'u':
		                case 'v':
		                case 'w':
		                case 'x':
		                case 'y':
		                case 'z':
		                case 'A':
		                case 'B':
		                case 'C':
		                case 'D':
		                case 'E':
		                case 'F':
		                case 'G':
		                case 'H':
		                case 'I':
		                case 'J':
		                case 'K':
		                case 'L':
		                case 'M':
		                case 'N':
		                case 'O':
		                case 'P':
		                case 'Q':
		                case 'R':
		                case 'S':
		                case 'T':
		                case 'U':
		                case 'V':
		                case 'W':
		                case 'X':
		                case 'Y':
		                case 'Z':
		                	//fp= getStream(fp,&c);
		                	/*if((c >= 97 && c <= 122) || (c >= 65 && c <= 90))
		                	{
		                		
		                	}
		                	else
		                	{*/
		                		//bufIndex--;
		                		if(strcmp(t->lexeme, "_main") == 0)
		                	 	{
		                	 		fp= getStream(fp,&c);
		                	 		if(c=='\n' || c=='\t' || c=='\r' || c == ' ')
		                    			{	
		                        		t->tokenType = TK_MAIN;
		                       			// bufIndex--;
		                       			
		                       			
		                       
		                       	 		t->lexeme[count]='\0';
		                        		return;
		                        		}
		                        		bufIndex--;
		                     		}
		                     	
		                    	if(count == 31) 
		                    	{
		                        	while((c >= 97 && c <= 122) || (c >= 65 && c <= 90)) 
		                        		fp= getStream(fp,&c);
		                        	while(c >= 48 && c <= 57) 
		                        		fp= getStream(fp,&c);
		                        	bufIndex--;
				                t->tokenType = TK_ERROR;
				                t->lexeme[31] = '\0';
				                sprintf(t->lexeme, "Identifier is longer than the prescribed length of 30 characters");
				                return;
		                    	}
		                    break;
		                case '0':
		                case '1':
		                case '2':
		                case '3':
		                case '4':
		                case '5':
		                case '6':
		                case '7':
		                case '8':
		                case '9':
		                    if(count == 31) 
		                    {
		                        while(c >= 48 && c <= 57) 
		                        	fp= getStream(fp,&c);
		                        bufIndex--;
		                        t->tokenType = TK_ERROR;
		                        t->lexeme[31] = '\0';
		                        sprintf(t->lexeme, "Identifier is longer than the prescribed length of 30 characters");
		                        return;
		                    }
		                    state = 21;
		                    break;
		                default:
		                    t->lexeme[count - 1] = '\0';
		                    bufIndex--;		                    
		                    if(strcmp(t->lexeme, "_main") == 0)
		                       t->tokenType = TK_MAIN;
		                    else
		                        t->tokenType = TK_FUNID;
		                    return;
				}
				break; 
	 
		            			
			case 21:
				switch(c) 
				{
		                case '0':
		                case '1':
		                case '2':
		                case '3':
		                case '4':
		                case '5':
		                case '6':
		                case '7':
		                case '8':
		                case '9':
		                    if(count == 31) 
		                    {
		                        while(c >= 48 && c <= 57) 
		                        	fp= getStream(fp,&c);
		                        bufIndex--;
		                        t->tokenType = TK_ERROR;
		                        t->lexeme[31] = '\0';
		                        sprintf(t->lexeme, "Identifier is longer than the prescribed length of 30 characters");
		                        return;
		                    }
		                    break;
		                default: // TK_FUNID found
		                    bufIndex--;
		                    t->tokenType = TK_FUNID;
		                    t->lexeme[count - 1] = '\0';
		                    return;
				}
				break;			
				

			case 22:
				switch(c) 
				{
		                case 'a':
		                case 'b':
		                case 'c':
		                case 'd':
		                case 'e':
		                case 'f':
		                case 'g':
		                case 'h':
		                case 'i':
		                case 'j':
		                case 'k':
		                case 'l':
		                case 'm':
		                case 'n':
		                case 'o':
		                case 'p':
		                case 'q':
		                case 'r':
		                case 's':
		                case 't':
		                case 'u':
		                case 'v':
		                case 'w':
		                case 'x':
		                case 'y':
		                case 'z':
		                    state = 23;
		                    break;
		                default:
		                    bufIndex--;
		                    sprintf(t->lexeme, "Unknown pattern #");
		                    t->tokenType = TK_ERROR;		                   
		                    return;
				}
				break;
				
				
				
			case 23:
				switch(c) 
				{
		                case 'a':
		                case 'b':
		                case 'c':
		                case 'd':
		                case 'e':
		                case 'f':
		                case 'g':
		                case 'h':
		                case 'i':
		                case 'j':
		                case 'k':
		                case 'l':
		                case 'm':
		                case 'n':
		                case 'o':
		                case 'p':
		                case 'q':
		                case 'r':
		                case 's':
		                case 't':
		                case 'u':
		                case 'v':
		                case 'w':
		                case 'x':
		                case 'y':
		                case 'z':
		                    break;
		                default: // TK_RECORDID found
		                    bufIndex--;
		                    t->tokenType = TK_RECORDID;
		                    t->lexeme[count-1] = '\0';//------------------
		                    return;
				}
				break;					
				
				
			case 24:
				count = 0; // initialize count to zero after every token read
	            if(c == '\n')
	                lineNum++;
	            else if(c == ' ' || c == '\t' || c == '\r')
	                break;
	            else if(c == 26)
	            {
	                t->tokenType = TK_EOF;
	                sprintf(t->lexeme, "End of File");
	                return;
	            }
	            else
	            { 
	                bufIndex--;
	                state = 0;
	            }
		        break;

			default:
				; 			
				
		}
	}
}

 
