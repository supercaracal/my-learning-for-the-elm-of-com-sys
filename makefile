SHELL := /bin/bash
CC := gcc
CFLAGS += -Wall
LEX := flex
LFLAGS += -X
YACC := bison
YFLAGS += -d -y
sources_of_assembler := assembler/*.c
sources_of_vm_translator := vm-translator/*.h vm-translator/*.c
sources_of_compiler = compiler/*.h compiler/*.c
asm_files := $(shell find projects/06/ -type f -name *.asm)
hack_dir := projects/06/actual
vm_dirs := $(shell echo projects/0{7,8}/**/**)

define build-bin
  $(strip $(LINK.c)) $(OUTPUT_OPTION) $^
endef

.PHONY: lint assemble vm-translate

build-all: bin build-assembler build-vm-translator build-compiler

build-assembler: bin bin/assembler bin/assembler-debug

build-vm-translator: bin bin/vm-translator bin/vm-translator-debug

build-scanner: compiler/lex.yy.c

build-parser: compiler/y.tab.h compiler/y.tab.c

build-compiler: bin build-scanner build-parser bin/compiler bin/compiler-debug

bin:
	@mkdir -p bin

clean: bin
	@rm -rf $^

lint:
	@cpplint $(sources_of_assembler) $(sources_of_vm_translator)

bin/assembler: $(sources_of_assembler)
	$(build-bin)

bin/assembler-debug: CFLAGS += -g
bin/assembler-debug: CPPFLAGS += -DDEBUG
bin/assembler-debug: $(sources_of_assembler)
	$(build-bin)

bin/vm-translator: $(sources_of_vm_translator)
	$(build-bin)

bin/vm-translator-debug: CFLAGS += -g
bin/vm-translator-debug: CPPFLAGS += -DDEBUG
bin/vm-translator-debug: $(sources_of_vm_translator)
	$(build-bin)

compiler/lex.yy.c: compiler/jack.l
	$(strip $(LEX.l)) $^ > $@

compiler/y.tab.h compiler/y.tab.c: compiler/jack.y
	$(strip $(YACC.y)) $^
	@mv y.tab.* compiler/

bin/compiler: $(sources_of_compiler)
	$(build-bin)

bin/compiler-debug: CFLAGS += -g
bin/compiler-debug: CPPFLAGS += -DDEBUG
bin/compiler-debug: $(sources_of_compiler)
	$(build-bin)

assemble: bin bin/assembler
	@mkdir -p $(hack_dir)
	@for input in $(asm_files); do\
		output="$(hack_dir)/$$(basename -s .asm $$input).hack";\
		echo "ASSEMBLE: $$input > $$output";\
		bin/assembler "$$input" > "$$output";\
	done

vm-translate: bin bin/vm-translator
	@for input in $(vm_dirs); do\
		output="$${input}/$$(basename $$input).asm";\
		echo "VM-TRANSLATE: $$input > $$output";\
		bin/vm-translator "$$input" > "$$output";\
	done
