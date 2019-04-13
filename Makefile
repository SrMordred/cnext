CC		:= clang
C_FLAGS := -Wall -Wextra -g

BIN		:= bin
SRC		:= src
INCLUDE	:= 
LIB		:= 

LIBRARIES	:= -lm

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main
else
EXECUTABLE	:= main
endif

all: $(BIN)/$(EXECUTABLE)
	./$(BIN)/$(EXECUTABLE)
clean:
	-$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c
	$(CC) $(C_FLAGS) $(INCLUDE) $(LIB) $^ -o $@ $(LIBRARIES)
