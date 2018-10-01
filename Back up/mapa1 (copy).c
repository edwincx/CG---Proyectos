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

typedef struct {
    GLubyte  *dibujo;        // Un puntero a los datos de la imagen
	GLuint	bpp;		// bpp significa bits per pixel (bits por punto) es la calidad en palabras sensillas
	GLuint largo;		// Largo de la textura
	GLuint ancho;		// Ancho de la textura
	GLuint ID;		// ID de la textura, es como su nombre para opengl
}textura;

textura	texturas[1];		// Definimos nuestras texturas, por ahora solo 1
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
void processSpecialKeys(int key, int x, int y);
void keyBoard(unsigned char key, int x, int y);
void ArrowKey(int key, int x, int y);
void readFile(char* filename, vertice* puntos,int numVert);
void mostrarProvinciasPintadasRotadas();


float grados = 0.0;
float angulo=0.0;



int flag = -1;
float xmin = 0.5;
float ymin = 0.5;
float xmax = 1029.5;
float ymax = 1029.5;
float zoom = 1.0;

int numVerticesEstrella = 11;
vertice verticesEstrella[15];


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

int verticesActivos[100];
vertice intersecciones[100];




void setcolor(float r, float g, float b){
	glColor3f(r, g, b);
}

int cargarTGA( char *nombre, textura *imagen) {
	GLubyte		cabezeraTGA[12]={0,0,2,0,0,0,0,0,0,0,0,0};		// Cabezera de un tga sin compresion
        GLubyte         compararTGA[12];                                        // Aca vamos a comprar la cabezera
	GLubyte		cabezera[6];						// Los 6 bytes importantes
	GLuint		bytesporpunto;						// Cuantos bytes hay por punto
	GLuint		tamanoimagen;						// Aca guardaremos el tamaño de la imagen
	GLuint		temp,i;							// Variable temporal, y una para usar con el for
	GLuint		tipo=GL_RGBA;						// Nuestro typo por defecto, lo veremos mas abajo

	FILE *archivo=fopen(nombre, "rb");	// Cargamos nuestro archivo en memoria
	if( archivo == NULL ||			// Existe nuestro archivo??
            fread(compararTGA,1,sizeof(compararTGA),archivo)!=sizeof(compararTGA)  ||      // Hay 12 bytes para leer??
	    memcmp(cabezeraTGA,compararTGA,sizeof(compararTGA))!=0		||	// Son iguales??
	    fread(cabezera,1,sizeof(cabezera),archivo)!=sizeof(cabezera)) {
		if(archivo==NULL) {
			printf("No se encontro el archivo %s\n",nombre);
			return 0;		// No se abrio el archivo
		}
		else {
			fclose(archivo);
			return 0;
		}
	}
	/* Ahora hay que leer la cabezera del archivo, para saber cuanto es el largo, ancho, y los bytes por puntos,
	para eso aca hay una ilustracion de la cabezera :
	6 bytes -> xxxxxxx xxxxxxx xxxxxxx xxxxxxx xxxxxxx xxxxxxx	
		   |--- Largo ---| |---Ancho-----| |-bpp-|
	El dato del largo se guarda en el cabezera[0] y cabezera[1], para leerlo hay que multiplicar cabezera[0] por 256 y 	sumarselo a cabezera[1], para leer ancho y bpp es el mismo procedimiento */
	imagen->largo=256*cabezera[1]+ cabezera[0];
	imagen->ancho=256*cabezera[3]+ cabezera[2];

	/* Ahora vemos si hay datos no validos, como largo o ancho iguales menores a 0 o iguales a 0 */
	if( imagen->largo <= 0 ||	// Largo mayor que 0?? 
	    imagen->ancho <= 0 ||	// Ancho mayor que 0??
	    (cabezera[4]!=24 && cabezera[4]!=32)) {	// bpp es 24 o 32?? (solo se cargan 24 y 32 bpp)
                printf("Daton invalidos\n");
		fclose(archivo);
		return 0;
	}
	imagen->bpp=cabezera[4];	// Aca se guardan los bits por punto
	bytesporpunto=cabezera[4]/8;	// Aca los bytes por punto (1 byte = 8 bits)
        tamanoimagen=imagen->largo * imagen->ancho * bytesporpunto;      // Esta es la memoria que nesecitaremos para guardar los datos de la textura
	/*Ahora reservamos espacio en memoria para nuestra textura, luego leemos la textura del archivo */
	imagen->dibujo = (GLubyte *)malloc(tamanoimagen);	// Reservamos la memoria necesaria para nuestra textura
	if(imagen->dibujo== NULL ||		// Se logro reservar la memoria???
	   fread(imagen->dibujo, 1, tamanoimagen, archivo) != tamanoimagen ) {	// Se lee, y se comprueba que lo leido es de la misma longitud que la asignada a a dibujo.
		if(imagen->dibujo != NULL) {
			printf("Error leyendo imagen\n");
			free(imagen->dibujo);
		} else printf("Error asignando memoria\n");
		fclose(archivo);
		return 0;
	}
	/* El formato tga guarda las imagenes en BGR, y opengl usa RGB,por lo cambiamos de lugares */
        for(i=0; i< (int)tamanoimagen; i+=bytesporpunto)
	{
		temp=imagen->dibujo[i];				// Guardamos el primer valor
		imagen->dibujo[i] = imagen->dibujo[i + 2];	// Asignamos el nuevo primer valor
		imagen->dibujo[i + 2] = temp;			// Asignamos el ultimo valor
	}
	
	fclose (archivo);	// Cerramos el archivo
	
	/* Listo, terminamos con el codigo de carga, volvemos a opengl, ahora hay que asignarle a la textura un ID, luego 	decirle a opengl cuales son el largo, el ancho y los bpp */

        glGenTextures( 1 , &imagen[0].ID);    // Crea un ID para la textura, buscando un id que este vacio
        glBindTexture(GL_TEXTURE_2D, imagen[0].ID);       // Seleccionamos nuestra textura
	if(imagen->bpp ==24) tipo= GL_RGB;	// Si nuestra textura es de 24 bits, entonces se crea una textura rgb, sino una rgba
	/* Ahora creamos nuestra textura, entrando el largo, ancho y tipo */
        glTexImage2D(GL_TEXTURE_2D, 0, tipo, imagen[0].ancho, imagen[0].largo, 0, tipo, GL_UNSIGNED_BYTE, imagen[0].dibujo);
	/* Ahora le decimos a opengl como queremos que se vea nuestra textura, MAG_FILTER es cuando la textura es mas grande que el lugar donde la asignamos, y MIG_FILTER, es cuando la textura es mas pequeña que el lugar donde la asignamos, GL_LINEAR es para que se vea bien tanto cerca como lejos, pero ocupa bastante procesador. Otra opcion el GL_NEARES, que ocupa menos prosesador */
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return 1;	// Todo salio bien
}

