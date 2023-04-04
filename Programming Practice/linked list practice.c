/*
* Programming Practice prac8
* Singly Linked List 
*/
#include<stdio.h>
#include<stdlib.h>

typedef struct __node{
int data;
struct __node* next;
}node;

typedef struct __list{
  node* head;
int cnt;
}list;

void clear_list(list*);
void append_node(list*);
void insert_node(list*);  
void delete_node(list*);
void print_list(list*);
void reverse_list(); 
void sort_list();    

int main(){
  printf("\033[2J\033[H"); //clear screen
  printf("\t**week10 practice**\n");
/* init a list */
  list* L = (list*)malloc(sizeof(list));
if(!L) printf("Failed to Init.\n");
  L->head = NULL;
  L->cnt = 0;
while(1){
    printf("a : append   i : insert  d : delete\nr : reverse  s : sort    p : print\nq : quit\n");
    printf("Press key : ");    
char c = getchar();
    getchar(); // remove '\n'   
    printf("\033[2J\033[H"); //clear screen   
switch(c){
case'a' : append_node(L);  break;
case'i' : insert_node(L);  break;
case'd' : delete_node(L);  break;
case'r' : reverse_list(L); break;
case's' : sort_list(L);    break;
case'p' : print_list(L);   break;
case'q' : clear_list(L);   return 0;
default : printf("Invalid Key\n");
    }
  }
return 0;
}

void clear_list(list* L){
while(L->head){
    node* tmp = L->head;
    L->head = L->head->next;
    free(tmp);
  }
  free(L);
}

void append_node(list* L){
  node* N = (node*)malloc(sizeof(node));
if(!N){
    printf("Failed to create a node\n");
return;
  }
int n;
  printf("Data : ");
  scanf("%d", &n);
  getchar(); // remove '\n'
  N->data = n;
  N->next = L->head;
  L->head = N;
  L->cnt++;
  printf("\033[2J\033[H"); //clear screen
  printf("\t Append succeeded\n");   
}

void insert_node(list* L){
/* implement this function */
  node* N = (node*)malloc(sizeof(node));
  if(!N){
    printf("Failed to create a node\n");
return;
  }
  node* curr = L->head;
  node* prev = NULL;
  int d=0;
  int n=0;
  printf("Data: ");
  scanf("%d", &d);
  getchar();
  printf("Insert after Index(0~): ");
  scanf("%d", &n);
  getchar();
  printf("\033[2J\033[H");
  if (n> L->cnt || n < 0){
    printf("Invalid Index");
    return;}

  else if(n == 0){
  N->data= d;
  N->next = L->head;
  L->head = N;
  L->cnt++;
  }
 else {
 while (n--) {
prev = curr;
curr = curr->next;
 }
 
 N->data= d;
 N->next = curr;
 prev->next= N;
 L->cnt++;
}

}
 




void delete_node(list* L){
if(L->cnt == 0){
    printf("Empty\n");
return;
  }
int idx;
  node* curr = L->head;
  node* prev = NULL;
  printf("Index(0~) : ");
  scanf("%d", &idx);
  getchar(); // remove '\n'
  printf("\033[2J\033[H"); //clear screen
if(idx > L->cnt-1 || idx < 0){
    printf("Invalid Index\n");
return;
  }
else if(idx == 0){
    L->head = L->head->next;
    free(curr);
  }
else{    
while(idx--){
      prev = curr;
      curr = curr->next;
    }
    prev->next = curr->next;
    free(curr);
  }
  L->cnt--;
  printf("\t Delete succeeded\n");
}

void print_list(list* L){
if(L->cnt == 0){
    printf("Empty\n");
return;
  }
  node* t = L->head;
while(t){
    printf("%d ", t->data);
    t = t->next;
  }
  printf("\n");
}

void reverse_list(list* L){
/* hw */
if(L->cnt == 0){
    printf("Empty\n");
return;
  }

for (int k=0; k<L->cnt-1;k++){
  node* a = L->head;
  node* b = a->next;
  int c=0;
  for(int j=0; j<L->cnt-k-1;j++){
    if (a->data < b->data){
      c=b->data;
      b->data=a->data;
      a->data=c;
  
    }
      a=a->next;
      b=b->next;
  }
}
}


void sort_list(list* L){
/* hw */
if(L->cnt == 0){
    printf("Empty\n");
return;
  }

for (int k=0; k<L->cnt-1;k++){
  node* a = L->head;
  node* b = a->next;
 
  int c=0;
  for(int j=0; j<L->cnt-k-1;j++){
    if (a->data > b->data){
      c=b->data;
      b->data=a->data;
      a->data=c;
    
    }
      a=a->next;
      b=b->next;
  }
}
}