#include <stdio.h>

int main(){
int size=0;

scanf("%d",&size);
int matrix[size][size];
for(int k=0;k<size;k++){ 
  
  
  for(int j=0;j<size;j++){
    scanf("%d",&matrix[k][j]);


  }

}

printf("\n");
int temp=0;
if (size%2!=0){
for(int k=0;k<(size/2)+1;k++){ 
  
  for(int j=0;j<size;j++){
   if(k==(size/2)&& j==(size/2))
   break;
   temp=matrix[k][j];
   matrix[k][j]=matrix[size-1-k][size-1-j];
   matrix[size-1-k][size-1-j]=temp;
 
  }

}
}
else
{
for(int k=0;k<(size/2);k++){ 
  
  for(int j=0;j<size;j++){
   if(k==(size/2)&& j==(size/2))
   break;
   temp=matrix[k][j];
   matrix[k][j]=matrix[size-1-k][size-1-j];
   matrix[size-1-k][size-1-j]=temp;
  
 
  }

}
}

for(int k=0;k<size;k++){ 
  
  for(int j=0;j<size;j++){
    printf("%d ",matrix[k][j]);

  }



printf("\n");
}

}