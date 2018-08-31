#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h> 
#ifndef max
    #define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
    #define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

#define XMAX -82.55178451538086
#define YMAX 11.219658078656096
#define XMIN -85.94956398010254
#define YMIN 8.039853040522544
#define H_SIZE_MAP 800
#define V_SIZE_MAP 800

typedef struct{
    int X;
    int Y;
}
vertice;
#include "provincias.h"

vertice puntos_Heredia[321];
vertice puntos_Alajuela[686];
vertice puntos_Limon[932];
vertice puntos_SanJose[414];
vertice puntos_Cartago[198];
vertice puntos_Puntarenas[2028];
vertice puntos_Guanacaste[2787];
vertice verticesActivos[100];
vertice intersecciones[100];

void readFile(char* filename,vertice* puntos);
void dibujarProvincia(int cant_vertices,vertice* puntos);
void mostrarProvinciasPintadas();
void pintarProvincia(vertice* vertices, int numVertices);
int activarVertices(int scanline, vertice* vertices, int numVertices);
int eliminarIntersecciones(int numIntersecciones);
int calcularIntersecciones(int scanline, int numVerticesActivos);
void mostrarBordes();
void init();
void bresenham(int x0, int y0,int x1, int y1);
void strip(char *s);
int UC_TO_FB(long double UC_value, char flag);
void plot(int x, int y);
int calcularYmax(vertice* vertices, int numVertices);
int calcularYmin(vertice* vertices, int numVertices);

void ordenarIntersecciones(int numIntersecciones);
int eliminarHorizontales(int numVerticesActivos);
int eliminarVerticales(int numVerticesActivos);
int verificarPuntas(int scanline, int numVerticesActivos);
int evaluarY(int Y1, int Y2, int scanline);
int eliminarRodilla(int pos, int numVerticesActivos);
void quicksort(vertice* number,int first,int last);

int main(int argc, char** argv)
{

    readFile("Heredia.txt",puntos_Heredia);
    
    quicksort(puntos_Heredia,0,320);
    readFile("Alajuela.txt",puntos_Alajuela);
    readFile("Limon.txt",puntos_Limon);
    readFile("SanJose.txt",puntos_SanJose);
    readFile("Cartago.txt",puntos_Cartago);
    readFile("Puntarenas.txt",puntos_Puntarenas);
    readFile("Guanacaste.txt",puntos_Guanacaste);
    cargarCartago();
    cargarGuanacaste();
    cargarHeredia();
    cargarSJ();
    cargarAlajuela();
    cargarLimon();
    cargarPuntarenas();
    int resolution = 800;
    

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800,800);
    glutCreateWindow("Progra 0");
    glClear(GL_COLOR_BUFFER_BIT);
    gluOrtho2D(-0.5, 800 +0.5, -0.5, 800 + 0.5);
    glColor3f (1.0f, 1.0f, 1.0f);
    glutDisplayFunc(init);
    glutMainLoop();

}

void quicksort(vertice* number,int first,int last){
   int i, j, pivot, temp;

   if(first<last){
      pivot=first;
      i=first;
      j=last;

      while(i<j){
         while(number[i].Y<=number[pivot].Y&&i<last)
            i++;
         while(number[j].Y>number[pivot].Y)
            j--;
         if(i<j){
            temp=number[i].Y;
            number[i].Y=number[j].Y;
            number[j].Y=temp;
         }
      }

      temp=number[pivot].Y;
      number[pivot].Y=number[j].Y;
      number[j].Y=temp;
      quicksort(number,first,j-1);
      quicksort(number,j+1,last);

   }
}


