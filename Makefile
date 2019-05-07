CC      = clang
TARGET  = -target x86_64-w64-mingw32-gcc
DEBUG   = -Wall -Wextra -g
STD     = -std=c11
INCLUDE = -Isrc -Iinclude
MAIN    = src/main.c
LIB     = -Llib
LIBS    = 
OUT     = bin/main.exe

#STD MATH
LIBS    += -lm

#SDL2
LIBS    += -lmingw32 -lSDL2main -lSDL2 
#SDL2 IMAGE
LIBS    += -lSDL2_image

all:
	$(CC) $(TARGET) $(DEBUG) $(STD) $(INCLUDE) $(MAIN) $(LIB) $(LIBS) -o $(OUT)
	bin/main.exe

	
