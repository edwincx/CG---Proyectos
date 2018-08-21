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

struct line line_list [10001];
struct data screen;
int modo = 0;

void init();
void plot();
void bresenham(int x0, int y0, int x1, int y1 );
void bresenham_sinPlot(int x0, int y0,int x1, int y1);
void fuerzaBruta(int x0, int y0, int x1, int y1 );
void fuerzaBruta_sinPlot(int x0, int y0, int x1, int y1 );
void incremental(int x0, int y0, int x1, int y1 );
void incremental_sinPlot(int x0, int y0, int x1, int y1 );
void incremental2 (int x0, int y0, int x1, int y1 );
void incremental2_sinPlot(int x0, int y0, int x1, int y1 );
void createRandomLines(int lines,int resolution);

int main(int argc, char** argv)
{
    if(argv[1]==NULL||argv[2]==NULL||argv[3]==NULL||argv[4]==NULL){
        printf("El formato tiene que ser ./main \"cantidad de líneas\" \"cantidad de cíclos\" \"modo(0 ó 1)\"\n");
        exit(0);    
    }
    screen.resolution = atoi(argv[1]);
    screen.lines = atoi(argv[2]);
    screen.cicles = atoi(argv[3]);
    modo = atoi(argv[4]);
    if(screen.resolution < 200){
        printf("La resolución tiene que ser mayor que 200.\n");
        exit(0);    
    }
    if(screen.lines < 1 ||screen.lines > 10000){
        printf("LA cantidad de líneas tiene que ser entre 1 y 10000.\n");
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
	glutCreateWindow("Programa 0");
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(-0.5, screen.resolution +0.5, -0.5, screen.resolution + 0.5);
    glColor3f (1.0f, 1.0f, 1.0f);
    glutDisplayFunc(init);
    glutMainLoop();
}

void move(){
    int i = 0;
    int l = 10;
    while(i < screen.lines){
        line_list[i].x0 += l;
        line_list[i].x1 += l;
        line_list[i].y0 += l;
        line_list[i].y1 += l;
        i++;
    }
}

void init(){

    int amount_cicles;
    int amount_lines;
	
    printf("FuerzaBruta - Amarillo\nBresenham - Verde\nIncremental - Rojo\nIncremental2 - Morado\n\n");

    if(modo==1){move();}
    glColor3f (1.0f, 1.0f, 0.0f);
    clock_t start_all = clock();
    clock_t start = clock();
    for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
        for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
            fuerzaBruta(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);

        }
    }
    printf("FuerzaBruta: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);

    if(modo==1){move();}
    glColor3f (0.0f, 1.0f, 0.0f);
    start = clock();
    for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
        for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
            bresenham(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);
            
        }
    }
    printf("Bresenham: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);

    if(modo==1){move();}
    glColor3f (1.0f, 0.0f, 0.0f);
    start = clock();
    for(amount_cicles = 0; amount_cicles < screen.cicles;amount_cicles++){
        for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
            incremental(line_list[amount_lines].x0,line_list[amount_lines].y0,line_list[amount_lines].x1,line_list[amount_lines].y1);

        }
    }
    printf("Incremental: %f seg.\n\n", ((double)clock() - start) / CLOCKS_PER_SEC);

    if(modo==1){move();}
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
        }
    }
    else{  
        m = (float)(y1 - y0) / (float)(x1 - x0);
        b = y0 - m*x0; 
        for (i = x0; i <= x1; i++){
            y = m*i + b;
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
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
}

void plotVacio(int x, int y) {
  return;
}
