OBJECTS=main.o
OUTPUT=main

LIBS=-lX11 -lglut -lGLU -lGL -lm -lXext 


$(OUTPUT): $(OBJECTS)
	$(CC) main.c -o $(OUTPUT) $(LIBS)

clean:
	rm -f *.o
	rm -f main