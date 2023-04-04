#include <stdio.h>
#include <math.h>
int main()
{
 
    long bin1, bin2;
    int i = 0, rem = 0, result[15];
 
    printf("input 1:");
    scanf("%ld", &bin1);
    printf("input 2:");
    scanf("%ld", &bin2);
    while (bin1 != 0 || bin2 != 0)
    {
        result[i] =(bin1% 10 + bin2% 10 + rem) % 2;
        i++;
        rem =(bin1% 10 + bin2% 10 + rem) / 2;
        bin1 = bin1 / 10;
        bin2 = bin2 / 10;
    }
    if (rem != 0){
        result[i] = rem;
        i++;}
        
    --i;
    printf("Result: ");
    
 
int dec=0;

    
    for (int j=i; j>=0;j--){
        printf("%d", result[j]);
      
       dec=dec+(result[j]*pow(2,j));
        }
    

    printf(" (%d)\n",dec);
    printf("%d",result[0]);
    return 0;
}