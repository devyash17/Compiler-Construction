/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */

#include "parserDef.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct element* Element;
//typedef int parseTree;
typedef struct key* Key;
struct key{
	int id;
	parseTree parent;
};

struct element{
	Key k;
	Element next;
};

struct stack{
	int count;
	Element head;
	Element tail;
};
typedef struct stack* Stack;


Stack newStack();

void insert_at_front(Stack s, Element e);
void insert_at_end(Stack s, Element e);

void delete_at_front(Stack s);

void push(Stack s, int id, parseTree pt);
void pop(Stack s);
Key top1(Stack s);
Element newElement(Key k);
Key newKey(int id, parseTree pt);
