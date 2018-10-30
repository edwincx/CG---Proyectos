#include <math.h>
#include <malloc.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv)
{

  FILE *inFile;
  inFile = fopen("input_file.txt", "a");
    if(!inFile){
    printf("Error: is_not_Ak7: Imposible abrir archivo fuente!\n"); exit(1);
    }
    srand(time(NULL)); 
    char v[10000];
    char integ[30];
    int lim = 200;
    float grados = 0.0;
 
    
    
  return 0;
}
