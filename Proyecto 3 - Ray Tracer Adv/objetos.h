

struct OBJETO {
	char* tipo;
  	COLOR color;
	///// Texturas
	char* tex_path;
	unsigned char* tex_buffer;
	unsigned int THres;
	unsigned int TVres;
	VECTOR U;		
	VECTOR V;		
	float u;
	float v;
	void (*map_texture)(INTERSECCION interseccion); //Puntero a funcion que calcula u y v para mapeo de texturas
  	///// Iluminacion
	float Kd;
  	float Ka;
  	float Kn;
  	float Ks;
  	void *p;
    double O1;
    double O2;
    double O3;
  	struct OBJETO *sig;
};


struct ESFERA{
  	PUNTO3D centro;
  	float radio;
	PLANO * planos_corte;
	VECTOR Norte; //vector para texturas 
};

struct CILINDRO{
	PUNTO3D centro;
	float radio;
	VECTOR Q; //vector de Dirección
	VECTOR G; //vector para texturas 
	double d1;
	double d2;
	PLANO * planos_corte;
};

struct CONO{
	PUNTO3D centro;
	VECTOR Q; //vector de Dirección
	double d1;
	double d2;
	double k1;
	double k2;
	PLANO * planos_corte;
};

struct VERTICE{
  long double X;
  long double Y;
  long double Z;
  struct VERTICE *sig;
};

struct POLIGONO{
	float A;
	float B;
	float C;
	float D;
	struct VERTICE * vertices_list;
	struct VERTICE *  vertices_list_2D;
	PUNTO3D centro;
	float escala;
	int vertices_num;
	VECTOR normal;
	PLANO * planos_corte;
};

struct DISCO{
	PUNTO3D centro;
	PUNTO3D d1;
	PUNTO3D d2;
  	float radio;
	float anillo;
	float A;
	float B;
	float C;
	float D;
	VECTOR normal;
	PLANO * planos_corte;
};

struct ELIPSE{
	float anillo;
	float k; 
	float A;
	float B;
	float C;
	float D;
	PUNTO3D f1;//foco 1 equidistante al centro, entre mas cerca al radio es mas redondo
	PUNTO3D f2;//foco 2 equidistante al centro, entre mas cerca al radio es mas redondo
	PUNTO3D d; //auxiliar
	//float excentricidad;//epsilon valor constante entre [0,1]
	VECTOR normal;
	PLANO * planos_corte;
};


struct LUZ{
	COLOR color;
	float Ip;
	float C1;
	float C2;
	float C3;
	PUNTO3D pos;
	struct LUZ *sig;
};

struct INTERSECCIONES
{
  struct OBJETO *objeto;
  long double tmin;
  PUNTO3D punto_interseccion;
  struct INTERSECCIONES *sig;
};


//////////////////////////////////////////////
////		Funciones de Objetos
/////////////////////////////////////////////


void map_texture_poligono(INTERSECCION interseccion)
{
	
	struct OBJETO * objeto;
	struct POLIGONO * poligono;
	struct VERTICE *  vertices;
	VECTOR U, V, A;
	long double Xmin, Ymin, Xmax, Ymax, H, L, d, e, u, v;
	Xmin = Ymin = 9999999999.0;
	Xmax = Ymax = 0.0;
	

	objeto = interseccion.objeto;
	poligono =  objeto->p;
	/////Recorer los vertices para encontrar los extremos y formar un rectangulo imaginario
	struct VERTICE * vertice = poligono->vertices_list_2D;
	while(vertice != NULL){
		Xmin = min(Xmin, vertice->X);
		Ymin = min(Ymin, vertice->Y);
		Xmax = max(Xmax, vertice->X);
		Ymax = max(Ymax, vertice->Y);
		vertice = vertice->sig;
	}
	
	H = sqrt( pow(Xmax-Xmin,2) + pow(Ymin-Ymin,2));
	L = sqrt( pow(Xmin-Xmin,2) + pow(Ymax-Ymin,2));
	
	U.X = (Xmax-Xmin)/H;
	U.Y = (Ymin-Ymin)/H;
	U.Z = 0;
	
	V.X = (Xmin - Xmin)/L ;
	V.Y = (Ymax - Ymin)/L;
	V.Z = 0;
	
	//Calcular Punto de Interseccion!
	PUNTO3D punto_interseccion = interseccion.punto_interseccion;

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
	//printf("d:%Lf, e:%Lf\n", punto_interseccion_2D.Xw,punto_interseccion_2D.Yw );

	//Vector de ezquina minima a punto de interseccion
	A.X = (punto_interseccion_2D.Xw - Xmin) ;
	A.Y = (punto_interseccion_2D.Yw - Ymin);
	A.Z = 0;
	
	d = ProductoPunto(A,U);
	e = ProductoPunto(A,V);

	u = d/H;
	v = e/L;

	//printf("Xmin:%Lf, Ymin:%Lf, Xmaz:%Lf, Ymax:%Lf\n",Xmin, Ymin, Xmax, Ymax);
	objeto->u = u;
	objeto->v = v;

	return;
}

