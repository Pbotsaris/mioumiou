
CC=clang++
SRC=src
OBJ=obj
BIN=bin
TARGET=$(BIN)/2dengine

$(shell mkdir -p obj bin)

# Libs
SDL=$(shell pkg-config --cflags --libs SDL2_image)
LUA=$(shell pkg-config --cflags --libs lua)
SPDLOG=$(shell pkg-config --cflags --libs spdlog)

INCLUDES=-Ilibs/glm -Ilibs/imgui -Ilibs/lua -Ilibs/sol -Isrc/include 


CFLAGS= -Wall -Werror -Wextra -std=c++17 -Wno-unknown-warning-option
LIBFLAGS= $(SDL) $(LUA) $(SPDLOG)

SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIBFLAGS) $(INCLUDES)

$(OBJ)/%.o: $(SRC)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(BIN)$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)/*.o 


