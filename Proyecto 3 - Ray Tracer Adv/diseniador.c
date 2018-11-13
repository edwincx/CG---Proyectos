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
		printf("No se pudo abrir archivo fuente!\n"); exit(1);
  	}
    srand(time(NULL)); 
  	char v[10000];
  	char integ[30];
  	int lim = 200;
    float grados = 0.0;
 
  	int x =504;
  	int y = 233;
    int z = 1500;


    



    for(int i = 0;i <100;i++){

      int rad = 5+ rand()%5;
      
        strcpy(v,"/////////////////\n#esfera\nradio:");
        sprintf(integ,"%d",rad);
        strcat(v,integ);
        strcat(v,"\ncentro: (");
     


      //ojo:(804, 533, -1000)

      x =-2016+rand()%4032;
      y =-1134+rand()%2268;

      sprintf(integ,"%d,%d,%d",x,y,z);
      strcat(v,integ);

      strcat(v,")\ncolor:(");

        sprintf(integ,"0.%d,0.%d,0.%d",99,99,99);
        strcat(v,integ);
        strcat(v,")\nkd:1.0\nka:0.9\nks:0\nkn:1\nO1:0.999999\nO2:0.000001\nO3:0.0\n\n");
      
        
      fprintf(inFile, v);
    }

    
	return 0;
}
