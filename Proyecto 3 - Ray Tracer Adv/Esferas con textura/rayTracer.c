/*
Compilar
gcc -o rayTracer rayTracer.c lodepng/lodepng.c -lm


*/

#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
// #include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()


#include "lodepng/lodepng.h"
#include "rayTracer.h"
#include "objetos.h"
#include "scanner.h"


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

	if(norma == 1) return V;
	//printf("V.X = %Lf\n", V.X);
	//printf("V.Y = %Lf\n", V.Y);
	//printf("V.Z = %Lf\n", V.Z);
	//printf("norma = %Lf\n", norma);
	V.X = V.X/norma;
	V.Y = V.Y/norma;
	V.Z = V.Z/norma;

	//printf("V.X/norma = %Lf\n", V.X);
	//printf("V.Y/norma = %Lf\n", V.Y);
	//printf("V.Z/norma = %Lf\n", V.Z);
	//printf("finaliza\n");
	return V;
}

VECTOR normalEsfera(struct ESFERA *e, PUNTO3D punto_interseccion){
	VECTOR N;
	N.X = (punto_interseccion.Xw - e->centro.Xw) / e->radio;
	N.Y = (punto_interseccion.Yw - e->centro.Yw) / e->radio;
	N.Z = (punto_interseccion.Zw - e->centro.Zw) / e->radio;
	return N;
}

VECTOR normalCilindro(struct CILINDRO *c, PUNTO3D i){
	VECTOR N;
	long double a, e;
	VECTOR Q;
	PUNTO3D O;
	Q = c->Q;
	O = c->centro;
	VECTOR A;
	//Vector de ancla a punto de interseccion
	A.X = (i.Xw - O.Xw);
	A.Y = (i.Yw - O.Yw);
	A.Z = (i.Zw - O.Zw);
	e = ProductoPunto(A,Q);
	
	//Punto en el eje
	PUNTO3D punto_eje;
	punto_eje.Xw =  O.Xw + e * Q.X;
	punto_eje.Yw =  O.Yw + e * Q.Y;
	punto_eje.Zw =  O.Zw + e * Q.Z;

	N.X = i.Xw - punto_eje.Xw;
	N.Y = i.Yw - punto_eje.Yw;
	N.Z = i.Zw - punto_eje.Zw;
	
	return N;
	a = (i.Xw * Q.X - O.Xw * Q.X + i.Yw * Q.Y - O.Yw * Q.Y + i.Zw * Q.Z - O.Zw * Q.Z)/10;

	//printf("Comienza\n");

	//printf("i.Xw = %Lf\n", i.Xw);
	//printf("i.Yw = %Lf\n", i.Yw);
	//printf("i.Zw = %Lf\n", i.Zw);
	////printf("%Lf\n", a);


	//printf("O.Xw = %Lf\n", O.Xw);
	//printf("O.Yw = %Lf\n", O.Yw);
	//printf("O.Zw = %Lf\n", O.Zw);

	//printf("Q.X = %Lf\n", Q.X);
	//printf("Q.Y = %Lf\n", Q.Y);
	//printf("Q.Z = %Lf\n", Q.Z);
	N.X = ((2 * (O.Xw + a * Q.X - i.Xw) * (Q.X * Q.X - 1)) + (2 * (O.Yw + a * Q.Y - i.Yw) * (Q.X * Q.Y)) + (2 * (O.Zw + a * Q.Z - i.Zw) * (Q.X * Q.Z)))/10;
	N.Y = ((2 * (O.Xw + a * Q.X - i.Xw) * (Q.Y * Q.X)) + (2 * (O.Yw + a * Q.Y - i.Yw) * (Q.Y * Q.Y - 1)) + ((2 * (O.Zw + a * Q.Z - i.Zw) * (Q.Y * Q.Z))))/10;
	N.Z = ((2 * (O.Xw + a * Q.X - i.Xw) * (Q.Z * Q.X)) + (2 * (O.Yw + a * Q.Y - i.Yw) * (Q.Z * Q.Y)) + (2 * (O.Zw + a * Q.Z - i.Zw) * (Q.Z * Q.Z - 1)));
	//printf("N.X = %Lf\n", N.X);
	//printf("N.Y = %Lf\n", N.Y);
	//printf("N.Z = %Lf\n", N.Z);



	N.X += N.X; 
	N.Y += N.X;
	N.Z += N.X;
	//printf("pm.Xw = %Lf\n", pm.Xw);
	//printf("pm.Yw = %Lf\n", pm.Yw);
	//printf("pm.Zw = %Lf\n", pm.Zw);
	//printf("i.Xw = %Lf\n", i.Xw);
	//printf("i.Yw = %Lf\n", i.Yw);
	//printf("i.Zw = %Lf\n", i.Zw);
	//printf("N.X = %Lf\n", N.X);
	//printf("N.Y = %Lf\n", N.Y);
	//printf("N.Z = %Lf\n", N.Z);

	//printf("N.X = %Lf\n", N.X);
	//printf("N.Y = %Lf\n", N.Y);
	//printf("N.Z = %Lf\n", N.Z);
	return N;
}

VECTOR normalCono(struct CONO *c, PUNTO3D i){
	VECTOR N;
	long double a;
	VECTOR Q;
	PUNTO3D O;
	Q = c->Q;
	O = c->centro;

	a = (i.Xw * Q.X - O.Xw * Q.X + i.Yw * Q.Y - O.Yw * Q.Y + i.Zw * Q.Z - O.Zw * Q.Z);


	/*
	printf("punto_interseccion.Xw = %Lf\n", punto_interseccion.Xw);
	printf("punto_interseccion.Yw = %Lf\n", punto_interseccion.Yw);
	printf("punto_interseccion.Zw = %Lf\n", punto_interseccion.Zw);*/


	/*
	printf("e->centro.Xw = %Lf\n", e->centro.Xw);
	printf("e->centro.Yw = %Lf\n", e->centro.Yw);
	printf("e->centro.Zw = %Lf\n", e->centro.Zw);*/
	N.X = 2 * (O.Xw + a * Q.X - i.Xw) * (Q.X * Q.X - 1) + 2 * (O.Yw + a * Q.Y - i.Yw) * (Q.X * Q.Y) + 2 * (O.Zw + a * Q.Z - i.Zw) * (Q.X * Q.Z) - 2 * ((c->k2/c->k1) * a) * ((c->k2/c->k1) * Q.X);
	N.Y = ((2 * (O.Xw + a * Q.X - i.Xw) * (Q.Y * Q.X) + 2 * (O.Yw + a * Q.Y - i.Yw) * (Q.Y * Q.Y - 1) + 2 * (O.Zw + a * Q.Z - i.Zw) * (Q.Y * Q.Z))) - 2 * ((c->k2/c->k1) * a) * ((c->k2/c->k1) * Q.Y);
	N.Z = ((2 * (O.Xw + a * Q.X - i.Xw) * (Q.Z * Q.X) + 2 * (O.Yw + a * Q.Y - i.Yw) * (Q.Z * Q.Y) + 2 * (O.Zw + a * Q.Z - i.Zw) * (Q.Z * Q.Z - 1))) - 2 * ((c->k2/c->k1) * a) * ((c->k2/c->k1) * Q.Z);
	//printf("N.X = %Lf\n", N.X);


	return N;
}


