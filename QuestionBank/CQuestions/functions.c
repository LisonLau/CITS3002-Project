// Write a C function reverse_string that takes a 
// string as input and returns the reverse of that string.
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Write a C function add_nums that takes two integers as input and returns their sum.
int add_nums(int num1, int num2)  {  
    return num1 + num2;
}  

int main(int argc, char const *argv[]) {
    printf("%d",add_nums(43,50));
    return 0;
}


// Write a C function string_len that takes a string as input and returns the length of the string.
int string_len(char *input)  {  
    return strlen(input);
}  

int main(int argc, char const *argv[]) {
    printf("%d",string_len("Happiness"));   
    return 0;
}

// Write a C function sum_even that takes an array of integers as input and returns the sum of all the even numbers in the array.
int sum_even(int *numbers)  {  
    int count = 0;
    int length = sizeof(numbers);
    length /= sizeof(int);
    for (int i = 0; i <= length + 1; i++){
        if (numbers[i]%2 == 0){ 
            count += numbers[i];
        }
    }
    return count;
}  

int main(int argc, char const *argv[]) {
    int numbers[] = {1,4,5,10};
    printf("%d",sum_even(numbers)); 
    return 0;
}


// Write a C function reverse_string that takes a string as input and reverses that string.
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

// Write a C function longest_len that takes an array of strings as input and returns the length of the longest string in the array.
int longest_len(char *strs[])  {  
    int max_len = 0;
    int length = sizeof(*strs);
    for (int i = 0; i < length/2; i++){
        if (strlen(strs[i]) > max_len){
            max_len = strlen(strs[i]);
        }
    }
    return max_len;    
}  

int main(int argc, char const *argv[]) {
    char *arr[] = {"hello", "gosh", "gorgeous", "me"};
    printf("%d",longest_len(arr));
    return 0;
}