void readFile(char* filename,vertice* puntos)
{   
    int lines = 0;
    FILE *linecounter;
    linecounter = fopen(filename, "r");
    int ch = 0;
    while(!feof(linecounter))
    {
      ch = fgetc(linecounter);
      if(ch == '\n')
      {
        lines++;
      }
    }
    fclose(linecounter);

    FILE *file;
    file = fopen(filename, "r");
    char array[100000];

    fgets(array, sizeof(array), file);
    strip(array);

    for(int i=0;i<lines;i++){    

        fgets(array, sizeof(array), file);
        strip(array); //Quita espacios null
        //printf("%d - %s\n",i,array );

        char *ch;
        ch = strtok(array, ",");
        int flag = 0;
        while (ch != NULL) {
            if(flag ==0){
                puntos[i].X = UC_TO_FB(atof(ch),'x');
                //printf("%dx - %d\n",i,puntos[i].x );
                flag = 1;
            }else{
                puntos[i].Y = UC_TO_FB(atof(ch),'y');
                //printf("%dy - %d\n",i,puntos[i].y );
                flag = 0;
            }
            ch = strtok(NULL, " ,");
        }  
    }
    fclose(file);
}
int evaluarY(int Y1, int Y2, int scanline){
    int n = 1;
    //printf("Y1 = %d, Y2 = %d, scanline = %d\n", Y1, Y2, scanline);
    if((Y1 <= scanline && Y2 <= scanline) || (Y1 >= scanline && Y2 >= scanline)){
        n = 0;
    }
    return n;
}
int eliminarRodilla(int pos, int numVerticesActivos){
    //printf("pos = %d. (%d, %d) (%d, %d) \n", pos, verticesActivos[pos].X, verticesActivos[pos].Y, verticesActivos[pos+1].X, verticesActivos[pos+1].Y);
    int i;
    if(pos == numVerticesActivos - 2)
        numVerticesActivos-=2;
    else{
        for (i = pos + 2 ; i < numVerticesActivos - 1; i+=2)
        {
            verticesActivos[pos] = verticesActivos[i];
            verticesActivos[pos+1] = verticesActivos[i+1];
        }
        numVerticesActivos-=2;
    }

    return numVerticesActivos;
}

void dibujarProvincia(int cant_vertices,vertice* puntos){
     for(int i = 0;i < cant_vertices;i++){
        if(i+1 < cant_vertices){
            bresenham(puntos[i].X,puntos[i].Y,puntos[i+1].X,puntos[i+1].Y);

        }
    }
}

void ordenarIntersecciones(int numIntersecciones){
    int i, k;
    vertice v;
    for (i = 0, k = 0; i < numIntersecciones-1; i++)
    {
        if(intersecciones[i].X > intersecciones[i+1].X){
            v = intersecciones[i];
            intersecciones[i] = intersecciones[i+1];
            intersecciones[i+1] = v;
            i = 0;
        }
    }
}


void mostrarProvinciasPintadas(){
    glColor3f (0.4f, 0.6f, 0.3f);
    pintarProvincia(verticesHeredia, numVerticesHeredia);
    glColor3f (1.0f, 0.0f, 0.0f);
    pintarProvincia(verticesSJ, numVerticesSJ);
    glColor3f (0.0f, 0.0f, 1.0f);
    pintarProvincia(verticesGuana, numVerticesGuana);
    glColor3f (0.0f, 1.0f, 0.0f);
    pintarProvincia(verticesPunta, numVerticesPunta);
    glColor3f (0.6f, 0.6f, 0.1f);
    pintarProvincia(verticesAlajuela, numVerticesAlajuela);
    glColor3f (0.5f, 0.5f, 0.5f);
    pintarProvincia(verticesCartago, numVerticesCartago);
    glColor3f (0.1f, 0.5f, 0.0f);
    pintarProvincia(verticesLimon, numVerticesLimon);
    glColor3f (0.0f, 0.5f, 0.1f);
}
void pintarProvincia(vertice* vertices, int numVertices){
    int scanline = calcularYmax(vertices, numVertices);
    
    while(scanline > calcularYmin(vertices, numVertices)){
    
        int numVerticesActivos = activarVertices(scanline, vertices, numVertices);
        int i;

    
        
        int numIntersecciones= calcularIntersecciones(scanline, numVerticesActivos);
    
        ordenarIntersecciones(numIntersecciones);
        for (i = 0; i < numIntersecciones; i+=2)
        {   
            bresenham(intersecciones[i].X,intersecciones[i].Y,intersecciones[i+1].X,intersecciones[i+1].Y);
        
        }
        scanline--;
    }

}

