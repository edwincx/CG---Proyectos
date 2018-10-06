#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "rayTracer.h"
#include "objetos.h"
#include "scanner.h"
#include "lodepng/lodepng.h"

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#define EPSILON 0.05

COLOR **framebuffer;
COLOR color;
PUNTO3D punto3D;
VECTOR vDir;
PANTALLA P;
PUNTO3D pm;
char * output_file_name = "image.png";

COLOR De_que_color();
INTERSECCION First_Intersection();
long double ProductoPunto();


VECTOR normalizarVector(VECTOR V){
	long double norma;

	norma = sqrt(pow(V.X,2) + pow(V.Y,2) + pow(V.Z,2));

	V.X = V.X/norma;
	V.Y = V.Y/norma;
	V.Z = V.Z/norma;

	return V;
}

VECTOR normalEsfera(struct ESFERA *e, PUNTO3D punto_interseccion){
	VECTOR N;
	N.X = (punto_interseccion.Xw - e->centro.Xw) / e->radio;
	N.Y = (punto_interseccion.Yw - e->centro.Yw) / e->radio;
	N.Z = (punto_interseccion.Zw - e->centro.Zw) / e->radio;
	return N;
}



VECTOR calcularNormal(struct OBJETO *Q, PUNTO3D punto_interseccion){
	VECTOR N;
	if(Q->tipo == "esfera"){
		N = normalEsfera(Q->p, punto_interseccion);
	}
	
	N = normalizarVector(N);
	return N;
}

