#include<stdio.h>
#include <stdlib.h>
#include <string.h>
int main(){
    char *p;
    char q[10] = "123456789";
    strncpy(p,q,2);
    printf("%s",p);
    printf("%s",q);
    return 0;
}