int verificarPuntas(int scanline, int numVerticesActivos){
    int i, k, j, n;
    vertice v1, v2;
    for (i = 0; i < numVerticesActivos -2; i+=2)
    {
        //printf("i = %d\n", i);
        v1 = verticesActivos[i];
        v2 = verticesActivos[i+1];

        for (j = i + 2; j < numVerticesActivos-1; j+=2)
        {
            n = 0;
            /*printf("v1.X = %d, v1.Y = %d\n", v1.X, v1.Y);
            printf("v2.X = %d, v2.Y = %d\n", v2.X, v2.Y);
            printf("verticesActivos[j].X = %d, verticesActivos[j].Y = %d\n", verticesActivos[j].X, verticesActivos[j].Y);
            printf("verticesActivos[j+1].X = %d, verticesActivos[j+1].Y = %d\n", verticesActivos[j+1].X, verticesActivos[j+1].Y);*/
            //printf("j = %d\n", j );
            if(v1.X == verticesActivos[j].X && v1.Y == verticesActivos[j].Y){
                //printf("1\n");
                n = evaluarY(v2.Y, verticesActivos[j+1].Y, scanline);
                //printf("rodilla %d\n", n);
            }
            else if(v1.X == verticesActivos[j+1].X && v1.Y == verticesActivos[j+1].Y){
                //printf("2\n");
                n = evaluarY(v2.Y, verticesActivos[j].Y, scanline);
                //printf("rodilla %d\n", n);
            }
            else if(v2.X == verticesActivos[j].X && v2.Y == verticesActivos[j].Y){
                //printf("3\n");
                n = evaluarY(v1.Y, verticesActivos[j+1].Y, scanline);
                //printf("rodilla %d\n", n);
            }
            else if(v2.X == verticesActivos[j+1].X && v2.Y == verticesActivos[j+1].Y){
                //printf("4\n");
                n = evaluarY(v1.Y, verticesActivos[j].Y, scanline);
                //printf("rodilla %d\n", n);
            }
            //printf("HOLA SOY LA N = %d\n", n);
            if(n == 1)
                numVerticesActivos = eliminarRodilla(j, numVerticesActivos);
        }
    }
    return numVerticesActivos;
}

int activarVertices(int scanline, vertice* vertices, int numVertices){
    //vertice activos[100];
    //vertice *activos = (vertice*)malloc(100*sizeof(vertice));
    int i;
    int k = 0;
    //printf("activos2: %ld\n", sizeof(activos2)/sizeof(int));
    for (i = 0; i < numVertices-1; i++)
    {
        /*
            printf("i = %d\n", i);
            printf("i+1 = %d\n", i+1);
            printf("X%d = %d\n", i, vertices[i].X);
            printf("Y%d = %d\n", i, vertices[i].Y);
            printf("X%d = %d\n", i+1, vertices[i+1].X);
            printf("Y%d = %d\n", i+1, vertices[i+1].Y);*/
        if(vertices[i].Y >= scanline && vertices[i+1].Y <= scanline
            || vertices[i].Y <= scanline && vertices[i+1].Y >= scanline){

            verticesActivos[k] = vertices[i];
            verticesActivos[k+1] = vertices[i+1];
            k+=2;
        }
    }
    k = eliminarHorizontales(k);
    k = eliminarVerticales(k);
    k = verificarPuntas(scanline,k);
    return k;
}

int eliminarIntersecciones(int numIntersecciones){
    int i, k;
    for (i = 0, k = 0; i < numIntersecciones -1; i++)
    {

        intersecciones[k] = intersecciones[i];
        k++;
        if(intersecciones[i].X+1 == intersecciones[i+1].X
            || intersecciones[i].X-1 == intersecciones[i+1].X){
            intersecciones[k] = intersecciones[i];
            i++;
        }
    }

    if(intersecciones[k].X+1 != intersecciones[i].X
        || intersecciones[k].X-1 != intersecciones[i].X){
        intersecciones[k] = intersecciones[i];
    }
    return k;
}

