#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int shuffledArr[800];
int finalThreadArray[800];
int firstThreadArray[300];
int secondThreadArray[500];

 typedef struct Arrays{
    int arraySize;
    int *array;
}arrays;

void instantiateArray(){

    int numberPool[2000];
    size_t firstThreadArrayLenght = 300;
    size_t secondThreadArrayLenght = 500;

    for(int i = 0; i< 2000; i++ ){
        numberPool[i] = i;
    }

    for(int i = 0; i< 800 -1 ; i++){
            size_t j = i + rand() / (RAND_MAX / (2000 - i) + 1);
            int t = numberPool[j];
            numberPool[j] = numberPool[i];
            numberPool[i] = t;
    }
    for(int j = 0; j < 800; j++){
        shuffledArr[j] = numberPool[j];
        //printf("%d %d\n",j, shuffledArr[j]);
    }
    for(int i = 0; i<300;i++){
        firstThreadArray[i] = shuffledArr[i];
    }
    for(int i = 300; i<800;i++){
        secondThreadArray[i-300] = shuffledArr[i];
    }
}

void *Sort(void *array){
    arrays *arr = (arrays*)array;

    for(int i=0;i<(arr->arraySize-1);i++){
        for(int j=0;j<(arr->arraySize-i-1);j++){
            if(arr->array[j]>arr->array[j+1]){
                int temp = arr->array[j];
                arr->array[j]=arr->array[j+1];
                arr->array[j+1] = temp;
            }
        }
    }

    for(int i = 0; i<arr->arraySize;i++){
        printf("%d %d\n",i,arr->array[i]);
    }
    pthread_exit(NULL);
}
void *Merge(){
    int i= 0,j=0,k=0;
    while(i<500 && j< 300){
        if(firstThreadArray[j]<secondThreadArray[i]){
         finalThreadArray[k++] = firstThreadArray[j++];
         }else{
         finalThreadArray[k++] = secondThreadArray[i++];
         }
    }
    while(j<300){
        finalThreadArray[k++]=firstThreadArray[j++];
    }

    while(i<500){
        finalThreadArray[k++]=secondThreadArray[i++];
    }
    pthread_exit(NULL);
}

int main()
{
    char str[2000];
    
    instantiateArray();

    pthread_t tid[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    arrays a1;
    a1.array = firstThreadArray;
    a1.arraySize = 300;
    arrays a2;
    a2.array = secondThreadArray;
    a2.arraySize = 500;

    pthread_create(&tid[0],&attr,Sort,&a1);
    pthread_join(tid[0],NULL);
    pthread_create(&tid[1],&attr,Sort,&a2);
    pthread_join(tid[1],NULL);
    pthread_create(&tid[2],&attr,Merge,NULL);
    printf("--------------------final-----------------");
    pthread_join(tid[2],NULL);

    for(int i = 0; i<800;i++){
        printf("%d %d\n",i,finalThreadArray[i]);
    }
    
    FILE *f = fopen("son.txt","w");
    for(int i=0;i<800;i++){
        fprintf(f,"%d \n",finalThreadArray[i]);
    }
    fclose(f);
    
    FILE *file = fopen("son.txt","r");
    if(file){
        while(fscanf(file,"%s",str)!=EOF){
            printf("%s\n",str);
        }
        fclose(file);
    }
    return 0;
}
