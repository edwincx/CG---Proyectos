#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#define n 500000
#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

struct linea {
      int x0;
      int x1;
      int y0;
      int y1;
};
struct linea listaLinea [n];

int resolucion;
int cant_lineas;
int cant_ciclos;

struct datos {
      int resolucion;
      int NumLineas;
      int ciclos;
};
struct datos pantalla;

void plot();
void plotV();
void init();
int generarNumRandom();
void generarLineas();
void MostrarTabla();
void fuerzaBruta();
void incremental();
void dobleIncremental();
void bresenham();
void fuerzaBrutaP();
void incrementalP();
void dobleIncrementalP();
void bresenhamP();
void bresenham2P();
void bresenham2();

int main(int argc, char** argv)
{
  resolucion = atoi(argv[1]);
  cant_lineas = atoi(argv[2]);
  cant_ciclos = atoi(argv[3]);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(resolucion,resolucion);
  glutCreateWindow("Programa 0");
  glClear(GL_COLOR_BUFFER_BIT);
  gluOrtho2D(-0.5, resolucion +0.5, -0.5, resolucion + 0.5);
	glColor3f (1.0f, 1.0f, 1.0f);
  //glutDisplayFunc(init);
  glutMainLoop();

}

void generarLineas (int NumLineas, int resolucion) {
  int i;
     for (i = 0; i < NumLineas; i++) {
       listaLinea[i].x0 = generarNumRandom(resolucion);
       listaLinea[i].x1 = generarNumRandom(resolucion);
       listaLinea[i].y0 = generarNumRandom(resolucion);
       listaLinea[i].y1 = generarNumRandom(resolucion);
     }


}

int generarNumRandom (int resolucion) {
  int num;
  //time_t t;

  //srand((unsigned) time(&t));
  num = rand() % resolucion;
  //printf("%d\n", num);
  return num;
}

void MostrarTabla( int NumLineas) {
  int i;
  for (i = 0; i < NumLineas; i++) {
      printf("(x0 = %d, y0 = %d),(x1 = %d, y1 = %d),\n", listaLinea[i].x0, listaLinea[i].y0,listaLinea[i].x1,listaLinea[i].y1);
  }
}

