SRCDIR = src/

JELLIFISHDIR = $(SRCDIR) jellyfish/

#Files that will be compiled
OBJS = $(addprefix src/jellyfish/, *.cpp) $(SRCDIR)stbi/stb_image.cpp main.cpp $(SRCDIR)glad/glad.c

#Used compiler
CC = g++

#Path to aditional files needed to compile
INCLUDE_PATHS = -Iinclude

#Path to libraries needed to compile
LIBRARY_PATHS = -Llibs

#Aditional compile options
COMPILER_FLAGS = -g

#Specifies which libraries we're linking
LINKER_FLAGS = -lglfw3 -lOpenGL32

#Executable name
EXE_NAME = bin/demo

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXE_NAME)