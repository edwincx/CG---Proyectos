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

#include "valores.h"
#include "bresenham.h"
typedef struct{
	int X;
	int Y;
}
vertice;



void plot();
void strip(char *s);
void init();
void mouse();
void mostrarBordes();
void mostrarProvinciasPintadas();
void dibujarBotones();

void keyBoard(unsigned char key, int x, int y);
void ArrowKey(int key, int x, int y);
void readFile(char* filename, vertice* puntos,int numVert);
void mostrarProvinciasPintadasRotadas();
void mostrarBordesRotados();
void leerTextura(char* filename,int prov);

int ventana_principal;  
int ventana_instrucciones;  
static char label[100];  

float grados = 0.0;

float **imagenTexturaHeredia;
float **imagenTexturaSanJose;
float **imagenTexturaGuana;
float **imagenTexturaAlajuela;
float **imagenTexturaPunta;
float **imagenTexturaCartago;
float **imagenTexturaLimon;
vertice verticesActivos[100];
vertice intersecciones[100];
int flag = 0;
int flagTexture = 0;
int flagRotacion = 0;
float xmin = 0.5;
float ymin = 0.5;
float xmax = 1029.5;
float ymax = 1029.5;
float zoom = 1.0;

int tamanioTextura;

vertice verticesGuana[numVerticesGuana];
vertice verticesAlajuela[numVerticesAlajuela];
vertice verticesPuntaGolfo[numVerticesPuntaGolfo];
vertice verticesPunta[numVerticesPunta];
vertice verticesHeredia[numVerticesHeredia];
vertice verticesCartago[numVerticesCartago];
vertice verticesLimon[numVerticesLimon];
vertice verticesSJ[numVerticesSJ];

vertice verticesAuxSJ[numVerticesSJ];
vertice verticesAuxGuana[numVerticesGuana];
vertice verticesAuxAlajuela[numVerticesAlajuela];
vertice verticesAuxPuntaGolfo[numVerticesPuntaGolfo];
vertice verticesAuxPunta[numVerticesPunta];
vertice verticesAuxHeredia[numVerticesHeredia];
vertice verticesAuxCartago[numVerticesCartago];
vertice verticesAuxLimon[numVerticesLimon];
vertice verticesAuxSJ[numVerticesSJ];

vertice verticesActivos[100];
vertice intersecciones[100];

   
void   drawStringBig (char *s) 
{ 
  unsigned int i; 
  for (i = 0; i < strlen (s); i++) 
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, s[i]); 
}; 
 

void  mostrar_ventana_instrucciones () 
{ 
  /* Clear subwindow */ 
  glutSetWindow (ventana_instrucciones); 
  glClearColor (0.20, 0.20, 0.20, 0.0); 
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
  /* Draw border */ 
  glColor3f (0.0F, 0.0F, 1.0F); 
  glBegin (GL_LINE_LOOP); 
  glVertex2f (0.0F, 0.0F); 
  glVertex2f (0.0F, 0.99F); 
  glVertex2f (0.999F, 0.99F); 
  glVertex2f (0.999F, 0.0F); 
  glEnd (); 
 
  glColor3f (0.0F, 0.0F, 1.0F); 
  sprintf (label, "Instrucciones"); 
  glRasterPos2f (0.40F, 0.70F); 
  drawStringBig (label); 
 
  glColor3f (1.0F, 1.0F, 1.0F); 
  sprintf (label, "Tecla 1: Muestra el mapa sin relleno                                     Tecla 4: Para activar la rotacion sin relleno"); 
  glRasterPos2f (0.10F, 0.50F); 
  drawStringBig (label); 

  glColor3f (1.0F, 1.0F, 1.0F); 
  sprintf (label, "Tecla 2: Muestra el mapa con relleno                                   Tecla 5: Para salir del programa"); 
  glRasterPos2f (0.10F, 0.30F); 
  drawStringBig (label);

  glColor3f (1.0F, 1.0F, 1.0F); 
  sprintf (label, "Tecla 3: Para activar la rotacion                                            Tecla 'a' para hacer rotacion a la izquierda"); 
  glRasterPos2f (0.10F, 0.13F); 
  drawStringBig (label);

  glutSwapBuffers (); 
}; 

