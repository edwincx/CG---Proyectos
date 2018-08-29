
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

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

struct line line_list [990001];
struct line line_list2 [990001];
struct data screen;
struct ventana rectanguloRecorte;

enum {
    TOP = 0x1, BOTTOM = 0x2, RIGHT = 0x4, LEFT = 0x8
};

enum {
    FALSE, TRUE
};

typedef unsigned int outcode;

outcode compute_outcode(int x, int y, int xmin, int ymin, int xmax, int ymax) {
    outcode oc = 0;
    if (y > ymax) oc |= TOP;
    else if (y < ymin) oc |= BOTTOM;
    if (x > xmax) oc |= RIGHT;
    else if (x < xmin) oc |= LEFT;
    return oc;
}




void init();

void createRandomLines(int lines,int resolution);
static int clipT(float num, float denom, float *tE, float *tL);
static int is_zero(float v);



int main(int argc, char** argv)
{
    if(argv[1]==NULL||argv[2]==NULL){
        printf("El formato tiene que ser ./main \"Resolución\" \"Cantidad de líneas\" \"Cantidad de cíclos\" \"Modo(0 ó 1)\"\n");
        exit(0);    
    }
    screen.resolution = atoi(argv[1]);
    screen.lines = atoi(argv[2]);
  
    if(screen.resolution < 200){
        printf("La resolución tiene que ser mayor o igual a 200.\n");
        exit(0);    
    }
  
  
    srand(time(NULL));
    createRandomLines(screen.lines,screen.resolution);

    
    init();
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

    
    int amount_lines;
    rectanguloRecorte.xmin=100;
    rectanguloRecorte.ymin=200;
    rectanguloRecorte.xmax=450;
    rectanguloRecorte.ymax=350;
    printf("--------------Liang-Basky-----------------\n");
    printf("Cantidad de lineas: %d\n",screen.lines );
    printf("Tamaño de la ventana: xmin = %d, ymin = %d, xmax = %d, ymax = %d\n",rectanguloRecorte.xmin,rectanguloRecorte.ymin,rectanguloRecorte.xmax,rectanguloRecorte.ymax );
    
    clock_t start = clock();

	float dx, dy, tE, tL;
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
            
        }
    }
    printf("Tiempo Liang_barsky: %f seg.\n\n\n\n\n",((double)clock() - start)/CLOCKS_PER_SEC );


    printf("--------------Cohen-Sutherland-----------------\n");
    printf("Cantidad de lineas: %d\n",screen.lines );
    printf("Tamaño de la ventana: xmin = %d, ymin = %d, xmax = %d, ymax = %d\n",rectanguloRecorte.xmin,rectanguloRecorte.ymin,rectanguloRecorte.xmax,rectanguloRecorte.ymax );
    
    start = clock();
    int accept;
    int done;


    outcode outcode1, outcode2;
    amount_lines = 0;
    for(amount_lines = 0;amount_lines < screen.lines;amount_lines++){
    accept = FALSE;
    done = FALSE;
    outcode1 = compute_outcode(line_list2[amount_lines].x0, line_list2[amount_lines].y0, rectanguloRecorte.xmin, rectanguloRecorte.ymin, rectanguloRecorte.xmax, rectanguloRecorte.ymax);
    outcode2 = compute_outcode(line_list2[amount_lines].x1, line_list2[amount_lines].y1, rectanguloRecorte.xmin, rectanguloRecorte.ymin, rectanguloRecorte.xmax, rectanguloRecorte.ymax);
    do {
        if (outcode1 == 0 && outcode2 == 0) {
            accept = TRUE;
            done = TRUE;
        } else if (outcode1 & outcode2) {
            done = TRUE;
        } else {
            double x, y;
            int outcode_ex = outcode1 ? outcode1 : outcode2;
            if (outcode_ex & TOP) {
                x = line_list2[amount_lines].x0 + (line_list2[amount_lines].x1 - line_list2[amount_lines].x0) * (rectanguloRecorte.ymax - line_list2[amount_lines].y0) / (line_list2[amount_lines].y1 - line_list2[amount_lines].y0);
                y = rectanguloRecorte.ymax;
            } else if (outcode_ex & BOTTOM) {
                x = line_list2[amount_lines].x0 + (line_list2[amount_lines].x1 - line_list2[amount_lines].x0) * (rectanguloRecorte.ymin - line_list2[amount_lines].y0) / (line_list2[amount_lines].y1 - line_list2[amount_lines].y0);
                y = rectanguloRecorte.ymin;
            } else if (outcode_ex & RIGHT) {
                y = line_list2[amount_lines].y0 + (line_list2[amount_lines].y1 - line_list2[amount_lines].y0) * (rectanguloRecorte.xmax - line_list2[amount_lines].x0) / (line_list2[amount_lines].x1 - line_list2[amount_lines].x0);
                x = rectanguloRecorte.xmax;
            } else {
                y = line_list2[amount_lines].y0 + (line_list2[amount_lines].y1 - line_list2[amount_lines].y0) * (rectanguloRecorte.xmin - line_list2[amount_lines].x0) / (line_list2[amount_lines].x1 - line_list2[amount_lines].x0);
                x = rectanguloRecorte.xmin;
            } 
            if (outcode_ex == outcode1) {
                line_list2[amount_lines].x0 = x;
                line_list2[amount_lines].y0 = y;
                outcode1 = compute_outcode(line_list2[amount_lines].x0, line_list2[amount_lines].y0, rectanguloRecorte.xmin, rectanguloRecorte.ymin, rectanguloRecorte.xmax, rectanguloRecorte.ymax);
            } else {
                line_list2[amount_lines].x1 = x;
                line_list2[amount_lines].y1 = y;
                outcode2 = compute_outcode(line_list2[amount_lines].x1, line_list2[amount_lines].y1, rectanguloRecorte.xmin, rectanguloRecorte.ymin, rectanguloRecorte.xmax, rectanguloRecorte.ymax);
            }
        }
    } while (done == FALSE);
}


    printf("Tiempo Cohen-Sutherland: %f seg.\n\n\n\n\n",((double)clock() - start)/CLOCKS_PER_SEC );






    printf("FIN------------------------\n\n");


}

void createRandomLines(int lines,int resolution){
	int i;
    for (i = 0; i < lines; i++) {
        line_list[i].x0 = rand() % resolution;
        line_list[i].x1 = rand() % resolution;
        line_list[i].y0 = rand() % resolution;
        line_list[i].y1 = rand() % resolution;
        line_list2[i].x0 = line_list[i].x0;
        line_list2[i].y0 = line_list[i].y0;
        line_list2[i].x1 = line_list[i].x1;
        line_list2[i].y1 = line_list[i].y1;
    }
}