VECTOR normalPoligono(struct POLIGONO *p, PUNTO3D punto_interseccion)
{
	if(p->normal.X == 0 && p->normal.Y == 0 && p->normal.Z == 0 ) //Si el vector esta vacio, calcularlo
	{
		VECTOR N;
		N.X = abs(p->A);// - punto_interseccion.Xw;
		N.Y = abs(p->B);// - punto_interseccion.Yw;
		N.Z = abs(p->C);// - punto_interseccion.Zw;
	

		p->normal = N;
	}
	return p->normal;
}

VECTOR normalDisco(struct DISCO *p, PUNTO3D punto_interseccion)
{
	if(p->normal.X == 0 && p->normal.Y == 0 && p->normal.Z == 0 ) //Si el vector esta vacio, calcularlo
	{
		VECTOR N;
		N.X = abs(p->A);// - punto_interseccion.Xw;
		N.Y = abs(p->B);// - punto_interseccion.Yw;
		N.Z = abs(p->C);// - punto_interseccion.Zw;
	

		p->normal = N;
	}
	return p->normal;
}

VECTOR normalElipse(struct ELIPSE *p, PUNTO3D punto_interseccion)
{
	if(p->normal.X == 0 && p->normal.Y == 0 && p->normal.Z == 0 ) //Si el vector esta vacio, calcularlo
	{
		VECTOR N;
		N.X = abs(p->A);// - punto_interseccion.Xw;
		N.Y = abs(p->B);// - punto_interseccion.Yw;
		N.Z = abs(p->C);// - punto_interseccion.Zw;
	

		p->normal = N;
	}
	return p->normal;
}

VECTOR calcularNormal(struct OBJETO *Q, PUNTO3D punto_interseccion){
	VECTOR N;
	if(Q->tipo == "esfera"){
		N = normalEsfera(Q->p, punto_interseccion);
	}
	else if(Q->tipo == "cilindro"){
		N = normalCilindro(Q->p, punto_interseccion);
	}
	else if(Q->tipo == "poligono"){
		N = normalPoligono(Q->p, punto_interseccion);
	}
	else if(Q->tipo == "disco"){
		N = normalDisco(Q->p, punto_interseccion);
	}
	else if(Q->tipo == "cono"){
		N = normalCono(Q->p, punto_interseccion);
	}
	else if(Q->tipo == "elipse"){
		N = normalElipse(Q->p, punto_interseccion);
	}
	N = normalizarVector(N);
	//printf("N:%Lf,%Lf,%Lf\n",N.X, N.Y, N.Z);
	return N;
}

