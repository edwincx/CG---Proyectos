CC=gcc
DEPS= objetos.h rayTracer.h stdio.h math.h stdlib.h malloc.h
lineas: rayTracer.c
	$(CC) -o diseniador diseniador.c -w -lm
	./diseniador
	$(CC) -o rayTracer rayTracer.c -w lodepng/lodepng.c -lm 
	./rayTracer -i input_file_textura.txt
	xdg-open image.png 
