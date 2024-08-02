#include <stdio.h>
#include <string.h>  
#include <mpi.h>
#include <math.h>

float getArea(float width, float height){
  return width * height;
}

float generatingFunction(float x){
    return pow(x, 2) + 1;
}

float getDeltaX(float startX, float endX, int processSize){
    return (endX - startX)/processSize;
}

// Soma a Direita - Soma de Riemann
float rightRiemann(float startX, int itemIndex, float deltaX){
    return startX + itemIndex * deltaX;
}

int main (void) {
  
    int processSize;
    int itemIndex;
    float startX = 1;
    float endX = 10;
    float deltaX;
    float localResult;
    float finalResult = 0;
    
    MPI_Init(NULL, NULL); 
    MPI_Comm_size(MPI_COMM_WORLD, &processSize); 
    MPI_Comm_rank(MPI_COMM_WORLD, &itemIndex);
    
    deltaX = getDeltaX(startX, endX, processSize);
    
    if(itemIndex != 0){
        float rightRiemannResult = rightRiemann(startX, itemIndex, deltaX);
        localResult = generatingFunction(rightRiemannResult);
        MPI_Send(&localResult, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        return 0;
    }
    
    float sum = 0;
    float lastLocalResult = generatingFunction(endX);
    sum += lastLocalResult;
    
    for (int i = 1; i < processSize; i++){
        MPI_Recv(&localResult, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sum += localResult;
    }
    
    finalResult = (sum) * deltaX;
    
    printf("O area da curva nos intervalos %.2f a %.2f foi de: %.2f \n", startX, endX, finalResult);
    
    return 0;
}