void remodelar_ventana (int w, int h) 
{ 
  glViewport (0, 0, w, h); 
  glMatrixMode (GL_PROJECTION); 
  glLoadIdentity (); 
  gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F); 
}; 


void setcolor(float r, float g, float b){
	glColor3f(r, g, b);
}

int main(int argc, char** argv)
{
	printf("\n\n----------------------------\nInstrucciones\n----------------------------\n\n\n1 - Mostrar bordes\n2 - Pintar provincias\n3 - Agregar texturas a provincias pintadas\n4 - Habilitar o deshabilitar rotación\n5 - Salir\n\n\n");

	leerTextura("heredia.tga",1);
	leerTextura("sanjose.tga",2);
	leerTextura("guanacaste.tga",3);
	leerTextura("puntarenas.tga",4);
	leerTextura("alajuela.tga",5);
	leerTextura("cartago.tga",6);
	leerTextura("limon.tga",7);


	readFile("Guanacaste.txt",verticesGuana,numVerticesGuana);
	readFile("Alajuela.txt",verticesAlajuela,numVerticesAlajuela);
	readFile("Heredia.txt",verticesHeredia,numVerticesHeredia);
	readFile("Limon.txt",verticesLimon,numVerticesLimon);
	readFile("Cartago.txt",verticesCartago,numVerticesCartago);
	readFile("SanJose.txt",verticesSJ,numVerticesSJ);
	readFile("Puntarenas.txt",verticesPunta,numVerticesPunta);
	readFile("PuntaGolfo.txt",verticesPuntaGolfo,numVerticesPuntaGolfo);

	rotacion(0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(resolucion,resolucion);

	ventana_principal = glutCreateWindow("Proyecto 1: Mapa de Costa Rica");
	setcolor (1.0f, 1.0f, 1.0f);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyBoard);
	glutSpecialFunc(ArrowKey);
	glutDisplayFunc(init);

	glutMainLoop();

}


void mouse(int button, int state , int x , int y) {
	
	float xc = (xmin+xmax)/ 2;
	float yc = (ymin+ymax)/ 2;

	if(button == 3 && state == GLUT_DOWN){ // Rueda hacia abajo

		zoom -= 0.05;

		xmin = ((xmin - xc) * zoom) + xc;
		ymin = ((ymin - yc) * zoom) + yc;

		xmax = ((xmax - xc) * zoom) + xc;
		ymax = ((ymax - yc) * zoom) + yc;
	}

	if(button == 3 && state == GLUT_UP){

		zoom += 0.05;
		xmin = ((xmin - xc)*zoom) + xc;
		ymin = ((ymin - yc)*zoom) + yc;

		xmax = ((xmax - xc)*zoom) + xc;
		ymax = ((ymax - yc)*zoom) + yc;	
	}

	if(button == 4 && state == GLUT_UP){ // Rueda hacia arriba

		zoom -= 0.05;

		xmin = ((xmin - xc) * zoom) + xc;
		ymin = ((ymin - yc) * zoom) + yc;

		xmax = ((xmax - xc) * zoom) + xc;
		ymax = ((ymax - yc) * zoom) + yc;
	}

	if(button == 4 && state == GLUT_DOWN){

		zoom += 0.05;
		xmin = ((xmin - xc)*zoom) + xc;
		ymin = ((ymin - yc)*zoom) + yc;

		xmax = ((xmax - xc)*zoom) + xc;
		ymax = ((ymax - yc)*zoom) + yc;
		
	}
	glutPostRedisplay();
			
}

