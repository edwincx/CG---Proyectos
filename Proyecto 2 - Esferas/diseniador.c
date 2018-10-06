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
    int z = 1;





    x =504;
    y = 233;
    z = 0;

    for(int i = 0;i < 90;i++){

      int rad = 5 + rand()%5;
      
        strcpy(v,"/////////////////\n#esfera\nradio:");
        sprintf(integ,"%d",rad);
        strcat(v,integ);
        strcat(v,"\ncentro: (");
     


      //ojo:(804, 533, -1000)

      x =504+220*cos(grados);
      z =200*sin(grados);
      
      float xc = (1008)/ 2;
      float yc = (567)/ 2;
      int tempX;
      int tempY;
      tempX = x;
      tempY = y;
      double  tempXX = xc +(tempX - xc) * cos(1.7) - (tempY - yc)* sin(1.7);
      double tempYY = yc + (tempX - xc) * sin(1.7) + (tempY - yc)* cos(1.7);


      x = (int) tempXX;
      y = (int) tempYY;

      x += rand()%5;
      y += rand()%5;
      z += rand()%5;

      sprintf(integ,"%d,%d,%d",x,y,z);
      strcat(v,integ);
      grados+=0.4;


  
      strcat(v,")\ncolor:(");

      int r = 53 +rand()% 20;
      int g = 32 + rand()% 18;
      int b = 11 + rand()% 24;
      
        sprintf(integ,"0.%d,0.%d,0.%d",r,g,b);
        strcat(v,integ);
        strcat(v,")\nkd:1.0\nka:0.5\n\n");
      
        
      fprintf(inFile, v);
    }

    x =504;
    y = 233;
    z = 0;

    for(int i = 0;i < 80;i++){

      int rad = 5 + rand()%5;
      
        strcpy(v,"/////////////////\n#esfera\nradio:");
        sprintf(integ,"%d",rad);
        strcat(v,integ);
        strcat(v,"\ncentro: (");
     


      //ojo:(804, 533, -1000)

      x =504+200*cos(grados);
      z =200*sin(grados);
      
      float xc = (1008)/ 2;
      float yc = (567)/ 2;
      int tempX;
      int tempY;
      tempX = x;
      tempY = y;
      double  tempXX = xc +(tempX - xc) * cos(1.7) - (tempY - yc)* sin(1.7);
      double tempYY = yc + (tempX - xc) * sin(1.7) + (tempY - yc)* cos(1.7);


      x = (int) tempXX;
      y = (int) tempYY;

      x += rand()%5;
      y += rand()%5;
      z += rand()%5;

      sprintf(integ,"%d,%d,%d",x,y,z);
      strcat(v,integ);
      grados+=0.4;


  
      strcat(v,")\ncolor:(");

      int r = 53 +rand()% 20;
      int g = 32 + rand()% 18;
      int b = 11 + rand()% 24;
      
        sprintf(integ,"0.%d,0.%d,0.%d",r,g,b);
        strcat(v,integ);
        strcat(v,")\nkd:1.0\nka:0.5\n\n");
      
        
      fprintf(inFile, v);
    }

for(int i = 0;i < 70;i++){

      int rad = 5 + rand()%5;
      
        strcpy(v,"/////////////////\n#esfera\nradio:");
        sprintf(integ,"%d",rad);
        strcat(v,integ);
        strcat(v,"\ncentro: (");
     


      //ojo:(804, 533, -1000)

      x =504+240*cos(grados);
      z =200*sin(grados);
      
      float xc = (1008)/ 2;
      float yc = (567)/ 2;
      int tempX;
      int tempY;
      tempX = x;
      tempY = y;
      double  tempXX = xc +(tempX - xc) * cos(1.7) - (tempY - yc)* sin(1.7);
      double tempYY = yc + (tempX - xc) * sin(1.7) + (tempY - yc)* cos(1.7);


      x = (int) tempXX;
      y = (int) tempYY;

      x += rand()%5;
      y += rand()%5;
      z += rand()%5;

      sprintf(integ,"%d,%d,%d",x,y,z);
      strcat(v,integ);
      grados+=0.4;


  
      strcat(v,")\ncolor:(");

      int r = 53 +rand()% 20;
      int g = 32 + rand()% 18;
      int b = 11 + rand()% 24;
      
        sprintf(integ,"0.%d,0.%d,0.%d",r,g,b);
        strcat(v,integ);
        strcat(v,")\nkd:1.0\nka:0.5\n\n");
      
        
      fprintf(inFile, v);
    }



z =1500;

    for(int i = 0;i <180;i++){

      int rad = rand()%5;
      
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
        strcat(v,")\nkd:1.0\nka:0.9\n\n");
      
        
      fprintf(inFile, v);
    }

    
	return 0;
}