void inicializar() {
        glShadeModel(GL_SMOOTH);       // Esto hace que al dibujar las sombran en los objetos, se vean mas parejas haciendo que se vean mejor.
        glClearColor(0.0, 0.0, 0.0, 0.0);       // Pondremos aca la funcion glclearcolor
        // Las 3 lineas siguientes hacen que el depth buffer ordene los objetos que deben ser puestos primero en panatalla y los que deben ser dibujados despues dependiendo de cuan cerca estan de la camara.
        glClearDepth(1.0);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Esto hace que opengl calcule las perspectivas de la mejor forma, quita un poco de rendimiento, pero hace que las perspectivas se vean un poco mejor.
		glEnable(GL_TEXTURE_2D);
        if(!cargarTGA("galaxias.tga", &texturas[0])) { 
                printf("Error cargando textura\n");
		exit(0); // Cargamos la textura y chequeamos por errores
	}
}

void dibujar () {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Se borra el buffer de la pantalla y el de profundidad
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, -3.0);
	glRotatef(angulo, 0.0, 1.0, 1.0);	// Rotamos nuestro objeto
        glBindTexture(GL_TEXTURE_2D,texturas[0].ID);
	// Vamos a usar un cubo
	glBegin(GL_QUADS);
	/* La textura es una imagen cuadrada, ahora hay que asignarle los lugares donde va, para eso esta glTexCoord2f(float x, float y) donde x e y son las coordenadas de la textura que van a coincidir con el punto que vamos a definir */	
		glTexCoord2i(300.0, 0.0); glVertex3f(800-200, -0.5,  0.5);
		glTexCoord2i(1.0, 0.0); glVertex3f( 0.5, -0.5,  0.5);
		glTexCoord2i(1.0, 1.0); glVertex3f( 0.5,  0.5,  0.5);
		glTexCoord2i(0.0, 1.0); glVertex3f(-0.5,  0.5,  0.5);

	glEnd();
	//angulo++;
	glLoadIdentity();
	glPopMatrix();
	glutSwapBuffers();
}