long double ProductoPunto(VECTOR a, VECTOR b){
	long double r;
	r =  (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
	return r;
}

int longitudObjetos(struct OBJETO *objetos){
	struct OBJETO *o;
	int n;
	n = 0;
	o = objetos;
	while(o != NULL){
		n++;
		o = o-> sig;
	}
	return n;
}

int longitudLuces(struct LUZ *luces){
	struct LUZ *o;
	int n;
	n = 0;
	o = luces;
	while(o != NULL){
		n++;
		o = o-> sig;
	}
	return n;
}




struct OBJETO *creanodo(void) {
  return (struct OBJETO *) malloc(sizeof(struct OBJETO));
}

struct LUZ *creaLuz(void) {
  return (struct LUZ *) malloc(sizeof(struct LUZ));
}

struct OBJETO *insertafinal(struct OBJETO *lista, struct OBJETO *q) {
  	struct OBJETO *p;
  	q->sig = NULL;
  	if (lista == NULL){
    	return q;
  	}
	else{
	  	p = lista;
	  	while (p->sig != NULL){
	    	p = p->sig;
	  	}
	  	p->sig = q;
	  	return lista;
	}
}

struct LUZ *insertarLuz(struct LUZ *lista, struct LUZ *l) {
  	struct LUZ *p;
  	l->sig = NULL;
  	if (lista == NULL){
    	return l;
  	}
	else{
	  	p = lista;
	  	while (p->sig != NULL){
	    	p = p->sig;
	  	}
	  	p->sig = l;
	  	return lista;
	}
}


struct VERTICE * append_vertice(struct VERTICE *lista, struct VERTICE * elemento) {
  	struct VERTICE * p;
  	elemento->sig = NULL;
  	if (lista == NULL){
		return elemento;
  	}
	else{
	  	p = lista;
	  	while (p->sig != NULL){
	    		p = p->sig;
	  	}
	  	p->sig = elemento;
		return elemento;
	}
}



long double calcularDiscriminante(float a, float b, float c){
	return (pow(b,2) - 4*a*c);
}

long double interseccionEsfera(struct ESFERA *e, PUNTO3D o, VECTOR d){
	long double b,c,dis,t,t1,t2;
	b = 2*(d.X * (o.Xw - e->centro.Xw) + d.Y * (o.Yw - e->centro.Yw) + d.Z * (o.Zw - e->centro.Zw));
	c = pow((o.Xw - e->centro.Xw), 2) + pow((o.Yw - e->centro.Yw), 2) + pow((o.Zw - e->centro.Zw), 2) - pow((e->radio), 2);

	dis = calcularDiscriminante(1,b,c);

	if(dis < 0){
		t = -1;
	}else{
		t1 = (-b + sqrt(dis))/2;
		t2 = (-b - sqrt(dis))/2;
		if(t1<0 && t2>0)
			t = t2;
		else if(t1>0 && t2<0)
			t = t1;
		else if(t1<0 && t2<0)
			t = -1;
		else{
			t = min(t1, t2);
		}
	}

	return t;
}


long double calcularInterseccion(struct OBJETO *objeto, PUNTO3D ojo, VECTOR dir){
	char* tipo;
	long double t = 0.00000;
	tipo = objeto->tipo;
	if(tipo == "esfera"){

		t = interseccionEsfera(objeto->p, ojo, dir);
	}
	
	return t;
}

INTERSECCION First_Intersection(PUNTO3D o, VECTOR dir){
	struct OBJETO *p;
	INTERSECCION interseccion;
	long double tmin, t;
	int i;

	interseccion.objeto = NULL;
	tmin = 99999.0;
	p = objetos;
	i = 1;

  	while(i <= longitudObjetos(objetos)){
  		t = calcularInterseccion(p, o, dir);
  		if(t < tmin && t > EPSILON){
  			interseccion.objeto = p;
  			tmin = t;
  		}
  		p = p->sig;
  		i++;
  	}
  	if(interseccion.objeto != NULL){
  	  	interseccion.tmin = tmin;
  	  	interseccion.punto_interseccion.Xw = o.Xw + tmin * dir.X;
  	  	interseccion.punto_interseccion.Yw = o.Yw + tmin * dir.Y;
  	  	interseccion.punto_interseccion.Zw = o.Zw + tmin * dir.Z;
  	}
	return interseccion;
}

int read_input_file(char* file_name)
{
	//printf("Archivo de entrada:%s\n", file_name);
	FILE *inFile;
	inFile = freopen(file_name, "r", stdin);
  	if(!inFile){
		printf("Error: is_not_Ak7: Imposible abrir archivo fuente!\n"); exit(1);
  	}

	return scanner(inFile);
}

long double calcularFatt(struct LUZ *luz, INTERSECCION i){
	long double dL, fatt;

	dL = sqrt(pow((luz->pos.Xw - i.punto_interseccion.Xw), 2)+pow((luz->pos.Yw - i.punto_interseccion.Yw), 2)+pow((luz->pos.Zw - i.punto_interseccion.Zw), 2));
	fatt = min(1.0, 1/(luz->C1 + luz->C2 * dL + luz->C3 * pow(dL,2)));

	return fatt;
}

long double calcularDistancia(PUNTO3D a, PUNTO3D b){
	long double d;

	d = sqrt(pow((a.Xw - b.Xw), 2)+pow((a.Yw - b.Yw), 2)+pow((a.Zw - b.Zw), 2));

	return d;
}

VECTOR calcularL(PUNTO3D punto_interseccion, PUNTO3D posLuz){
	VECTOR L;

	L.X = (posLuz.Xw - punto_interseccion.Xw);
	L.Y = (posLuz.Yw - punto_interseccion.Yw);
	L.Z = (posLuz.Zw - punto_interseccion.Zw);
	L = normalizarVector(L);

	return L;
}

VECTOR calcularVectorR(VECTOR N, VECTOR L){
	VECTOR R;
	long double producto;

	producto = ProductoPunto(N,L);
	R.X = 2*N.X*producto - L.X;
	R.Y = 2*N.Y*producto - L.Y;
	R.Z = 2*N.Z*producto - L.Z;
	return R;
}

VECTOR calcularVectorV(PUNTO3D interseccion){
	VECTOR V;

	V.X = (ojo.Xw - interseccion.Xw);
	V.Y = (ojo.Yw - interseccion.Yw);
	V.Z = (ojo.Zw - interseccion.Zw);

	return V;
}


COLOR De_que_color(PUNTO3D ojo, VECTOR vDir){
	COLOR color= BACKGROUND;
	INTERSECCION interseccion, obstaculo;
	VECTOR N, L, V, R;
	long double I, Fatt, E;
	int k;
	struct OBJETO *Q;
	struct LUZ *p;
	

	V.X = -1 * vDir.X;
	V.Y = -1 * vDir.Y;
	V.Z = -1 * vDir.Z;

	interseccion = First_Intersection(ojo, vDir);

	if(!interseccion.objeto || longitudLuces(luces) == 0){
		color = BACKGROUND;
	}
	else
	{	
		color = interseccion.objeto->color;

		Q = interseccion.objeto;
		N = calcularNormal(Q, interseccion.punto_interseccion);

		
		if(ProductoPunto(N,vDir) > 0 && interseccion.objeto->tipo != "poligono"){
			N.X = -1 * N.X;
			N.Y = -1 * N.Y;
			N.Z = -1 * N.Z;
		}
		
		
		I = 0.00000;
		E = 0.00000;
		p = luces;
		for(k = 0; k < longitudLuces(luces); k++){			
			L = calcularL(interseccion.punto_interseccion, p->pos);
			Fatt = calcularFatt(p, interseccion);
			if(ProductoPunto(N,L) > 0.0){
				obstaculo = First_Intersection(interseccion.punto_interseccion, L);
				if(!obstaculo.objeto || calcularDistancia(interseccion.punto_interseccion, p->pos) > interseccion.tmin){
	
						
					I = I + (ProductoPunto(N,L) * Q->Kd * Fatt * p->Ip);
					R = calcularVectorR(N, L);
					
					
				}
			}
			p = p->sig;
		}
		I = I + (escena.Ia * Q->Ka);
		I = min(1.0, I);
		E = min(1.0, E);


		color.r = I * Q->color.r;
		color.g = I * Q->color.g;
		color.b = I * Q->color.b;

		color.r = color.r + E * (1.0 - color.r);
		color.g = color.g + E * (1.0 - color.g);
		color.b = color.b + E * (1.0 - color.b);


	}
	return color;
}

void saveImage(COLOR **framebuffer){


  	/*generate some image*/
  	unsigned char* image = malloc(Hres * Vres * 4);
  	unsigned x, y;
  	for(y = 0; y < Vres; y++)
	for(x = 0; x < Hres; x++)
	{
	image[4 * Hres * y + 4 * x + 0] = (unsigned char) round((255*framebuffer[x][y].r));
	image[4 * Hres * y + 4 * x + 1] = (unsigned char) round((255*framebuffer[x][y].g));
	image[4 * Hres * y + 4 * x + 2] = (unsigned char) round((255*framebuffer[x][y].b));
	image[4 * Hres * y + 4 * x + 3] = 255;
	}
	/*Encode the image*/
	unsigned error = lodepng_encode32_file(output_file_name, image, Hres, Vres);
	/*if there's an error, display it*/
	if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
	free(image);
}




int main(int argc, char** argv)
{

	
	int opt;
	//Parsear los argumentos de entrada
	while ((opt = getopt(argc, argv, "i:o:")) != -1)
	{
		switch (opt)
		{
			case 'i':
			      	//printf("Archivo de entrada:%s\n", optarg);
				input_file_name = optarg;
			      	break;
			case 'o':
			      	//printf("Archivo de entrada:%s\n", optarg);
				output_file_name = optarg;
			      	break;
			default:
				fprintf(stderr, "Usage: %s [-i] [input_file]\n", argv[0]);
				exit(0);
		}
	}


	int i, j;
	long double Xw, Yw, Zw, L, Xd ,Yd ,Zd;
	objetos = NULL;

	struct LUZ *luz1 = creaLuz();


	luz1->pos.Xw = 0,0;
	luz1->pos.Yw = 0.0;
	luz1->pos.Zw = -500.0;
	//intensidad
	luz1->Ip = 40;
	//valores de atenuacion
	luz1->C1 = 1.0;
	luz1->C2 = 0.005;
	luz1->C3 = 0.0;
	//color
	luz1->color.r = 1;
	luz1->color.g = 0;
	luz1->color.b = 0;

	
	P.Xmax = Hres; //400.0;
	P.Ymax = Vres; //400.0;
	P.Zmax = 0.0;//0.0
	//ventana min
	P.Xmin = 0.0;
	P.Ymin = 0.0;
	P.Zmin = 0.0;//0.0

	//ojo
	ojo.Xw = 540.0;
	ojo.Yw = 540.0;
	ojo.Zw = -1500.0;//negativo

	
	escena.Ia = 0.6;
	

	

	framebuffer = (COLOR **)malloc(Hres * sizeof(COLOR*));
	for (i = 0; i < Hres; i++)
	{
	 framebuffer[i] = (COLOR *)malloc(Vres * sizeof(COLOR));
	}

	for (i = 0; i < Hres; i++)
	{
	 for (j = 0; j < Vres; j++)
	    {
	    	framebuffer[i][j].r = 0;
	        framebuffer[i][j].g = 0;
	        framebuffer[i][j].b = 0;
	    }
	}

	if(read_input_file(input_file_name)!=0)
	{
		puts("\nError en archivo de entrada!!!! \n");
		exit(-1);
	}



	for(i = 0; i < Hres; i++){
		for(j = 0; j < Vres; j++){

			punto3D.Xw = ((i + 1/2)*(P.Xmax - P.Xmin) / Hres) + P.Xmin;
			punto3D.Yw = ((j + 1/2)*(P.Ymax - P.Ymin) / Vres) + P.Ymin;
			punto3D.Zw = 0.0;

			
			L = sqrt(pow((punto3D.Xw - ojo.Xw), 2)+pow((punto3D.Yw - ojo.Yw), 2)+pow((punto3D.Zw - ojo.Zw), 2));
			vDir.X = (punto3D.Xw - ojo.Xw) / L;
			vDir.Y = (punto3D.Yw - ojo.Yw) / L;
			vDir.Z = (punto3D.Zw - ojo.Zw) / L;
			

			color = De_que_color(ojo,vDir);
			framebuffer[i][Vres-j] = color;
		}
	}

	//Generar la imagen y guardarla
	saveImage(framebuffer);


	return 0;
}