void test(){
  fuerzaBruta(72, 102, 550, 154);
  fuerzaBruta(148, 543, 2, 397);
  fuerzaBruta(444, 587, 454, 112);
  fuerzaBruta(577, 469, 524, 451);
  fuerzaBruta(217, 216, 452, 556);
  fuerzaBruta(71, 55, 189, 302);
  fuerzaBruta(440, 582, 85, 392);
  fuerzaBruta(398, 94, 360, 222);
  fuerzaBruta(63, 377, 548, 563);
  fuerzaBruta(550, 112, 72, 395);

  incremental(72, 102, 550, 154);
  incremental(148, 543, 2, 397);
  incremental(444, 587, 454, 112);
  incremental(577, 469, 524, 451);
  incremental(217, 216, 452, 556);
  incremental(71, 55, 189, 302);
  incremental(440, 582, 85, 392);
  incremental(398, 94, 360, 222);
  incremental(63, 377, 548, 563);
  incremental(550, 112, 72, 395);

  //dobleIncremental(72, 102, 550, 154);
  //dobleIncremental(148, 543, 2, 397);
  //dobleIncremental(444, 587, 454, 112);
  //dobleIncremental(577, 469, 524, 451);
  //dobleIncremental(217, 216, 452, 556);
  //dobleIncremental(71, 55, 189, 302);
  //dobleIncremental(440, 582, 85, 392);
  //dobleIncremental(398, 94, 360, 222);
  //dobleIncremental(63, 377, 548, 563);
  //dobleIncremental(550, 112, 72, 395);

  //dobleIncremental(403, 188, 262, 381);

}
void init(){
  int i, k;
  generarLineas(pantalla.NumLineas,resolucion);
  //MostrarTabla(NumLineas);
  //test();

  clock_t start = clock();
  for(k = 0; k < pantalla.ciclos; k++){
    for (i = 0; i < pantalla.NumLineas; i++)
    {
      fuerzaBruta(listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
      //printf("fuerzaBruta(%d, %d, %d, %d); \n", listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
    }
  }
  printf("FuerzaBruta: %f \n", ((double)clock() - start) / CLOCKS_PER_SEC);

  start = clock();
  for(k = 0; k < pantalla.ciclos; k++){
    for (i = 0; i < pantalla.NumLineas; i++)
    {
      fuerzaBrutaP(listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
      //printf("fuerzaBruta(%d, %d, %d, %d); \n", listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
    }
  }
  printf("FuerzaBruta Puro: %f \n", ((double)clock() - start) / CLOCKS_PER_SEC);

  start = clock();
  for(k = 0; k < pantalla.ciclos; k++){
    for (i = 0; i < pantalla.NumLineas; i++)
    {

      incremental(listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
      //printf("incremental(%d, %d, %d, %d); \n", listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);

    }
  }
  printf("Incremental: %f \n", ((double)clock() - start) / CLOCKS_PER_SEC);


  start = clock();
  for(k = 0; k < pantalla.ciclos; k++){
    for (i = 0; i < pantalla.NumLineas; i++)
    {

      incrementalP(listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
      //printf("incremental(%d, %d, %d, %d); \n", listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);

    }
  }
  printf("Incremental Puro: %f \n", ((double)clock() - start) / CLOCKS_PER_SEC);

  start = clock();
  for(k = 0; k < pantalla.ciclos; k++){
    for (i = 0; i < pantalla.NumLineas; i++)
    {

      dobleIncremental(listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
      //printf("dobleIncremental(%d, %d, %d, %d); \n", listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);

    }
  }
  printf("Doble incremental: %f \n", ((double)clock() - start) / CLOCKS_PER_SEC);

  start = clock();
  for(k = 0; k < pantalla.ciclos; k++){
    for (i = 0; i < pantalla.NumLineas; i++)
    {

      dobleIncrementalP(listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
      //printf("dobleIncremental(%d, %d, %d, %d); \n", listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);

    }
  }
  printf("Doble incremental Puro: %f \n", ((double)clock() - start) / CLOCKS_PER_SEC);

  start = clock();
  for(k = 0; k < pantalla.ciclos; k++){
    for (i = 0; i < pantalla.NumLineas; i++)
    {
      //printf("bresenham(%d, %d, %d, %d); \n", listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
      bresenham2(listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
    }
  }
  printf("Bresenham: %f \n", ((double)clock() - start) / CLOCKS_PER_SEC);

  start = clock();
  for(k = 0; k < pantalla.ciclos; k++){
    for (i = 0; i < pantalla.NumLineas; i++)
    {
      //printf("bresenham(%d, %d, %d, %d); \n", listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
      bresenham2P(listaLinea[i].x0, listaLinea[i].y0, listaLinea[i].x1, listaLinea[i].y1);
    }
  }
  printf("Bresenham Puro: %f \n", ((double)clock() - start) / CLOCKS_PER_SEC);
	//MostrarTabla(pantalla.NumLineas);
}

void fuerzaBruta(int x0, int y0, int x1, int y1){
  long double m, b, y;
  int i, tempx, tempy, Ymayor, Ymenor;
  //printf("x0 = %d, y0 = %d, x1 = %d, y1 = %d \n", x0, y0, x1, y1);

  if((x0 > x1)){
    tempx = x0;
    tempy = y0;
    x0 = x1;
    y0 = y1;
    x1 = tempx;
    y1 = tempy;
  }

  if(abs(y1-y0) >= abs(x1-x0)){
    //printf("en el if\n");
    m = (float)(x1 - x0) / (float)(y1 - y0);
    b = x0 - m*y0;
    //printf("m = %Lf, b = %Lf \n", m, b);
    if(y0 > y1){
      Ymayor = y0;
      Ymenor = y1;
    }else{
      Ymayor = y1;
      Ymenor = y0;
    }
    for (i = Ymenor; i <= Ymayor; i++)
    {
      y = m*i + b;
      int yy = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
      plot(yy, i);
    }
  }
  else{
    //printf("en el else\n");
    m = (float)(y1 - y0) / (float)(x1 - x0);
    b = y0 - m*x0;
    //printf("m = %Lf, b = %Lf \n", m, b);
    for (i = x0; i <= x1; i++)
    {
      y = m*i + b;
      int yy = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
      plot(i, yy);
    }
  }

}

void incremental(int x0, int y0, int x1, int y1){
	long double m, b, y;
  int i, tempx, tempy, Ymayor, Ymenor;;

  if((x0 > x1)){
    tempx = x0;
    tempy = y0;
    x0 = x1;
    y0 = y1;
    x1 = tempx;
    y1 = tempy;
  }

  if(abs(y1-y0) >= abs(x1-x0)){
  	m = (float)(x1 - x0) / (float)(y1 - y0);

	  if(y0 > y1){
      y = x1;
      Ymayor = y0;
      Ymenor = y1;
    }else{
      y = x0;
      Ymayor = y1;
      Ymenor = y0;
    }
	  for (i = Ymenor; i <= Ymayor; i++)
	  {
      int yy = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
	    //printf("yy= %d\n", yy);
      plot(yy, i);
	    y += m;
	  }
  }else{
  	m = (float)(y1 - y0) / (float)(x1 - x0);
	  y = y0;
	  for (i = x0; i <= x1; i++)
	  {
      int yy = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
      plot(i, yy);
	    y += m;
	  }
  }
}

void dobleIncremental (int x0, int y0, int x1, int y1 ){
 float x, y, paso_x, paso_y;
 int i, ancho;

 ancho = max(abs(x1-x0), abs(y1-y0));
 //printf("%d\n", ancho);
 paso_x = (float)(x1 - x0)/ancho;
 paso_y = (float)(y1 - y0)/ancho ;
 x = (float)x0; y = (float)y0;
  for(i = 0 ; i <= ancho ; i++){
      //printf("%i\n", i);
      int XX = (int)(x < 0 ? (x - 0.5) : (x + 0.5));
      int YY = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
       plot (XX,YY);
       x += paso_x;
       y += paso_y;
  }
}


void fuerzaBrutaP(int x0, int y0, int x1, int y1){
  long double m, b, y;
  int i, tempx, tempy, Ymayor, Ymenor;
  //printf("x0 = %d, y0 = %d, x1 = %d, y1 = %d \n", x0, y0, x1, y1);

  if((x0 > x1)){
    tempx = x0;
    tempy = y0;
    x0 = x1;
    y0 = y1;
    x1 = tempx;
    y1 = tempy;
  }

  if(abs(y1-y0) >= abs(x1-x0)){
    //printf("en el if\n");
    m = (float)(x1 - x0) / (float)(y1 - y0);
    b = x0 - m*y0;
    //printf("m = %Lf, b = %Lf \n", m, b);
    if(y0 > y1){
      Ymayor = y0;
      Ymenor = y1;
    }else{
      Ymayor = y1;
      Ymenor = y0;
    }
    for (i = Ymenor; i <= Ymayor; i++)
    {
      y = m*i + b;
      int yy = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
      plotV(yy, i);
    }
  }
  else{
    //printf("en el else\n");
    m = (float)(y1 - y0) / (float)(x1 - x0);
    b = y0 - m*x0;
    //printf("m = %Lf, b = %Lf \n", m, b);
    for (i = x0; i <= x1; i++)
    {
      y = m*i + b;
      int yy = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
      plotV(i, yy);
    }
  }

}

void incrementalP(int x0, int y0, int x1, int y1){
  long double m, b, y;
  int i, tempx, tempy, Ymayor, Ymenor;;

  if((x0 > x1)){
    tempx = x0;
    tempy = y0;
    x0 = x1;
    y0 = y1;
    x1 = tempx;
    y1 = tempy;
  }

  if(abs(y1-y0) >= abs(x1-x0)){
    m = (float)(x1 - x0) / (float)(y1 - y0);

    if(y0 > y1){
      y = x1;
      Ymayor = y0;
      Ymenor = y1;
    }else{
      y = x0;
      Ymayor = y1;
      Ymenor = y0;
    }
    for (i = Ymenor; i <= Ymayor; i++)
    {
      int yy = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
      //printf("yy= %d\n", yy);
      plotV(yy, i);
      y += m;
    }
  }else{
    m = (float)(y1 - y0) / (float)(x1 - x0);
    y = y0;
    for (i = x0; i <= x1; i++)
    {
      int yy = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
      plotV(i, yy);
      y += m;
    }
  }
}

void dobleIncrementalP(int x0, int y0, int x1, int y1 ){
 float x, y, paso_x, paso_y;
 int i, ancho;

 ancho = max(abs(x1-x0), abs(y1-y0));
 //printf("%d\n", ancho);
 paso_x = (float)(x1 - x0)/ancho;
 paso_y = (float)(y1 - y0)/ancho ;
 x = (float)x0; y = (float)y0;
  for(i = 0 ; i <= ancho ; i++){
      //printf("%i\n", i);
      int XX = (int)(x < 0 ? (x - 0.5) : (x + 0.5));
      int YY = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
       plotV (XX,YY);
       x += paso_x;
       y += paso_y;
  }
}


void bresenham2(int x0, int y0,int x1, int y1) {
      int d, Delta_E, Delta_NE, Delta_S, Delta_SE, Delta_SO, Delta_O, Delta_NO, Delta_N,xp, yp;
      int difX = fabs((x0 - x1));
      int dify = fabs((y0 - y1));
      xp = x0; yp = y0;
      plot(xp, yp);
      if (difX > dify) {
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
        } else if (x0 > x1 && y0 > y1) {
          //cuadrante5
          //printf("cuadrante5 x = %d y Y = %d\n", xp,yp);
					Delta_SO = -2*(y1-y0) + 2*(x1-x0);
					Delta_O = -2*(y1-y0);
          d = -2*(y1-y0)+(x1-x0);
          while (xp >= x1) {
            //printf("5");
            if (d <= 0) {
              xp--;
              d+= Delta_O;
            } else {
              xp--; yp--;
              d += Delta_SO;
            }
            plot(xp, yp);
          }
          ////////////
        } else if (x0 < x1 && y0 > y1) {
          //cuadrante8
          //printf("cuadrante8 x = %d y Y = %d\n", xp,yp);
					Delta_SE = 2*(y1-y0) + 2*(x1-x0);
					Delta_E = 2*(y1-y0);
          d = 2*(y1-y0)+(x1-x0);
          while (xp <= x1) {
            //printf("8");
            if (d >= 0) {
              xp++;
              d+= Delta_E;
            } else {
              xp++; yp--;
              d += Delta_SE;
            }
            plot(xp, yp);
          }
          ///////////
        }
      } else if (difX < dify) {
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
        } else if (x0 >= x1 && y0 > y1) {
          //cuadrante6
          //printf("cuadrante6 x = %d y Y = %d\n", xp,yp);
					Delta_SO = -2*(y1-y0) + 2*(x1-x0);
					Delta_S = 2*(x1-x0);
          d = -(y1-y0)+2*(x1-x0);
          while (yp >= y1) {
            //printf("6");
            if (d >= 0) {
              yp--;
              d+= Delta_S;
            } else {
              xp--; yp--;
              d += Delta_SO;
            }
            plot(xp, yp);
          }
          //
        } else if (x0 < x1 && y0 > y1) {
          //printf("cuadrante7 x = %d y Y = %d\n", xp,yp);
					Delta_SE = 2*(y1-y0) + 2*(x1-x0);
					Delta_S = 2*(x1-x0);
          d = (y1-y0)+2*(x1-x0);
          while (yp >= y1) {
            //printf("7");
            if (d <= 0) {
              yp--;
              d+= Delta_S;
            } else {
              xp++; yp--;
              d += Delta_SE;
            }
            plot(xp, yp);
          }
        }
			}
}

void bresenham2P(int x0, int y0,int x1, int y1) {
      int d, Delta_E, Delta_NE, Delta_S, Delta_SE, Delta_SO, Delta_O, Delta_NO, Delta_N,xp, yp;
      int difX = fabs((x0 - x1));
      int dify = fabs((y0 - y1));
      xp = x0; yp = y0;

      if (difX > dify) {
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

          }
          /////////////////
        } else if (x0 > x1 && y0 > y1) {
          //cuadrante5
          //printf("cuadrante5 x = %d y Y = %d\n", xp,yp);
					Delta_SO = -2*(y1-y0) + 2*(x1-x0);
					Delta_O = -2*(y1-y0);
          d = -2*(y1-y0)+(x1-x0);
          while (xp >= x1) {
            //printf("5");
            if (d <= 0) {
              xp--;
              d+= Delta_O;
            } else {
              xp--; yp--;
              d += Delta_SO;
            }

          }
          ////////////
        } else if (x0 < x1 && y0 > y1) {
          //cuadrante8
          //printf("cuadrante8 x = %d y Y = %d\n", xp,yp);
					Delta_SE = 2*(y1-y0) + 2*(x1-x0);
					Delta_E = 2*(y1-y0);
          d = 2*(y1-y0)+(x1-x0);
          while (xp <= x1) {
            //printf("8");
            if (d >= 0) {
              xp++;
              d+= Delta_E;
            } else {
              xp++; yp--;
              d += Delta_SE;
            }

          }
          ///////////
        }
      } else if (difX < dify) {
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

          }
          /////
        } else if (x0 >= x1 && y0 > y1) {
          //cuadrante6
          //printf("cuadrante6 x = %d y Y = %d\n", xp,yp);
					Delta_SO = -2*(y1-y0) + 2*(x1-x0);
					Delta_S = 2*(x1-x0);
          d = -(y1-y0)+2*(x1-x0);
          while (yp >= y1) {
            //printf("6");
            if (d >= 0) {
              yp--;
              d+= Delta_S;
            } else {
              xp--; yp--;
              d += Delta_SO;
            }

          }
          //
        } else if (x0 < x1 && y0 > y1) {
          //printf("cuadrante7 x = %d y Y = %d\n", xp,yp);
					Delta_SE = 2*(y1-y0) + 2*(x1-x0);
					Delta_S = 2*(x1-x0);
          d = (y1-y0)+2*(x1-x0);
          while (yp >= y1) {
            //printf("7");
            if (d <= 0) {
              yp--;
              d+= Delta_S;
            } else {
              xp++; yp--;
              d += Delta_SE;
            }

          }
        }
			}
}
void plot(int x, int y) {

	//glColor3f (1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
	 glFlush();
}

void plotV(int x, int y) {
  return;
}
