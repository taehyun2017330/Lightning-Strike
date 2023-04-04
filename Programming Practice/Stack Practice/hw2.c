#include <stdio.h>
#include <stdlib.h>

typedef struct __node{
  int data;
  struct _node* next;
}
node;
typedef struct __stack{
  node* top;
}stack;

void push(stack*, int);
int pop(stack*);
void print_stack(stack*);

int main(){
  stack* stk= (stack*)malloc(sizeof(stack));
  push(stk,1);
  push(stk,3);
  push(stk,5);
  push(stk,7);
  print_stack(stk);
  free(stk);
  return 0;
}

void push(stack* stck, int i){
 node* a= (node*)malloc(sizeof(node));
 if(!a){
   fprintf(stderr,"\nCant create node");
   return;
 }
 a->data=i;
 a->next= stck->top;
 stck->top=a;


  }
int pop(stack* stck){
  node* a=stck->top;
  if(a==NULL){
    printf("\nEmpty");
    return 0;
  }
  int j=a->data;
  stck->top= stck->top->next;
  free(a);
  return j;
}
void print_stack(stack* stck){
  if(stck->top==NULL){
    printf("\nempty");
    return;
  }
  while(stck->top!=NULL){
    printf("\n%d",pop(stck));
  }
}

