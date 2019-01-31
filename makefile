SHELL := /bin/bash
CC := gcc
CFLAGS += -Wall
sources_of_assembler := assembler/*.c
sources_of_vm_translater := vm-translater/*.h vm-translater/*.c
asm_files := $(shell find projects/06/ -type f -name *.asm)
hack_dir := projects/06/actual
vm_dirs := $(shell echo projects/0{7,8}/**/**)

define build-bin
  $(strip $(LINK.c)) $(OUTPUT_OPTION) $^
endef

.PHONY: lint assemble vm-translate

build-all: bin build-assembler build-vm-translater

build-assembler: bin bin/assembler bin/assembler-debug

build-vm-translater: bin bin/vm-translater bin/vm-translater-debug

bin:
	@mkdir -p bin

clean: bin
	@rm -rf $^

lint:
	@cpplint $(sources_of_assembler) $(sources_of_vm_translater)

bin/assembler: $(sources_of_assembler)
	$(build-bin)

bin/assembler-debug: CFLAGS += -g
bin/assembler-debug: CPPFLAGS += -DDEBUG
bin/assembler-debug: $(sources_of_assembler)
	$(build-bin)

bin/vm-translater: $(sources_of_vm_translater)
	$(build-bin)

bin/vm-translater-debug: CFLAGS += -g
bin/vm-translater-debug: CPPFLAGS += -DDEBUG
bin/vm-translater-debug: $(sources_of_vm_translater)
	$(build-bin)

assemble: bin bin/assembler
	@mkdir -p $(hack_dir)
	@for input in $(asm_files); do\
		output="$(hack_dir)/$$(basename -s .asm $$input).hack";\
		echo "ASSEMBLE: $$input > $$output";\
		bin/assembler "$$input" > "$$output";\
	done

vm-translate: bin bin/vm-translater
	@for input in $(vm_dirs); do\
		output="$${input}/$$(basename $$input).asm";\
		echo "VM-TRANSLATE: $$input > $$output";\
		bin/vm-translater "$$input" > "$$output";\
	done