int eliminarHorizontales(int numVerticesActivos){
    int i, k;
    for (i = 0, k = 0; i < numVerticesActivos -1; i+=2)
    {
        if(verticesActivos[i].Y != verticesActivos[i+1].Y){
            verticesActivos[k] = verticesActivos[i];
            verticesActivos[k+1] = verticesActivos[i+1];
            k+=2;
        }
    }
    return k;
}

int eliminarVerticales(int numVerticesActivos){
    int i, k;
    for (i = 0, k = 2; i < numVerticesActivos -1; i+=2)
    {
        if(verticesActivos[i].X == verticesActivos[i+1].X){
            verticesActivos[i].X += 1;
        }
    }
    return numVerticesActivos;
}

int calcularIntersecciones(int scanline, int numVerticesActivos){
    int i, k, Xi, Yi, Xj, Yj;
    vertice punto;
    punto.Y = scanline;
    for (i = 0, k = 0; i < numVerticesActivos - 1; i+=2)
    {
        Xi = verticesActivos[i].X;
        Yi = verticesActivos[i].Y;
        Xj = verticesActivos[i+1].X;
        Yj = verticesActivos[i+1].Y;

        punto.X = (scanline-(Yi-((float)(Yj-Yi)/(Xj-Xi))*Xi))/((float)(Yj-Yi)/(Xj-Xi));
        intersecciones[k] = punto;
        k++;
    }

    //k = eliminarIntersecciones(k);
    return k;
}


void mostrarBordes(){
    dibujarProvincia(320,puntos_Heredia);
    dibujarProvincia(685,puntos_Alajuela);
    dibujarProvincia(931,puntos_Limon);
    dibujarProvincia(413,puntos_SanJose);
    dibujarProvincia(198,puntos_Cartago);
    dibujarProvincia(2028,puntos_Puntarenas);
    dibujarProvincia(2787,puntos_Guanacaste);
}

