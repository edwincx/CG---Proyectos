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

struct line {
      int x0;
      int x1;
      int y0;
      int y1;
};
struct line line_list [n];


struct data {
      int resolution;
      int lines;
      int cicles;
};
struct data screen;


void plotV();
void init();

int generarNumRandom();
void generarLineas();
void MostrarTabla();
void fuerzaBruta();
void incremental();
void dobleIncremental();
void fuerzaBrutaP();
void incrementalP();
void dobleIncrementalP();



void init_nuevo();
void plot();
void bresenham();
void prueba();
float random_color();
void createRandomLines(int lines,int resolution);

int main(int argc, char** argv)
{
	screen.resolution = atoi(argv[1]);
  	screen.lines = atoi(argv[2]);
   	screen.cicles = atoi(argv[3]);

  	createRandomLines(screen.lines,screen.resolution);
	
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screen.resolution,screen.resolution);
	glutCreateWindow("Programa 0");
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(-0.5, screen.resolution +0.5, -0.5, screen.resolution + 0.5);
	glColor3f (1.0f, 1.0f, 1.0f);
	glutDisplayFunc(init_nuevo);
	glutMainLoop();

}

void init_nuevo(){
	
	int amount_cicles;
	int amount_lines;

	clock_t start = clock();
	for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
		for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
			bresenham(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);
			//printf("(x0: %d,y0: %d )   (x1 = %d,y1 =%d)\n",line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);
		}
	}

	printf("Bresenham: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);
	
	printf("FIN------------------------\n\n");

}

void createRandomLines(int lines,int resolution){
	for (int i = 0; i < lines; i++) {
		line_list[i].x0 = rand() % resolution;
    	line_list[i].x1 = rand() % resolution;
    	line_list[i].y0 = rand() % resolution;
    	line_list[i].y1 = rand() % resolution;
    }
}

void bresenham(int x0, int y0,int x1, int y1) {
    int d, xp, yp;
    int Delta_1,Delta_2;
    int distanceX = fabs((x0 - x1));
    int distanceY = fabs((y0 - y1));
    xp = x0; 
    yp = y0;
    plot(xp, yp);

    //Cuadrantes 1 y 2
    if (x0 < x1 && y0 <= y1) {
    	
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
    else if(x1 < x0 && y0 <= y1){

    	
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
    else if(x1 < x0 && y0 > y1){
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

    else if(x1 > x0 && y0 > y1){
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


void plot(int x, int y) {
	
	//glColor3f (float(random_color()), float(random_color()), float(random_color()));
	//glColor3i (random_color(), random_color(), random_color());
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
	glFlush();
}

void plotV(int x, int y) {
  return;
}
