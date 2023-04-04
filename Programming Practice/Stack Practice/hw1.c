#include <stdio.h>
#define STACK_SIZE 20

void push(int*, int);
int pop(int*);
void print_stack(int*);

int stk[STACK_SIZE];
int top= -1;

int main(){
  push(stk,1);
  push(stk,3);
  push(stk,5);
  push(stk,7);
  print_stack(stk);
  return 0;
}

void push(int* stck, int n){
  if(top==STACK_SIZE -1){
    printf("\nStackoverflow");
    return;
  }
    stck[++top]=n;
  }
int pop(int* stck){
  if(top== STACK_SIZE-1){
    printf("\nEmpty");
    return 0;
  }
  int j=stck[top];
  stck[top--]=0;
  return j;
}

void print_stack(int* stck){
  while(top!=-1)
  printf("\n%d",pop(stck));
}

