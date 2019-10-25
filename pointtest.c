#include<stdio.h>
#include<stdlib.h>
void fun(char **i);
int main(){
    char *i = malloc(10);
    i = "123456789";
    fun(&i);
    if(i = NULL){
        printf("I is null\n");
    }else{
        printf("I is not null\n");
    }


}
void fun(char **i){
    free(i);
    *i = NULL;
}