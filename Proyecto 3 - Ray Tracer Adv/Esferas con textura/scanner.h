#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>




char *  input_file_name = "input_file.txt";
int token_buffer_index =0;			//indice para el buffer
char token_buffer[256];     

///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////



void generar_estrell_poligono(float x_sum, float y_sum, float escala, float color_vector[3])
{

	///////// Aca crear objeto con estructura Poligono!!!!!
	struct OBJETO *ptr1;
	struct POLIGONO *poligono;
	poligono = (struct POLIGONO*) malloc(sizeof(struct POLIGONO));

	//Inicializar todos los datos de Poligono!
	poligono->vertices_list = NULL;
	poligono->vertices_list_2D = NULL;
	VECTOR vector_vacio;
	vector_vacio.X = 0;
	vector_vacio.Y = 0;
	vector_vacio.Z = 0;
	poligono->normal = vector_vacio;
	poligono->vertices_num= 0;

	//Enlazar Objeto y Poligono
  	ptr1 = creanodo("poligono");		
  	ptr1->p = poligono;
  	objetos = insertafinal(objetos, ptr1);				
  	ptr1->tipo = "poligono";

	poligono->A = 0;
	poligono->B = 0;
	poligono->C = 1;
	poligono->D = 0;

	//Color
	ptr1->color.r = color_vector[0];
  	ptr1->color.g = color_vector[1];
  	ptr1->color.b = color_vector[2];
	//Ka
	ptr1->Ka = 1;
	//kd
	ptr1->Kd = 1;
	//ks
	ptr1->Ks = 0;
	//kn
	ptr1->Kn = 1;


	float vertices_estrella[][2]={{6,9},{0,24},{-6,9},{-26,10},{-11,0},{-20,-20},{0,-7},{19,-19},{10,0},{25,9}};

	int a;
	for(a=0; a <10;a++)
	{
		float vert_x = x_sum + vertices_estrella[a][0]* escala;
		float vert_y = y_sum + vertices_estrella[a][1]* escala;
		struct VERTICE *p_vertice;
		p_vertice = (struct VERTICE*) malloc(sizeof(struct VERTICE));				
		p_vertice->X = vert_x;
	  	p_vertice->Y = vert_y;
	  	p_vertice->Z = 0;
	
	
		if (poligono->vertices_list == NULL){
			poligono->vertices_list = append_vertice(poligono->vertices_list, p_vertice);
	  	}
		else append_vertice(poligono->vertices_list, p_vertice);
	
		poligono->vertices_num++;

		// Vertices Aplastados 2D
		struct VERTICE *p_vertice_2D;
		p_vertice_2D = (struct VERTICE*) malloc(sizeof(struct VERTICE));
		p_vertice_2D->X = vert_x;
		p_vertice_2D->Y = vert_y;
		p_vertice_2D->Z = 0;
		if (poligono->vertices_list_2D == NULL){
		poligono->vertices_list_2D = append_vertice(poligono->vertices_list_2D, p_vertice_2D);
	  	}
		else append_vertice(poligono->vertices_list_2D, p_vertice_2D);
	}

}

void generar_estrellas(int cantidad, float profundidad)
{
	/* initialize random seed: */
    	srand ( time(NULL) );
	
	int i;  	 

	for(i=0;i<cantidad;i++)
	{
		///////// Aca crear objeto con estructura esfera!!!!!
		struct OBJETO *ptr1;
		struct ESFERA *e1;
		e1 = (struct ESFERA*) malloc(sizeof(struct ESFERA));
	  	ptr1 = creanodo("esfera");		
	  	ptr1->p = e1;
	  	objetos = insertafinal(objetos, ptr1);				
	  	ptr1->tipo = "esfera";
		//Radio	
		e1->radio = 30;
		//Centro
		e1->centro.Xw = (rand() % (Vres)*8 )-100;
	  	e1->centro.Yw = (rand() % (Hres)*8 )-100;
	  	e1->centro.Zw = profundidad;
		//Color
		ptr1->color.r = 1;
	  	ptr1->color.g = 1;
	  	ptr1->color.b = 1;
		//Ka
		ptr1->Ka = 1;
		//kd
		ptr1->Kd = 1;
		//ks
		ptr1->Ks = 0;
		//kn
		ptr1->Kn = 1;
	
	}

}

///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


int check_file_exist(char *file_name)
{
  	struct stat   buffer;  
	
	if(stat(file_name, &buffer) < 0)
	{
		printf("\nError en archivo de entrada, imposble abrir archivo \"%s\" o no existe!!!\n\n",file_name );
		return -1;
	}

  	return 1;
}


int parse_vector(int vector_length, char* vector_string, int string_length, float* output_vector)
{
	int i, j, vector_index = 0;
	char  temp_value[20]; 
	
	if(vector_string[0] != '(') return -1;	//Si no empieza con '(' rechazar
	for(i=1; i < string_length; i++  )
	{
		if(isspace(vector_string[i])) continue; 		//si es un espacio continuar
		if(vector_string[i] == '.' || vector_string[i] == ',') return -1; //Si el numero empieza con ',' o '. rechazar
		//Mientras sea el mismo numero
		memset(temp_value, 0, sizeof temp_value);
		for(j=0; i < string_length && vector_string[i] != ',' && vector_string[i] != ')' && !isspace(vector_string[i]); j++  )
		{
			temp_value[j] = vector_string[i];
			i++;
		}
		if(vector_index >= vector_length ) return -1;		//Si el vector mide mas de 3
		output_vector[vector_index] = atof(temp_value);
		vector_index++;
	}
	if (vector_index == vector_length) return 0; //Si el vector esta completo 
	else return -1;	//Si esta incompleto
}




void clear_buffer(){					//funcion que limpia el buffer
	memset(token_buffer, 0, sizeof token_buffer);
	token_buffer_index=0;
}

