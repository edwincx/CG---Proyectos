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


vertice puntos_Heredia[350];
vertice puntos_Alajuela[686];
vertice puntos_Limon[932];
vertice puntos_SanJose[414];
vertice puntos_Cartago[198];
vertice puntos_Puntarenas[2028];
vertice puntos_Guanacaste[2787];
int verticesActivos[100];
vertice intersecciones[100];

int numVerticesEstrella = 10;
vertice verticesEstrella[15];
int picosrodillasEstrella[15];

int numVerticesHeredia = 320;
int picosrodillasHeredia[500];

int numVerticesAlajuela = 685;
int picosrodillasAlajuela[700];

int numVerticesGuanacaste = 2786;
int picosrodillasGuanacaste[2800];

int numVerticesPuntarenas = 2028;
int picosrodillasPuntarenas[2100];

vertice verticesSJ[450];
vertice verticesGuanacaste[3000];
vertice verticesAlajuela[700];
vertice verticesPuntarenas[2200];
vertice verticesHeredia[350];
vertice verticesCartago[250];
vertice verticesLimon[1000];


void readFile(char* filename,vertice* puntos);
void dibujarProvincia(int cant_vertices,vertice* puntos);
void mostrarProvinciasPintadas();
void pintarProvincia(vertice* vertices, int numVertices,int*picosrodillas);
int activarVertices(int scanline, vertice* vertices, int numVertices,int*picosrodillas);
int eliminarIntersecciones(int numIntersecciones);
int calcularIntersecciones(int scanline, int*verticesAct, int numVerticesActivos,vertice *vert);
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
void quicksort(vertice *target, int left, int right);
void sort(vertice *target, int left, int right,int*picosrodillas);
int limpiarRepetido(vertice* vert,int n);
int limpiarRepetidoActivos(int* vert,int n);
int limpieza2(vertice* vert,int n);
void evaluarPicoRodilla(int *lista,vertice *vert,int numVert);


