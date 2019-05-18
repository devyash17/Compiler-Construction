/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */

#include "Stack.h"


Stack newStack(){
	Stack s = malloc(sizeof( struct stack));
	s->head = NULL;
	s->tail = NULL;
	s->count = 0;
	return s;
}


void insert_at_front(Stack s, Element e){
	if(s->count == 0){
		s->head = e;
		s->tail=e;
		e->next = NULL;
		s->count++;
	}
	else{
		e->next = s->head;
		s->head= e;
		s->count++;
	}
}
void insert_at_end(Stack s, Element e){
	if(s->count == 0){
		s->head = e;
		s->tail=e;
		e->next = NULL;
		s->count++;
	}
	else{
		s->tail->next = e;
		s->tail=e;
		e->next = NULL;
		s->count++;
	}
}

void delete_at_front(Stack s){
	if(s->count == 0){
		printf("empty stack\n");
	}
	else if(s->count == 1){
		Element curr = s->head;
		s->head = NULL;
		s->tail = NULL;
		free(curr);
		s->count = 0;
	}
	else if(s->count > 1){
		Element enext = s->head->next;
		Element curr = s->head;
		s->head = enext;
		curr->next = NULL;
		free(curr);
		s->count--;
	}
}

Key top1(Stack s){
	if(s->head != NULL){return s->head->k;}
	else{
		printf("Empty Stack\n");
		return NULL;
	}
}

		
Element newElement(Key k)
{
	Element a = malloc(sizeof(struct element));
	a->k = k;
	a->next=NULL;
	return a;

}		   

Key newKey(int id, parseTree pt){
	Key k = malloc(sizeof(struct key));
	(k)->id = id;
	(k)->parent = pt;
	return k;
}
		
void push(Stack s, int id, parseTree pt){
	Key key = newKey(id, pt);
	Element e = newElement(key);
	insert_at_front(s, e);
}

void pop(Stack s){
	delete_at_front(s);
}		
			
		