long double ProductoPunto(VECTOR a, VECTOR b){
	long double r;
	r =  (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
	return r;
}


VECTOR producto_cruz(VECTOR v, VECTOR w)
{

		VECTOR r;
		r.X = v.Y*w.Z - v.Z*w.Y;
		r.Y = v.Z*w.X - v.X*w.Z;
		r.Z = v.X*w.Y - v.Y*w.X;
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


int longitudIntersecciones(struct INTERSECCIONES *intersecciones){
	struct INTERSECCIONES *i;
	int n;
	n = 0;
	i = intersecciones;
	while(i != NULL){
		n++;
		i = i-> sig;
	}
	return n;
}


struct OBJETO *creanodo(char* tipo) {
	struct OBJETO* new_objeto = (struct OBJETO *) malloc(sizeof(struct OBJETO));
	
	//Inicializar Objeto con valores predeterminados 
	new_objeto->sig = NULL;
	new_objeto->tex_buffer =NULL;
	new_objeto->u = -1;
	new_objeto->v = -1;
	//Set funcion de texturizado
	if(tipo == "poligono") 	new_objeto->map_texture = &map_texture_poligono;
	else if(tipo == "cilindro") 	new_objeto->map_texture = &map_texture_cilindro;		
	else if(tipo == "esfera") 	new_objeto->map_texture = &map_texture_esfera;		


	
  	return new_objeto;
}

struct LUZ *creaLuz(void) {
  return (struct LUZ *) malloc(sizeof(struct LUZ));
}

struct INTERSECCIONES *crearInterseccion(void){
	return (struct INTERSECCIONES *) malloc(sizeof(struct INTERSECCIONES));
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

struct INTERSECCIONES *insertarInterseccion(struct INTERSECCIONES *lista, struct INTERSECCIONES *i)
{	

	struct INTERSECCIONES *p;
  	i->sig = NULL;
  	if (lista == NULL){
  		//printf("estoy en if insertarInterseccion\n");
    	return i;
  	}
	else{
		//printf("estoy en else insertarInterseccion\n");
	  	p = lista;
	  	while (p->sig != NULL){
			//printf("estoy en while insertarInterseccion\n");
	    	p = p->sig;
	  	}
		//printf("salí del while insertarInterseccion\n");
	  	p->sig = i;
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
		return lista;
	}
}



PLANO *insertar_plano_corte( PLANO *lista,  PLANO *elemento)
{
	PLANO * p;
  	elemento->sig = NULL;
  	if (lista == NULL){
		lista = elemento;
		return elemento;
  	}
	else{
	  	p = lista;
	  	while (p->sig != NULL){
	    		p = p->sig;
	  	}
	  	p->sig = elemento;
		return lista;
	}
}




long double calcularDiscriminante(float a, float b, float c){
	return (pow(b,2) - 4*a*c);
}




long double interseccion_plano(PLANO *plano, PUNTO3D ojo, VECTOR dir){
	long double numerador,denominador,t;
	numerador = -( ( plano->A * ojo.Xw ) + ( plano->B * ojo.Yw ) + ( plano->C * ojo.Zw ) + plano->D);
	denominador = ( (plano->A * dir.X) + (plano->B * dir.Y ) + (plano->C * dir.Z));
	if(denominador == 0 ){	// Si no hay interseccion con el Poligono!
		t = -1;
		return t;
	}
	t = numerador/ denominador;
	if(t<0 )	//Si la interseccion esta detras de la camara!
	{
		t = -1;
		return t;
	}
	return t;
}


//Detecta interseccion con Planos de Corte, recive: t del objeto, lista de planos de corte, ojo, vector dir
long double interseccion_plano_corte(long double  t, PLANO *plano_corte, PUNTO3D ojo, VECTOR dir)
{
	///////////////////Planos de Corte///////////////
	long double t_plano;
	while(plano_corte != NULL){
		t_plano = interseccion_plano(plano_corte,  ojo,  dir);
		
		if( t_plano > 0  &&  t_plano > t &&   plano_corte->signo == 1  ) t = -1;//Interseccion con planos de corte POSITIVO
		else if( t_plano > 0  &&  t_plano < t &&   plano_corte->signo == -1  ) t = -1;//Interseccion con planos de corte NEGATIVO
		plano_corte = plano_corte-> sig;
	}
	////////////////////////////////////////////
	return t;

}

long double interseccionEsfera(struct ESFERA *e, PUNTO3D o, VECTOR d){
	long double b,c,dis,t,t1,t2;
	b = 2*(d.X * (o.Xw - e->centro.Xw) + d.Y * (o.Yw - e->centro.Yw) + d.Z * (o.Zw - e->centro.Zw));
	c = pow((o.Xw - e->centro.Xw), 2) + pow((o.Yw - e->centro.Yw), 2) + pow((o.Zw - e->centro.Zw), 2) - pow((e->radio), 2);

	dis = calcularDiscriminante(1,b,c);

	
	if(dis < 0){
		t = -1;
	}	
	else{
		t1 = (-b + sqrt(dis))/2;
		t1 = interseccion_plano_corte(t1, e->planos_corte, o, d);

		t2 = (-b - sqrt(dis))/2;
		t2 = interseccion_plano_corte(t2, e->planos_corte, o, d);

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
/*
β = 2(X D (X E – X C ) + Y D (Y E – Y C ) + Z D (Z E – Z C ))
γ = (X E - X C ) 2 + (Y E - Y C ) 2 + (Z E - Z C ) 2 – R 2
*/

long double interseccionCilindro(struct CILINDRO *c, PUNTO3D e, VECTOR d){
	long double a,b,y,discriminante,t,t1,t2,dist;
	VECTOR Q;
	PUNTO3D O;
	Q = normalizarVector(c->Q);
	O = c->centro;


	/*printf("Q = [%Lf, %Lf, %Lf]\n", Q.X, Q.Y, Q.Z);
	printf("O = [%Lf, %Lf, %Lf]\n", O.Xw, O.Yw, O.Zw);
	printf("e = (%Lf, %Lf, %Lf)\n", e.Xw, e.Yw, e.Zw);
	printf("d = [%Lf, %Lf, %Lf]\n", d.X, d.Y, d.Z);*/
	//printf("%Lf\n", i.Yw);
	//printf("%Lf, %Lf, %Lf\n", i.Xw, i.Yw, i.Zw);
	//printf("%Lf\n", dis);
	//if(c->d1 <= dis && dis <= c->d2){

	//a = ((pow(Q.X,2) + pow(Q.Y,2) + pow(Q.Z,2) - 2) * pow((d.X*Q.X + d.Y*Q.Y + d.Z*Q.Z),2)) + pow(d.X,2) + pow(d.Y,2) + pow(d.Z,2);
	//b = 2*((Q.X*(d.X*Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.X) * ((O.Xw - e.Xw) * (1 - pow(Q.X,2)) + Q.X*(Q.Y*(e.Yw - O.Yw) + Q.Z*(e.Zw - O.Zw))) + (Q.Y*(d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.Y) * ((O.Yw - e.Yw) * (1 - pow(Q.Y,2)) + Q.Y*(Q.X*(e.Xw - O.Xw) + Q.Z*(e.Zw - O.Zw))) + (Q.Z*(d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.Z) * ((O.Zw - e.Zw) * (1 - pow(Q.Z,2)) + Q.Z*(Q.X*(e.Xw - O.Xw) + Q.Y*(e.Yw - O.Yw))));
	//y = pow(((O.Xw - e.Xw) * (1 - pow(Q.X,2)) + Q.X*(e.Yw * Q.Y - O.Yw * Q.Y + e.Zw * Q.Z - O.Zw * Q.Z)), 2) + pow(((O.Yw - e.Yw) * (1 - pow(Q.Y,2)) + Q.Y*(e.Xw * Q.X - O.Xw * Q.X + e.Zw * Q.Z - O.Zw * Q.Z)), 2) + pow(((O.Zw - e.Zw) * (1 - pow(Q.Z,2)) + Q.Z*(e.Xw * Q.X - O.Xw * Q.X + e.Yw * Q.Y - O.Yw * Q.Y)), 2) - pow(c->radio,2);
	//discriminante = calcularDiscriminante(a,b,y);
	//printf("a = %Lf\n", a);
	//printf("b = %Lf\n", b);
	//printf("y = %Lf\n", y);
	//printf("dis = %Lf\n", discriminante);

	a = pow(Q.X * (d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.X,2) + pow(Q.Y * (d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.Y,2) + pow(Q.Z * (d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.Z,2);
	b = 2*((Q.X * (d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.X) * (O.Xw - e.Xw + Q.X * (Q.X * (e.Xw - O.Xw) + Q.Y * (e.Yw - O.Yw) + Q.Z * (e.Zw - O.Zw))) + (Q.Y * (d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.Y) * (O.Yw - e.Yw + Q.Y * (Q.X * (e.Xw - O.Xw) + Q.Y * (e.Yw - O.Yw) + Q.Z * (e.Zw - O.Zw))) + (Q.Z * (d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.Z) * (O.Zw - e.Zw + Q.Z * (Q.X * (e.Xw - O.Xw) + Q.Y * (e.Yw - O.Yw) + Q.Z * (e.Zw - O.Zw))));
	y = pow(O.Xw - e.Xw + Q.X * (Q.X * (e.Xw - O.Xw) + Q.Y * (e.Yw - O.Yw) + Q.Z * (e.Zw - O.Zw)),2) + pow(O.Yw - e.Yw + Q.Y * (Q.X * (e.Xw - O.Xw) + Q.Y * (e.Yw - O.Yw) + Q.Z * (e.Zw - O.Zw)),2) + pow(O.Zw - e.Zw + Q.Z * (Q.X * (e.Xw - O.Xw) + Q.Y * (e.Yw - O.Yw) + Q.Z * (e.Zw - O.Zw)),2) - pow(c->radio,2);
	discriminante = calcularDiscriminante(a,b,y);

	//printf("a = %Lf\n", a);
	//printf("b = %Lf\n", b);
	//printf("y = %Lf\n", y);
	//printf("dis = %Lf\n", discriminante);

	//a = pow(Q.X * (d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.X,2) + pow( Q.Y * (d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.Y,2) + pow(Q.Z * ( d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.Z,2);
	//b = 2*((Q.X * (d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.X) * (O.Xw - e.Xw + Q.X * (Q.X * (e.Xw - O.Xw) + Q.Y * (e.Yw - O.Yw) + Q.Z * (e.Zw - O.Zw))) + (Q.Y * (d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.Y) * (O.Yw - e.Yw + Q.Y * (Q.X * (e.Xw - O.Xw) + Q.Y * (e.Yw - O.Yw) + Q.Z * (e.Zw - O.Zw))) + (Q.Z * (d.X * Q.X + d.Y * Q.Y + d.Z * Q.Z) - d.Z) * (O.Zw - e.Zw + Q.Z * ( (e.Xw - O.Xw) *Q.X + (e.Yw - O.Yw) * Q.Y + (e.Zw - O.Zw) * Q.Z)));
	//y = pow( ( (e.Xw - O.Xw) * Q.X + (e.Yw - O.Yw) * Q.Y + (e.Zw - O.Zw) * Q.Z) * Q.X - e.Xw + O.Xw,2) + pow( ( (e.Xw - O.Xw) * Q.X + (e.Yw - O.Yw) * Q.Y + (e.Zw - O.Zw) * Q.Z) * Q.Y - e.Yw + O.Yw, 2) + pow( ( (e.Xw - O.Xw) * Q.X + (e.Yw - O.Yw) * Q.Y + (e.Zw - O.Zw) * Q.Z) * Q.Z - e.Zw + O.Zw,2) - pow(c->radio,2);
	//discriminante = calcularDiscriminante(a,b,y);

	if(discriminante < 0){
		t = -1;
	}else{
		t1 = (-b + sqrt(discriminante))/a*2;
		t1 = interseccion_plano_corte(t1, c->planos_corte, e, d);

		t2 = (-b - sqrt(discriminante))/a*2;
		t2 = interseccion_plano_corte(t2, c->planos_corte, e, d);

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



	if(t != -1){
		dist = ((e.Xw + t * d.X) - O.Xw) * Q.X + ((e.Yw + t * d.Y) - O.Yw) * Q.Y  + ((e.Zw + t * d.Z) - O.Zw) * Q.Z;
		if(!(c->d1 < dist &&  dist < c->d2)){
			//printf("holis\n");
			t = max(t1, t2);

			dist = ((e.Xw + t * d.X) - O.Xw) * Q.X + ((e.Yw + t * d.Y) - O.Yw) * Q.Y  + ((e.Zw + t * d.Z) - O.Zw) * Q.Z;
			if(!(c->d1 < dist &&  dist < c->d2)){
				//printf("holis\n");
				t = -1;
			}
		}
	}
	//printf("t = %Lf\n", t);
	//printf("holis t\n");


	pm.Xw = O.Xw + dist * Q.X;
	pm.Yw = O.Yw + dist * Q.Y;
	pm.Zw = O.Zw + dist * Q.Z;
	return t;
}



long double interseccionCono(struct CONO *c, PUNTO3D ojo, VECTOR dir){
	long double a,b,y,discriminante,t,t1,t2,dist;
	VECTOR Q;
	PUNTO3D O;
	Q = normalizarVector(c->Q);
	O = c->centro;

	a = pow(Q.X * (dir.X * Q.X + dir.Y * Q.Y + dir.Z * Q.Z) - dir.X,2) + pow(Q.Y * (dir.X * Q.X + dir.Y * Q.Y + dir.Z * Q.Z) - dir.Y,2) + pow(Q.Z * (dir.X * Q.X + dir.Y * Q.Y + dir.Z * Q.Z) - dir.Z,2) - pow((c->k2/c->k1)*(dir.X*Q.X + dir.Y*Q.Y + dir.Z*Q.Z),2);
	b = 2*((Q.X * (dir.X * Q.X + dir.Y * Q.Y + dir.Z * Q.Z) - dir.X) * (O.Xw - ojo.Xw + Q.X * (Q.X * (ojo.Xw - O.Xw) + Q.Y * (ojo.Yw - O.Yw) + Q.Z * (ojo.Zw - O.Zw))) + (Q.Y * (dir.X * Q.X + dir.Y * Q.Y + dir.Z * Q.Z) - dir.Y) * (O.Yw - ojo.Yw + Q.Y * (Q.X * (ojo.Xw - O.Xw) + Q.Y * (ojo.Yw - O.Yw) + Q.Z * (ojo.Zw - O.Zw))) + (Q.Z * (dir.X * Q.X + dir.Y * Q.Y + dir.Z * Q.Z) - dir.Z) * (O.Zw - ojo.Zw + Q.Z * (Q.X * (ojo.Xw - O.Xw) + Q.Y * (ojo.Yw - O.Yw) + Q.Z * (ojo.Zw - O.Zw))) - ((c->k2/c->k1)*(dir.X*Q.X + dir.Y*Q.Y + dir.Z*Q.Z)*((c->k2/c->k1)*(Q.X*(ojo.Xw - O.Xw) + Q.Y*(ojo.Yw - O.Yw) + Q.Z*(ojo.Zw - O.Zw)))) );
	y = pow(O.Xw - ojo.Xw + Q.X * (Q.X * (ojo.Xw - O.Xw) + Q.Y * (ojo.Yw - O.Yw) + Q.Z * (ojo.Zw - O.Zw)),2) + pow(O.Yw - ojo.Yw + Q.Y * (Q.X * (ojo.Xw - O.Xw) + Q.Y * (ojo.Yw - O.Yw) + Q.Z * (ojo.Zw - O.Zw)),2) + pow(O.Zw - ojo.Zw + Q.Z * (Q.X * (ojo.Xw - O.Xw) + Q.Y * (ojo.Yw - O.Yw) + Q.Z * (ojo.Zw - O.Zw)),2) - pow((c->k2/c->k1)*(Q.X*(ojo.Xw - O.Xw) + Q.Y*(ojo.Yw - O.Yw) + Q.Z*(ojo.Zw - O.Zw)),2);
	discriminante = calcularDiscriminante(a,b,y);

	if(discriminante < 0){
		t = -1;
	}else{
		t1 = (-b + sqrt(discriminante))/a*2;
		t1 = interseccion_plano_corte(t1, c->planos_corte, ojo, dir);

		t2 = (-b - sqrt(discriminante))/a*2;        
        t2 = interseccion_plano_corte(t2, c->planos_corte, ojo, dir);

		t = min(t1, t2);
		//printf("t1 = %Lf\n", t1);
		//printf("t2 = %Lf\n", t2);
		//printf("t = %Lf\n", t);
	}



	if(t != -1){
		dist = ((ojo.Xw + t * dir.X) - O.Xw) * Q.X + ((ojo.Yw + t * dir.Y) - O.Yw) * Q.Y  + ((ojo.Zw + t * dir.Z) - O.Zw) * Q.Z;
		if(!(c->d1 < dist &&  dist < c->d2)){
			//printf("holis\n");
			t = max(t1, t2);

			dist = ((ojo.Xw + t * dir.X) - O.Xw) * Q.X + ((ojo.Yw + t * dir.Y) - O.Yw) * Q.Y  + ((ojo.Zw + t * dir.Z) - O.Zw) * Q.Z;
			if(!(c->d1 <= dist &&  dist <= c->d2)){
				//printf("holis\n");
				t = -1;
			}
		}
	}
	//printf("t = %Lf\n", t);
	//printf("holis t\n");
	return t;
}



int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
     (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}


long double interseccionPoligono(struct POLIGONO *poligono, PUNTO3D ojo, VECTOR dir){
	long double numerador,denominador,t;


	numerador = -( ( poligono->A * ojo.Xw ) + ( poligono->B * ojo.Yw ) + ( poligono->C * ojo.Zw ) + poligono->D);	
	denominador = ( (poligono->A * dir.X) + (poligono->B * dir.Y ) + (poligono->C * dir.Z));

	if(denominador == 0 ){	// Si no hay interseccion con el Poligono!
		t = -1;
		return t;
	}

	t = numerador/ denominador;

	t = interseccion_plano_corte(t, poligono->planos_corte, ojo, dir);

	if(t<0 )	//Si la interseccion esta detras de la camara!
	{
		t = -1;
		return t;
	}

	//Calcular Punto de Interseccion!
	PUNTO3D punto_interseccion;
  	punto_interseccion.Xw = ojo.Xw + t * dir.X;
  	punto_interseccion.Yw = ojo.Yw + t * dir.Y;
  	punto_interseccion.Zw = ojo.Zw + t * dir.Z;


	//Aplanar punto de interseccion a 2D!
	PUNTO3D punto_interseccion_2D;
	//Si el coeficiente A es mayor, eliminar X
	if(abs(poligono->B) < abs(poligono->A) && abs(poligono->C) < abs(poligono->A))
	{
		punto_interseccion_2D.Xw = punto_interseccion.Yw;
  		punto_interseccion_2D.Yw = punto_interseccion.Zw;
  		punto_interseccion_2D.Zw = 0;
	}
	//Si el coeficiente B es mayor, eliminar Y
	else if(abs(poligono->A) < abs(poligono->B) && abs(poligono->C) < abs(poligono->B))
	{
		punto_interseccion_2D.Xw = punto_interseccion.Xw;
  		punto_interseccion_2D.Yw = punto_interseccion.Zw;
  		punto_interseccion_2D.Zw = 0;
	}
	else  	//Si el coeficiente C es mayor, eliminar Z
	{
		punto_interseccion_2D.Xw = punto_interseccion.Xw;
  		punto_interseccion_2D.Yw = punto_interseccion.Yw;
  		punto_interseccion_2D.Zw = 0;
	}


	int NV = poligono->vertices_num;
	float IX = punto_interseccion_2D.Xw;
	float IY = punto_interseccion_2D.Yw;



	float vertx[NV];
	float verty[NV];

	struct VERTICE  * vert_temp = poligono->vertices_list_2D;
	int a;
	for(a=0; a < NV; a++)
	{
		vertx[a]= (float) vert_temp->X;
		verty[a]= (float) vert_temp->Y;
		vert_temp=vert_temp->sig;
	}


	int c = pnpoly(NV, vertx, verty, IX, IY);

	if (c ==0){
		t =-1;
		
	}
	return t;

}




long double interseccionDisco(struct DISCO *disco, PUNTO3D ojo, VECTOR dir){
	long double numerador,denominador,t;


	numerador = -( ( disco->A * ojo.Xw ) + ( disco->B * ojo.Yw ) + ( disco->C * ojo.Zw ) + disco->D);
	denominador = ( (disco->A * dir.X) + (disco->B * dir.Y ) + (disco->C * dir.Z));



	if(denominador == 0 ){	// Si no hay interseccion con el Poligono!
		t = -1;
		return t;
	}

	t = numerador/ denominador;

	t = interseccion_plano_corte(t, disco->planos_corte, ojo, dir);

	if(t<0 )	//Si la interseccion esta detras de la camara!
	{
		t = -1;
		return t;
	}

	PUNTO3D centro = disco -> centro;
	
	//Calcular Punto de Interseccion!
	PUNTO3D punto_interseccion;
  	punto_interseccion.Xw = ojo.Xw + t * dir.X;
  	punto_interseccion.Yw = ojo.Yw + t * dir.Y;
  	punto_interseccion.Zw = ojo.Zw + t * dir.Z;
	
	//Calcular que el punto de interseccion con el plano sea menor al radio del disco
	//verifica si el punto (xi,yi,zi) vive en el disco
	long double delta_dist = ( pow((punto_interseccion.Xw - centro.Xw),2) + pow((punto_interseccion.Yw - centro.Yw),2) + pow((punto_interseccion.Zw - centro.Zw),2) - pow(disco->radio,2));
	if (delta_dist > 0) t = -1;

	long double delta_anillo = ( pow((punto_interseccion.Xw - centro.Xw),2) + pow((punto_interseccion.Yw - centro.Yw),2) + pow((punto_interseccion.Zw - centro.Zw),2) - pow(disco->anillo,2));
	if (delta_anillo < 0) t = -1;
		
	
	return t;
}



long double interseccionElipse(struct ELIPSE *elipse, PUNTO3D ojo, VECTOR dir){
	long double numerador,denominador,t, d1,d2,d3,excentricidad;

	numerador = -( ( elipse->A * ojo.Xw ) + ( elipse->B * ojo.Yw ) + ( elipse->C * ojo.Zw ) + elipse->D);
	denominador = ( (elipse->A * dir.X) + (elipse->B * dir.Y ) + (elipse->C * dir.Z));


	if(denominador == 0 ){	// Si no hay interseccion con el Poligono!
		t = -1;
		return t;
	}

	t = numerador/ denominador;

	t = interseccion_plano_corte(t, elipse->planos_corte, ojo, dir);

	if(t<0 )	//Si la interseccion esta detras de la camara!
	{
		t = -1;
		return t;
	}

	
	//Calcular Punto de Interseccion!
	PUNTO3D punto_interseccion;
  	punto_interseccion.Xw = ojo.Xw + t * dir.X;
  	punto_interseccion.Yw = ojo.Yw + t * dir.Y;
  	punto_interseccion.Zw = ojo.Zw + t * dir.Z;


  	PUNTO3D f1 = elipse -> f1;//foco 1
  	PUNTO3D f2 = elipse -> f2;//foco 2

  	// calcular d1 distancia entre Foco 1 con el centro u origen
  	d1 = sqrt(pow((f1.Xw - punto_interseccion.Xw),2) + pow(f1.Yw - punto_interseccion.Yw,2) + pow(f1.Zw - punto_interseccion.Zw,2));
	//if (d1 < (elipse->anillo)) return-1;

  	// calcular d2
  	d2 = sqrt(pow((f2.Xw - punto_interseccion.Xw),2) + pow(f2.Yw - punto_interseccion.Yw,2) + pow(f2.Zw - punto_interseccion.Zw,2));
	//if (d2 < (elipse->anillo)) return-1;


  	// calcula la distancia de d1 y d2
  	d3 = sqrt(pow((f1.Xw - f2.Xw),2) + pow(f1.Yw - f2.Yw,2) + pow(f1.Zw - f2.Zw,2));
 

	long double delta_dist = (d1+d2);

	//verifica si el punto (xi,yi,zi) vive en el elipse
	if (delta_dist > 2*(elipse->k) || d3>2*(elipse->k) ) t = -1;
	
	//Anillo
	if (delta_dist < 2*(elipse->anillo)) t = -1;
	return t;
}



long double calcularInterseccion(struct OBJETO *objeto, PUNTO3D ojo, VECTOR dir){
	char* tipo;
	long double t = 0.00000;
	tipo = objeto->tipo;
	if(tipo == "esfera"){
		//printf("soy una bolita\n");
		//printf("%Lf\n", interseccionEsfera(objeto->p, ojo, dir));
		t = interseccionEsfera(objeto->p, ojo, dir);

		//if(t>0) printf("Esfera: %Lf\n",t);
	}
	else if(tipo == "cilindro"){
		//printf("soy un cilindro\n");
		//printf("%Lf\n", interseccionCilindro(objeto->p, ojo, dir));
		//printf("%Lf\n", i.Xw);
		t = interseccionCilindro(objeto->p, ojo, dir);
	}
	else if(tipo == "poligono"){
		//printf("soy un poligono\n");
		//printf("%Lf\n", interseccionCilindro(objeto->p, ojo, dir));
		//printf("%Lf\n", i.Xw);
		t = interseccionPoligono(objeto->p, ojo, dir);
		//if(t>0)	printf("Poligono: %Lf\n",t);

		//printf("Interseccion con poligono %Lf\n", t);
	}
	else if(tipo == "disco"){
		t = interseccionDisco(objeto->p, ojo, dir);
		//printf("Interseccion %Lf\n", t);
	}
	else if(tipo == "cono"){
		t = interseccionCono(objeto->p, ojo, dir);
	}
	else if(tipo == "elipse"){
		t = interseccionElipse(objeto->p, ojo, dir);
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
	//printf("punto_interseccion = (%Lf, %Lf, %Lf) \n", punto_interseccion.Xw, punto_interseccion.Yw, punto_interseccion.Zw);
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


COLOR get_tex_color(INTERSECCION interseccion)  
{
	COLOR color;
	struct OBJETO* objeto;
	struct CILINDRO * cilindro;
	int x,y;
	unsigned char* image;
	VECTOR N;
	double r, theta;
	
	objeto = interseccion.objeto;
	cilindro = objeto->p;
  	image = objeto->tex_buffer; // Bufer con la textura
  	PUNTO3D punto_interseccion = interseccion.punto_interseccion;
  	PUNTO3D ancla = cilindro->centro ;

  	if(objeto->tipo=="poligono"){

  		objeto->map_texture(interseccion);
		
		x = round(objeto->u * objeto->THres);
		y = round(objeto->v * objeto->TVres);
		

		int tex_r, tex_g, tex_b;
		tex_r = image[4 * objeto->THres * y + 4 * x + 0];
		tex_g = image[4 * objeto->THres * y + 4 * x + 1];
		tex_b = image[4 * objeto->THres * y + 4 * x + 2];
		

		
		color.r = tex_r/255.0;
		color.g = tex_g/255.0;
		color.b = tex_b/255.0;
	
    }


  	if(objeto->tipo =="cilindro"){

  		/*objeto->map_texture(interseccion);
		
		x = round(objeto->u * objeto->THres);
		y = round(objeto->v * objeto->TVres);
		

		int tex_r, tex_g, tex_b;
		tex_r = image[4 * objeto->THres * y + 4 * x + 0];
		tex_g = image[4 * objeto->THres * y + 4 * x + 1];
		tex_b = image[4 * objeto->THres * y + 4 * x + 2];
		

		
		color.r = tex_r/255.0;
		color.g = tex_g/255.0;
		color.b = tex_b/255.0;*/
	



  		N = calcularNormal(objeto,interseccion.punto_interseccion);
		
  		N = normalizarVector(N);

		objeto->u = 0.5 + atan2(N.Z, N.X) / 3.14159265359 * 0.5;
		objeto->v = N.Y / 3.14159265359;
		objeto->v = objeto->v- floor(objeto->v);


		x = round(objeto->u * objeto->THres);
		y = round(objeto->v * objeto->TVres);
		

		int tex_r, tex_g, tex_b;
		tex_r = image[4 * objeto->THres * y + 4 * x + 0];
		tex_g = image[4 * objeto->THres * y + 4 * x + 1];
		tex_b = image[4 * objeto->THres * y + 4 * x + 2];
		

		
		color.r = tex_r/255.0;
		color.g = tex_g/255.0;
		color.b = tex_b/255.0;


		
		/*theta = atan(cilindro->Q.Y/ cilindro->Q.X);

		objeto->u = theta;
		objeto->v = cilindro->Q.Z;


  		x = round(objeto->u * objeto->THres);
		y = round(objeto->v * objeto->TVres);
		
			
		int tex_r, tex_g, tex_b;
		tex_r = image[4 * objeto->THres * y + 4 * x + 0];
		tex_g = image[4 * objeto->THres * y + 4 * x + 1];
		tex_b = image[4 * objeto->THres * y + 4 * x + 2];
			

		color.r = tex_r/255.0;
		color.g = tex_g/255.0;
		color.b = tex_b/255.0;*/

    }
	
	

  	if(objeto->tipo=="esfera"){

  		N = calcularNormal(objeto, interseccion.punto_interseccion);

  		objeto->u = 0.5 + atan2(N.Z, N.X) / (2 * 3.14159265359);
  		objeto->v = 0.5 - asin(N.Y) / 3.14159265359;


  		x = round(objeto->u * objeto->THres);
		y = round(objeto->v * objeto->TVres);
		
			
		int tex_r, tex_g, tex_b;
		tex_r = image[4 * objeto->THres * y + 4 * x + 0];
		tex_g = image[4 * objeto->THres * y + 4 * x + 1];
		tex_b = image[4 * objeto->THres * y + 4 * x + 2];
			

		color.r = tex_r/255.0;
		color.g = tex_g/255.0;
		color.b = tex_b/255.0;
  	}

  

	return color;

}

struct INTERSECCIONES *allIntersections(PUNTO3D o, VECTOR dir, struct INTERSECCIONES* lista_intersecciones){
	//printf("entrando a allIntersections\n");
	struct OBJETO *p;
	PUNTO3D punto_interseccion;
	struct INTERSECCIONES *interseccion_aux;
	long double t, tmin, t_aux;
	int i;

	interseccion_aux = crearInterseccion();
	interseccion_aux->sig = NULL;
	p = objetos;
	i = 1;
	tmin = 99999.0;
	t_aux = tmin;
	while(i <= longitudObjetos(objetos)){
  		t = calcularInterseccion(p, o, dir);
  		//printf("t = %Lf\n", t);
  		if(t < tmin && t > EPSILON){
  			tmin = t;
  			interseccion_aux->objeto = p;
  		}
  		p = p->sig;
  		i++;
  	}
  	//printf("%Lf\n", tmin);
  	if(tmin != t_aux){
  		interseccion_aux->tmin = tmin;
		interseccion_aux->punto_interseccion.Xw = o.Xw + tmin * dir.X;
  		interseccion_aux->punto_interseccion.Yw = o.Yw + tmin * dir.Y;
  		interseccion_aux->punto_interseccion.Zw = o.Zw + tmin * dir.Z +1;

		lista_intersecciones = insertarInterseccion(lista_intersecciones, interseccion_aux);
  		//printf("holis\n");
  		lista_intersecciones = allIntersections(interseccion_aux->punto_interseccion, dir, lista_intersecciones);
  	}

	return lista_intersecciones;
}

struct INTERSECCIONES *ajustarIntersecciones(struct INTERSECCIONES* lista_intersecciones){
	struct INTERSECCIONES* cont;
	long double suma;
	cont = lista_intersecciones;
	suma = 0;
	while(cont != NULL){
		cont->tmin += suma;
		suma = cont->tmin;
		cont = cont->sig;
	}
	return lista_intersecciones;
}

COLOR calcularColorTransparencia(struct INTERSECCIONES* interseccion, COLOR color_transparencia, int niveles){
	struct OBJETO *objeto;
	//printf("%d\n", niveles);
	if(interseccion != NULL && niveles > 0){
		objeto = interseccion->objeto;	
		if(objeto->O3 != 0){
			color_transparencia.r += objeto->color.r * objeto->O3;
			color_transparencia.g += objeto->color.g * objeto->O3;
			color_transparencia.b += objeto->color.b * objeto->O3;
			color_transparencia = calcularColorTransparencia(interseccion->sig, color_transparencia, niveles - 1);
		}else{
			color_transparencia.r += objeto->color.r;
			color_transparencia.g += objeto->color.g;
			color_transparencia.b += objeto->color.b;
		}
		
	}

	return color_transparencia;

}


COLOR De_que_color(PUNTO3D ojo, VECTOR vDir, int levels){
	COLOR color, color_reflejo, color_transparencia;
	INTERSECCION interseccion, obstaculo;
	VECTOR N, L, V, R, vReflejo;
	long double I, Fatt, E;
	int k;
	struct OBJETO *Q;
	struct LUZ *p;
	
	struct INTERSECCIONES* intersecciones, *cont;

	color= BACKGROUND;

	V.X = -1 * vDir.X;
	V.Y = -1 * vDir.Y;
	V.Z = -1 * vDir.Z;

	interseccion = First_Intersection(ojo, vDir);

	if(!interseccion.objeto ){ //|| longitudLuces(luces) == 0
		color = BACKGROUND;
	}
	else
	{	
		color = interseccion.objeto->color;
		
		if(interseccion.objeto->tex_buffer != NULL) color = get_tex_color(interseccion); //Si tiene textura optener color de la textura
		//printf("holis2\n");
		//printf("color = (%f, %f, %f)\n", color.r, color.g, color.b);	
		Q = interseccion.objeto;
		N = calcularNormal(Q, interseccion.punto_interseccion);
		//printf("Vector N = [%Lf, %Lf, %Lf]\n", N.X, N.Y, N.Z);
		
		
		if(ProductoPunto(N,vDir) > 0 ){ //&& interseccion.objeto->tipo != "poligono"
			N.X = -1 * N.X;
			N.Y = -1 * N.Y;
			N.Z = -1 * N.Z;
			//if(interseccion.objeto->tipo == "poligono"){printf("<<< %s\n",interseccion.objeto->tipo);}
		}
		
		
		if(Q->O2 > 0){
			vReflejo.X = 2*N.X*ProductoPunto(N,V) - V.X;
			vReflejo.Y = 2*N.Y*ProductoPunto(N,V) - V.Y;
			vReflejo.Z = 2*N.Z*ProductoPunto(N,V) - V.Z;
		}
		
		if(levels > 0)
			color_reflejo = De_que_color(interseccion.punto_interseccion, vReflejo, levels - 1);

		color_transparencia.r = 0;
		color_transparencia.g = 0;
		color_transparencia.b = 0;
		intersecciones = allIntersections(ojo, vDir, NULL);
		intersecciones = ajustarIntersecciones(intersecciones);
		//printf("%d\n", longitudIntersecciones(intersecciones));
		color_transparencia = calcularColorTransparencia(intersecciones, color_transparencia, NIVELES_TRANSPARENCIA*2);
		/*
		struct OBJETO *objetoCanichi;
		cont = intersecciones;
		while(cont != NULL){
			objetoCanichi = cont->objeto;
			printf("%lf\n", objetoCanichi->O3);
			cont = cont->sig;
		}
		*/

		color.r += color_transparencia.r;
		color.g += color_transparencia.g;
		color.b += color_transparencia.b;
		

		I = 0.00000;
		E = 0.00000;
		p = luces;
		for(k = 0; k < longitudLuces(luces); k++){
			L = calcularL(interseccion.punto_interseccion, p->pos);
			//printf("Vector L = [%Lf, %Lf, %Lf]\n", L.X, L.Y, L.Z);
			Fatt = calcularFatt(p, interseccion);
			//printf("ProductoPunto(N,L) = %Lf\n", ProductoPunto(N,L));
			if(ProductoPunto(N,L) > 0.0){
				obstaculo = First_Intersection(interseccion.punto_interseccion, L);
				if(!obstaculo.objeto || calcularDistancia(interseccion.punto_interseccion, p->pos) > interseccion.tmin){
	
						
					I = I + (ProductoPunto(N,L) * Q->Kd * Fatt * p->Ip);
					R = calcularVectorR(N, L);
					//printf("Vector R = [%Lf, %Lf, %Lf]\n", R.X, R.Y, R.Z);
					//printf("Vector V = [%Lf, %Lf, %Lf]\n", V.X, V.Y, V.Z);
					//printf("ProductoPunto(V,R) = %Lf\n", ProductoPunto(V,R));
					
					if(ProductoPunto(V,R) > 0.0){
						E += (pow(ProductoPunto(V,R), Q->Kn) * Q->Ks * p->Ip * Fatt);
					}
				}
			}
			p = p->sig;
		}


		I = I + (escena.Ia * Q->Ka);
		I = min(1.0, I);
		E = min(1.0, E);
		//printf("color(%lf,%lf,%lf)\n", color.r, color.g, color.b);
		//printf("I = %Lf\n", I);

		color.r = I * color.r;
		color.g = I * color.g;
		color.b = I * color.b;


		color.r = color.r * Q->O1 + color_reflejo.r * Q->O2;
		color.g = color.g * Q->O1 + color_reflejo.g * Q->O2;
		color.b = color.b * Q->O1 + color_reflejo.b * Q->O2;
		//printf("color(%lf,%lf,%lf)\n", color.r, color.g, color.b);
		//printf("E = %Lf\n", E);



		color.r = color.r + E * (1.0 - color.r);
		color.g = color.g + E * (1.0 - color.g);
		color.b = color.b + E * (1.0 - color.b);


		//printf("%Lf\n", I);
		//printf("color(%lf,%lf,%lf)\n", color.r, color.g, color.b);

		//printf("BACKGROUND(%lf,%lf,%lf)\n", BACKGROUND.r, BACKGROUND.g, BACKGROUND.b);
		//printf("color(%lf,%lf,%lf)\n", color.r, color.g, color.b);
		

		//printf("color(%lf,%lf,%lf)\n", color.r, color.g, color.b);
		
	}
	return color;
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


//método para iniciar objetos de prueba
void iniciarOjectosP(){
	struct OBJETO *ptr1;
	struct CONO *c1;

	c1 = (struct CONO*) malloc(sizeof(struct CONO));

  	ptr1 = creanodo("cono");
  	ptr1->tipo = "cono";
  	ptr1->color.r = 1.0;
  	ptr1->color.g = 0.0;
  	ptr1->color.b = 0.0;
  	ptr1->Kd = 0.8;
  	ptr1->Ka = 0.2;
  	ptr1->Kn = 12;
  	ptr1->Ks = 1.0;
  	c1->centro.Xw = 30.0;
  	c1->centro.Yw = 200.0;
  	c1->centro.Zw = -500.0;
  	c1->Q.X = 90.0;
  	c1->Q.Y = 10.0;
  	c1->Q.Z = 0.0;
  	c1->d1 = -180;
  	c1->d2 = 50;
  	c1->k1 = 6;
  	c1->k2 = 2;

  	ptr1->p = c1;

  	objetos = insertafinal(objetos, ptr1);


}

COLOR antialiasing(int i, int j, float a, float b){
	COLOR color;
	long double L;
	punto3D.Xw = ((i + a)*(P.Xmax - P.Xmin) / Hres) + P.Xmin;
	punto3D.Yw = ((j + b)*(P.Ymax - P.Ymin) / Vres) + P.Ymin;
	punto3D.Zw = 0.0;

	
	L = sqrt(pow((punto3D.Xw - ojo.Xw), 2)+pow((punto3D.Yw - ojo.Yw), 2)+pow((punto3D.Zw - ojo.Zw), 2));
	vDir.X = (punto3D.Xw - ojo.Xw) / L;
	vDir.Y = (punto3D.Yw - ojo.Yw) / L;
	vDir.Z = (punto3D.Zw - ojo.Zw) / L;
	

	color = De_que_color(ojo,vDir,MAX_LEVELS);

	return color;
}

COLOR calcularPixel(int i, int j, int niveles_anti, float xmin, float xmax, float ymin, float ymax){
	COLOR color, colorNorte, colorSur, colorEste, colorOeste, color1, color2, color3, color4;

	if(niveles_anti == 0){
		
		color1 = antialiasing(i, j, xmax/2.0, ymin);
		color2 = antialiasing(i, j, xmin, ymax/2.0);
		color3 = antialiasing(i, j, xmax, ymin/2.0);
		color4 = antialiasing(i, j, xmax/2.0, ymax);

		color.r = (color1.r + color2.r + color3.r + color4.r)/4.0;
		color.g = (color1.g + color2.g + color3.g + color4.g)/4.0;
		color.b = (color1.b + color2.b + color3.b + color4.b)/4.0;
		//printf("holis\n");
	}else{

		//printf("holis2\n");
		
		colorNorte = calcularPixel(i, j, niveles_anti - 1, xmin, ymin, xmax/2.0, ymax/2.0);
		colorSur = calcularPixel(i, j, niveles_anti - 1, xmax/2.0,ymin, xmax, ymax/2.0);
		colorEste = calcularPixel(i, j, niveles_anti - 1, xmin, ymax/2.0, xmax/2.0,ymax);
		colorOeste = calcularPixel(i, j, niveles_anti - 1, xmax/2.0, ymax/2.0, xmax, ymax);

		color.r = (colorNorte.r + colorSur.r + colorEste.r + colorOeste.r)/4.0;
		color.g = (colorNorte.g + colorSur.g + colorEste.g + colorOeste.g)/4.0;
		color.b = (colorNorte.b + colorSur.b + colorEste.b + colorOeste.b)/4.0;
	}
	return color;
}

int main(int argc, char** argv)
{

	///////////////////////////////////
	// Escanear argumentos de entrada
	///////////////////////////////////
	int opt;
	//Parsear los argumentos de entrada
	while ((opt = getopt(argc, argv, "i:o:")) != -1) // : significa que -i resive un argumento, se guarda en "optarg"
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
	/////////////////////////////////////////
	/////////////////////////////////////////

	int i, j;
	long double Xw, Yw, Zw, L, Xd ,Yd ,Zd;
	objetos = NULL;


	//////LUZ DE PRUEBA/////////////////////////////////////////////////////////////
	
	struct LUZ *luz1 = creaLuz();
	//posicion
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
	
	//luces = insertarLuz(luces, luz1);
		
	
	////////////////////////datos de prueba////////////////////////////////////////////
	//ventana max
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

	//iluminacion ambiente
	escena.Ia = 0.6;
	////////////////////////datos de prueba/////////////////////////////////////////////

	

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

	


	//iniciarOjectosP();

	//for(i = 454; i < 455; i++){
	//	for(j = 349; j < 350; j++){
	//for(i = 454; i < 455; i++){
	//	for(j = 375; j < 376; j++){
	//for(i = 499; i < 500; i++){
	//	for(j = 399; j < 400; j++){
	
	for(i = 0; i < Hres; i++){
		for(j = 0; j < Vres; j++){

			if(antialiasingActivo == 0){
				long double L;
				punto3D.Xw = ((i + 0.5)*(P.Xmax - P.Xmin) / Hres) + P.Xmin;
				punto3D.Yw = ((j + 0.5)*(P.Ymax - P.Ymin) / Vres) + P.Ymin;
				punto3D.Zw = 0.0;

				
				L = sqrt(pow((punto3D.Xw - ojo.Xw), 2)+pow((punto3D.Yw - ojo.Yw), 2)+pow((punto3D.Zw - ojo.Zw), 2));
				vDir.X = (punto3D.Xw - ojo.Xw) / L;
				vDir.Y = (punto3D.Yw - ojo.Yw) / L;
				vDir.Z = (punto3D.Zw - ojo.Zw) / L;
				

				framebuffer[i][Vres-j] = De_que_color(ojo,vDir,MAX_LEVELS);
			}else{
				framebuffer[i][Vres-j] = calcularPixel(i,j,anti,0,1,0,1);
			}
		}
	}

	//Generar la imagen y guardarla
	saveImage(framebuffer);


	return 0;
}
