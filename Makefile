ASM = nasm
LD = ld 
NASM_FLAGS = -f elf64
LD_FLAGS = 
SOURCES = hello.asm 
OBJECTS = hello.o
EXECUTABLE = hello

all: $(EXECUTABLE)

$(OBJECTS): $(SOURCES)
	$(ASM) $(NASM_FLAGS) $(SOURCES) -o $(OBJECTS)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LD_FLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)
