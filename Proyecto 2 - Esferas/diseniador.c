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

  	// for(int i = 0;i < 70;i++){

  	// 	if(i%10 ==0){
   //      strcpy(v,"/////////////////\n#esfera\nradio:10\ncentro: (");
   //    }
   //    else{
   //      strcpy(v,"/////////////////\n#esfera\nradio:5\ncentro: (");
   //    }

   //    x =504+200*cos(grados);
   //    y =233+200*sin(grados);
      
		 //  sprintf(integ,"%d,%d,%d",x,y,z);
  	// 	strcat(v,integ);
   //    grados+=0.1;
  	// 	strcat(v,")\ncolor:(");
   //    //z+=15;


   //    if(i%10 ==0){
   //      sprintf(integ,"0.%d,0.%d,0.%d",99,0,99);
   //      strcat(v,integ);
   //      strcat(v,")\nkd:1.0\nka:0.7\nks:0.8\nkn:1\n\n");
   //    }
   //    else{
   //      sprintf(integ,"0.%d,0.%d,0.%d",0,0,0);
   //      strcat(v,integ);
   //      strcat(v,")\nkd:0.1\nka:0.1\nks:0.1\nkn:0.1\n\n");
   //    }


	  // 	fprintf(inFile, v);
  	// }

    x =100;
    y = 233;
    z = 0;

    for(int i = 0;i < 80;i++){

      if(i%10 ==0){
        strcpy(v,"/////////////////\n#esfera\nradio:4\ncentro: (");
      }
      else{
        strcpy(v,"/////////////////\n#esfera\nradio:2\ncentro: (");
      }

      x =104+200*cos(grados);
      z =100+200*sin(grados);
      
      sprintf(integ,"%d,%d,%d",x,y,z);
      strcat(v,integ);
      grados+=0.1;


  
      strcat(v,")\ncolor:(");


      if(i%10 ==0){
        sprintf(integ,"0.%d,0.%d,0.%d",99,0,99);
        strcat(v,integ);
        strcat(v,")\nkd:1.0\nka:0.7\nks:0.8\nkn:1\n\n");
      }
      else{
        sprintf(integ,"0.%d,0.%d,0.%d",0,0,0);
        strcat(v,integ);
        strcat(v,")\nkd:0.1\nka:0.1\nks:0.1\nkn:0.1\n\n");
      }

      fprintf(inFile, v);
    }


    x =504;
    y = 233;
    z = 0;

    for(int i = 0;i < 80;i++){

      if(i%10 ==0){
        strcpy(v,"/////////////////\n#esfera\nradio:10\ncentro: (");
      }
      else{
        strcpy(v,"/////////////////\n#esfera\nradio:5\ncentro: (");
      }

      x =504+200*cos(grados);
      z =200*sin(grados);
      
      sprintf(integ,"%d,%d,%d",x,y,z);
      strcat(v,integ);
      grados+=0.1;


  
      strcat(v,")\ncolor:(");


      if(i%10 ==0){
        sprintf(integ,"0.%d,0.%d,0.%d",99,0,99);
        strcat(v,integ);
        strcat(v,")\nkd:1.0\nka:0.7\nks:0.8\nkn:1\n\n");
      }
      else{
        sprintf(integ,"0.%d,0.%d,0.%d",0,0,0);
        strcat(v,integ);
        strcat(v,")\nkd:0.1\nka:0.1\nks:0.1\nkn:0.1\n\n");
      }

      fprintf(inFile, v);
    }


    x =504;
    y = 233;
    z = 0;

    for(int i = 0;i < 80;i++){

      if(i%10 ==0){
        strcpy(v,"/////////////////\n#esfera\nradio:10\ncentro: (");
      }
      else{
        strcpy(v,"/////////////////\n#esfera\nradio:5\ncentro: (");
      }

      y =233+200*cos(grados);
      z =200*sin(grados);
      
      sprintf(integ,"%d,%d,%d",x,y,z);
      strcat(v,integ);
      grados+=0.1;


  
      strcat(v,")\ncolor:(");


      if(i%10 ==0){
        sprintf(integ,"0.%d,0.%d,0.%d",99,0,99);
        strcat(v,integ);
        strcat(v,")\nkd:1.0\nka:0.7\nks:0.8\nkn:1\n\n");
      }
      else{
        sprintf(integ,"0.%d,0.%d,0.%d",0,0,0);
        strcat(v,integ);
        strcat(v,")\nkd:0.1\nka:0.1\nks:0.1\nkn:0.1\n\n");
      }

      fprintf(inFile, v);
    }

    for(int i = 0;i < 20;i++){

     
      strcpy(v,"/////////////////\n#esfera\nradio:17\ncentro: (");
     

      x =480+rand() % 40;
      y =210+rand() % 40;
      z =rand() % 120;
      
      sprintf(integ,"%d,%d,%d",x,y,z);
      strcat(v,integ);
      grados+=0.1;


  
      strcat(v,")\ncolor:(");

      int c = rand() % 120;

      if(c > 60){
        sprintf(integ,"0.%d,0.%d,0.%d",99,0,0);
      }
      else{
        sprintf(integ,"0.%d,0.%d,0.%d",0,0,99);
      }

      
      strcat(v,integ);
      strcat(v,")\nkd:1.0\nka:0.7\nks:0.8\nkn:1\n\n");

      fprintf(inFile, v);
    }
    
	return 0;
}
