FRONT_TARGET = front
BACK_TARGET  = back
CC           = g++
CFLAGS       = -c -std=c++17 -Wall

MODULES = ./backend/processor

GR_DIR    = ./tree/graph_lib/
LOGS_DIR  = ./logs/
TREE_DIR  = ./tree/
FRONT_DIR = ./frontend/
BACK_DIR  = ./backend/
PREF_OBJ  = ./obj/
PREF_STAT = ./logs/log_pics/

STR_LIB_DIR = ./my_str_func/

##################################################################################################################

#Str func
STR_LIB_SRC = $(wildcard $(STR_LIB_DIR)*.cpp)
OBJ_STR_LIB = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(STR_LIB_SRC))
#Tree files
TREE_SRC = $(wildcard $(TREE_DIR)*.cpp)
OBJ_TREE = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(TREE_SRC))
#Graphviz files
GR_LIB   = $(wildcard $(GR_DIR)*.cpp)
OBJ_LIB  = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(GR_LIB))
#Logs files
LOGS_SRC = $(wildcard $(LOGS_DIR)*.cpp)
OBJ_LOGS = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(LOGS_SRC))
#Rec_descent files
FRONT_SRC= $(wildcard $(FRONT_DIR)*.cpp)
OBJ_FRONT= $(patsubst $(PREF_OBJ)%.cpp, %.o, $(FRONT_SRC))
#Backend files
BACK_SRC = $(wildcard $(BACK_DIR)*.cpp)
OBJ_BACK = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(BACK_SRC))

##################################BACK_AND_FRONT################################################################################
all: front back


front: $(FRONT_TARGET)
$(FRONT_TARGET):  $(OBJ) $(OBJ_LIB) $(OBJ_LOGS) $(OBJ_TREE) $(OBJ_FRONT) $(OBJ_STR_LIB)
	$(CC) -o $(FRONT_TARGET) $(OBJ)  $(OBJ_TREE) $(OBJ_LIB) $(OBJ_LOGS) $(OBJ_FRONT)  $(OBJ_STR_LIB)

$(PREF_OBJ)%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@


back: $(BACK_TARGET)
$(BACK_TARGET):  $(OBJ) $(OBJ_LIB) $(OBJ_LOGS) $(OBJ_TREE) $(OBJ_BACK) $(OBJ_STR_LIB)
	$(CC) -o $(BACK_TARGET) $(OBJ)  $(OBJ_TREE) $(OBJ_LIB) $(OBJ_LOGS) $(OBJ_BACK)  $(OBJ_STR_LIB)

$(PREF_OBJ)%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

###################################################################################################################

prog:
	./front
	./back
	for dir in $(MODULES); do $(MAKE) -C $$dir; done

################################PHONIES################################################################################

.PHONY : front_valgrind
front_valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./front

.PHONY : back_valgrind
front_valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./back

.PHONY : graphviz
graphviz:
	dot $(GR_DIR)tree_dump.dot -T pdf -o $(PREF_STAT)tree_dump.pdf
	dot $(GR_DIR)tree_dump.dot -T png -o $(PREF_STAT)tree_dump.png

.PHONY : clean
clean:
	rm -rf $(PREF_OBJ)*.o  $(FRONT_TARGET) $(BACK_TARGET) *.aux *.log vgcore.*
