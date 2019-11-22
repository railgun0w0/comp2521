#include <stdio.h>
int main(){
    int array[3]= {0, 1, 2};
    int array1[1] = {0};
    array1[0] = array[2];
    printf("%d\n",array1[0]);
    array[2] = 1000;
    printf("array1[0] %d\n",array1[0]);
    printf("array[2] %d\n",array[2]);
}