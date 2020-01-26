TARGET = rungl

CC = g++
CFLAGS = -I ~/include -std=c++11 
OBJ = glad.o shader.o mesh.o stb_image.o rungl.o camera.o

OSNAME := $(shell uname -s)
ifeq ($(OSNAME),Darwin)
	CFLAGS += -D OSX
	LDFLAGS = -lglfw -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon 
else
	LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
endif

%.o: %.cpp
	$(CC) -c -g -o $@ $< $(CFLAGS) 

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm *.o  $(TARGET)