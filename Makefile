
CC=clang++
OBJ=obj
BIN=bin
TARGET=$(BIN)/mioumiou
SRC=src

$(shell mkdir -p obj obj/game obj/components obj/ecs obj/systems bin)

# Libs
SDL=$(shell pkg-config --cflags --libs SDL2_image SDL2_ttf)
LUA=$(shell pkg-config --cflags --libs lua)
SPDLOG=$(shell pkg-config --cflags --libs spdlog)

INCLUDES=-Ilibs/glm -Ilibs/imgui -Ilibs/lua -Ilibs/sol -Isrc -Ilibs/doctest

CFLAGS= -Wall -Werror -Wextra -std=c++20 -Wno-unknown-warning-option 
LIBFLAGS= $(SDL) $(LUA) $(SPDLOG)

# -gdwarf-4 is necessary for valgrind
DEBUGFLAGS= -g3 -gdwarf-4

SRCS=$(wildcard $(SRC)/**/*.cpp) $(wildcard $(SRC)/*.cpp) 
OBJS=$(patsubst $(SRC)%.cpp, $(OBJ)%.o, $(SRCS)) 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIBFLAGS) $(INCLUDES) $(DEBUGFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(INCLUDES) -c $< -o $@

test: all testing

run: all exec

run_debug: all exec_debug

exec:
	./$(TARGET)

exec_debug:
	./$(TARGET) -log=debug

testing:
	./$(TARGET) -test

clean:
	rm -f $(TARGET) $(OBJ)/*.o $(OBJ)/game/*.o $(OBJ)/ecs/*.o $(OBJ)/components/*.o $(OBJ)/systems/*.o 