int main(int argc, char** argv)
{

    // readFile("Heredia.txt",puntos_Heredia);
    // readFile("Heredia.txt",verticesHeredia);
    // numVerticesHeredia = limpiarRepetido(verticesHeredia,numVerticesHeredia);
    // numVerticesHeredia = limpieza2(verticesHeredia,numVerticesHeredia);
    // evaluarPicoRodilla(picosrodillasHeredia,verticesHeredia,numVerticesHeredia);

    // readFile("Alajuela.txt",puntos_Alajuela);
    // readFile("Alajuela.txt",verticesAlajuela);
    // numVerticesAlajuela=limpiarRepetido(verticesAlajuela,numVerticesAlajuela);
    // numVerticesAlajuela=limpieza2(verticesAlajuela,numVerticesAlajuela);
    // evaluarPicoRodilla(picosrodillasAlajuela,verticesAlajuela,numVerticesAlajuela);

    // readFile("Guanacaste.txt",puntos_Guanacaste);
    // readFile("Guanacaste.txt",verticesGuanacaste);
    // numVerticesGuanacaste=limpiarRepetido(verticesGuanacaste,numVerticesGuanacaste);
    // numVerticesGuanacaste=limpieza2(verticesGuanacaste,numVerticesGuanacaste);
    // evaluarPicoRodilla(picosrodillasGuanacaste,verticesGuanacaste,numVerticesGuanacaste);

    // readFile("Puntarenas.txt",puntos_Puntarenas);
    // readFile("Puntarenas.txt",verticesPuntarenas);
    // numVerticesPuntarenas=limpiarRepetido(verticesPuntarenas,numVerticesPuntarenas);
    // numVerticesPuntarenas=limpieza2(verticesPuntarenas,numVerticesPuntarenas);
    // evaluarPicoRodilla(picosrodillasPuntarenas,verticesPuntarenas,numVerticesPuntarenas);

    verticesEstrella[0].X = 400;//pico
    verticesEstrella[0].Y = 400;

    verticesEstrella[1].X = 350;
    verticesEstrella[1].Y = 300;

    verticesEstrella[2].X = 250;//pico
    verticesEstrella[2].Y = 350;

    verticesEstrella[3].X = 350;
    verticesEstrella[3].Y = 200;

    verticesEstrella[4].X = 250;//pico
    verticesEstrella[4].Y = 100;

    verticesEstrella[5].X = 400;
    verticesEstrella[5].Y = 150;

    verticesEstrella[6].X = 500;//pico
    verticesEstrella[6].Y = 100;

    verticesEstrella[7].X = 450;
    verticesEstrella[7].Y = 200;

    verticesEstrella[8].X = 600;//pico
    verticesEstrella[8].Y = 75;

    verticesEstrella[9].X = 450;
    verticesEstrella[9].Y = 275;

    verticesEstrella[10].X = 400;//pico
    verticesEstrella[10].Y = 400;

    evaluarPicoRodilla(picosrodillasEstrella,verticesEstrella,numVerticesEstrella);

    //sort(verticesHeredia,0,numVerticesHeredia);
    
    // for(int i = 0;i < numVerticesHeredia;i++){
    //          printf("%dx - %d\n", i,verticesHeredia[i].X);
    //          printf("%dy - %d\n\n", i,verticesHeredia[i].Y);
        
    // }
    for(int i = 0;i < numVerticesEstrella;i++){
             printf("%di - %d\n\n", i,picosrodillasEstrella[i]);
        
    }


    // readFile("Alajuela.txt",puntos_Alajuela);
    // readFile("Limon.txt",puntos_Limon);
    // readFile("SanJose.txt",puntos_SanJose);
    // readFile("Cartago.txt",puntos_Cartago);
    // readFile("Puntarenas.txt",puntos_Puntarenas);
    // readFile("Guanacaste.txt",puntos_Guanacaste);
    
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

void evaluarPicoRodilla(int *lista,vertice *vert,int numVert){
    int i,j=1;
    for(i=0;i<numVert;i++){
        if(vert[numVert-1].Y < vert[i].Y&&vert[i].Y  > vert[i+1].Y && i==0){
            lista[i] = 1;
        }
        else if(vert[i-1].Y < vert[i].Y&&vert[i].Y  > vert[i+1].Y && i+1<numVert){
            lista[i] = 1;
        }
        else if(vert[i-1].Y < vert[i].Y&&vert[i].Y  > vert[0].Y && i+1>=numVert){
            lista[i] = 1;
        }
        else if(vert[numVert-1].Y > vert[i].Y &&vert[i].Y< vert[i+1].Y && i==0){
            lista[i] = 2;
        }
        else if(vert[i-1].Y > vert[i].Y &&vert[i].Y< vert[i+1].Y && i+1<numVert){
            lista[i] = 2;
        }
        else if(vert[i-1].Y > vert[i].Y &&vert[i].Y< vert[0].Y && i+1>=numVert){
            lista[i] = 2;
        }


    //     if(vert[i-1].X < vert[i].X  > vert[i+1].X && i+1<numVert&&(vert[i-1].Y < vert[i].Y  < vert[i+1].Y||vert[i-1].Y > vert[i].Y  > vert[i+1].Y)){
    //         lista[i] = 2;
    //     }
    //     else if(vert[i-1].X > vert[i].X  < vert[i+1].X && i+1<numVert&&(vert[i-1].Y < vert[i].Y  < vert[i+1].Y||vert[i-1].Y > vert[i].Y  > vert[i+1].Y)){
    //         lista[i] = 2;
    //     }
    //     else if(vert[i-1].X < vert[i].X  > vert[0].X && i+1>=numVert){
    //         lista[i] = 2;
    //     }
    //     else if(vert[i-1].X > vert[i].X  < vert[0].X && i+1>=numVert){
    //         lista[i] = 2;
    //     }


    }
}

void sort(vertice *target, int left, int right,int*picosrodillas){
    int ind;
    int tempInd = 0;
    for(ind = 0;ind < right;ind++){
        for(tempInd = ind;tempInd < right;tempInd++){
            if(target[ind].Y < target[tempInd].Y){
                int tmpX = target[ind].X;
                int tmpY = target[ind].Y;
                target[ind].X = target[tempInd].X;
                target[ind].Y = target[tempInd].Y;
                target[tempInd].X = tmpX;
                target[tempInd].Y = tmpY;

                tmpX = picosrodillas[ind];
                picosrodillas[ind]=picosrodillas[tempInd];
                picosrodillas[tempInd]=tmpX;
            }
        }
    }
}



int limpiarRepetido(vertice* vert,int n){
    int i,j,k;
    for(i = 0; i < n; i++)
    {
        for(j = i+1; j < n;j++ )
        {
            if(vert[j].X == vert[i].X &&vert[j].Y == vert[i].Y)
            {
                for(k = j; k < n; k++)
                {
                    vert[k] = vert[k+1];
                }
                //printf("limpiar i:%d\n",j );
                n--;
            }
 
        }
    }
    return n;
}

int limpiarRepetidoActivos(int* vert,int n){
    int i,j,k;
    for(i = 0; i < n; i++)
    {
        for(j = i+1; j < n;j++ )
        {
            if(vert[j] == vert[i] &&vert[j] == vert[i])
            {
                for(k = j; k < n; k++)
                {
                    vert[k] = vert[k+1];
                }
                //printf("limpiar i:%d\n",j );
                n--;
            }
 
        }
    }
    return n;
}

int limpieza2(vertice* vert,int n){
    int i,j,k;
    for(i = 0; i < n; i++)
    {
        for(j = i+1; j < n;)
        {
            if(vert[j].Y == vert[i].Y &&j+1 < n && vert[j+1].Y == vert[i].Y)
            {
                for(k = j; k < n; k++)
                {
                    vert[k] = vert[k+1];
                }
                n--;
            }else{
                j++;
            }
 
        }
    }
    return n;
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
    lines++;
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
                //printf("%dyRead - %d\n",i,puntos[i].Y );
                flag = 0;
            }
            ch = strtok(NULL, " ,");
        }  
    }
    puntos[lines-1].Y = puntos[0].Y;
    puntos[lines-1].X = puntos[0].X;
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
        if(i+1 <= cant_vertices){
            bresenham(puntos[i].X,puntos[i].Y,puntos[i+1].X,puntos[i+1].Y);
            // printf("%dx - %d\n", i,puntos_Heredia[i].X);
            // printf("%dy - %d\n", i,puntos_Heredia[i].Y);
        }
    }
}

