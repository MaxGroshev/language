MODULES = assembler CPU
all:
	for dir in $(MODULES); do $(MAKE) -C $$dir; done
	./asm
	./cpu_t

.PHONY: valgrind
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./asm
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./cpu_t
