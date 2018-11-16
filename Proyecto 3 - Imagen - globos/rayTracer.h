#define M_PI 3.14159265358979323846  


#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#define EPSILON 0.05
#define MAX_LEVELS 6
#define NIVELES_TRANSPARENCIA 4
#define anti 0
#define antialiasingActivo 0

///////////////////////////
///// ESTRUCTURAS
///////////////////////////
typedef struct {
  float Xmax;
  float Ymax;
  float Zmax;
  float Xmin;
  float Ymin;
  float Zmin;
} PANTALLA;

typedef struct {
  double r;
  double g;
  double b;
} COLOR;


typedef struct {
  long double Xw;
  long double Yw;
  long double Zw;
} PUNTO3D;

typedef struct {
  long double X;
  long double Y;
  long double Z;
} VECTOR;


typedef struct PLANO{
  int signo;
  long double A;
  long double B;
  long double C;
  long double D;
  struct PLANO * sig;
} PLANO;


typedef struct {
	struct OBJETO *objeto;
  	long double tmin;
  	PUNTO3D punto_interseccion;
}INTERSECCION;



typedef struct {
  long double Ia;
} ESCENA;



///////////////////////////
///// Variables Globales
///////////////////////////



unsigned Hres = 1008, Vres = 567;
COLOR BACKGROUND;
PUNTO3D ojo;
ESCENA escena;
struct OBJETO *objetos;
struct LUZ *luces;




///////////////////////////
///// Funciones PUBLICAS
///////////////////////////

struct OBJETO *creanodo(char * tipo);

struct OBJETO *insertafinal(struct OBJETO *lista, struct OBJETO *q);

struct VERTICE * append_vertice(struct VERTICE *lista, struct VERTICE * elemento);

struct LUZ *insertarLuz(struct LUZ *lista, struct LUZ *l);

PLANO *insertar_plano_corte( PLANO *lista,  PLANO *plano);

struct LUZ *creaLuz(void);

VECTOR producto_cruz(VECTOR v, VECTOR w);

long double ProductoPunto(VECTOR a, VECTOR b);

VECTOR normalizarVector(VECTOR V);

////////////