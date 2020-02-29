TARGET = bout

CC = g++
CFLAGS = -I . -I ~/include -I ../include -std=c++11
OBJ = ../common/glad.o ../common/stb_image.o game.o shader.o texture.o main.o

OSNAME := $(shell uname -s)
ifeq ($(OSNAME),Darwin)
	CFLAGS += -D OSX -Wno-deprecated-declarations
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