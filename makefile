 # the compiler: gcc for C program.
CC = gcc

# compiler flags:
CFLAGS = -std=gnu11 -o
LDFLAGS = -lOpenGL -lglfw -lGLEW -lm

# the build target executable:
TARGET = /usr/bin/carcade
# for development
# TARGET = ./carcade

all: $(TARGET)

$(TARGET): cArcade.c
	$(CC) $(CFLAGS) $(TARGET) cArcade.c headers/cShader.c $(LDFLAGS)

clean:
	$(RM) $(TARGET)