void cambiarTamano(int largo, int ancho) {
        if(ancho==0) ancho=1;           // Previene que dividamos por 0
        glMatrixMode(GL_PROJECTION);    // Escojemos la matriz de proyeccion
        glLoadIdentity();               // Se resetea la matriz
        glViewport(0,0,largo, ancho);   // Se va a usar toda la ventana para mostrar graficos
        gluPerspective( 45 ,            // Angulo de vision
                        (float)largo/(float)ancho,
                        1.0,              // Cuan cerca se puede ver
                        1000);          // Cuan lejos se puede ver
        glMatrixMode(GL_MODELVIEW);     // Escojemos la matriz de vista
	glLoadIdentity();		// Se resetea la matriz
        gluLookAt( 0.0, 0.0, 0.0,       // Hacia donde miramos
                   0.0, 0.0, -3.0,      // Desde donde miramos
                   0.0, 1.0, 0.0);      // Que eje es el que esta hacia arriba
}

int main(int argc, char** argv)
{

	readFile("Guanacaste.txt",verticesGuana,numVerticesGuana);
	readFile("Alajuela.txt",verticesAlajuela,numVerticesAlajuela);
	// readFile("Heredia.txt",verticesHeredia,numVerticesHeredia);
	// readFile("Limon.txt",verticesLimon,numVerticesLimon);
	// readFile("Cartago.txt",verticesCartago,numVerticesCartago);
	// readFile("SanJose.txt",verticesSJ,numVerticesSJ);
	// readFile("Puntarenas.txt",verticesPunta,numVerticesPunta);
	// readFile("PuntaGolfo.txt",verticesPuntaGolfo,numVerticesPuntaGolfo);

	verticesEstrella[0].X = 400;//pico
    verticesEstrella[0].Y = 400;

    verticesEstrella[1].X = 350;
    verticesEstrella[1].Y = 300;

    verticesEstrella[2].X = 250;//pico
    verticesEstrella[2].Y = 400;

    verticesEstrella[3].X = 350;
    verticesEstrella[3].Y = 200;

    verticesEstrella[4].X = 250;//pico
    verticesEstrella[4].Y = 100;

    verticesEstrella[5].X = 400;
    verticesEstrella[5].Y = 150;

    verticesEstrella[6].X = 500;//pico
    verticesEstrella[6].Y = 50;

    verticesEstrella[7].X = 450;
    verticesEstrella[7].Y = 200;

    verticesEstrella[8].X = 550;//pico
    verticesEstrella[8].Y = 650;

    verticesEstrella[9].X = 450;
    verticesEstrella[9].Y = 275;

    verticesEstrella[10].X = 400;//pico
    verticesEstrella[10].Y = 400;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(resolucion,resolucion);

	glutCreateWindow("Progra 1");
	setcolor (1.0f, 1.0f, 1.0f);
	glutMouseFunc(mouse);
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(keyBoard);
	glutSpecialFunc(ArrowKey);
	glutDisplayFunc(init);
        glutIdleFunc(init);
	glutMainLoop();

	// glutInit(&argc,argv);       
 //        glutInitWindowSize( resolucion, resolucion);
 //        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
 //        // glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
 //        glutCreateWindow("Un cubo con textura");
 //        inicializar();
 //        glutDisplayFunc(dibujar);
 //        glutIdleFunc(dibujar);
 //        glutReshapeFunc(cambiarTamano);
 //        glutMainLoop();

}

