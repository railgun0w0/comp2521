#include<stdio.h>
#include<string.h>
int main(void)
{
    char buf[]="hello\n\n\nheima";
    char*temp = strtok(buf,"\n");
    while(temp)
    {
        temp += '\n';
        printf("%s",temp);

        temp = strtok(NULL,"\n");
    }
    return 0;
}