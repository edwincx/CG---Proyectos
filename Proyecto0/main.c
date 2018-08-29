#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#ifndef max
    #define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif
#ifndef min
    #define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

struct data {
      int resolution;
      int lines;
      int cicles;
};
struct line {
      int x0;
      int x1;
      int y0;
      int y1;
};

struct ventana {
      int xmin;
      int xmax;
      int ymin;
      int ymax;
};

struct line line_list [10001];
struct data screen;
struct ventana rectanguloRecorte;
int modo = 0;

void init();
void plot();
void plotVacio();
void bresenham(int x0, int y0, int x1, int y1 );
void bresenham_sinPlot(int x0, int y0,int x1, int y1);
void fuerzaBruta(int x0, int y0, int x1, int y1 );
void fuerzaBruta_sinPlot(int x0, int y0, int x1, int y1 );
void incremental(int x0, int y0, int x1, int y1 );
void incremental_sinPlot(int x0, int y0, int x1, int y1 );
void incremental2 (int x0, int y0, int x1, int y1 );
void incremental2_sinPlot(int x0, int y0, int x1, int y1 );
void createRandomLines(int lines,int resolution);
static int clipT(float num, float denom, float *tE, float *tL);
static int is_zero(float v);



int main(int argc, char** argv)
{
    if(argv[1]==NULL||argv[2]==NULL||argv[3]==NULL||argv[4]==NULL){
        printf("El formato tiene que ser ./main \"Resolución\" \"Cantidad de líneas\" \"Cantidad de cíclos\" \"Modo(0 ó 1)\"\n");
        exit(0);    
    }
    screen.resolution = atoi(argv[1]);
    screen.lines = atoi(argv[2]);
    screen.cicles = atoi(argv[3]);
    modo = atoi(argv[4]);
    if(screen.resolution < 200){
        printf("La resolución tiene que ser mayor o igual a 200.\n");
        exit(0);    
    }
    if(screen.lines < 1 ||screen.lines > 10000){
        printf("La cantidad de líneas tiene que ser entre 1 y 10000.\n");
        exit(0);    
    }
    if(screen.cicles < 1){
        printf("Tiene que poner al menos un cíclo.\n");
        exit(0);    
    }
    srand(time(NULL));
    createRandomLines(screen.lines,screen.resolution);
    

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screen.resolution,screen.resolution);
	glutCreateWindow("Proyecto 0: Lineas Rectas");
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(-0.5, screen.resolution +0.5, -0.5, screen.resolution + 0.5);
    glColor3f (1.0f, 1.0f, 1.0f);
    glutDisplayFunc(init);
    glutMainLoop();
}

static int is_zero(float v)
{
    return (v > -0.000001f && v < 0.000001f);
}

static inline int point_inside(struct ventana rect, int x, int y)
{
    return (x >= rect.xmin && x <= rect.xmax &&
        y >= rect.ymin && y <= rect.ymax);
}


static int clipT(float num, float denom, float *tE, float *tL)
{
    float t;

    if (is_zero(denom))
        return num < 0.0;

    t = num / denom;

    if (denom > 0) {
        if (t > *tL)
            return 0;
        if (t > *tE)
            *tE = t;
    } else {
        if (t < *tE)
            return 0;
        if (t < *tL)
            *tL = t;
    }
    return 1;
}

void init(){

    int amount_cicles;
    int amount_lines;
	float dx, dy, tE, tL;
    rectanguloRecorte.xmin=100;
    rectanguloRecorte.ymin=100;
    rectanguloRecorte.xmax=250;
    rectanguloRecorte.ymax=250;

    glColor3f (0.0f, 1.0f, 0.0f);
  
    
    for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
        
        dx=line_list[amount_lines].x1-line_list[amount_lines].x0;
        dy=line_list[amount_lines].y1-line_list[amount_lines].y0;
        
        if (is_zero(dx) && is_zero(dy) && point_inside(rectanguloRecorte, line_list[amount_lines].x0, line_list[amount_lines].y0)){
            continue;
        }
            
        tE = 0;
        tL = 1;

        if (clipT(rectanguloRecorte.xmin - line_list[amount_lines].x0,  dx, &tE, &tL) &&
        clipT(line_list[amount_lines].x0 - rectanguloRecorte.xmax, -dx, &tE, &tL) &&
        clipT(rectanguloRecorte.ymin - line_list[amount_lines].y0,  dy, &tE, &tL) &&
        clipT(line_list[amount_lines].y0 - rectanguloRecorte.ymax, -dy, &tE, &tL)) {
            if (tL < 1) {
                line_list[amount_lines].x1 = (int) (line_list[amount_lines].x0 + tL * dx);
                line_list[amount_lines].y1 = (int) (line_list[amount_lines].y0 + tL * dy);
            }
            if (tE > 0) {
                line_list[amount_lines].x0 += tE * dx;
                line_list[amount_lines].y0 += tE * dy;
            }
            bresenham(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);
        }
    }

    printf("FIN------------------------\n\n");
    glFlush();

}

void createRandomLines(int lines,int resolution){
	int i;
    for (i = 0; i < lines; i++) {
        line_list[i].x0 = rand() % resolution;
        line_list[i].x1 = rand() % resolution;
        line_list[i].y0 = rand() % resolution;
        line_list[i].y1 = rand() % resolution;
    }
}



