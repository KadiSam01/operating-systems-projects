#include <stdio.h>
#include <ctype.h>

struct word{
    char letter;
    int count;
};

struct word count;
struct word letter[256];


void count_letters(char ch){
    /*for(int i = 0; i<letter; i++){
        letter[ch].count++;
    }*/
    if(isalpha(ch)){
        ch = tolower(ch);
        letter[ch].count++;
    }
}


int main(){
    char ch;
    while(!feof(stdin)){
        scanf("%c",&ch);
        count_letters(ch);
    }
    for(int i = 0; i<256; i++){
        if(letter[i].count > 0){
            printf("%c %d\n",i,letter[i].count);
        }
    }
    return 0;
    
}