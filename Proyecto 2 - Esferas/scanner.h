

struct OBJETO *objetos;


char *  input_file_name = "input_file.txt";
int token_buffer_index =0;			//indice para el buffer
char token_buffer[100];     





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
  	ptr1 = creanodo();		
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
	  	ptr1 = creanodo();		
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





int parse_vector(char* vector_string, int string_length, float output_vector[3])
{
	int i, j, vector_index = 0;
	char  temp_value[10]; 
	
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
		if(vector_index >= 3 ) return -1;		//Si el vector mide mas de 3
		output_vector[vector_index] = atof(temp_value);
		vector_index++;
	}
	if (vector_index == 3) return 0; //Si el vector esta completo 
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
						if( parse_vector(token_buffer, token_buffer_index, color_vector) !=0) return -1;
					
						
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
						if( parse_vector(token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						

						//background-color
						BACKGROUND.r = temp_vector[0];
						BACKGROUND.g = temp_vector[1];
						BACKGROUND.b = temp_vector[2];


						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
					}
					else if((strcmp (token_buffer,"ojo:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
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
						if( parse_vector(token_buffer, token_buffer_index, temp_vector) !=0) return -1;
								
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
						if( parse_vector(token_buffer, token_buffer_index, temp_vector) !=0) return -1;
					
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


			//////////////////////
			//	Esfera
			////////////////////////////////			
			else if((strcmp (token_buffer,"#esfera"))==0){ 
				clear_buffer();

				///////// Aca crear objeto con estructura esfera!!!!!
				struct OBJETO *ptr1;
				struct ESFERA *e1;
				e1 = (struct ESFERA*) malloc(sizeof(struct ESFERA));
			  	ptr1 = creanodo();		
			  	ptr1->p = e1;
			  	objetos = insertafinal(objetos, ptr1);				
			  	ptr1->tipo = "esfera";

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
						if( parse_vector(token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						e1->centro.Xw = temp_vector[0];
					  	e1->centro.Yw = temp_vector[1];
					  	e1->centro.Zw = temp_vector[2];

						//printf("Centro: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);

					}
					//Color
					else if((strcmp (token_buffer,"color:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						ptr1->color.r = temp_vector[0];
					  	ptr1->color.g = temp_vector[1];
					  	ptr1->color.b = temp_vector[2];

						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
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
				VECTOR vector_vacio;
				vector_vacio.X = 0;
				vector_vacio.Y = 0;
				vector_vacio.Z = 0;
				poligono->normal = vector_vacio;
				poligono->vertices_num= 0;

				//Enlazar Objeto y Poligono
			  	ptr1 = creanodo();		
			  	ptr1->p = poligono;
			  	objetos = insertafinal(objetos, ptr1);				
			  	ptr1->tipo = "poligono";
				
				

			  	while ((in_char = getc(inFile)) != '#'){ //meintras no sea otro objeto => get next token
					if( scanner(inFile) != 0 ) return -1;
					if((in_char = getc(inFile)) == EOF) return 0; //Corroborar que no se ha terminado el archivo 
					ungetc(in_char, inFile);
	
					//Radio
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
						if( parse_vector(token_buffer, token_buffer_index, temp_vector) !=0) return -1;

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
					//Color
					else if((strcmp (token_buffer,"color:"))==0){
						if( scanner(inFile) != 0 ) return -1;
						float temp_vector[3];
						if( parse_vector(token_buffer, token_buffer_index, temp_vector) !=0) return -1;
						
						ptr1->color.r = temp_vector[0];
					  	ptr1->color.g = temp_vector[1];
					  	ptr1->color.b = temp_vector[2];

						//printf("Color: %f, %f, %f \n",temp_vector[0],temp_vector[1],temp_vector[2]);
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
					else
					{
						printf("Error en archivo, propiedad \"%s\" de esfera no identificada!!!\n",token_buffer );
						return -1;
					}
				}
				
				ungetc(c, inFile);	
			}


			else if((strcmp (token_buffer,"#luz"))==0){ 


				//Toda la puya

				ungetc(c, inFile);
			}


			else
			{
				printf("Error objeto \"%s\" irreconocible!!! \n", token_buffer);
				return -1;
			}
	
			
		}
		
		// Identificador
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
		
		// Valor
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

		else{
	        	printf("Error en el archivo de entrada, caracter \"%c\" invalido!!!!", in_char);
			return -1;
	    }
    	}
	//Si termina el scanner sin errores
	return 0;
}





/*
//Experimento No Descomentar!!!!
int esfera_data_nums = 5;
char esfera_ids[5]={"radio","centro","color","kd","ka"};
int esfera_flags[5];




//Encuentra en que posicion se encuentra cierto dato, recive: nombre del dato, la lista de los nombres y el tamaño de lista
int get_position(const char* name, const char** list, int sizeof_list){
  int i;
  for(i=0; i < sizeof_list; i++){
    if(strcmp(name,list[i]) == 0){
      return i;
    }
  }
  return -1;
}

*/