void mostrarBordes(){
    //dibujarProvincia(319,puntos_Heredia);
    //dibujarProvincia(10,verticesEstrella);
    // dibujarProvincia(685,puntos_Alajuela);
    // dibujarProvincia(931,puntos_Limon);
    // dibujarProvincia(413,puntos_SanJose);
    // dibujarProvincia(198,puntos_Cartago);
    // dibujarProvincia(2028,puntos_Puntarenas);
    // dibujarProvincia(2787,puntos_Guanacaste);
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
    // glColor3f (0.4f, 0.6f, 0.3f);
    // pintarProvincia(verticesHeredia, numVerticesHeredia,picosrodillasHeredia);
    glColor3f (0.4f, 0.6f, 0.3f);
    pintarProvincia(verticesEstrella, numVerticesEstrella,picosrodillasEstrella);
    // // glColor3f (1.0f, 0.0f, 0.0f);
    // // pintarProvincia(verticesSJ, numVerticesSJ);
    // glColor3f (0.0f, 0.0f, 1.0f);
    // pintarProvincia(verticesGuanacaste, numVerticesGuanacaste,picosrodillasGuanacaste);
    // glColor3f (0.0f, 1.0f, 0.0f);
    // pintarProvincia(verticesPuntarenas, numVerticesPuntarenas,picosrodillasPuntarenas);
    // glColor3f (0.6f, 0.6f, 0.1f);
    // pintarProvincia(verticesAlajuela, numVerticesAlajuela,picosrodillasAlajuela);
    // glColor3f (0.5f, 0.5f, 0.5f);
    // pintarProvincia(verticesCartago, numVerticesCartago);
    // glColor3f (0.1f, 0.5f, 0.0f);
    // pintarProvincia(verticesLimon, numVerticesLimon);
    // glColor3f (0.0f, 0.5f, 0.1f);
}
void pintarProvincia(vertice* vertices, int numVertices,int*picosrodillas){
    int scanline = calcularYmax(vertices, numVertices);
    int k;
    
    while(scanline > calcularYmin(vertices, numVertices)){
    
        int numVerticesActivos = activarVertices(scanline, vertices, numVertices,picosrodillas);
        //printf("vertices activos: %ld\n", (sizeof(verticesActivos)/sizeof(verticesActivos[0])));
        int i;

    
        
        int numIntersecciones= calcularIntersecciones(scanline, verticesActivos,numVerticesActivos,vertices);
    
        ordenarIntersecciones(numIntersecciones);
      

        for (i = 0; i < numIntersecciones; i+=2)
        {   
            if(intersecciones[i].X > 0 && intersecciones[i+1].X > 0&&intersecciones[i].Y==intersecciones[i+1].Y){

                bresenham(intersecciones[i].X,intersecciones[i].Y,intersecciones[i+1].X,intersecciones[i+1].Y);
            }
            //printf("i:%d x:%d y:%d a x1: %d y1:%d \n\n",scanline, intersecciones[i].X,scanline,intersecciones[i+1].X,scanline);
            
        }
        //printf("Fin scanline %d -----------------------------\n\n",scanline);
        k = eliminarIntersecciones(numIntersecciones);
        scanline--;
    }

}

