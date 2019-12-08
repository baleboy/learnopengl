CC = g++
CFLAGS = -I ~/include
OBJ = hellotriangle.o glad.o 
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) 

hellotriangle: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm *.o  hellotriangle