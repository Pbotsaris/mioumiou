
CC=clang++
OBJ=obj
BIN=bin
TARGET=$(BIN)/2dengine
SRC=src

$(shell mkdir -p obj obj/game obj/components obj/ecs obj/systems bin)

# Libs
SDL=$(shell pkg-config --cflags --libs SDL2_image)
LUA=$(shell pkg-config --cflags --libs lua)
SPDLOG=$(shell pkg-config --cflags --libs spdlog)

INCLUDES=-Ilibs/glm -Ilibs/imgui -Ilibs/lua -Ilibs/sol -Isrc

CFLAGS= -Wall -Werror -Wextra -std=c++17 -Wno-unknown-warning-option
LIBFLAGS= $(SDL) $(LUA) $(SPDLOG)

SRCS=$(wildcard $(SRC)/**/*.cpp) $(wildcard $(SRC)/*.cpp) 
OBJS=$(patsubst $(SRC)%.cpp, $(OBJ)%.o, $(SRCS)) 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIBFLAGS) $(INCLUDES)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(BIN)$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)/*.o $(OBJ)/game/*.o $(OBJ)/ecs/*.o $(OBJ)/components/*.o $(OBJ)/systems/*.o 


