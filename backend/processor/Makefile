MODULES = assembler CPU
all:
	for dir in $(MODULES); do $(MAKE) -C $$dir; done
	./asm
	./cpu_t
core.autocrlf = input
