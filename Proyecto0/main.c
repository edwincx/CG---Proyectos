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




void init_nuevo();
void plot();
void bresenham(int x0, int y0, int x1, int y1 );
void bresenham_sinPlot(int x0, int y0,int x1, int y1);
void fuerzaBruta(int x0, int y0, int x1, int y1 );
void incremental(int x0, int y0, int x1, int y1 );
void incremental2_sinPlot(int x0, int y0, int x1, int y1 );
void fuerzaBruta_sinPlot(int x0, int y0, int x1, int y1 );
void incremental_sinPlot(int x0, int y0, int x1, int y1 );
void incremental2 (int x0, int y0, int x1, int y1 );
void createRandomLines(int lines,int resolution);

int main(int argc, char** argv)
{
	screen.resolution = atoi(argv[1]);
  	screen.lines = atoi(argv[2]);
   	screen.cicles = atoi(argv[3]);

	
    
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
    
    srand(time(NULL));
    createRandomLines(screen.lines,screen.resolution);

    int amount_cicles;
    int amount_lines;
	
    glColor3f (1.0f, 1.0f, 0.0f);
    clock_t start_all = clock();

    clock_t start = clock();

    for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
        for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
            fuerzaBruta(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);

        }
    }

    printf("FuerzaBruta: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);


    glColor3f (0.0f, 1.0f, 0.0f);
    
    
    start = clock();

    for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
        for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
            bresenham(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);
            //printf("(x0: %d,y0: %d )   (x1 = %d,y1 =%d)\n",line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);
        }
    }

    printf("Bresenham: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);



    glColor3f (1.0f, 0.0f, 0.0f);
    start = clock();

    for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
        for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
            incremental(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);

        }
    }

    printf("Incremental: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);

    glColor3f (1.0f, 0.1f, 1.0f);
    start = clock();

    for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
        for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
            incremental2(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);

        }
    }

    printf("Incremental2: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);


    printf("Total: %f seg.\n\n", ((double)clock() - start_all) / CLOCKS_PER_SEC);


    start_all = clock();



    
    start = clock();

    for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
        for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
            fuerzaBruta_sinPlot(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);

        }
    }

    printf("FuerzaBruta sin plot: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);

    start = clock();

    for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
        for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
            bresenham_sinPlot(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);
            //printf("(x0: %d,y0: %d )   (x1 = %d,y1 =%d)\n",line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);
        }
    }

    printf("Bresenham sin plot: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);
    
    start = clock();

    for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
        for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
            incremental_sinPlot(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);

        }
    }

    printf("Incremental sin plot: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);

    start = clock();

    for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
        for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
            incremental2_sinPlot(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);

        }
    }

    printf("Incremental2 sin plot: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);




    printf("Total: %f seg.\n\n", ((double)clock() - start_all) / CLOCKS_PER_SEC);



    printf("FIN------------------------\n\n");
    glFlush();

}

void createRandomLines(int lines,int resolution){
    for (int i = 0; i < lines; i++) {
        line_list[i].x0 = rand() % resolution;
        line_list[i].x1 = rand() % resolution;
        line_list[i].y0 = rand() % resolution;
        line_list[i].y1 = rand() % resolution;
    }
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
      plot((int)y, i);
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
      plot(i, (int)y);
    }
  }

}

void fuerzaBruta_sinPlot(int x0, int y0, int x1, int y1){
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
      
      plot((int)y, i);
        y += m;
      }
  }else{
    m = (float)(y1 - y0) / (float)(x1 - x0);
      y = y0;
      for (i = x0; i <= x1; i++)
      {
      int yy = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
      plot(i, (int)y);
        y += m;
      }
  }
}

void incremental_sinPlot(int x0, int y0, int x1, int y1){
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
        y += m;
      }
  }else{
    m = (float)(y1 - y0) / (float)(x1 - x0);
      y = y0;
      for (i = x0; i <= x1; i++)
      {
      int yy = (int)(y < 0 ? (y - 0.5) : (y + 0.5));
        y += m;
      }
  }
}


void incremental2 (int x0, int y0, int x1, int y1 ){
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

void incremental2_sinPlot (int x0, int y0, int x1, int y1 ){
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
       x += paso_x;
       y += paso_y;
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

void bresenham_sinPlot(int x0, int y0,int x1, int y1) {
    int d, xp, yp;
    int Delta_1,Delta_2;
    int distanceX = fabs((x0 - x1));
    int distanceY = fabs((y0 - y1));
    xp = x0; 
    yp = y0;

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
	
}

void plotVacio(int x, int y) {
  return;
}
