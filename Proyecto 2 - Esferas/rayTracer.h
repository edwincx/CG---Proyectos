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

struct OBJETO *creanodo(void);

struct OBJETO *insertafinal(struct OBJETO *lista, struct OBJETO *q);

struct VERTICE * append_vertice(struct VERTICE *lista, struct VERTICE * elemento);

struct LUZ *insertarLuz(struct LUZ *lista, struct LUZ *l);

struct LUZ *creaLuz(void);
