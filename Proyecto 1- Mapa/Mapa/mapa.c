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

#define resolucion 1024
#define numVerticesSJ 23
#define numVerticesHeredia 13
#define numVerticesCartago 13
#define numVerticesGuana 35
#define numVerticesAlajuela 23
#define numVerticesPuntaGolfo 11
#define numVerticesPunta 49
#define numVerticesLimon 29
void plot();
void strip(char *s);
void plotV();
void init();
void bresenham2();
void printGuana();
void printAlajuela();
void printHeredia();
void printPunta();
void printSanJo();
void printLimon();
void printCar();
void mouse();
void mostrarBordes();
void mostrarProvinciasPintadas();
void dibujarBotones();
void processSpecialKeys(int key, int x, int y);
void keyBoard(unsigned char key, int x, int y);
void ArrowKey(int key, int x, int y);

typedef struct{
	int X;
	int Y;
}
vertice;

float grados = 0.0;
float zoom = 1.0;

float xmin = -0.5;
float ymin = -0.5;
float xmax = resolucion + 0.5;
float ymax = resolucion + 0.5;


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
vertice verticesAuxPunta[numVerticesPunta];
vertice verticesAuxHeredia[numVerticesHeredia];
vertice verticesAuxCartago[numVerticesCartago];
vertice verticesAuxLimon[numVerticesLimon];
vertice verticesAuxSJ[numVerticesSJ];

vertice verticesActivos[100];
vertice intersecciones[100];

void readFile(char* filename,vertice* puntos,int numVert);


void setcolor(float r, float g, float b){
	glColor3f(r, g, b);
}

int main(int argc, char** argv)
{

	readFile("Guanacaste.txt",verticesGuana,numVerticesGuana);
	readFile("Alajuela.txt",verticesAlajuela,numVerticesAlajuela);
	readFile("Heredia.txt",verticesHeredia,numVerticesHeredia);
	readFile("Limon.txt",verticesLimon,numVerticesLimon);
	readFile("Cartago.txt",verticesCartago,numVerticesCartago);
	readFile("SanJose.txt",verticesSJ,numVerticesSJ);
	readFile("Puntarenas.txt",verticesPunta,numVerticesPunta);
	readFile("PuntaGolfo.txt",verticesPuntaGolfo,numVerticesPuntaGolfo);

	// verticesSJ[19].Y = 1029-373; 
	// verticesSJ[20].X = 408; 
	// verticesSJ[20].Y = 1029-374; 

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(resolucion,resolucion);
	glutCreateWindow("Progra 1");
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(xmin, xmax, ymin, ymax);
	setcolor (1.0f, 1.0f, 1.0f);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyBoard);
	glutSpecialFunc(ArrowKey);
	glutDisplayFunc(init);
	glutMainLoop();

}