void map_texture_cilindro(INTERSECCION interseccion) 
{
	struct OBJETO * objeto;
	struct CILINDRO * cilindro;
	long double  L, d, e, u, v;
	VECTOR  A, N;
	objeto = interseccion.objeto;
	cilindro = objeto->p;
	
	
	//Calcular v
	L = cilindro->d2 - cilindro->d1;
	PUNTO3D punto_interseccion = interseccion.punto_interseccion;
	PUNTO3D ancla = cilindro->centro ;
	//Vector de ancla a punto de interseccion
	A.X = (punto_interseccion.Xw - ancla.Xw);
	A.Y = (punto_interseccion.Yw - ancla.Yw);
	A.Z = (punto_interseccion.Zw - ancla.Zw);
	e = ProductoPunto(A,cilindro->Q);
	u = e/L;
	//printf("v:%Lf\n", v);
	

	//Calcular u
	//Punto en el eje
	PUNTO3D punto_eje;
	punto_eje.Xw =  ancla.Xw + e * cilindro->Q.X;
	punto_eje.Yw =  ancla.Yw + e * cilindro->Q.Y;
	punto_eje.Zw =  ancla.Zw + e * cilindro->Q.Z;

	N.X = punto_interseccion.Xw - punto_eje.Xw;
	N.Y = punto_interseccion.Yw - punto_eje.Yw;
	N.Z = punto_interseccion.Zw - punto_eje.Zw;
	N = normalizarVector(N);

	//printf("N:(%Lf, %Lf, %Lf)\n", N.X, N.Y, N.Z);
	//Si no se ha calculado G, G = N como valor arbitrario
	if(cilindro->G.X == 0.0 && cilindro->G.Y == 0.0 && 	cilindro->G.Z == 0.0)
	{
		cilindro->G.X = N.X;
		cilindro->G.Y = N.Y;
		cilindro->G.Z = N.Z;
	}
	
	d = ProductoPunto(N,cilindro->G);
	v = acos(d)/2*M_PI;

	objeto->u = min(1.0,u);//u;
	objeto->v = min(1.0, v);

	return;
	
}



void map_texture_esfera(INTERSECCION interseccion) 
{
	struct OBJETO * objeto;
	struct ESFERA * esfera;
	long double  L, d, e, u, v;
	VECTOR  A, N;
	PUNTO3D polo_Norte, polo_Sur;
	objeto = interseccion.objeto;
	esfera = objeto->p;
	
	//Calcular u
	PUNTO3D punto_interseccion = interseccion.punto_interseccion;

	polo_Sur.Xw = esfera->centro.Xw - (esfera->radio * esfera->Norte.X);
	polo_Sur.Yw = esfera->centro.Yw - (esfera->radio * esfera->Norte.Y);;
	polo_Sur.Zw = esfera->centro.Zw - (esfera->radio * esfera->Norte.Z);;
	
	A.X = punto_interseccion.Xw - polo_Sur.Xw;
	A.Y = punto_interseccion.Yw - polo_Sur.Yw;
	A.Z = punto_interseccion.Zw - polo_Sur.Zw;
	
	d = ProductoPunto(esfera->Norte, A);
	v = d/(2.0 * esfera->radio);

	
	objeto->u = 0.2; //min(1.0,u);//u;
	objeto->v = min(1.0, v);

	return;
}







