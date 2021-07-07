 # the compiler: gcc for C program.
CC = g++

# compiler flags:
CFLAGS = -std=c++1z -o
LDFLAGS = -lOpenGL -lglfw -lGLEW

# the build target executable:
TARGET = /usr/bin/carcade
# for development
# TARGET = ./carcade

all: $(TARGET)

$(TARGET): cArcade.c
	$(CC) $(CFLAGS) $(TARGET) cArcade.c headers/shader.cpp $(LDFLAGS)

clean:
	$(RM) $(TARGET)