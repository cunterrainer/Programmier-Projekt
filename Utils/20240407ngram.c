#include <stdio.h>
#include <ctype.h>

#define ALPHABET 52

typedef struct{
    int character;
    double probability;
}Next;



int main(){
//intiazile an array of the next struct for each letter in the alphabet

    Next letters[256];
        for (int i=0;i<=255;i++){

            //if the letter doent exist, skip it
            if(letters[i].character==NULL){
                continue;
            
            }

            for(int j=65;j<=255;j++){
                letters[i].character=j;
         
                //letters[i].probability;
                printf("probability of %c appearing after %c ",letters[i].character,j);
            }
        }

//what shpuld I do with the space and !,. etc?

//counting how many letters are in the text


//read the text file

// Open a file in read mode
    FILE *pF = fopen("SampleText.txt", "r");

    //we need a container for the text(one line at a time?)

    char container[255];
    while(fgets(container,255,pF)!=NULL){

        if(pF == NULL) {
            printf("Not able to open the file.");
        }

    }
//counting the whole number of letters in the text(for next time.how should i deal with the spaces etc)
        fseek(pF,0,SEEK_END);
    long TextSize=ftell(pF);


// Print some text if the file does not exist


    fclose(pF);

    return 0;

}