void fuerzaBruta(int x0, int y0, int x1, int y1){
    long double m, b, y;
    int i, tempx, tempy, Ymayor, Ymenor;

    if((x0 > x1)){tempx = x0; tempy = y0; x0 = x1; y0 = y1; x1 = tempx; y1 = tempy;}

    if(abs(y1-y0) < abs(x1-x0)){
        m = (float)(y1 - y0) / (float)(x1 - x0);
        b = y0 - m*x0; 
        for (i = x0; i <= x1; i++){
            y = m*i + b;
            plot(i, (int)y);
        }
    }
    else{  
        m = (float)(x1 - x0) / (float)(y1 - y0);
        b = x0 - m*y0;
    
        if(y0 > y1){
            Ymayor = y0;
            Ymenor = y1;
        }else{
            Ymayor = y1;
            Ymenor = y0;
        }
        for (i = Ymenor; i <= Ymayor; i++){
            y = m*i + b;
            plot((int)y, i);
        }
    }

}

void fuerzaBruta_sinPlot(int x0, int y0, int x1, int y1){
    long double m, b, y;
    int i, tempx, tempy, Ymayor, Ymenor;

    if((x0 > x1)){tempx = x0; tempy = y0; x0 = x1; y0 = y1; x1 = tempx; y1 = tempy;}

    if(abs(y1-y0) >= abs(x1-x0)){    
        m = (float)(x1 - x0) / (float)(y1 - y0);
        b = x0 - m*y0;
    
        if(y0 > y1){
            Ymayor = y0;
            Ymenor = y1;
        }else{
            Ymayor = y1;
            Ymenor = y0;
        }
        for (i = Ymenor; i <= Ymayor; i++){
            y = m*i + b;
            plotVacio((int)y, i);
        }
    }
    else{  
        m = (float)(y1 - y0) / (float)(x1 - x0);
        b = y0 - m*x0; 
        for (i = x0; i <= x1; i++){
            y = m*i + b;
            plotVacio(i, (int)y);
        }
    }
}

void incremental(int x0, int y0, int x1, int y1){
    long double m, b, y;
    int i, tempx, tempy, Ymayor, Ymenor;;

    if((x0 > x1)){tempx = x0;tempy = y0;x0 = x1;y0 = y1;x1 = tempx;y1 = tempy;}

    if(abs(y1-y0) < abs(x1-x0)){
        m = (float)(y1 - y0) / (float)(x1 - x0);
        y = y0;
        for (i = x0; i <= x1; i++){
            plot(i, (int)y);
            y += m;
        }
    }else{
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
        for (i = Ymenor; i <= Ymayor; i++){
            plot((int)y, i);
            y += m;
        }
    }
}

void incremental_sinPlot(int x0, int y0, int x1, int y1){
    long double m, b, y;
    int i, tempx, tempy, Ymayor, Ymenor;;

    if((x0 > x1)){tempx = x0;tempy = y0;x0 = x1;y0 = y1;x1 = tempx;y1 = tempy;}

    if(abs(y1-y0) < abs(x1-x0)){
        m = (float)(y1 - y0) / (float)(x1 - x0);
        y = y0;
        for (i = x0; i <= x1; i++){
            plotVacio(i, (int)y);
            y += m;
        }
    }else{
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
        for (i = Ymenor; i <= Ymayor; i++){
            plotVacio((int)y, i);
            y += m;
        }
    }
}


void incremental2 (int x0, int y0, int x1, int y1 ){
    float x, y, move_x, move_y;
    int i, width_temp;

    width_temp = max(abs(x1-x0), abs(y1-y0));
 
    move_x = (float)(x1 - x0)/width_temp;
    move_y = (float)(y1 - y0)/width_temp ;
    x = (float)x0; y = (float)y0;
    for(i = 0 ; i <= width_temp ; i++){
        plot ((int)x,(int)y);
        x += move_x;
        y += move_y;
    }
}

void incremental2_sinPlot (int x0, int y0, int x1, int y1 ){
    float x, y, move_x, move_y;
    int i, width_temp;

    width_temp = max(abs(x1-x0), abs(y1-y0));
 
    move_x = (float)(x1 - x0)/width_temp;
    move_y = (float)(y1 - y0)/width_temp ;
    x = (float)x0; y = (float)y0;
    for(i = 0 ; i <= width_temp ; i++){
        plotVacio ((int)x,(int)y);
        x += move_x;
        y += move_y;
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

void bresenham_sinPlot(int x0, int y0,int x1, int y1) {
    int d, xp, yp;
    int Delta_1,Delta_2;
    int distanceX = fabs((x0 - x1));
    int distanceY = fabs((y0 - y1));
    xp = x0; 
    yp = y0;
    plotVacio(xp, yp);
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
                plotVacio(xp, yp);
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
                plotVacio(xp, yp);
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
                plotVacio(xp, yp);
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
                plotVacio(xp, yp);
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
                plotVacio(xp, yp);
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
                plotVacio(xp, yp);
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
                plotVacio(xp, yp);
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
                plotVacio(xp, yp);
            }

        }
    }

}

void plot(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
}

void plotVacio(int x, int y) {
  return;
}