void buffer_char(int in_char){			//funcion que agrega un caracter al buffer en la posicion del index	
	token_buffer[token_buffer_index]=in_char;
	token_buffer_index+=1;
}


int scanner(FILE *inFile){
	int in_char, c;		// variable quie apunta el caracter actual
	
	
	clear_buffer();	
	
	if (feof(inFile)){
	    //printf("EOF\n");
		return -1;
	}
	while (in_char = getc(inFile)){
		clear_buffer();

	    	if(in_char == EOF) return 0;	//Se termino el archivo sin errores

		else if(isspace(in_char)){					//si es un espacio continuar
			continue; /* do nothing */
		}
		
		else if(in_char == '/'){				//si es un comentario ignorarlo			
			in_char = getc(inFile);
			if(in_char == '/'){	
				while ((in_char = getc(inFile)) != '\n'){
					continue; /* do nothing */		
				}
			}
			else if(in_char=='*'){
				while ((in_char = getc(inFile)) != EOF){					
					if(in_char=='*'){
						if ((in_char = getc(inFile)) == '/'){
							break;
						}
					}
				}
			}
			
		}
		else if(in_char == '#'){
			
			buffer_char(in_char);
			for (c = getc(inFile); isalpha(c) || c == '_' || c == '-' ; c = getc(inFile)){
				buffer_char(c);
			}
			//printf("\nObjeto: %s\n", token_buffer);
			ungetc(c, inFile);
			
			//////////////////////////////////////
			/////// Resolucion
			///////////////////////////////////////
			if((strcmp (token_buffer,"#resolucion"))==0){ 
				clear_buffer();				

				while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF)return 0; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);

					//X
					if((strcmp (token_buffer,"X:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float res_X = atoi(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						Hres = res_X;
					}
					//Y
					else if((strcmp (token_buffer,"Y:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float res_Y = atoi(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						Vres = res_Y;
					}					
					else
					{
						//printf("Error en archivo, propiedad \"%s\" de esfera no identificada!!!\n",token_buffer );
						return -1;
					}
				}
				ungetc(c, inFile);
				
			}



			//////////////////////////////////////
			/////// GENERADOR
			///////////////////////////////////////
			if((strcmp (token_buffer,"#generador"))==0){ 
				clear_buffer();				
				int cantidad=0;
				float profundidad=0;
				while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF) break; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);

					//X
					if((strcmp (token_buffer,"cantidad:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						cantidad = atoi(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);						
					}
					//Y
					else if((strcmp (token_buffer,"profundidad:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						profundidad = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
					}					
					else
					{
					printf("Error en archivo, propiedad \"%s\" de esfera no identificada!!!\n",token_buffer );
						return -1;
					}
				}

				generar_estrellas(cantidad, profundidad);

				//ungetc(c, inFile);
				return 0;
				
			}


			//////////////////////////////////////
			/////// ESTRTELLA
			///////////////////////////////////////
			if((strcmp (token_buffer,"#estrella"))==0){ 
				clear_buffer();			
	
				float x_sum;
				float y_sum;
				float escala;
				float color_vector[3];
				
				
				while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF) break; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);

					//X
					if((strcmp (token_buffer,"x:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						x_sum = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);						
					}
					//Y
					else if((strcmp (token_buffer,"y:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						y_sum = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
					}
					//Y
					else if((strcmp (token_buffer,"escala:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						escala = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
					}

					//Color
					else if((strcmp (token_buffer,"color:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						if( parse_vector(3,token_buffer, token_buffer_index, color_vector) !=0) return -1;
					
						
						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}					
					else
					{
					printf("Error en archivo, propiedad \"%s\" de esfera no identificada!!!\n",token_buffer );
						return -1;
					}
				}

				generar_estrell_poligono( x_sum,  y_sum,  escala,  color_vector);
				
				//ungetc(c, inFile);
				return 0;
				
			}

			//////////////////////////////////////
			/////// BACKGROUND
			///////////////////////////////////////
			if((strcmp (token_buffer,"#escena"))==0){ 
				clear_buffer();				

				while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF)return 0; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);

					//Color
					if((strcmp (token_buffer,"background:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						

						//background-color
						BACKGROUND.r = temp_vector[0];
						BACKGROUND.g = temp_vector[1];
						BACKGROUND.b = temp_vector[2];


						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}
					else if((strcmp (token_buffer,"ojo:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						ojo.Xw = temp_vector[0];
						ojo.Yw = temp_vector[1];
						ojo.Zw = temp_vector[2];

						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}
					//iluminacion ambiente
					else if((strcmp (token_buffer,"ia:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ia_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						escena.Ia = ia_temp;
					}

									
					else
					{
						//printf("Error en archivo, propiedad \"%s\" de esfera no identificada!!!\n",token_buffer );
						return -1;
					}
				}
				ungetc(c, inFile);				
			}

			/////////////////////////////////
			//	LUZ
			////////////////////////////////			
			else if((strcmp (token_buffer,"#luz"))==0){ 
				clear_buffer();

				///////// Aca crear objeto con estructura LUZ!!!!!
				struct LUZ *luz1 = creaLuz();
				luces = insertarLuz(luces, luz1);

				while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF)return 0; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);
	
					//Radio					
					//Centro
					if((strcmp (token_buffer,"posicion:"))==0){
						if( scanner(inFile) != 0 ) return -1;						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
								
						//posicion
						luz1->pos.Xw = temp_vector[0];
						luz1->pos.Yw = temp_vector[1];
						luz1->pos.Zw = temp_vector[2];
						//printf("Centro: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);

					}
					//Color
					else if((strcmp (token_buffer,"color:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
					
						//color
						luz1->color.r = temp_vector[0];
						luz1->color.g = temp_vector[1];
						luz1->color.b = temp_vector[2];
						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}
					
					//Ip
					else if((strcmp (token_buffer,"ip:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ip_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						//intensidad
						luz1->Ip = ip_temp;
					}
					//C1
					else if((strcmp (token_buffer,"c1:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float c1_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						luz1->C1 = c1_temp;						
					}
					//C2
					else if((strcmp (token_buffer,"c2:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float c2_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						luz1->C2 = c2_temp;						
					}
					//C3
					else if((strcmp (token_buffer,"c3:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float c3_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						luz1->C3 = c3_temp;
					}
					else
					{
						//printf("Error en archivo, propiedad \"%s\" de esfera no identificada!!!\n",token_buffer );
						return -1;
					}
				}
				ungetc(c, inFile);
				
			}


			///////////////////////////////
			//	Esfera
			////////////////////////////////			
			else if((strcmp (token_buffer,"#esfera"))==0){ 
				clear_buffer();

				///////// Aca crear objeto con estructura esfera!!!!!
				struct OBJETO *ptr1;
				struct ESFERA *e1;
				e1 = (struct ESFERA*) malloc(sizeof(struct ESFERA));
			  	ptr1 = creanodo("esfera");		
			  	ptr1->p = e1;
			  	objetos = insertafinal(objetos, ptr1);				
			  	ptr1->tipo = "esfera";
			  	ptr1->tex_buffer = NULL;
				e1->planos_corte = NULL;
				while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF)return 0; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);
	
					//Radio
					if((strcmp (token_buffer,"radio:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float radio_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						e1->radio = radio_temp;
					}
					//Centro
					else if((strcmp (token_buffer,"centro:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						e1->centro.Xw = temp_vector[0];
					  	e1->centro.Yw = temp_vector[1];
					  	e1->centro.Zw = temp_vector[2];

						//printf("Centro: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);

					}
					//Color
					else if((strcmp (token_buffer,"color:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						ptr1->color.r = temp_vector[0];
					  	ptr1->color.g = temp_vector[1];
					  	ptr1->color.b = temp_vector[2];

						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}

					//Textura: Cargar la textura 
					else if((strcmp (token_buffer,"textura:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						ptr1->tex_path = token_buffer;
						if(check_file_exist(ptr1->tex_path))
						{	
							unsigned char* image;
							unsigned width, height;
							unsigned error = lodepng_decode32_file(&image, &width, &height, ptr1->tex_path);
							if(error) printf("error %u: %s\n", error, lodepng_error_text(error));							
							ptr1->tex_buffer = image;
							ptr1->THres = width;
							ptr1->TVres = height;													
						}
					}	
					//Norte Vector
					else if((strcmp (token_buffer,"norte:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						e1->Norte.X = temp_vector[0];
					  	e1->Norte.Y = temp_vector[1];
					  	e1->Norte.Z = temp_vector[2];
					}
					//Greenwich
					else if((strcmp (token_buffer,"greenwich:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						e1->Norte.X = temp_vector[0];
					  	e1->Norte.Y = temp_vector[1];
					  	e1->Norte.Z = temp_vector[2];
					}
					//ka
					else if((strcmp (token_buffer,"ka:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ka_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ka = ka_temp;
					}
					//ka
					else if((strcmp (token_buffer,"kd:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kd_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kd = kd_temp;
					}
					//kn
					else if((strcmp (token_buffer,"kn:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kn_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kn = kn_temp;
					}
					//ks
					else if((strcmp (token_buffer,"ks:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ks_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ks = ks_temp;
					}
					//O1
					else if((strcmp (token_buffer,"O1:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O1_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O1 = O1_temp;
					}
					//O2
					else if((strcmp (token_buffer,"O2:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O2_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O2 = O2_temp;
					}
					//O3
					else if((strcmp (token_buffer,"O3:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O3_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O3 = O3_temp;
					}
					//Plano de Corte Positivo y Negativo
					else if( (strcmp (token_buffer,"clip:") ==0) || (strcmp (token_buffer,"clip_p:") ==0) || (strcmp (token_buffer,"clip_n:") ==0))
					{
						int signo_p = 1;
						if ((strcmp (token_buffer,"clip_n:") ==0)) signo_p = -1;
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[4];
						if( parse_vector(4,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						//Plano nuewo
						PLANO * new_plano = (PLANO*) malloc(sizeof(PLANO));
						
						new_plano->signo = signo_p;						
						new_plano->A = temp_vector[0];
						new_plano->B = temp_vector[1];
						new_plano->C = temp_vector[2];
						new_plano->D = temp_vector[3];
						
						e1->planos_corte = insertar_plano_corte(e1->planos_corte , new_plano);		
					}		 
					else
					{
						//printf("Error en archivo, propiedad \"%s\" de esfera no identificada!!!\n",token_buffer );
						return -1;
					}
				}
				ungetc(c, inFile);
				
			}		

			
			///////////////////////////////
			//	DISCO
			////////////////////////////////			
			else if((strcmp (token_buffer,"#disco"))==0){ 
				clear_buffer();

				///////// Aca crear objeto con estructura esfera!!!!!
				struct OBJETO *ptr1;
				struct DISCO *e1;
				e1 = (struct DISCO*) malloc(sizeof(struct DISCO));
			  	ptr1 = creanodo("disco");		
			  	ptr1->p = e1;
			  	objetos = insertafinal(objetos, ptr1);				
			  	ptr1->tipo = "disco";
				e1->planos_corte = NULL;
				//Inicializar Normal vacia
				VECTOR vector_vacio;
				vector_vacio.X = 0;
				vector_vacio.Y = 0;
				vector_vacio.Z = 0;
				e1->normal = vector_vacio;

				while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF) break;//return 0; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);
	
					//Radio
					if((strcmp (token_buffer,"radio:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float radio_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						e1->radio = radio_temp;
					}
					//Radio
					else if((strcmp (token_buffer,"anillo:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float a_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						e1->anillo = a_temp;
					}
					//Centro
					else if((strcmp (token_buffer,"centro:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						e1->centro.Xw = temp_vector[0];
					  	e1->centro.Yw = temp_vector[1];
					  	e1->centro.Zw = temp_vector[2];

						//printf("Centro: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);

					}
					//D1
					else if((strcmp (token_buffer,"d1:"))==0){
						if( scanner(inFile) != 0 ) return -1;						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;						
						e1->d1.Xw = temp_vector[0];
					  	e1->d1.Yw = temp_vector[1];
					  	e1->d1.Zw = temp_vector[2];
					}
					//D2
					else if((strcmp (token_buffer,"d2:"))==0){
						if( scanner(inFile) != 0 ) return -1;						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;						
						e1->d2.Xw = temp_vector[0];
					  	e1->d2.Yw = temp_vector[1];
					  	e1->d2.Zw = temp_vector[2];
					}										
					
					//Color
					else if((strcmp (token_buffer,"color:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						ptr1->color.r = temp_vector[0];
					  	ptr1->color.g = temp_vector[1];
					  	ptr1->color.b = temp_vector[2];

						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}
					
					//A
					else if((strcmp (token_buffer,"a:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						e1->A = temp;
					}
					else if((strcmp (token_buffer,"b:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						e1->B = temp;
					}	
					else if((strcmp (token_buffer,"c:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						e1->C = temp;
					}
					else if((strcmp (token_buffer,"d:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						e1->D = temp;
					}
					
					//ka
					else if((strcmp (token_buffer,"ka:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ka_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ka = ka_temp;
					}
					//ka
					else if((strcmp (token_buffer,"kd:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kd_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kd = kd_temp;
					}
					//kn
					else if((strcmp (token_buffer,"kn:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kn_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kn = kn_temp;
					}
					//ks
					else if((strcmp (token_buffer,"ks:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ks_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ks = ks_temp;
					}

					//O1
					else if((strcmp (token_buffer,"O1:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O1_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O1 = O1_temp;
					}
					//O2
					else if((strcmp (token_buffer,"O2:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O2_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O2 = O2_temp;
					}
					//O3
					else if((strcmp (token_buffer,"O3:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O3_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O3 = O3_temp;
					}
					//Plano de Corte Positivo y Negativo
					else if( (strcmp (token_buffer,"clip:") ==0) || (strcmp (token_buffer,"clip_p:") ==0) || (strcmp (token_buffer,"clip_n:") ==0))
					{
						int signo_p = 1;
						if ((strcmp (token_buffer,"clip_n:") ==0)) signo_p = -1;
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[4];
						if( parse_vector(4,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						//Plano nuewo
						PLANO * new_plano = (PLANO*) malloc(sizeof(PLANO));
						
						new_plano->signo = signo_p;						
						new_plano->A = temp_vector[0];
						new_plano->B = temp_vector[1];
						new_plano->C = temp_vector[2];
						new_plano->D = temp_vector[3];
						
						e1->planos_corte = insertar_plano_corte(e1->planos_corte , new_plano);		
					}		 
					
					else
					{
						printf("Error en archivo, propiedad \"%s\" de disco no identificada!!!\n",token_buffer );
						return -1;
					}
				}
				
				PUNTO3D d1, d2, centro;
				d1 = e1->d1;
				d2 = e1->d2;
				centro = e1->centro;
				
				VECTOR v,w, perpendicular;
				// v: Centro -> D1
				v.X = d1.Xw - centro.Xw;
				v.Y = d1.Yw - centro.Yw;
				v.Z = d1.Zw - centro.Zw;
				// w: Centro -> D2
				w.X = d2.Xw - centro.Xw;
				w.Y = d2.Yw - centro.Yw;
				w.Z = d2.Zw - centro.Zw;
				
				perpendicular = producto_cruz( v,  w);
				e1->normal = perpendicular;

				e1->A = perpendicular.X;
				e1->B = perpendicular.Y;
				e1->C = perpendicular.Z;
				e1->D = -(e1->A*centro.Xw + e1->B*centro.Yw  + e1->C*centro.Zw  );	
			
				//printf("A:%f, B:%f, C:%f, D:%f\n",e1->A,e1->B,e1->C,e1->D);
				
				ungetc(c, inFile);
				
			}		


			
			
			////////////////////////////////////////
			////// Cilindro
			///////////////////////////////////////
			else if((strcmp (token_buffer,"#cilindro"))==0){ 
				clear_buffer();

				///////// Aca crear objeto con estructura Cilindro!!!!!
				struct OBJETO *ptr1;
				struct CILINDRO *c1;
				c1 = (struct CILINDRO*) malloc(sizeof(struct CILINDRO));
			  	ptr1 = creanodo("cilindro");		
			  	ptr1->p = c1;
			  	objetos = insertafinal(objetos, ptr1);				
			  	ptr1->tipo = "cilindro";
				c1->planos_corte = NULL;
				
				VECTOR vector_vacio;
				vector_vacio.X = 0;
				vector_vacio.Y = 0;
				vector_vacio.Z = 0;
				c1->G = vector_vacio;
				c1->d1 = 0;
				
			  	while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF)return 0; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);
	
					//Radio
					if((strcmp (token_buffer,"radio:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float radio_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						c1->radio = radio_temp;
					}
					//Centro
					else if((strcmp (token_buffer,"centro:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						c1->centro.Xw = temp_vector[0];
					  	c1->centro.Yw = temp_vector[1];
					  	c1->centro.Zw = temp_vector[2];

						//printf("Centro: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);

					}
					else if((strcmp (token_buffer,"q:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						c1->Q.X = temp_vector[0];
					  	c1->Q.Y = temp_vector[1];
					  	c1->Q.Z = temp_vector[2];

						//printf("Centro: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);

					}
					//Color
					else if((strcmp (token_buffer,"color:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						ptr1->color.r = temp_vector[0];
					  	ptr1->color.g = temp_vector[1];
					  	ptr1->color.b = temp_vector[2];

						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}

					//Textura: Cargar la textura 
					else if((strcmp (token_buffer,"textura:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						ptr1->tex_path = token_buffer;
						if(check_file_exist(ptr1->tex_path))
						{	
							unsigned char* image;
							unsigned width, height;
							unsigned error = lodepng_decode32_file(&image, &width, &height, ptr1->tex_path);
							if(error) printf("error %u: %s\n", error, lodepng_error_text(error));							
							ptr1->tex_buffer = image;
							ptr1->THres = width;
							ptr1->TVres = height;													
						}
					}		
				
					//ka
					else if((strcmp (token_buffer,"ka:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ka_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ka = ka_temp;
					}
					//ka
					else if((strcmp (token_buffer,"kd:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kd_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kd = kd_temp;
					}
					//kn
					else if((strcmp (token_buffer,"kn:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kn_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kn = kn_temp;
					}
					//ks
					else if((strcmp (token_buffer,"ks:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ks_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ks = ks_temp;
					}
					//d1
					else if((strcmp (token_buffer,"d1:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float d1_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						c1->d1 = 0;	// d1_temp;
					}
					//d2
					else if((strcmp (token_buffer,"d2:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float d2_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						c1->d2 = d2_temp;
					}
					
					//O1
					else if((strcmp (token_buffer,"O1:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O1_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O1 = O1_temp;
					}
					//O2
					else if((strcmp (token_buffer,"O2:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O2_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O2 = O2_temp;
					}
					//O3
					else if((strcmp (token_buffer,"O3:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O3_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O3 = O3_temp;
					}
					//Plano de Corte Positivo y Negativo
					else if( (strcmp (token_buffer,"clip:") ==0) || (strcmp (token_buffer,"clip_p:") ==0) || (strcmp (token_buffer,"clip_n:") ==0))
					{
						int signo_p = 1;
						if ((strcmp (token_buffer,"clip_n:") ==0)) signo_p = -1;
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[4];
						if( parse_vector(4,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						//Plano nuewo
						PLANO * new_plano = (PLANO*) malloc(sizeof(PLANO));
						
						new_plano->signo = signo_p;						
						new_plano->A = temp_vector[0];
						new_plano->B = temp_vector[1];
						new_plano->C = temp_vector[2];
						new_plano->D = temp_vector[3];
						
						c1->planos_corte = insertar_plano_corte(c1->planos_corte , new_plano);		
					}
					else
					{
						//printf("Error en archivo, propiedad \"%s\" de esfera no identificada!!!\n",token_buffer );
						return -1;
					}
				}


				ungetc(c, inFile);
			}

			
			////////////////////////////////////////
			////// Conos
			///////////////////////////////////////
			else if((strcmp (token_buffer,"#cono"))==0){ 
				clear_buffer();

				///////// Aca crear objeto con estructura Cilindro!!!!!
				struct OBJETO *ptr1;
				struct CONO *c1;
				c1 = (struct CONO*) malloc(sizeof(struct CONO));
			  	ptr1 = creanodo("cono");		
			  	ptr1->p = c1;
			  	objetos = insertafinal(objetos, ptr1);				
			  	ptr1->tipo = "cono";
				c1->planos_corte = NULL;
			  	while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF)return 0; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);
					//Centro
					if((strcmp (token_buffer,"centro:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						c1->centro.Xw = temp_vector[0];
					  	c1->centro.Yw = temp_vector[1];
					  	c1->centro.Zw = temp_vector[2];

						//printf("Centro: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);

					}
					else if((strcmp (token_buffer,"q:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						c1->Q.X = temp_vector[0];
					  	c1->Q.Y = temp_vector[1];
					  	c1->Q.Z = temp_vector[2];

						//printf("Centro: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);

					}
					//Color
					else if((strcmp (token_buffer,"color:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						ptr1->color.r = temp_vector[0];
					  	ptr1->color.g = temp_vector[1];
					  	ptr1->color.b = temp_vector[2];

						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}

					//Textura: Cargar la textura 
					else if((strcmp (token_buffer,"textura:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						//Copiar direccion de la textura
						//strcpy(ptr1->tex_path, token_buffer);
						//printf("\nArchivo: %s\n",token_buffer );
						ptr1->tex_path = token_buffer;
						if(check_file_exist(ptr1->tex_path))
						{	
							unsigned char* image;
							unsigned width, height;
							unsigned error = lodepng_decode32_file(&image, &width, &height, ptr1->tex_path);
							if(error) printf("error %u: %s\n", error, lodepng_error_text(error));							
							ptr1->tex_buffer = image;
							ptr1->THres = width;
							ptr1->TVres = height;													
						}
					}	
					
					//ka
					else if((strcmp (token_buffer,"ka:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ka_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ka = ka_temp;
					}
					//ka
					else if((strcmp (token_buffer,"kd:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kd_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kd = kd_temp;
					}
					//kn
					else if((strcmp (token_buffer,"kn:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kn_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kn = kn_temp;
					}
					//ks
					else if((strcmp (token_buffer,"ks:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ks_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ks = ks_temp;
					}
					//d1
					else if((strcmp (token_buffer,"d1:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float d1_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						c1->d1 = d1_temp;
					}
					//d2
					else if((strcmp (token_buffer,"d2:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float d2_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						c1->d2 = d2_temp;
					}
					//k1
					else if((strcmp (token_buffer,"k1:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float k1_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						c1->k1 = k1_temp;
					}
					//m2
					else if((strcmp (token_buffer,"k2:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float k2_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						c1->k2 = k2_temp;
					}

					//O1
					else if((strcmp (token_buffer,"O1:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O1_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O1 = O1_temp;
					}
					//O2
					else if((strcmp (token_buffer,"O2:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O2_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O2 = O2_temp;
					}
					//O3
					else if((strcmp (token_buffer,"O3:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O3_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O3 = O3_temp;
					}
					//Plano de Corte Positivo y Negativo
					else if( (strcmp (token_buffer,"clip:") ==0) || (strcmp (token_buffer,"clip_p:") ==0) || (strcmp (token_buffer,"clip_n:") ==0))
					{
						int signo_p = 1;
						if ((strcmp (token_buffer,"clip_n:") ==0)) signo_p = -1;
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[4];
						if( parse_vector(4,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						//Plano nuewo
						PLANO * new_plano = (PLANO*) malloc(sizeof(PLANO));
						
						new_plano->signo = signo_p;						
						new_plano->A = temp_vector[0];
						new_plano->B = temp_vector[1];
						new_plano->C = temp_vector[2];
						new_plano->D = temp_vector[3];
						
						c1->planos_corte = insertar_plano_corte(c1->planos_corte , new_plano);		
					}
					else
					{
						//printf("Error en archivo, propiedad \"%s\" de esfera no identificada!!!\n",token_buffer );
						return -1;
					}
				}


				ungetc(c, inFile);
			}




			//////////////////////////////
			/// POLIGONOS
			//////////////////////////////
			else if((strcmp (token_buffer,"#poligono"))==0){ 
				clear_buffer();

				///////// Aca crear objeto con estructura Poligono!!!!!
				struct OBJETO *ptr1;
				struct POLIGONO *poligono;
				poligono = (struct POLIGONO*) malloc(sizeof(struct POLIGONO));

				//Inicializar todos los datos de Poligono!
				poligono->vertices_list = NULL;
				poligono->vertices_list_2D = NULL;
				poligono->planos_corte = NULL;

				VECTOR vector_vacio;
				vector_vacio.X = 0;
				vector_vacio.Y = 0;
				vector_vacio.Z = 0;
				poligono->normal = vector_vacio;
				poligono->vertices_num= 0;

				//Enlazar Objeto y Poligono
			  	ptr1 = creanodo("poligono");		
			  	ptr1->p = poligono;
			  	objetos = insertafinal(objetos, ptr1);				
			  	ptr1->tipo = "poligono";
				

			  	while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF) break; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);
	
					//A
					if((strcmp (token_buffer,"a:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						poligono->A = temp;
					}
					else if((strcmp (token_buffer,"b:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						poligono->B = temp;
					}	
					else if((strcmp (token_buffer,"c:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						poligono->C = temp;
					}
					else if((strcmp (token_buffer,"d:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						poligono->D = temp;
					}
					//Vertice
					else if((strcmp (token_buffer,"vertice:"))==0){

						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;

						struct VERTICE *p_vertice;
						p_vertice = (struct VERTICE*) malloc(sizeof(struct VERTICE));				
						p_vertice->X = temp_vector[0];
					  	p_vertice->Y = temp_vector[1];
					  	p_vertice->Z = temp_vector[2];
						
						
						if (poligono->vertices_list == NULL){
							poligono->vertices_list = append_vertice(poligono->vertices_list, p_vertice);
					  	}
						else append_vertice(poligono->vertices_list, p_vertice);
						
						poligono->vertices_num++;

				
						
						// Vertices Aplastados 2D
						struct VERTICE *p_vertice_2D;
						p_vertice_2D = (struct VERTICE*) malloc(sizeof(struct VERTICE));

						//Si el coeficiente A es mayor, eliminar X
						if(abs(poligono->B) <= abs(poligono->A) && abs(poligono->C) <= abs(poligono->A))
						{
							p_vertice_2D->X = temp_vector[1];
					  		p_vertice_2D->Y = temp_vector[2];
					  		p_vertice_2D->Z = 0;
						}
						//Si el coeficiente B es mayor, eliminar Y
						else if(abs(poligono->A) <= abs(poligono->B) && abs(poligono->C) <= abs(poligono->B)) 
						{
							p_vertice_2D->X = temp_vector[0];
					  		p_vertice_2D->Y = temp_vector[2];
					  		p_vertice_2D->Z = 0;
						}
						else  	//Si el coeficiente C es mayor, eliminar Z
						{	
							p_vertice_2D->X = temp_vector[0];
					  		p_vertice_2D->Y = temp_vector[1];
					  		p_vertice_2D->Z = 0;
						}
						//printf("Vert_2D:%Lf, %Lf\n",p_vertice_2D->X,p_vertice_2D->Y);
						//append_vertice(poligono->vertices_list_2D, p_vertice_2D);

						if (poligono->vertices_list_2D == NULL){
						poligono->vertices_list_2D = append_vertice(poligono->vertices_list_2D, p_vertice_2D);
					  	}
						else append_vertice(poligono->vertices_list_2D, p_vertice_2D);

						
						
					}
					//Centro
					else if((strcmp (token_buffer,"centro:"))==0){
						if( scanner(inFile) != 0 ) return -1;						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;					
						poligono->centro.Xw = temp_vector[0];
					  	poligono->centro.Yw = temp_vector[1];
					  	poligono->centro.Zw = temp_vector[2];
					}
					else if((strcmp (token_buffer,"escala:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						poligono->escala = temp;
					}
					//Color
					else if((strcmp (token_buffer,"color:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						ptr1->color.r = temp_vector[0];
					  	ptr1->color.g = temp_vector[1];
					  	ptr1->color.b = temp_vector[2];

						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}
					
					
					//Textura: Cargar la textura 
					else if((strcmp (token_buffer,"textura:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						//Copiar direccion de la textura
						//strcpy(ptr1->tex_path, token_buffer);
						//printf("\nArchivo: %s\n",token_buffer );
						ptr1->tex_path = token_buffer;
						if(check_file_exist(ptr1->tex_path))
						{	
							unsigned char* image;
							unsigned width, height;
							unsigned error = lodepng_decode32_file(&image, &width, &height, ptr1->tex_path);
							if(error) printf("error %u: %s\n", error, lodepng_error_text(error));							
							ptr1->tex_buffer = image;
							ptr1->THres = width;
							ptr1->TVres = height;													
						}
					}		
					
					
					//ka
					else if((strcmp (token_buffer,"ka:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ka_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ka = ka_temp;
					}
					//ka
					else if((strcmp (token_buffer,"kd:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kd_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kd = kd_temp;
					}
					//kn
					else if((strcmp (token_buffer,"kn:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kn_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kn = kn_temp;
					}
					//ks
					else if((strcmp (token_buffer,"ks:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ks_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ks = ks_temp;
					}

					//O1
					else if((strcmp (token_buffer,"O1:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O1_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O1 = O1_temp;
					}
					//O2
					else if((strcmp (token_buffer,"O2:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O2_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O2 = O2_temp;
					}
					//O3
					else if((strcmp (token_buffer,"O3:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O3_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O3 = O3_temp;
					}
					//Plano de Corte Positivo y Negativo
					else if( (strcmp (token_buffer,"clip:") ==0) || (strcmp (token_buffer,"clip_p:") ==0) || (strcmp (token_buffer,"clip_n:") ==0))
					{
						int signo_p = 1;
						if ((strcmp (token_buffer,"clip_n:") ==0)) signo_p = -1;
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[4];
						if( parse_vector(4,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						//Plano nuewo
						PLANO * new_plano = (PLANO*) malloc(sizeof(PLANO));
						
						new_plano->signo = signo_p;						
						new_plano->A = temp_vector[0];
						new_plano->B = temp_vector[1];
						new_plano->C = temp_vector[2];
						new_plano->D = temp_vector[3];
						
						poligono->planos_corte = insertar_plano_corte(poligono->planos_corte , new_plano);		
					}
					else
					{
						printf("Error en archivo, propiedad \"%s\" de esfera no identificada!!!\n",token_buffer );
						return -1;
					}
				}
				
				
				// Modificar los vertices con Centro y Escala
				struct VERTICE  * vert_temp = poligono->vertices_list_2D;
				PUNTO3D centro = poligono -> centro;
				float escala = 1;
				if(poligono -> escala > 0){	escala =poligono -> escala; }
				poligono -> escala = 1;
				while(vert_temp != NULL)
				{
					//Si el coeficiente A es mayor, eliminar X
					if(abs(poligono->B) <= abs(poligono->A) && abs(poligono->C) <= abs(poligono->A))
					{
						vert_temp->X = (vert_temp->X + centro.Yw)* escala;
						vert_temp->Y = (vert_temp->Y + centro.Zw)* escala;
						vert_temp->Z = 0;
					}
					//Si el coeficiente B es mayor, eliminar Y
					else if(abs(poligono->A) <= abs(poligono->B) && abs(poligono->C) <= abs(poligono->B)) 
					{
						vert_temp->X = (vert_temp->X + centro.Xw)* escala;
						vert_temp->Y = (vert_temp->Y + centro.Zw)* escala;
						vert_temp->Z = 0;
					}
					else  	//Si el coeficiente C es mayor, eliminar Z
					{	
						vert_temp->X = (vert_temp->X + centro.Xw)* escala;
						vert_temp->Y = (vert_temp->Y + centro.Yw)* escala;
						vert_temp->Z = 0;
					}
					
					vert_temp=vert_temp->sig;
				}
				
				
				ungetc(c, inFile);	
			}

			



			///////////////////////////////
			//	ELIPSE
			////////////////////////////////			
			else if((strcmp (token_buffer,"#elipse"))==0){ 
				clear_buffer();

				///////// Aca crear objeto con estructura esfera!!!!!
				struct OBJETO *ptr1;
				struct ELIPSE *e1;
				e1 = (struct ELIPSE*) malloc(sizeof(struct ELIPSE));
			  	ptr1 = creanodo("elipse");		
			  	ptr1->p = e1;
			  	objetos = insertafinal(objetos, ptr1);				
			  	ptr1->tipo = "elipse";
				e1->planos_corte = NULL;

				//Inicializar Normal vacia
				VECTOR vector_vacio;
				vector_vacio.X = 0;
				vector_vacio.Y = 0;
				vector_vacio.Z = 0;
				e1->normal = vector_vacio;

				while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF) break; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);
	
					//K
					if((strcmp (token_buffer,"k:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float radio_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						e1->k = radio_temp;
					}
					//Anillo
					else if((strcmp (token_buffer,"anillo:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float a_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						e1->anillo = a_temp;
					}
					//Color
					else if((strcmp (token_buffer,"color:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						ptr1->color.r = temp_vector[0];
					  	ptr1->color.g = temp_vector[1];
					  	ptr1->color.b = temp_vector[2];

						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}
					
					
					//foco 1
					else if((strcmp (token_buffer,"f1:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						e1->f1.Xw = temp_vector[0];
					  	e1->f1.Yw = temp_vector[1];
					  	e1->f1.Zw = temp_vector[2];

						//printf("f1: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}
					//foco 2
					else if((strcmp (token_buffer,"f2:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						e1->f2.Xw = temp_vector[0];
					  	e1->f2.Yw = temp_vector[1];
					  	e1->f2.Zw = temp_vector[2];

						//printf("f1: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}
					//Auxuiliar D
					else if((strcmp (token_buffer,"d:"))==0){
						if( scanner(inFile) != 0 ) return -1;						
						float temp_vector[3];
						if( parse_vector(3,token_buffer, token_buffer_index, temp_vector) !=0) return -1;						
						e1->d.Xw = temp_vector[0];
					  	e1->d.Yw = temp_vector[1];
					  	e1->d.Zw = temp_vector[2];
						//printf("Dx:%Lf, Dy:%Lf, Dz:%Lf\n",temp_vector[0], temp_vector[1], temp_vector[2]);
					}
					//ka
					else if((strcmp (token_buffer,"ka:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ka_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ka = ka_temp;
					}
					//ka
					else if((strcmp (token_buffer,"kd:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kd_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kd = kd_temp;
					}
					//kn
					else if((strcmp (token_buffer,"kn:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float kn_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Kn = kn_temp;
					}
					//ks
					else if((strcmp (token_buffer,"ks:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float ks_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->Ks = ks_temp;
					}

					//O1
					else if((strcmp (token_buffer,"O1:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O1_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O1 = O1_temp;
					}
					//O2
					else if((strcmp (token_buffer,"O2:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O2_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O2 = O2_temp;
					}
					//O3
					else if((strcmp (token_buffer,"O3:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float O3_temp = atof(token_buffer);
						//printf("Radio:%f\n",radio_temp);
						// Aca asignar el radio con atof(token_buffer);
						ptr1->O3 = O3_temp;
					}
					//Plano de Corte Positivo y Negativo
					else if( (strcmp (token_buffer,"clip:") ==0) || (strcmp (token_buffer,"clip_p:") ==0) || (strcmp (token_buffer,"clip_n:") ==0))
					{
						int signo_p = 1;
						if ((strcmp (token_buffer,"clip_n:") ==0)) signo_p = -1;
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[4];
						if( parse_vector(4,token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						//Plano nuewo
						PLANO * new_plano = (PLANO*) malloc(sizeof(PLANO));
						
						new_plano->signo = signo_p;						
						new_plano->A = temp_vector[0];
						new_plano->B = temp_vector[1];
						new_plano->C = temp_vector[2];
						new_plano->D = temp_vector[3];
						
						e1->planos_corte = insertar_plano_corte(e1->planos_corte , new_plano);		
					}		 
					
					else
					{
						printf("Error en archivo, propiedad \"%s\" de elipse no identificado!!!\n",token_buffer );
						return -1;
					}
				}
				
				
				PUNTO3D f1, f2, d;
				f1 = e1->f1;
				f2 = e1->f2;
				d  = e1->d;
				printf("Dx:%Lf, Dy:%Lf, Dz:%Lf\n",d.Xw, d.Yw, d.Zw);

				VECTOR v,w, perpendicular;
				// v: Centro -> D1
				v.X = f1.Xw - d.Xw;
				v.Y = f1.Yw - d.Yw;
				v.Z = f1.Zw - d.Zw;
				// w: Centro -> D2
				w.X = f2.Xw - d.Xw;
				w.Y = f2.Yw - d.Yw;
				w.Z = f2.Zw - d.Zw;
				
				perpendicular = producto_cruz( v,  w);
				e1->normal = perpendicular;

				e1->A = perpendicular.X;
				e1->B = perpendicular.Y;
				e1->C = perpendicular.Z;
				e1->D = -(e1->A*d.Xw + e1->B*d.Yw  + e1->C*d.Zw  );	
				

				printf("A:%f, B:%f, C:%f, D:%f\n",e1->A,e1->B,e1->C,e1->D);
				
				
				ungetc(c, inFile);
				
			}	





			else
			{
				printf("Error objeto \"%s\" irreconocible!!! \n", token_buffer);
				return -1;
			}
	
			
		}
		/////////////////////////////
		///// IDENTIFICADOR
		///////////////////////////////
		else if(isalpha(in_char) ){ // obtener identificador	
			buffer_char(in_char);
			for (c = getc(inFile); isalnum(c) || c == '_' || c == '-'; c = getc(inFile)){//extraer el id, hasta ":"
				buffer_char(c);
			}	
			if(c == ':') {
				buffer_char(c);
				return 0;
			}
			else{
				printf("Error en el archivo de entrada, identificador \"%s\" incompleto o invalido!!!\n", token_buffer);
				return -1;
			}
		}		
		///////////////////
		//// VALOR
		///////////////////
		else if(isdigit(in_char) || in_char== '(' || in_char== '[' || in_char== '-'){ // obtener valor
			buffer_char(in_char);
			for (c = getc(inFile);isdigit(c)||c == '-'||c == ','|| c == '.'||c == ')' ||c == ']' ||c==' '; c = getc(inFile)){
				if(c==' ') continue; //Si es epsacio ignorar
				buffer_char(c);
			}	
			if(isspace(c)) {
				ungetc(c, inFile);
				return 0;
			}
			else{
				printf("Error en el archivo de entrada, valor \"%s\" incompleto o invalido!!!!", token_buffer);
				return -1;
			}
		}	
		
		
		
		/////////////////////////////
		///// STRING
		///////////////////////////////
		else if(in_char== '"' || in_char== '\''){ // obtener string	
			//buffer_char(in_char);
			int i=0;	

			for (c = getc(inFile); isalnum(c) || c == '.' || c == '/' || c == ' ' ||  c == '-' || c == '_'  ; c = getc(inFile)) //extraer el string, hasta "  o ' o termine
			{
				if(i>=sizeof(token_buffer)) break; 
				buffer_char(c);
				i++;
			}	
			if(c == '"' || c == '\'') { //Si se termino el string 
				return 0;
			}
			else{
				printf("Error en el archivo de entrada, string \"%s\" incompleto o invalido!!!\n", token_buffer);
				return -1;
			}
		}		
		
		
		
		//////////////////////////
		//	 ERROR
		/////////////////////////
		else{
	        	printf("Error en el archivo de entrada, caracter \"%c\" invalido!!!!", in_char);
			return -1;
	    }
    }
	//Si termina el scanner sin errores
	return 0;
}





