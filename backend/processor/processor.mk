CC=g++

CFLAGS  = -c
ASM_DIR = assembler/
CPU_DIR = CPU_T/
OBJ_CPU = $(CPU_DIR)main.o $(CPU_DIR)stack_check.o $(CPU_DIR)stack_functions.o \
          $(CPU_DIR)processor_functions.o $(CPU_DIR)stack_dump.o
OBJ_ASM = $(ASM_DIR)main.o $(ASM_DIR)assembler_functions.o

all: asm CPU

CPU: $(OBJ_CPU)
	$(CC) -o CPU $(OBJ_CPU)

main.o:                $(CPU_DIR)main.cpp $(CPU_DIR)CPU.h test-code.bin
stack_check.o:         $(CPU_DIR)stack_check.cpp $(CPU_DIR)CPU.h test-code.bin
stack_functions.o:     $(CPU_DIR)stack_functions.cpp $(CPU_DIR)CPU.h test-code.bin
processor_functions.o: $(CPU_DIR)processor_functions.cpp $(CPU_DIR)CPU.h test-code.bin
stack_dump.o:          $(CPU_DIR)stack_dump.cpp $(CPU_DIR)CPU.h test-code.bin

#####################################################################################################

asm:  $(OBJ_ASM)
	$(CC) -o asm $(OBJ_ASM)

main.o:                $(ASM_DIR)main.cpp $(ASM_DIR)assembler.h fact.asm test-code.bin test.code my_ASSSERT.h
assembler_functions.o: $(ASM_DIR)assembler_functions.cpp $(ASM_DIR)assembler.h fact.asm test-code.bin test.code my_ASSSERT.h

# valgrind:
# 	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./main
#
# clean:
# 	rm -rf *.o CPU/

