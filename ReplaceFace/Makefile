SHELL = /bin/bash

DEBUG = -g

INC = include
SRC = src

CXX = g++
FLAGS = $(DEBUG) -Wall -std=c++0x -I$(INC) `pkg-config --cflags opencv` `curl-config --cflags`
LIBS = `pkg-config --libs opencv` `curl-config --libs`

EXE = replace_face

INC_FILES = $(wildcard $(INC)/*.h)
SRC_FILES = $(wildcard $(SRC)/*.cpp)


$(EXE): $(SRC_FILES) $(INC_FILES)
	$(CXX) $(FLAGS) $(SRC_FILES) -o $@ $(LIBS)

clean:
	rm -rf $(EXE) access_token