void rotacion(float input_grados){
	float xc = (xmin+xmax)/ 2;
		float yc = (ymin+ymax)/ 2;
		grados += input_grados;

		int i = 0;
		//SJ
		for ( i = 0; i < numVerticesSJ; i++) {
			int tempX;
			int tempY;
			tempX = verticesSJ[i].X;
			tempY = verticesSJ[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);
			int a = (int) tempXX;
			int b = (int) tempYY;
		    verticesAuxSJ[i].X = a;
			verticesAuxSJ[i].Y = b;

		}
		//guanacaste
		for ( i = 0; i < numVerticesGuana; i++) {
			int tempX;
			int tempY;
			tempX = verticesGuana[i].X;
			tempY = verticesGuana[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxGuana[i].X = a;
			verticesAuxGuana[i].Y = b;

		}
		//alajuela
		for ( i = 0; i < numVerticesAlajuela; i++) {
			int tempX;
			int tempY;
			tempX = verticesAlajuela[i].X;
			tempY = verticesAlajuela[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxAlajuela[i].X = a;
			verticesAuxAlajuela[i].Y = b;

		}
		//heredia
		for ( i = 0; i < numVerticesHeredia; i++) {
			int tempX;
			int tempY;
			tempX = verticesHeredia[i].X;
			tempY = verticesHeredia[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxHeredia[i].X = a;
			verticesAuxHeredia[i].Y = b;

		}
		//cartago
		for ( i = 0; i < numVerticesCartago; i++) {
			int tempX;
			int tempY;
			tempX = verticesCartago[i].X;
			tempY = verticesCartago[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxCartago[i].X = a;
			verticesAuxCartago[i].Y = b;

		}
		//Limon
		for ( i = 0; i < numVerticesLimon; i++) {
			int tempX;
			int tempY;
			tempX = verticesLimon[i].X;
			tempY = verticesLimon[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxLimon[i].X = a;
			verticesAuxLimon[i].Y = b;

		}
		//Puntarenas
		for ( i = 0; i < numVerticesPunta; i++) {
			int tempX;
			int tempY;
			tempX = verticesPunta[i].X;
			tempY = verticesPunta[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxPunta[i].X = a;
			verticesAuxPunta[i].Y = b;

		}
		//Puntarenas
		for ( i = 0; i < numVerticesPuntaGolfo; i++) {
			int tempX;
			int tempY;
			tempX = verticesPuntaGolfo[i].X;
			tempY = verticesPuntaGolfo[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxPuntaGolfo[i].X = a;
			verticesAuxPuntaGolfo[i].Y = b;
		}
}




void keyBoard(unsigned char key, int x, int y){
	
	if (key == '1') {
		setcolor(1.0f, 1.0f, 1.0f);
		flag = 0;
	} else if (key == '2') {
		flagTexture = 0;
		flag = 1;
	}
	else if (key == '4') {
		if(flagRotacion == 0){
			flagRotacion = 1;

		}else{
			flagRotacion = 0;
		}
		
	}
	else if (key == '3') {
		
		flagTexture = 1;
		
	}
	else if (key == '5') {
		
		exit(0);
		
	}

	
  //Rotacion hacia la izquierda
    if((key == 'a' || key == 'A')&& flagRotacion == 1){
		rotacion(0.17);
  }

  //Rotacion hacia la derecha
  
  if((key == 's' || key == 'S')&& flagRotacion == 1){
		rotacion(-0.17);
	}
	if(flag == 0){
		printf("Bordes            activado\n");
	}else{
		printf("Bordes            desactivado\n");
	}
	if(flag == 1){
		printf("Pintar Provincias activado\n");
	}else{
		printf("Pintar Provincias desactivado\n");
	}
	if(flagTexture == 1){
		printf("Texturas          activado\n");
	}else{
		printf("Texturas          desactivado\n");
	}
	if(flagRotacion == 1){
		printf("Rotación          activado\n");
	}else{
		printf("Rotación          desactivado\n");
	}
	printf("----------------------------------------\n\n\n");
	glutPostRedisplay();
}

//Función para el Pan
void ArrowKey(int key, int x, int y){

  //printf("%d\n", key);
  if(key == GLUT_KEY_DOWN){ //Abajo
		float auxmin = xmin + 0.0;
		float auxmax = xmax + 0.0;
		float auymin = ymin + 2.0;
		float  auymax = ymax + 2.0;
		
		xmin = xmin + (auxmin - xmin);
		ymin = ymin + (auymin - ymin);
		xmax = xmax + (auxmax - xmax);
		ymax = ymax + (auymax - ymax);
  }
  else if(key == GLUT_KEY_RIGHT){ //Derecha
		float auxmin = xmin - 2.0;
		float auxmax = xmax - 2.0;
		float auymin = ymin - 0.0;
		float  auymax = ymax - 0.0;
		xmin = xmin + (auxmin - xmin);
		ymin = ymin + (auymin - ymin);
		xmax = xmax + (auxmax - xmax);
		ymax = ymax + (auymax - ymax);
  }
  else if(key == GLUT_KEY_UP){ //Arriba
		float auxmin = xmin - 0.0;
		float auxmax = xmax - 0.0;
		float auymin = ymin - 2.0;
		float  auymax = ymax - 2.0;
		xmin = xmin + (auxmin - xmin);
		ymin = ymin + (auymin - ymin);
		xmax = xmax + (auxmax - xmax);
		ymax = ymax + (auymax - ymax);
  }
  else if(key == GLUT_KEY_LEFT){ //Izquierda
		float auxmin = xmin + 2.0;
		float auxmax = xmax + 2.0;
		float auymin = ymin - 0.0;
		float auymax = ymax - 0.0;
		xmin = xmin + (auxmin - xmin);
		ymin = ymin + (auymin - ymin);
		xmax = xmax + (auxmax - xmax);
		ymax = ymax + (auymax - ymax);
  }


	glutPostRedisplay();

 }



void leerTextura(char* filename,int prov){
	FILE *file;
	int byte,i;
	file = fopen(filename,"r");

	if(file == (FILE *) 0){
		printf("no se encontro texturas\n");
		exit(0);
	}

	for(i = 0; i < 18; i++){
		byte = getc(file);
	}
	

	
	
	switch(prov){
			case 1:
				imagenTexturaHeredia = malloc(800 * 800 * sizeof(int*));
				break;
			case 2:
				imagenTexturaSanJose = malloc(800 * 800 * sizeof(int*));
				break;
			case 3:
				imagenTexturaGuana = malloc(800 * 800 * sizeof(int*));
				break;
			case 4:
				imagenTexturaPunta = malloc(800 * 800 * sizeof(int*));
				break;
			case 5:
				imagenTexturaAlajuela = malloc(800 * 800 * sizeof(int*));
				break;
			case 6:
				imagenTexturaCartago = malloc(800 * 800 * sizeof(int*));
				break;
			case 7:
				imagenTexturaLimon = malloc(800 * 800 * sizeof(int*));
				break;
			default:
				break;
		}

	for(i = 0; i < 800*800; i++){

		switch(prov){
			case 1:
				imagenTexturaHeredia[i] = malloc(3 * sizeof(int));
				imagenTexturaHeredia[i][2] = getc(file) / 255.0;
				imagenTexturaHeredia[i][1] = getc(file) / 255.0;
				imagenTexturaHeredia[i][0] = getc(file) / 255.0;
			break;
			case 2:
				imagenTexturaSanJose[i] = malloc(3 * sizeof(int));
				imagenTexturaSanJose[i][2] = getc(file) / 255.0;
				imagenTexturaSanJose[i][1] = getc(file) / 255.0;
				imagenTexturaSanJose[i][0] = getc(file) / 255.0;
			break;
			case 3:
				imagenTexturaGuana[i] = malloc(3 * sizeof(int));
				imagenTexturaGuana[i][0] = getc(file) / 255.0;
				imagenTexturaGuana[i][1] = getc(file) / 255.0;
				imagenTexturaGuana[i][2] = getc(file) / 255.0;
			break;
			case 4:
				imagenTexturaPunta[i] = malloc(3 * sizeof(int));
				imagenTexturaPunta[i][2] = getc(file) / 255.0;
				imagenTexturaPunta[i][1] = getc(file) / 255.0;
				imagenTexturaPunta[i][0] = getc(file) / 255.0;
			break;
			case 5:
				imagenTexturaAlajuela[i] = malloc(3 * sizeof(int));
				imagenTexturaAlajuela[i][2] = getc(file) / 255.0;
				imagenTexturaAlajuela[i][1] = getc(file) / 255.0;
				imagenTexturaAlajuela[i][0] = getc(file) / 255.0;
			break;
			case 6:
				imagenTexturaCartago[i] = malloc(3 * sizeof(int));
				imagenTexturaCartago[i][2] = getc(file) / 255.0;
				imagenTexturaCartago[i][1] = getc(file) / 255.0;
				imagenTexturaCartago[i][0] = getc(file) / 255.0;
			break;
			case 7:
				imagenTexturaLimon[i] = malloc(3 * sizeof(int));
				imagenTexturaLimon[i][2] = getc(file) / 255.0;
				imagenTexturaLimon[i][1] = getc(file) / 255.0;
				imagenTexturaLimon[i][0] = getc(file) / 255.0;
			break;
			default:
				imagenTexturaHeredia[i] = malloc(3 * sizeof(int));
				imagenTexturaHeredia[i][2] = getc(file) / 255.0;
				imagenTexturaHeredia[i][1] = getc(file) / 255.0;
				imagenTexturaHeredia[i][0] = getc(file) / 255.0;
		}

	}
}



void readFile(char* filename,vertice* puntos,int numVert)
{   
	FILE *file;
    file = fopen(filename, "r");
    char array[100000];

    fgets(array, sizeof(array), file);
    strip(array);
    int i;
    for(i=0;i<numVert;i++){    

        fgets(array, sizeof(array), file);
        strip(array); //Quita espacios null
        //printf("%d - %s\n",i,array );

        char *ch;
        ch = strtok(array, ",");
        int flagRead = 0;
        while (ch != NULL) {
            if(flagRead ==0){
                puntos[i].X = atof(ch);
                //printf("%dx - %d\n",i,puntos[i].x );
                flagRead = 1;
            }else{
                puntos[i].Y = atof(ch);
                //printf("%dyRead - %d\n",i,puntos[i].Y );
                flagRead = 0;
            }
            ch = strtok(NULL, " ,");
        }  
    }
    fclose(file);
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

int eliminarRodilla(int pos, int numVerticesActivos){
	
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

// n = 1 es rodilla, n = 0 es punta
int evaluarY(int Y1, int Y2, int scanline){
	int n = 1;
	if((Y1 <= scanline && Y2 <= scanline) || (Y1 >= scanline && Y2 >= scanline)){
		n = 0;
	}

	return n;
}

int verificarPuntas(int scanline, int numVerticesActivos){
	int i, k, j, n;
	vertice v1, v2;
	for (i = 0; i < numVerticesActivos -2; i+=2)
	{
		v1 = verticesActivos[i];
		v2 = verticesActivos[i+1];

		for (j = i + 2; j < numVerticesActivos-1; j+=2)
		{	
			n = 0;
			if(v1.X == verticesActivos[j].X && v1.Y == verticesActivos[j].Y){
				n = evaluarY(v2.Y, verticesActivos[j+1].Y, scanline);
			}
			else if(v1.X == verticesActivos[j+1].X && v1.Y == verticesActivos[j+1].Y){
				n = evaluarY(v2.Y, verticesActivos[j].Y, scanline);
			}
			else if(v2.X == verticesActivos[j].X && v2.Y == verticesActivos[j].Y){
				n = evaluarY(v1.Y, verticesActivos[j+1].Y, scanline);
			}
			else if(v2.X == verticesActivos[j+1].X && v2.Y == verticesActivos[j+1].Y){
				n = evaluarY(v1.Y, verticesActivos[j].Y, scanline);
			}
			if(n == 1)
				numVerticesActivos = eliminarRodilla(j, numVerticesActivos);
		}
	}
	return numVerticesActivos;
}

int activarVertices(int scanline, vertice* vertices, int numVertices){

	int i;
	int k = 0;
	for (i = 0; i < numVertices-1; i++)
	{
		
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

void ordenarIntersecciones(int numIntersecciones){
	int i, v;
	i = 0;
	while(i < numIntersecciones-1){
		if(intersecciones[i].X > intersecciones[i+1].X){
			v = intersecciones[i].X;
			intersecciones[i].X = intersecciones[i+1].X;
			intersecciones[i+1].X = v;
			i = 0;
		}else{
			i++;
		}
	}
}

void pintarProvincia(vertice* vertices, int numVertices,int prov){
	int scanline = calcularYmax(vertices, numVertices);
	int textY = -1;
	while(scanline > calcularYmin(vertices, numVertices)){
	
	    int numVerticesActivos = activarVertices(scanline, vertices, numVertices);
		int i;

	
		
	    int numIntersecciones= calcularIntersecciones(scanline, numVerticesActivos);
	
		ordenarIntersecciones(numIntersecciones);
		for (i = 0; i < numIntersecciones; i+=2)
		{	
			if(intersecciones[i].Y==intersecciones[i+1].Y&&flagTexture == 0){
				bresenham(intersecciones[i].X,intersecciones[i].Y,intersecciones[i+1].X,intersecciones[i+1].Y);
			}else if(intersecciones[i].Y==intersecciones[i+1].Y&&flagTexture == 1){
				switch(prov) {

				   case 1  :

				      plotEspecialExtendido(textY,intersecciones[i].X,0,intersecciones[i+1].X,scanline,imagenTexturaHeredia);
				      break; /* optional */
					
				   case 2 :
				     	plotEspecialExtendido(textY,intersecciones[i].X,0,intersecciones[i+1].X,scanline,imagenTexturaSanJose);
				      break; /* optional */
				     	case 3 :
				     	plotEspecialExtendido(textY,intersecciones[i].X,0,intersecciones[i+1].X,scanline,imagenTexturaGuana);
				      break; /* optional */
				     	case 4 :
				     	plotEspecialExtendido(textY,intersecciones[i].X,0,intersecciones[i+1].X,scanline,imagenTexturaPunta);
				      break; /* optional */
				     	case 5 :
				     	plotEspecialExtendido(textY,intersecciones[i].X,0,intersecciones[i+1].X,scanline,imagenTexturaAlajuela);
				      break;
				      case 6 :
				     	plotEspecialExtendido(textY,intersecciones[i].X,0,intersecciones[i+1].X,scanline,imagenTexturaCartago);
				      break;
				  	case 7 :
				     	plotEspecialExtendido(textY,intersecciones[i].X,0,intersecciones[i+1].X,scanline,imagenTexturaLimon);
				      break;
			
				   default : /* Optional */
				   	bresenham(intersecciones[i].X,intersecciones[i].Y,intersecciones[i+1].X,intersecciones[i+1].Y);
				}
				
			}
			
		
		}
		textY++;
		if(textY >= 700){
			textY = -1;
		}
	    scanline--;
	}



}



void plotEspecial(int x, int y,float r, float g, float b) {
	setcolor (r, g, b);
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
}

void plotEspecialExtendido(int textY,int x1,int y1, int x2,int y2,float ** imagenTextura){
	int u;
	int newInd = 800 + 800*textY;
	for(u = x1;u < x2;u++){
		if(newInd < 800*750){
			plotEspecial(u,y2,imagenTextura[u+newInd][0],imagenTextura[u+newInd][1],imagenTextura[u+newInd][2]);
		}
	}
}

void bordeProvincia(vertice* provincia, int num){
	int i;
	for (i = 0; i < num - 1; i++)
	{
		bresenham(provincia[i].X,provincia[i].Y,provincia[i+1].X,provincia[i+1].Y);
	}
}



void mostrarBordes(){
	bordeProvincia(verticesGuana, numVerticesGuana);
	bordeProvincia(verticesAlajuela, numVerticesAlajuela);
	bordeProvincia(verticesPunta, numVerticesPunta);
	bordeProvincia(verticesPuntaGolfo, numVerticesPuntaGolfo);
	bordeProvincia(verticesSJ, numVerticesSJ);
	bordeProvincia(verticesHeredia, numVerticesHeredia);
	bordeProvincia(verticesLimon, numVerticesLimon);
	bordeProvincia(verticesCartago, numVerticesCartago); 
}

void mostrarProvinciasPintadas(){
	setcolor (0.4f, 0.6f, 0.3f);	
	pintarProvincia(verticesHeredia, numVerticesHeredia,1);
	setcolor (1.0f, 0.0f, 0.0f);	
	pintarProvincia(verticesSJ, numVerticesSJ,2);
	setcolor (0.0f, 0.0f, 1.0f);	
	pintarProvincia(verticesGuana, numVerticesGuana,3);
	setcolor (0.0f, 1.0f, 0.0f);	
	pintarProvincia(verticesPunta, numVerticesPunta,4);
	pintarProvincia(verticesPuntaGolfo, numVerticesPuntaGolfo,4);
	setcolor (0.6f, 0.6f, 0.1f);	
	pintarProvincia(verticesAlajuela, numVerticesAlajuela,5);
	setcolor (0.5f, 0.5f, 0.5f);	
	pintarProvincia(verticesCartago, numVerticesCartago,6);
	setcolor (0.1f, 0.5f, 0.0f);	
	pintarProvincia(verticesLimon, numVerticesLimon,7);
	// setcolor (0.0f, 0.5f, 0.1f);
}

void dibujarBotones(){
	int y = 1024;
	setcolor (1.0f, 1.0f, 1.0f);
	while(y >= 950){
		bresenham(0,y,50,y);
		y--; 
	}

	y = 945;
	while(y >= 871){
		setcolor (1.0f, 0.0f, 0.0f);
		bresenham(0,y,17,y);
		setcolor (0.0f, 1.0f, 0.0f);
		bresenham(18,y,34,y);
		setcolor (0.0f, 0.0f, 1.0f);
		bresenham(35,y,50,y);
		y--;
	}
}

void mostrarBordesRotados(){
	bordeProvincia(verticesAuxGuana, numVerticesGuana);
	bordeProvincia(verticesAuxAlajuela, numVerticesAlajuela);
	bordeProvincia(verticesAuxPunta, numVerticesPunta);
	bordeProvincia(verticesAuxPuntaGolfo, numVerticesPuntaGolfo);
	bordeProvincia(verticesAuxSJ, numVerticesSJ);
	bordeProvincia(verticesAuxHeredia, numVerticesHeredia);
	bordeProvincia(verticesAuxLimon, numVerticesLimon);
	bordeProvincia(verticesAuxCartago, numVerticesCartago);
}


void mostrarProvinciasPintadasRotadas(){
	setcolor (0.4f, 0.6f, 0.3f);
	pintarProvincia(verticesAuxHeredia, numVerticesHeredia,1);
	setcolor (1.0f, 0.0f, 0.0f);
	pintarProvincia(verticesAuxSJ, numVerticesSJ,2);
	setcolor (0.0f, 0.0f, 1.0f);
	pintarProvincia(verticesAuxGuana, numVerticesGuana,3);
	setcolor (0.0f, 1.0f, 0.0f);
	pintarProvincia(verticesAuxPunta, numVerticesPunta,4);
	pintarProvincia(verticesAuxPuntaGolfo, numVerticesPuntaGolfo,4);
	setcolor (0.6f, 0.6f, 0.1f);
	pintarProvincia(verticesAuxAlajuela, numVerticesAlajuela,5);
	setcolor (0.5f, 0.5f, 0.5f);
	pintarProvincia(verticesAuxCartago, numVerticesCartago,6);
	setcolor (0.1f, 0.5f, 0.0f);
	pintarProvincia(verticesAuxLimon, numVerticesLimon,7);
	setcolor (0.0f, 0.5f, 0.1f);
}

void init(){


	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(xmin, xmax, ymin, ymax);
	if ((flag == 0 || flag == -1)&&flagRotacion != 1) {
		setcolor (1.0f, 1.0f, 1.0f);
		mostrarBordes();
		glutSwapBuffers();
	} else if (flag == 1 && flagRotacion != 1) {
		glClearColor (0.0, 0.0, 0.0, 1.0);
		mostrarProvinciasPintadas();
		glutSwapBuffers();
	}
	else if (flag == 1 && flagRotacion == 1) {
		glClearColor (0.0, 0.0, 0.0, 1.0);
		mostrarProvinciasPintadasRotadas();
		glutSwapBuffers();
	}
	else if (flag == 0 && flagRotacion == 1) {
		glClearColor (0.0, 0.0, 0.0, 1.0);
		mostrarBordesRotados();
		glutSwapBuffers();
	}
	
   	glFlush();
}





void plot(int x, int y) {
	//setcolor (1.0f, 1.0f, 1.0f);


	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
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