void arreglar(){

}

int verificarPuntas(int scanline, int numVerticesActivos){
    
}
int calcularIntersecciones(int scanline, int*verticesAct,int numVerticesActivos,vertice *vert){
    int i, k, Xi, Yi, Xj, Yj;
    vertice punto;
    punto.Y = scanline;
    for (i = 0, k = 0; i < numVerticesActivos; i+=2)
    {
        
        
  
        Xi = vert[verticesAct[i]].X;
        Yi = vert[verticesAct[i]].Y;
        Xj = vert[verticesAct[i+1]].X;
        Yj = vert[verticesAct[i+1]].Y;
        

        //printf("Deg:x%d y:%d ind:%d\n",Xi,Yi, verticesAct[k]);
        punto.X = (scanline-(Yi-((float)(Yj-Yi)/(Xj-Xi))*Xi))/((float)(Yj-Yi)/(Xj-Xi));
        if(punto.X < 0){
            punto.X = Xi;
        }
        // if(Xi == Xj && Yi == Yj){
        //     intersecciones[k] = punto;
        //     k++;
        // }
        if(scanline ==300){
            for(int h = 0;h <numVerticesActivos;h++){
                printf("TablACT:x%d y:%d\n",verticesEstrella[verticesAct[h]].X,verticesEstrella[verticesAct[h]].Y );
            }
            printf("INT:%d x:%d y:%d Activos:%d numAct:%d\n\n",k,punto.X,punto.Y,i,numVerticesActivos);
        }
        
        //printf("(%d - (%d -(%d-%d)/(%d-%d)*%d) / (%d-%d)/(%d-%d))\n",scanline,Yi,Yj,Yi,Xj,Xi,Xi,Yj,Yi,Xj,Xi );

        intersecciones[k] = punto;


        //printf("INT:%d x:%d y:%d Activos:%d numAct:%d\n\n",k,punto.X,punto.Y,i,numVerticesActivos);
        k++;
    }
    //printf("Fin Scanline:%d--------------\n",scanline );
    //k = eliminarIntersecciones(k);
    return k;
}