void processSpecialKeys(int key, int x, int y) {

	if (key == GLUT_KEY_F1) {
		flag = 0;
	} else if (key == GLUT_KEY_F2) {
		flag = 1;
	}
	 else if (key == GLUT_KEY_F3) {
		flag = 2;
	}
	printf("%d\n", flag);
	glutPostRedisplay();
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
 //  	printf("%f %f\n", xmin, ymin);
	// printf("%f\n",zoom);
			
}

//Rotación
void keyBoard(unsigned char key, int x, int y){
	printf("%f\n",grados );

	if (key == '1') {
		flag = 0;
	} else if (key == '2') {
		printf("no se pego\n");
		flag = 1;
	}
	 else if (key == '3') {
	 	printf("si se pego\n");
		flag = 2;
	}
	else if (key == '4') {
		FILE *file;
    file = fopen("pic.jpg", "r");
    char array[1000];

    fgets(array, sizeof(array), file);
   
    int i;

      printf("%s\n",array );


    
    fclose(file);






	}
  //Rotacion hacia la izquierda
   else if(key == 'a' || key == 'A'){
		
		float xc = (xmin+xmax)/ 2;
		float yc = (ymin+ymax)/ 2;
		grados += 0.17;

		int i = 0;
		//SJ
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
		//guanacaste
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
		//alajuela
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
		//heredia
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
		//cartago
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
		//SJ
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
		//guanacaste
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
		//alajuela
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
		//heredia
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
		//cartago
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
                puntos[i].X = atoi(ch);
                //printf("%dx - %d\n",i,puntos[i].x );
                flagRead++;
            }
            else if(flagRead == 1){
                puntos[i].Y = atoi(ch);
                //printf("%dyRead - %d\n",i,puntos[i].Y );
                flagRead++;
            }
            ch = strtok(NULL, " ,");
        }  
    }
    fclose(file);
}

// int eliminarHorizontales(int numVerticesActivos){
// 	int i, k;
// 	for (i = 0, k = 0; i < numVerticesActivos -1; i+=2)
// 	{
// 		if(verticesActivos[i].Y != verticesActivos[i+1].Y){
// 			verticesActivos[k] = verticesActivos[i];
// 			verticesActivos[k+1] = verticesActivos[i+1];
// 			k+=2;
// 		}
// 	}
// 	return k;
// }

// int eliminarVerticales(int numVerticesActivos){
// 	int i, k;
// 	for (i = 0, k = 2; i < numVerticesActivos -1; i+=2)
// 	{
// 		if(verticesActivos[i].X == verticesActivos[i+1].X){
// 			verticesActivos[i].X += 1;
// 		}
// 	}
// 	return numVerticesActivos;
// }

// int eliminarRodilla(int pos, int numVerticesActivos){
	
// 	int i;
// 	if(pos == numVerticesActivos - 2)
// 		numVerticesActivos-=2;
// 	else{
// 		for (i = pos + 2 ; i < numVerticesActivos - 1; i+=2)
// 		{
// 			verticesActivos[pos] = verticesActivos[i];
// 			verticesActivos[pos+1] = verticesActivos[i+1];
// 		}
// 		numVerticesActivos-=2;
// 	}

// 	return numVerticesActivos;
// }

