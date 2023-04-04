#include <stdio.h>

int main(){
char input[100];
int k,result=0;

scanf("%s",input);
for(k=0;input[k]!='\0';k++)
  result=result+input[k]-'0';
printf("%d",result);
}