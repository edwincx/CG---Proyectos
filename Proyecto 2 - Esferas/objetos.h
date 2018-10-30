struct OBJETO {
	char* tipo;
  	COLOR color;
  	float Kd;
  	float Ka;
  	float Kn;
  	float Ks;
  	void *p;
  	struct OBJETO *sig;
};

struct ESFERA{
  PUNTO3D centro;
  float radio;
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
	int vertices_num;
	VECTOR normal;
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



