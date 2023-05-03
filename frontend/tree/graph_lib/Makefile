TARGET = graph_lib
CC     = gcc
CFLAGS = -g -fPIC -shared

PREF_OBJ  = ./obj/

SRC    = $(wildcard *.cpp)                          #include of all files with .cpp
OBJ    = $(patsubst %.cpp, $(PREF_OBJ)%.so, $(SRC)) #turns .cpp into .so

all:     $(TARGET)


$(TARGET):  $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

$(PREF_OBJ)%.so : %.cpp
	$(CC) $(CFLAGS) -o $@ $<


valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./$(TARGET)

.PHONY : clean
clean:
	rm -rf $(PREF_OBJ)*.so $(TARGET)
