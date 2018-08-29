OBJECTS=clip.o
OUTPUT=clip

LIBS=-lX11 -lglut -lGLU -lGL -lm -lXext 


$(OUTPUT): $(OBJECTS)
	$(CC) clip.c -o $(OUTPUT) $(LIBS)

clean:
	rm -f *.o
	rm -f clip