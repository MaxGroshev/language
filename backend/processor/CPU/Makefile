TARGET   = ../cpu_t
CC       = g++
CFLAGS   = -Wshadow    							\
		   -Winit-self 							\
		   -Wredundant-decls 					\
		   -Wcast-align							\
		   -Wundef 								\
		   -Wfloat-equal 						\
		   -Winline 							\
		   -Wunreachable-code 					\
		   -Wmissing-declarations 				\
		   -Wmissing-include-dirs 				\
		   -Wswitch-enum 						\
		   -Wswitch-default 					\
		   -Weffc++ 							\
		   -Wmain 								\
		   -Wextra 								\
		   -Wall 								\
		   -g 									\
		   -pipe 								\
		   -fexceptions 						\
		   -Wcast-qual 							\
		   -Wconversion 						\
		   -Wctor-dtor-privacy 					\
		   -Wempty-body 						\
		   -Wformat-security 					\
		   -Wformat=2 							\
		   -Wignored-qualifiers 				\
		   -Wlogical-op 						\
		   -Wmissing-field-initializers 		\
		   -Wnon-virtual-dtor 					\
		   -Woverloaded-virtual 				\
		   -Wpointer-arith 						\
		   -Wsign-promo 						\
		   -Wstack-usage=8192 					\
		   -Wstrict-aliasing 					\
		   -Wstrict-null-sentinel 				\
		   -Wtype-limits 						\
		   -Wwrite-strings 						\
		   -D_DEBUG 							\
		   -D_EJUDGE_CLIENT_SIDE

PREF_OBJ = ./obj/

STK_DIR = ./stack/
RAM_DIR = ./RAM/

###################################################################################################

STK_SRC = $(wildcard $(STK_DIR)*.cpp)
STK_OBJ = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(STK_SRC))

RAM_SRC = $(wildcard $(RAM_DIR)*.cpp)
RAM_OBJ = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(RAM_SRC))

CPU_SRC = $(wildcard *.cpp)
CPU_OBJ = $(patsubst $(PREF_OBJ)%.cpp, %.o, $(CPU_SRC))

###################################################################################################

all:     $(TARGET)

$(TARGET): $(STK_OBJ) $(RAM_OBJ) $(CPU_OBJ)
	$(CC) -o $(TARGET) $(STK_OBJ) $(RAM_OBJ) $(CPU_OBJ)

$(PREF_OBJ)%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

#####################################################################################################

.PHONY: valgrind
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./$(TARGET)

.PHONY: clean
clean:
	rm -rf $(PREF_OBJ)*.o $(TARGET)