// n = 1 es rodilla, n = 0 es punta
int evaluarY(int Y1, int Y2, int scanline){
	int n = 1;
	if((Y1 <= scanline && Y2 <= scanline) || (Y1 >= scanline && Y2 >= scanline)){
		n = 0;
	}

	return n;
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

// int verificarPuntas(int scanline, int numVerticesActivos){
// 	int i, k, j, n;
// 	vertice v1, v2;
// 	for (i = 0; i < numVerticesActivos -2; i+=2)
// 	{
// 		v1 = verticesActivos[i];
// 		v2 = verticesActivos[i+1];

// 		for (j = i + 2; j < numVerticesActivos-1; j+=2)
// 		{	
// 			n = 0;
// 			if(v1.X == verticesActivos[j].X && v1.Y == verticesActivos[j].Y){
// 				n = evaluarY(v2.Y, verticesActivos[j+1].Y, scanline);
// 			}
// 			else if(v1.X == verticesActivos[j+1].X && v1.Y == verticesActivos[j+1].Y){
// 				n = evaluarY(v2.Y, verticesActivos[j].Y, scanline);
// 			}
// 			else if(v2.X == verticesActivos[j].X && v2.Y == verticesActivos[j].Y){
// 				n = evaluarY(v1.Y, verticesActivos[j+1].Y, scanline);
// 			}
// 			else if(v2.X == verticesActivos[j+1].X && v2.Y == verticesActivos[j+1].Y){
// 				n = evaluarY(v1.Y, verticesActivos[j].Y, scanline);
// 			}
// 			if(n == 1)
// 				numVerticesActivos = eliminarRodilla(j, numVerticesActivos);
// 		}
// 	}
// 	return numVerticesActivos;
// }

int activarVertices(int scanline, vertice* vertices, int numVertices){

	int i;
	int k = 0;
	for (i = 0; i < numVertices-1; i++)
	{
		
		if(vertices[i].Y >= scanline && vertices[i+1].Y <= scanline
			|| vertices[i].Y <= scanline && vertices[i+1].Y >= scanline){
			
			if(vertices[i+1].Y!=scanline){
				verticesActivos[k] = i;
				verticesActivos[k+1] = i+1;
				k+=2;
			}
			
		}
	}
	// k = eliminarHorizontales(k);
	// k = eliminarVerticales(k);
	// k = verificarPuntas(scanline,k);
	//printf("k = %d\n\n",k  );
	return k;
}


int calcularIntersecciones(int scanline,vertice* vert, int numVerticesActivos, int numVerticesProv){
	int i, k, Xi, Yi, Xj, Yj;
	vertice punto;
	punto.Y = scanline;
	for (i = 0, k = 0; i < numVerticesActivos; i+=2)
	{
		Xi = vert[verticesActivos[i]].X;
		Yi = vert[verticesActivos[i]].Y;
		Xj = vert[verticesActivos[i+1]].X;
		Yj = vert[verticesActivos[i+1]].Y;

		punto.X = (scanline-(Yi-((float)(Yj-Yi)/(Xj-Xi))*Xi))/((float)(Yj-Yi)/(Xj-Xi));


		intersecciones[k] = punto;
		k++;

		if(vert[verticesActivos[i]].Y == scanline){
			// if(verticesActivos[i] == 0){
			// 	if((vert[verticesActivos[i]+1].Y <= vert[verticesActivos[i]].Y && vert[verticesActivos[numVerticesProv-1]].Y < vert[verticesActivos[i]].Y)||
			// 		(vert[verticesActivos[i]+1].Y >= vert[verticesActivos[i]].Y && vert[verticesActivos[numVerticesProv-1]].Y > vert[verticesActivos[i]].Y)){
			// 		intersecciones[k] = punto;
			// 		k++;
			// 	}
			// }
			// else if(verticesActivos[i]+1 >= numVerticesProv-1){
			// 	if((vert[verticesActivos[0]].Y <= vert[verticesActivos[i]].Y && vert[verticesActivos[i]-1].Y < vert[verticesActivos[i]].Y)||
			// 		(vert[verticesActivos[0]].Y >= vert[verticesActivos[i]].Y && vert[verticesActivos[i]-1].Y > vert[verticesActivos[i]].Y)){
			// 		intersecciones[k] = punto;
			// 		k++;
			// 	}
			// 	printf("aqui\n");
			// 	// else if((vert[verticesActivos[0]].Y == vert[verticesActivos[i]].Y)){
			// 	// 	intersecciones[k] = punto;
			// 	// 	k++;
			// 	// }
			// }
			if((vert[verticesActivos[i]+1].Y < vert[verticesActivos[i]].Y && vert[verticesActivos[i]-1].Y < vert[verticesActivos[i]].Y)||
					(vert[verticesActivos[i]+1].Y > vert[verticesActivos[i]].Y && vert[verticesActivos[i]-1].Y > vert[verticesActivos[i]].Y)){
					intersecciones[k] = punto;
					k++;
			}
			
		}
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
	int minimo = calcularYmin(vertices, numVertices);


	while(scanline >= minimo){
	    int numVerticesActivos = activarVertices(scanline, vertices, numVertices);
	
	
		
	    int numIntersecciones= calcularIntersecciones(scanline,vertices, numVerticesActivos,numVertices);
	
		int i;
		ordenarIntersecciones(numIntersecciones);
		for (i = 0; i < numIntersecciones; i+=2)
		{	
			if(intersecciones[i].Y==intersecciones[i+1].Y){
			bresenham(intersecciones[i].X,intersecciones[i].Y,intersecciones[i+1].X,intersecciones[i+1].Y);}
			
		
		}
		//printf("Fin scanline %d -----------------------------\n\n",scanline);

	    scanline--;
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
	// setcolor (0.4f, 0.6f, 0.3f);	
	// pintarProvincia(verticesHeredia, numVerticesHeredia);
	// setcolor (1.0f, 0.0f, 0.0f);	
	// pintarProvincia(verticesSJ, numVerticesSJ);
	setcolor (0.0f, 0.0f, 1.0f);	
	printf("aqui se pego1\n");
	pintarProvincia(verticesGuana, numVerticesGuana);
	// setcolor (0.0f, 1.0f, 0.0f);	
	// pintarProvincia(verticesPunta, numVerticesPunta);
	// pintarProvincia(verticesPuntaGolfo, numVerticesPuntaGolfo);
	setcolor (0.6f, 0.6f, 0.1f);	
	pintarProvincia(verticesAlajuela, numVerticesAlajuela);
	// glColor3f (0.4f, 0.6f, 0.3f);
 //    pintarProvincia(verticesEstrella, numVerticesEstrella);
	// setcolor (0.5f, 0.5f, 0.5f);	
	// pintarProvincia(verticesCartago, numVerticesCartago);
	// setcolor (0.1f, 0.5f, 0.0f);	
	// pintarProvincia(verticesLimon, numVerticesLimon);

	// Se borra el buffer de la pantalla y el de profundidad
	// glPushMatrix();
	// glPushMatrix();
	// Rotamos nuestro objeto
	
	// Vamos a usar un cubo
	/* La textura es una imagen cuadrada, ahora hay que asignarle los lugares donde va, para eso esta glTexCoord2f(float x, float y) donde x e y son las coordenadas de la textura que van a coincidir con el punto que vamos a definir */	
 //        glBindTexture(GL_TEXTURE_2D,texturas[0].ID);
	// glBegin(GL_QUADS);
	// 	glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.5);
	// 	glTexCoord2f(1.0, 0.0); glVertex2f( 0.5, -0.5);
	// 	glTexCoord2f(1.0, 1.0); glVertex2f( 0.5,  0.5);
	// 	glTexCoord2f(0.0, 1.0); glVertex2f(-0.5,  0.5);
	// 	glTexCoord2f(0.0, 0.5); glVertex2f(-0.9,  0.9);

	// 	glTexCoord2f(1.0, 0.0); glVertex2f(-0.5, -0.5);
	// 	glTexCoord2f(1.0, 1.0); glVertex2f(-0.5,  0.5);
	// 	glTexCoord2f(0.0, 1.0); glVertex2f( 0.5,  0.5);
	// 	glTexCoord2f(0.0, 0.0); glVertex2f( 0.5, -0.5);
	// 	glTexCoord2f(0.0, 0.5); glVertex2f(-0.9,  0.9);

	// 	glTexCoord2f(0.0, 1.0); glVertex2f(-0.5,  0.5);
	// 	glTexCoord2f(0.0, 0.0); glVertex2f(-0.5,  0.5);
	// 	glTexCoord2f(1.0, 0.0); glVertex2f( 0.5,  0.5);
	// 	glTexCoord2f(1.0, 1.0); glVertex2f( 0.5,  0.5);
	// 	glTexCoord2f(0.0, 0.5); glVertex2f(-0.9,  0.9);

	// 	glTexCoord2f(1.0, 1.0); glVertex2f(-0.5, -0.5);
	// 	glTexCoord2f(0.0, 1.0); glVertex2f( 0.5, -0.5);
	// 	glTexCoord2f(0.0, 0.0); glVertex2f( 0.5, -0.5);
	// 	glTexCoord2f(1.0, 0.0); glVertex2f(-0.5, -0.5);
	// 	glTexCoord2f(0.0, 0.5); glVertex2f(-0.9,  0.9);

	// 	glTexCoord2f(1.0, 0.0); glVertex2f( 0.5, -0.5);
	// 	glTexCoord2f(1.0, 1.0); glVertex2f( 0.5,  0.5);
	// 	glTexCoord2f(0.0, 1.0); glVertex2f( 0.5,  0.5);
	// 	glTexCoord2f(0.0, 0.0); glVertex2f( 0.5, -0.5);
	// 	glTexCoord2f(0.0, 0.5); glVertex2f(-0.9,  0.9);

	// 	glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.5);
	// 	glTexCoord2f(1.0, 0.0); glVertex2f(-0.5, -0.5);
	// 	glTexCoord2f(1.0, 1.0); glVertex2f(-0.5,  0.5);
	// 	glTexCoord2f(0.0, 1.0); glVertex2f(-0.5,  0.5);
	// 	glTexCoord2f(0.0, 0.5); glVertex2f(-0.9,  0.9);
	// glEnd();
	
	// glLoadIdentity();
	// glPopMatrix();
	// glutSwapBuffers();
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

void mostrarProvinciasPintadasRotadas(){
	setcolor (0.4f, 0.6f, 0.3f);
	pintarProvincia(verticesAuxHeredia, numVerticesHeredia);
	setcolor (1.0f, 0.0f, 0.0f);
	pintarProvincia(verticesAuxSJ, numVerticesSJ);
	setcolor (0.0f, 0.0f, 1.0f);
	pintarProvincia(verticesAuxGuana, numVerticesGuana);
	setcolor (0.0f, 1.0f, 0.0f);
	pintarProvincia(verticesAuxPunta, numVerticesPunta);
	setcolor (0.6f, 0.6f, 0.1f);
	pintarProvincia(verticesAuxAlajuela, numVerticesAlajuela);
	setcolor (0.5f, 0.5f, 0.5f);
	pintarProvincia(verticesAuxCartago, numVerticesCartago);
	setcolor (0.1f, 0.5f, 0.0f);
	pintarProvincia(verticesAuxLimon, numVerticesLimon);
	setcolor (0.0f, 0.5f, 0.1f);
}

void init(){

	//inicializar();
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(xmin, xmax, ymin, ymax);
	if (flag == 0 || flag == -1) {
		//glClearColor (0.0, 0.0, 0.0, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT);
		//dibujarBotones();
		setcolor (1.0f, 1.0f, 1.0f);
		mostrarBordes();
		glutSwapBuffers();
	} else if (flag == 1) {
		glClearColor (0.0, 0.0, 0.0, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT);
		//dibujarBotones();
		mostrarProvinciasPintadas();
		glutSwapBuffers();
	}
	else if (flag == 2) {
		glClearColor (0.0, 0.0, 0.0, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT);
		//dibujarBotones();
		mostrarProvinciasPintadasRotadas();
		glutSwapBuffers();
	}
	//dibujarBotones();
	
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