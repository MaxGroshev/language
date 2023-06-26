TARGET   = frontend
CC       = g++
CFLAGS   = -c -std=c++17 -Wall

MODULES = ./backend/processor

GR_DIR    = ./tree/graph_lib/
LOGS_DIR  = ./logs/
TREE_DIR  = ./tree/
FRONT_DIR = ./frontend/
BACK_DIR  = ./backend/
PREF_OBJ  = ./obj/
PREF_STAT = ./logs/log_pics/

STR_LIB_DIR = ./my_str_func/

#Str func
STR_LIB_SRC = $(wildcard $(STR_LIB_DIR)*.cpp)
OBJ_STR_LIB = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(STR_LIB_SRC))
#Graphviz files
GR_LIB   = $(wildcard $(GR_DIR)*.cpp)
OBJ_LIB  = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(GR_LIB))
#Tree files
TREE_SRC = $(wildcard $(TREE_DIR)*.cpp)
OBJ_TREE = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(TREE_SRC))
#Logs files
LOGS_SRC = $(wildcard $(LOGS_DIR)*.cpp)
OBJ_LOGS = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(LOGS_SRC))
#Rec_descent files
FRONT_SRC= $(wildcard $(FRONT_DIR)*.cpp)
OBJ_FRONT= $(patsubst $(PREF_OBJ)%.cpp, %.o, $(FRONT_SRC))
#Backend files
BACK_SRC = $(wildcard $(BACK_DIR)*.cpp)
OBJ_BACK = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(BACK_SRC))


front: $(TARGET)

$(TARGET):  $(OBJ)  $(OBJ_TREE) $(OBJ_STR_LIB)  $(OBJ_LIB) $(OBJ_LOGS) $(OBJ_FRONT)
	$(CC) -o $(TARGET) $(OBJ)  $(OBJ_TREE) $(OBJ_STR_LIB) $(OBJ_LIB) $(OBJ_LOGS) $(OBJ_FRONT)

$(PREF_OBJ)%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@


full_ver:
	for dir in $(MODULES); do $(MAKE) -C $$dir; done

.PHONY : valgrind
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./$(TARGET)

.PHONY : graphviz
graphviz:
	dot $(GR_DIR)tree_dump.dot -T pdf -o $(PREF_STAT)tree_dump.pdf
	dot $(GR_DIR)tree_dump.dot -T png -o $(PREF_STAT)tree_dump.png

.PHONY : clean
clean:
	rm -rf $(PREF_OBJ)*.o $(TARGET) *.aux *.log vgcore.*