//Rotación
void keyBoard(unsigned char key, int x, int y){

  printf("%d\n", key);

  //Rotacion hacia la izquierda
   if(key == 'a' || key == 'A'){
		
		float xc = (xmin+xmax)/ 2;
		float yc = (ymin+ymax)/ 2;
		
		grados += 0.17;

		int i = 0;
		//San José
		for ( i = 0; i < numVerticesSJ; i++) {
			int tempX;
			int tempY;
			tempX = verticesSJ[i].X;
			tempY = verticesSJ[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
			int a = (int) tempXX;
			int b = (int) tempYY;
		    verticesAuxSJ[i].X = a;
			verticesAuxSJ[i].Y = b;

		}
		//Guanacaste
		for ( i = 0; i < numVerticesGuana; i++) {
			int tempX;
			int tempY;
			tempX = verticesGuana[i].X;
			tempY = verticesGuana[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxGuana[i].X = a;
			verticesAuxGuana[i].Y = b;

		}
		//Alajuela
		for ( i = 0; i < numVerticesAlajuela; i++) {
			int tempX;
			int tempY;
			tempX = verticesAlajuela[i].X;
			tempY = verticesAlajuela[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxAlajuela[i].X = a;
			verticesAuxAlajuela[i].Y = b;

		}
		//Heredia
		for ( i = 0; i < numVerticesHeredia; i++) {
			int tempX;
			int tempY;
			tempX = verticesHeredia[i].X;
			tempY = verticesHeredia[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxHeredia[i].X = a;
			verticesAuxHeredia[i].Y = b;

		}
		//Cartago
		for ( i = 0; i < numVerticesCartago; i++) {
			int tempX;
			int tempY;
			tempX = verticesCartago[i].X;
			tempY = verticesCartago[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
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
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
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
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxPunta[i].X = a;
			verticesAuxPunta[i].Y = b;
  		}
  }

  //Rotacion hacia la derecha
  
  else if(key == 'd' || key == 'D'){
		float xc = (xmin+xmax)/ 2;
		float yc = (ymin+ymax)/ 2;
		
		grados -= 0.17;

		int i = 0;
		//San José
		for ( i = 0; i < numVerticesSJ; i++) {
			int tempX;
			int tempY;
			tempX = verticesSJ[i].X;
			tempY = verticesSJ[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
			int a = (int) tempXX;
			int b = (int) tempYY;
		  verticesAuxSJ[i].X = a;
			verticesAuxSJ[i].Y = b;

		}
		//Guanacaste
		for ( i = 0; i < numVerticesGuana; i++) {
			int tempX;
			int tempY;
			tempX = verticesGuana[i].X;
			tempY = verticesGuana[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxGuana[i].X = a;
			verticesAuxGuana[i].Y = b;

		}
		//Alajuela
		for ( i = 0; i < numVerticesAlajuela; i++) {
			int tempX;
			int tempY;
			tempX = verticesAlajuela[i].X;
			tempY = verticesAlajuela[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxAlajuela[i].X = a;
			verticesAuxAlajuela[i].Y = b;

		}
		//Heredia
		for ( i = 0; i < numVerticesHeredia; i++) {
			int tempX;
			int tempY;
			tempX = verticesHeredia[i].X;
			tempY = verticesHeredia[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxHeredia[i].X = a;
			verticesAuxHeredia[i].Y = b;

		}
		//Cartago
		for ( i = 0; i < numVerticesCartago; i++) {
			int tempX;
			int tempY;
			tempX = verticesCartago[i].X;
			tempY = verticesCartago[i].Y;
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
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
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
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
			double  tempXX = xc +(tempX - xc) * cos(grados) - (tempY - yc)* sin(grados);//( tempX * cos(grados)) + ( -tempY * sin (grados)) + xc - (xc * cos(grados)) + (yc * sin(grados));
			double tempYY = yc + (tempX - xc) * sin(grados) + (tempY - yc)* cos(grados);//(tempX * sin (grados)) +( tempY * cos(grados)) + yc - (xc * sin (grados)) + (yc *	cos (grados));
			int a = (int) tempXX;
			int b = (int) tempYY;
			verticesAuxPunta[i].X = a;
			verticesAuxPunta[i].Y = b;
  }
	glutPostRedisplay();
 }
}

//Función para el Pan
void ArrowKey(int key, int x, int y){

  printf("%d\n", key);
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



//Zoom in y Zoom out
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

	//Botones para el color

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN
		&& (x >= 0 && x <= 50) && (y >= 0 && y <= 50)){ 
		glClearColor (0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		dibujarBotones();
		setcolor (1.0f, 1.0f, 1.0f);
		mostrarBordes();
		glutSwapBuffers();

	}
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN
		&& (x >= 0 && x <= 50) && (y >= 55 && y <= 105)){ 
		glClearColor (0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		dibujarBotones();
		mostrarProvinciasPintadas();
		glutSwapBuffers();
	}
	glutPostRedisplay();
	

  	printf("%f %f\n", xmin, ymin);
	printf("%f\n",zoom);
			
}

void printGuana() {
	bresenham2(154,1029-33,169,1029-33);
	bresenham2(169,1029-33,182,1029-50);
	bresenham2(182,1029-50,249,1029-69);
	bresenham2(249,1029-69,196,1029-109);
	bresenham2(196,1029-109,240,1029-142);
	bresenham2(240,1029-142,298,1029-160);
	bresenham2(298,1029-160,320,1029-195);
	bresenham2(320,1029-195,256,1029-214);
	bresenham2(256,1029-214,354,1029-251);
	bresenham2(354,1029-251,331,1029-259);
	bresenham2(331,1029-259,333,1029-283);
	bresenham2(333,1029-283,322,1029-301);
	bresenham2(322,1029-301,281,1029-290);
	bresenham2(281,1029-290,241,1029-268);
	bresenham2(241,1029-268,257,1029-328);
	bresenham2(257,1029-328,240,1029-393);
	bresenham2(240,1029-393,142,1029-354);
	bresenham2(142,1029-354,94,1029-242);
	bresenham2(94,1029-242,153,1029-178);
	bresenham2(153,1029-178,139,1029-139);
	bresenham2(139,1029-139,79,1029-112);
	bresenham2(79,1029-112,136,1029-97);
	bresenham2(136,1029-97,154,1029-33);
}

void printAlajuela() {
	bresenham2(249,1029-69,196,1029-109);
	bresenham2(196,1029-109,240,1029-142);
	bresenham2(240,1029-142,298,1029-160);
	bresenham2(298,1029-160,320,1029-195);
	bresenham2(320,1029-195,256,1029-214);
	bresenham2(256,1029-214,354,1029-251);
	bresenham2(354,1029-251,406,1029-331);
	bresenham2(406,1029-331,387,1029-347);
	bresenham2(387,1029-347,406,1029-372);
	bresenham2(406,1029-372,437,1029-344);
	bresenham2(437,1029-344,492,1029-339);
	bresenham2(492,1029-339,506,1029-143);
	bresenham2(506,1029-143,381,1029-67);
	bresenham2(381,1029-67,328,1029-98);
	bresenham2(328,1029-98,249,1029-69);
}

void printPunta() {
	bresenham2(257,1029-328,240,1029-393);
	bresenham2(240,1029-393,276,1029-438);
	bresenham2(276,1029-438,333,1029-376);
	bresenham2(333,1029-376,310,1029-342);
	bresenham2(310,1029-342,257,1029-328);

	bresenham2(354,1029-251,406,1029-331);
	bresenham2(406,1029-331,387,1029-347);
	bresenham2(387,1029-347,406,1029-372);
	bresenham2(406,1029-372,411,1029-392);
	bresenham2(411,1029-392,400,1029-403);
	bresenham2(400,1029-403,419,1029-437);
	bresenham2(419,1029-437,450,1029-420);
	bresenham2(450,1029-420,503,1029-421);
	bresenham2(503,1029-421,660,1029-553);
	bresenham2(660,1029-553,673,1029-479);
	bresenham2(673,1029-479,790,1029-543);
	bresenham2(790,1029-543,846,1029-583);
	bresenham2(846,1029-583,802,1029-634);
	bresenham2(802,1029-634,826,1029-665);
	bresenham2(826,1029-665,817,1029-706);
	bresenham2(817,1029-706,777,1029-733);
	bresenham2(777,1029-733,825,1029-801);
	bresenham2(825,1029-801,746,1029-728);
	bresenham2(746,1029-728,741,1029-663);
	bresenham2(741,1029-663,681,1029-639);
	bresenham2(681,1029-639,679,1029-672);
	bresenham2(679,1029-672,715,1029-695);
	bresenham2(715,1029-695,710,1029-730);
	bresenham2(710,1029-730,635,1029-708);
	bresenham2(635,1029-708,600,1029-656);
	bresenham2(600,1029-656,637,1029-613);
	bresenham2(637,1029-613,550,1029-500);
	bresenham2(550,1029-500,397,1029-436);
	bresenham2(397,1029-436,354,1029-338);
	bresenham2(354,1029-338,281,1029-290);
	bresenham2(281,1029-290,322,1029-301);
	bresenham2(322,1029-301,333,1029-283);
	bresenham2(333,1029-283,331,1029-259);
	bresenham2(331,1029-259,354,1029-251);
}

void printSanJo(){

	bresenham2(406,1029-372,437,1029-344);
	bresenham2(437,1029-344,492,1029-339);
	bresenham2(492,1029-339,538,1029-323);
	bresenham2(538,1029-323,526,1029-292);
	bresenham2(526,1029-292,564,1029-294);
	bresenham2(564,1029-294,569,1029-331);
	bresenham2(569,1029-331,538,1029-384);
	bresenham2(538,1029-384,661,1029-452);
	bresenham2(661,1029-452,673,1029-479);
	bresenham2(673,1029-479,660,1029-553);
	bresenham2(660,1029-553,673,1029-479);
	bresenham2(673,1029-479,660,1029-553);
	bresenham2(660,1029-553,503,1029-421);
	bresenham2(503,1029-421,450,1029-421);
	bresenham2(450,1029-420,419,1029-437);
	bresenham2(419,1029-437,400,1029-403);
	bresenham2(400,1029-403,411,1029-392);
	bresenham2(411,1029-392,406,1029-372);
}

void printHeredia() {
	/*bresenham2(492,1029-339,506,1029-143);
	bresenham2(506,1029-143,544,1029-137);
	bresenham2(544,1029-137,565,1029-164);
	bresenham2(565,1029-164,603,1029-140);
	bresenham2(603,1029-140,609,1029-165);
	bresenham2(609,1029-165,581,1029-174);
	bresenham2(581,1029-174,564,1029-294);
	bresenham2(564,1029-294,526,1029-292);
	bresenham2(526,1029-292,538,1029-323);
	bresenham2(538,1029-323,492,1029-339);*/

	bresenham2(492,1029-339,506,1029-143);
	bresenham2(506,1029-143,544,1029-137);
	bresenham2(544,1029-137,565,1029-154);
	bresenham2(565,1029-154,603,1029-140);
	bresenham2(603,1029-140,609,1029-165);
	bresenham2(609,1029-165,581,1029-174);
	bresenham2(581,1029-174,564,1029-294);
	bresenham2(564,1029-294,526,1029-292);
	bresenham2(526,1029-292,538,1029-323);
	bresenham2(538,1029-323,492,1029-339);	
}

void printCar(/* arguments */) {
		bresenham2(564,1029-294,645,1029-338);
		bresenham2(645,1029-338,699,1029-334);
		bresenham2(699,1029-334,699,1029-377);
		bresenham2(699,1029-377,661,1029-452);
		bresenham2(661,1029-452,538,1029-384);
		bresenham2(538,1029-384,569,1029-331);
		bresenham2(569,1029-331,564,1029-294);
}

void printLimon() {
	bresenham2(673,1029-479,661,1029-452);
	bresenham2(661,1029-452,699,1029-377);
	bresenham2(699,1029-377,699,1029-334);
	bresenham2(699,1029-334,645,1029-338);
	bresenham2(645,1029-338,564,1029-294);
	bresenham2(564,1029-294,581,1029-174);
	bresenham2(581,1029-174,609,1029-165);
	bresenham2(609,1029-165,603,1029-140);
	bresenham2(603,1029-140,623,1029-106);
	bresenham2(623,1029-106,822,1029-395);
	bresenham2(822,1029-395,891,1029-435);
	bresenham2(891,1029-435,870,1029-452);
	bresenham2(870,1029-452,824,1029-425);
	bresenham2(824,1029-425,798,1029-548);
	bresenham2(798,1029-548,673,1029-479);
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
        int flag = 0;
        while (ch != NULL) {
            if(flag ==0){
                puntos[i].X = atof(ch);
                //printf("%dx - %d\n",i,puntos[i].x );
                flag = 1;
            }else{
                puntos[i].Y = atof(ch);
                //printf("%dyRead - %d\n",i,puntos[i].Y );
                flag = 0;
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

// n = 1 es rodilla, n = 0 es punta
int evaluarY(int Y1, int Y2, int scanline){
	int n = 1;
	//printf("Y1 = %d, Y2 = %d, scanline = %d\n", Y1, Y2, scanline);
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

void pintarProvincia(vertice* vertices, int numVertices){
	int scanline = calcularYmax(vertices, numVertices);
	
	while(scanline > calcularYmin(vertices, numVertices)){
	
	    int numVerticesActivos = activarVertices(scanline, vertices, numVertices);
		int i;

	
		
	    int numIntersecciones= calcularIntersecciones(scanline, numVerticesActivos);
	
		ordenarIntersecciones(numIntersecciones);
		for (i = 0; i < numIntersecciones; i+=2)
		{	
			if(intersecciones[i].Y==intersecciones[i+1].Y){
			bresenham2(intersecciones[i].X,intersecciones[i].Y,intersecciones[i+1].X,intersecciones[i+1].Y);}
		
		}
	    scanline--;
	}

}


void bordeProvincia(vertice* provincia, int num){
	int i;
	for (i = 0; i < num - 1; i++)
	{
		bresenham2(provincia[i].X,provincia[i].Y,provincia[i+1].X,provincia[i+1].Y);
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
	pintarProvincia(verticesHeredia, numVerticesHeredia);
	setcolor (1.0f, 0.0f, 0.0f);	
	pintarProvincia(verticesSJ, numVerticesSJ);
	setcolor (0.0f, 0.0f, 1.0f);	
	pintarProvincia(verticesGuana, numVerticesGuana);
	setcolor (0.0f, 1.0f, 0.0f);	
	pintarProvincia(verticesPunta, numVerticesPunta);
	pintarProvincia(verticesPuntaGolfo, numVerticesPuntaGolfo);
	setcolor (0.6f, 0.6f, 0.1f);	
	pintarProvincia(verticesAlajuela, numVerticesAlajuela);
	setcolor (0.5f, 0.5f, 0.5f);	
	pintarProvincia(verticesCartago, numVerticesCartago);
	setcolor (0.1f, 0.5f, 0.0f);	
	pintarProvincia(verticesLimon, numVerticesLimon);
	// setcolor (0.0f, 0.5f, 0.1f);
}

void dibujarBotones(){
	int y = 1024;
	setcolor (1.0f, 1.0f, 1.0f);
	while(y >= 950){
		bresenham2(0,y,50,y);
		y--; 
	}

	y = 945;
	while(y >= 871){
		setcolor (1.0f, 0.0f, 0.0f);
		bresenham2(0,y,17,y);
		setcolor (0.0f, 1.0f, 0.0f);
		bresenham2(18,y,34,y);
		setcolor (0.0f, 0.0f, 1.0f);
		bresenham2(35,y,50,y);
		y--;
	}
}

void init(){
	//printGuana();
	//printAlajuela();
	//printPunta();
	//printSanJo();
	//printHeredia();
	//printLimon();
	//printCar();
	//setcolor (0.4f, 0.6f, 0.3f);
	//bresenham2(0, 600, 1024, 600);

	//bresenham2(0, 694, 1024, 694);

	//bresenham2(506,886,551,886); 
	//bresenham2(594,886,603,886);

	//mostrarBordes();
	//mostrarProvinciasPintadas();

	dibujarBotones();
   	glFlush();
}

void bresenham2(int x0, int y0,int x1, int y1) {
      int d, Delta_E, Delta_NE, Delta_S, Delta_SE, Delta_SO, Delta_O, Delta_NO, Delta_N,xp, yp;
      int difX = fabs((x0 - x1));
      int dify = fabs((y0 - y1));

      if (difX >= dify) {
				if ((x0 > x1 && y0 > y1)|| (x0 < x1 && y0 > y1)){
					int var = x0;
					x0 = x1;
					x1 = var;
					var = y0;
					y0 = y1;
					y1 = var;
				}
				xp = x0; yp = y0;
				plot(xp, yp);
        if (x0 < x1 && y0 <= y1) {
          //Cuadrante 1
          //printf("cuadrante1 x = %d y Y = %d\n", xp,yp);
					Delta_NE = 2*(y1-y0) - 2*(x1-x0);
					Delta_E = 2*(y1-y0);
          d = 2*(y1-y0) - (x1-x0);
          while (xp <= x1) {
            //printf("1");
            if (d <= 0) {
              xp++;
              d+= Delta_E;
            } else {
              xp++; yp++;
              d += Delta_NE;
            }
            plot(xp, yp);
          }
        ////////////////////
      } else if (x0 > x1 && y0 <= y1) {
          //cuadrante4
          //printf("cuadrante4 x = %d y Y = %d\n", xp,yp);
					Delta_NO = -2*(y1-y0) - 2*(x1-x0);
				  Delta_O = -2*(y1-y0);
          d = -2*(y1-y0)-(x1-x0);
          while (xp >= x1) {
            //printf("4");
            if (d >= 0) {
              xp--;
              d+= Delta_O;
            } else {
              xp--; yp++;
              d += Delta_NO;
            }
            plot(xp, yp);
          }
          /////////////////
      }
      } else if (difX < dify) {
				if ((x0 >= x1 && y0 > y1)||(x0 < x1 && y0 > y1)){
					int var = x0;
					x0 = x1;
					x1 = var;
					var = y0;
					y0 = y1;
					y1 = var;
				}
				xp = x0; yp = y0;
				plot(xp, yp);
        if (x0 <= x1 && y0 < y1) {
          //Cuadrante 2
          //printf("cuadrante2 x = %d y Y = %d\n", xp,yp);
					Delta_NE = 2*(y1-y0) - 2*(x1-x0);
					Delta_N = -2*(x1-x0);
          d = (y1-y0) - 2*(x1-x0);
          while (yp <= y1) {
            //printf("2");
            if (d >= 0) {
              yp++;
              d+= Delta_N;
            } else {
              xp++; yp++;
              d += Delta_NE;
            }
            plot(xp, yp);
          }
          ////
        } else if (x0 > x1 && y0 < y1) {
          //cuadrante3
          //printf("cuadrante3 x = %d y Y = %d\n", xp,yp);
					Delta_NO = -2*(y1-y0) - 2*(x1-x0);
					Delta_N = -2*(x1-x0);
          d = - (y1-y0) - 2*(x1-x0);
          while (yp <= y1) {
            //printf("3");
            if (d <= 0) {
              yp++;
              d+= Delta_N;
            } else {
              xp--; yp++;
              d += Delta_NO;
            }
            plot(xp, yp);
          }
          /////

			}
		}
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