TARGET = rungl

CC = g++
CFLAGS = -I ~/include -I ../include -std=c++11 -fno-elide-constructors 
# FIXME: -fno-elide-constructors is to workaround a double delete error revealed by C++11 move semantics when
# returning a vector in model.c
OBJ = ../common/glad.o ../common/shader.o ../common/mesh.o ../common/stb_image.o rungl.o ../common/camera.o ../common/model.o

OSNAME := $(shell uname -s)
ifeq ($(OSNAME),Darwin)
	CFLAGS += -D OSX
	LDFLAGS = -L ~/lib -lassimp -lglfw -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon 
else
	LDFLAGS = -L ~/lib -lassimp -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
endif

%.o: %.cpp
	$(CC) -c -g -o $@ $< $(CFLAGS) 

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm *.o  $(TARGET)