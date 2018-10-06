struct OBJETO {
	char* tipo;
  	COLOR color;
  	float Kd;
  	float Ka;
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

struct LUZ{
	COLOR color;
	float Ip;
	float C1;
	float C2;
	float C3;
	PUNTO3D pos;
	struct LUZ *sig;
};



