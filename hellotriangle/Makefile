CC = g++
CFLAGS = -I ~/include
OBJ = hellotriangle.o glad.o shader.o
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

%.o: %.cpp
	$(CC) -c -g -o $@ $< $(CFLAGS) 

hellotriangle: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm *.o  hellotriangle