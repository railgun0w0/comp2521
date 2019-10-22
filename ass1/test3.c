#include <stdio.h>
#include <string.h>

int strpos(char *source, char *search, int pos){
	// pos start from 0
	if (strstr(source,search) == NULL) return -1;
	int i = pos;
	int sourcelen = strlen(source);
	int searchlen = strlen(search);
	int j;
	int found = 0;
    int check;
    int count;
	while(i < sourcelen){
		j = 0;
		if(source[i] == search[j]){
            check = i;
            count = 0;
			while(j < searchlen){
				if(source[i + j] == search[j]){
                    count++;
                }	
				j++;
			}
            if(count == searchlen) return check;
		}
		i++;
	}
    return -1;	
}

int main(){

    printf("%d",strpos("abcdefabcdefabcdef asdfgavcabcddd","abc",10));
    printf("%d",strpos("123456781AS2","1AS2",0));
    return 0;
}