void init(){

    mostrarBordes();
    mostrarProvinciasPintadas();
    
    glFlush();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    

}
void bresenham(int x0, int y0,int x1, int y1) {
    int d = 0;
    int xp = 0;
    int yp = 0;
    int Delta_1 = 0;
    int Delta_2 = 0;
    int distanceX = fabs((x0 - x1));
    int distanceY = fabs((y0 - y1));
    xp = x0; 
    yp = y0;
    plot(xp, yp);

    //Cuadrantes 1 y 2
    if (x0 <= x1 && y0 <= y1) {
    	
    	if (distanceX > distanceY) {
    		//Cuadrante 1
    		Delta_1 = 2*(y1-y0) - 2*(x1-x0);
    		Delta_2 = 2*(y1-y0);
    		d = 2*(y1-y0) - (x1-x0);

    		while (xp <= x1) {
	            if (d > 0) {
	            	xp++; yp++;
	             	d += Delta_1;
	            } else {
	             	xp++;
	             	d+= Delta_2;
	            }
	            plot(xp, yp);
	        }

    	}else{
    		//Cuadrante 2
    		
    		Delta_1 = 2*(y1-y0) - 2*(x1-x0);
			Delta_2 = -2*(x1-x0);
        	d = (y1-y0) - 2*(x1-x0);
        	while (yp <= y1) {

	            if (d > 0) {
	            	yp++;
	            	d+= Delta_2;
	            } else {
	            	xp++; yp++;
	            	d += Delta_1;
	            }
	            plot(xp, yp);
          }
    	}
    }
    //Cuadrante 3 y 4
    else if(x1 <= x0 && y0 <= y1){

    	
    	if (distanceX < distanceY) {
    		//Cuadrante 3
    		
    		Delta_1 = -2*(y1-y0) - 2*(x1-x0);
			Delta_2 = -2*(x1-x0);
        	d = - (y1-y0) - 2*(x1-x0);

        	while (yp <= y1) {
	            if (d >= 0) {
	            	xp--; yp++;
	            	d += Delta_1;
	            } else {
	            	yp++;
	            	d+= Delta_2;
	            }
	        	plot(xp, yp);
	        }
    	}else{
    		//Cuadrante 4
    		
    		Delta_1 = -2*(y1-y0) - 2*(x1-x0);
			Delta_2 = -2*(y1-y0);
        	d = -2*(y1-y0)-(x1-x0);
       		while (xp >= x1) {
        		if (d >= 0) {
	        		xp--;
	        		d+= Delta_2;
	            } else {
	        		xp--; yp++;
	        		d += Delta_1;
	            }
            	plot(xp, yp);
          	}
    	}
		

    }

    //Cuadrante 5 y 6
    else if(x1 <= x0 && y0 >= y1){
    	if (distanceX > distanceY) {
    		//Cuadrante 5
    		
    		Delta_1 = -2*(y1-y0) + 2*(x1-x0);
			Delta_2 = -2*(y1-y0);
        	d = -2*(y1-y0)+(x1-x0);
        	while (xp >= x1) {
	        	if (d <= 0) {
            		xp--;
            		d+= Delta_2;
            	}else {
            		xp--; yp--;
            		d += Delta_1;
            	}
            	plot(xp, yp);
          	}
    	}else{
    		//Cuadrante 6
    		
    		Delta_1 = -2*(y1-y0) + 2*(x1-x0);
			Delta_2 = 2*(x1-x0);
        	d = -(y1-y0)+2*(x1-x0);
        	while (yp >= y1) {
        		if (d >= 0) {
	            	yp--;
	            	d+= Delta_2;
	            } else {
	            	xp--; yp--;
	            	d += Delta_1;
            	}
            	plot(xp, yp);
          	}
    	}
    }

    //Cuadrante 7 y 8

    else if(x1 >= x0 && y0 >= y1){
    	if (distanceX < distanceY) {
    		//Cuadrante 7
    		
    		Delta_1 = 2*(y1-y0) + 2*(x1-x0);
			Delta_2 = 2*(x1-x0);
        	d = (y1-y0)+2*(x1-x0);
        	while (yp >= y1) {
            	if (d <= 0) {
	            	yp--;
	            	d+= Delta_2;
	            } else {
	             	xp++; yp--;
	              	d += Delta_1;
	            }
            	plot(xp, yp);
          	}
		}else{
    		//Cuadrante 8
          	
			Delta_2 = 2*(y1-y0) + 2*(x1-x0);
			Delta_1 = 2*(y1-y0);
          	d = 2*(y1-y0)+(x1-x0);
          	while (xp <= x1) {
            	if (d >= 0) {
	              	xp++;
	              	d+= Delta_1;
            	} else {
              		xp++; yp--;
            		  d += Delta_2;
            	}
            	plot(xp, yp);
          	}

    	}
    }

}

int calcularYmin(vertice* vertices, int numVertices){
    int i,Ymin;
    for (i = 0, Ymin = vertices[0].Y; i < numVertices; ++i)
    {
        if(vertices[i].Y < Ymin)
            Ymin = vertices[i].Y;
    }

    return Ymin;
}

int calcularYmax(vertice* vertices, int numVertices){
    int i,Ymax;
    for (i = 0, Ymax = 0; i < numVertices; ++i)
    {
        if(vertices[i].Y > Ymax)
            Ymax = vertices[i].Y;
    }
    return Ymax;
}

void plot(int x, int y) {
	//glColor3f (1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
}


int UC_TO_FB(long double UC_value, char flag){
  long double max = XMAX;
  long double min = XMIN;
  int size = H_SIZE_MAP;
  int FB_value;
  
  if(flag == 'y'){
    max = YMAX;
    min = YMIN;
    size = V_SIZE_MAP;
  }
  
  FB_value = ((UC_value - (min)) / (max - (min))) * size;
  FB_value = (int)(FB_value < 0 ? (FB_value - 0.5) : (FB_value + 0.5));
  return (int) FB_value;

}
void strip(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}

