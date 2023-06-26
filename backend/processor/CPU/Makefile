TARGET   = ../cpu_t
CC=g++
CFLAGS   = -c -std=c++11

PREF_OBJ = ./obj/

SRC      = $(wildcard *.cpp)                         #include of all files with .cpp
OBJ      = $(patsubst %.cpp, $(PREF_OBJ)%.o, $(SRC)) #turn .cpp into .o

all:     $(TARGET)

$(TARGET):  $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

$(PREF_OBJ)%.o : %.cpp                               #for whole project
	$(CC) $(CFLAGS) $< -o $@

$(PREF_OBJ)main.o : main.cpp                         #for one file
	$(CC) $(CFLAGS) main.cpp -o $(PREF_OBJ)main.o


valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./$(TARGET)

.PHONY : clean
clean:
	rm -rf $(PREF_OBJ)*.o $(TARGET)
