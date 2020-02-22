TARGET = rungl

CC = g++
CFLAGS = -I ~/include -I ../include -std=c++11
OBJ = rungl.o ../common/glad.o ../common/shader.o ../common/camera.o ../common/stb_image.o

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