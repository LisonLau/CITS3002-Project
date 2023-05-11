// Write a C function reverse_string that takes a 
// string as input and returns the reverse of that string.
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void reverse_string(char *str1)  {  
    int i;
    int len;
    int temp;  
    len = strlen(str1);
    for (i = 0; i < len/2; i++) {  
        temp = str1[i];  
        str1[i] = str1[len - i - 1];  
        str1[len - i - 1] = temp;  
    }  
}  

int main(int argc, char const *argv[]) {
    char ans[9] = "Sunshine";
    reverse_string(ans);
    printf("%s", ans);
    return 0;
}