int activarVertices(int scanline, vertice* vertices, int numVertices,int*picosrodillas){
    //vertice activos[100];
    //vertice *activos = (vertice*)malloc(100*sizeof(vertice));
    int i;
    int k = 0;
    // printf("activos: %d\n", numVertices);
    for (i = 0; i < numVertices; i++)
    {
        
            // printf("i = %d\n", i);
            // printf("i+1 = %d\n", i+1);
            // printf("X%d = %d\n", i, vertices[i].X);
            // printf("Y%d = %d\n", i, vertices[i].Y);
            // printf("X%d = %d\n", i+1, vertices[i+1].X);
            // printf("Y%d = %d\n", i+1, vertices[i+1].Y);
        
            if(vertices[i].Y == scanline){
                if(picosrodillas[i]==1){
                    verticesActivos[k] = i;
                    verticesActivos[k+1] = i;
                    // printf("i:%d pico\n",i );
                    // if(scanline==200){
                    //     printf("200\n");
                    // }
                    // printf("pico\n");
                     // printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d bien\n\n\n",scanline,k,verticesEstrella[verticesActivos[k]].X,verticesEstrella[verticesActivos[k]].Y );
                     // printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d bien\n\n\n",scanline,k+1,verticesEstrella[verticesActivos[k+1]].X,verticesEstrella[verticesActivos[k+1]].Y );
                    // printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d pico\n\n\n",scanline,k,verticesEstrella[verticesActivos[k]].X,verticesEstrella[verticesActivos[k]].Y );
                    //  printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d  pico\n\n\n",scanline,k+1,verticesEstrella[verticesActivos[k+1]].X,verticesEstrella[verticesActivos[k+1]].Y );
                    k+=2;
                }
                else if(picosrodillas[i]==2){
                    verticesActivos[k] = i;
                    verticesActivos[k+1] = i;
                    verticesActivos[k+2] = i;
                    verticesActivos[k+3] = i;
                    // printf("i:%d pico\n",i );
                    // if(scanline==200){
                    //     printf("200\n");
                    // }
                    // printf("pico\n");
                     // printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d bien\n\n\n",scanline,k,verticesEstrella[verticesActivos[k]].X,verticesEstrella[verticesActivos[k]].Y );
                     // printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d bien\n\n\n",scanline,k+1,verticesEstrella[verticesActivos[k+1]].X,verticesEstrella[verticesActivos[k+1]].Y );
                    // printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d pico\n\n\n",scanline,k,verticesEstrella[verticesActivos[k]].X,verticesEstrella[verticesActivos[k]].Y );
                    //  printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d  pico\n\n\n",scanline,k+1,verticesEstrella[verticesActivos[k+1]].X,verticesEstrella[verticesActivos[k+1]].Y );
                    k+=4;
                }
                else{
                    verticesActivos[k] = i;
                    verticesActivos[k+1] = i+1;
                    // printf("i:%d no pico\n",i );
                    // if(scanline==200){
                    //     printf("200\n");
                    // }
                     // printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d mal\n\n\n",scanline,k,verticesEstrella[verticesActivos[k]].X,verticesEstrella[verticesActivos[k]].Y );
                     // printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d mal\n\n\n",scanline,k+1,verticesEstrella[verticesActivos[k+1]].X,verticesEstrella[verticesActivos[k+1]].Y );
                    k+=2;
                }
            }
            else if(vertices[i].Y >= scanline && vertices[i+1].Y < scanline
            || vertices[i].Y <= scanline && vertices[i+1].Y >= scanline){
                verticesActivos[k] = i;
                verticesActivos[k+1] = i+1;
                // printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d Prob\n\n\n",scanline,k,verticesEstrella[verticesActivos[k]].X,verticesEstrella[verticesActivos[k]].Y );
                //      printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d nProb\n\n\n",scanline,k+1,verticesEstrella[verticesActivos[k+1]].X,verticesEstrella[verticesActivos[k+1]].Y );
                 // printf("Scanlinek: %d Vertice Act: x:%d y:%d\n",scanline,verticesEstrella[verticesActivos[k]].X,verticesHeredia[verticesActivos[k]].Y );
                 // printf("Scanlinek+1: %d Vertice Act: x:%d y:%d\n\n\n",scanline,verticesHeredia[verticesActivos[k+1]].X,verticesHeredia[verticesActivos[k+1]].Y );
                k+=2;

            
            }else if(i+1 >= numVertices){
                if(vertices[i].Y >= scanline && vertices[0].Y < scanline
                    || vertices[i].Y <= scanline && vertices[0].Y >= scanline){
                  // printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d no picoLAST\n\n\n",scanline,k,verticesEstrella[verticesActivos[k]].X,verticesEstrella[verticesActivos[k]].Y );
                  //    printf("Scanlinek: %d k: %d Vertice Act: x:%d y:%d no pico\n\n\n",scanline,k+1,verticesEstrella[verticesActivos[k+1]].X,verticesEstrella[verticesActivos[k+1]].Y );
                        verticesActivos[k] = i;
                        verticesActivos[0] = 0;
                //         printf("Scanlinek: %d Vertice Act: x:%d y:%d\n",scanline,verticesHeredia[verticesActivos[k]].X,verticesHeredia[verticesActivos[k]].Y );
                // printf("Scanlinek+1: %d Vertice Act: x:%d y:%d\n\n\n",scanline,verticesHeredia[verticesActivos[0]].X,verticesHeredia[verticesActivos[0]].Y );
                    
                        k+=2;
                }
            }

        // if(vertices[i].Y == scanline){
        //         verticesActivos[k] = i;
        //         printf("Scanlinek: %d Vertice Act: x:%d y:%d\n\n\n",scanline,verticesHeredia[verticesActivos[k]].X,verticesHeredia[verticesActivos[k]].Y );
        //         k++;
        //     }
        // else if(vertices[i].Y > scanline && vertices[i+1].Y < scanline
        //     || vertices[i].Y < scanline && vertices[i+1].Y > scanline){
        //         verticesActivos[k] = i;
        //         verticesActivos[k+1] = i+1;
          
        //         printf("Scanlinek: %d Vertice Act: x:%d y:%d\n",scanline,verticesHeredia[verticesActivos[k]].X,verticesHeredia[verticesActivos[k]].Y );
        //         printf("Scanlinek+1: %d Vertice Act: x:%d y:%d\n\n\n",scanline,verticesHeredia[verticesActivos[k+1]].X,verticesHeredia[verticesActivos[k+1]].Y );
        //         k+=2;

            
        // }
        // else if(i+1 >= numVertices){
        //         if(vertices[i].Y > scanline && vertices[0].Y < scanline
        //             || vertices[i].Y < scanline && vertices[0].Y > scanline){
                  
        //                 verticesActivos[k] = i;
        //                 verticesActivos[0] = 0;
        //                 printf("Scanlinek: %d Vertice Act: x:%d y:%d\n",scanline,verticesHeredia[verticesActivos[k]].X,verticesHeredia[verticesActivos[k]].Y );
        //         printf("Scanlinek+1: %d Vertice Act: x:%d y:%d\n\n\n",scanline,verticesHeredia[verticesActivos[0]].X,verticesHeredia[verticesActivos[0]].Y );
                    
        //                 k+=2;
        // }
        //     }
        
        


    }

    //k = limpiarRepetidoActivos(verticesActivos,k);
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

    return numVerticesActivos;
}

int eliminarVerticales(int numVerticesActivos){

    return numVerticesActivos;
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
  int FB_value = 0;
  
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

void quicksort(vertice *target, int left, int right) {
  if(left >= right) return;
  int i = left, j = right;
  vertice tmp; 
  int pivot = target[i].Y;
  for(;;) {
    while(target[i].Y < pivot) i++;
    while(pivot < target[j].Y) j--;
    if(i >= j) break;
    tmp = target[i]; target[i] = target[j]; target[j] = tmp;
    i++; j--;
  }
  quicksort(target, left, i-1);
  quicksort(target, j+1, right);
}