#include <stdio.h>


int main(){
  int n, i;
  int bin[32];
  printf("n: ");
  scanf("%d", &n);
int bina;
bina=n;
  for(int j=31; j>=0; j--){
    bin[j]=bina & 1;
    bina>>=1;
  }
  printf("<2진수>\n");
  for(i=0; i<32; i++){
    if(i%8==0){
    printf(" ");
  }
  printf("%d", bin[i]);

  }

  int k=1,a,b;
  long int c=(long) n;
  c=n;

  char hexa[100];

while(c!=0){
 
  a= c%16;
  if( a<10)
    a=a+48;
    else
    a=a+55;

    
  
  hexa[k++]=a;
  c=c/16;
}
printf("%d",k);
printf("\n<16진수>\n");
for (b = k-1; b > 0; b--)

printf("%c", hexa[b]);

    return 0;
}
