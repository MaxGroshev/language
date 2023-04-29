TARGET   = lang
CC       = gcc
CFLAGS   = -c -std=c++17

GR_DIR    = ./tree/graph_lib/
LOGS_DIR  = ./logs/
TREE_DIR  = ./tree/
PREF_OBJ  = ./obj/
PREF_STAT = ./logs/log_pics/


#Graphviz files
GR_LIB   = $(wildcard $(GR_DIR)*.cpp)
OBJ_LIB  = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(GR_LIB))
#Tree files
TREE_SRC = $(wildcard $(TREE_DIR)*.cpp)
OBJ_TREE = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(TREE_SRC))
#Common files
SRC      = $(wildcard *.cpp )                            #include of all files with .cpp
OBJ      = $(patsubst %.cpp, $(PREF_OBJ)%.o, $(SRC))     #turn .cpp into .o
#Logs files
LOGS_SRC = $(wildcard $(LOGS_DIR)*.cpp)
OBJ_LOGS = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(LOGS_SRC))



all:     $(TARGET)

$(TARGET):  $(OBJ) $(OBJ_TREE) $(OBJ_LIB) $(OBJ_LOGS)
	$(CC) -o $(TARGET) $(OBJ) $(OBJ_TREE) $(OBJ_LIB) $(OBJ_LOGS)

$(PREF_OBJ)%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@


valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./$(TARGET)

graphviz:
	dot $(GR_DIR)tree_dump.dot -T pdf -o $(PREF_STAT)tree_dump.pdf
	dot $(GR_DIR)tree_dump.dot -T png -o $(PREF_STAT)tree_dump.png

.PHONY : clean
clean:
	rm -rf $(PREF_OBJ)*.o $(TARGET) *.aux *.log vgcore.*
