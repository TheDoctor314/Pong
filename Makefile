#OBJS specifies which files to compile
OBJS = main.cpp Paddle.cpp Ball.cpp LTexture.cpp

#CC specifies the compiler
CC = g++

#COMPILER_FLAGS specifies the additional options to use
COMPILER_FLAGS = -std=c++11 -g

#LINKER FLAGS specifies the libraries we are linking
LINKER_FLAGS = -lSDL2 -lSDL2_ttf

#EXECUTABLE specifies the name of the executable
EXECUTABLE = pong

#target that compiles the executable
all : $(OBJS)
		$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXECUTABLE)
