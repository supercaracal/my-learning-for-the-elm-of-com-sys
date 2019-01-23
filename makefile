CC := gcc
CFLAGS += -Wall
sources_of_assembler := assembler/*.c
sources_of_vm_translater := vm-translater/*.h vm-translater/*.c
asm_files := $(shell find ./projects/06/ -type f -name *.asm)

define build-bin
  $(strip $(LINK.c)) $(OUTPUT_OPTION) $^
endef

.PHONY: assemble

build-all: bin build-assembler build-vm-translater

build-assembler: bin bin/assembler  bin/assembler-debug

build-vm-translater: bin bin/vm-translater bin/vm-translater-debug

bin:
	@mkdir -p bin

clean: bin
	@rm -rf $^

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

assemble:
	@for file in ${asm_files}; do\
		echo "ASSEMBLE: $${file}" ;\
		basename $$file | xargs -I{} sh -c 'bin/assembler $$1 > projects/06/actual/$${2%.*}.hack' -- $$file {};\
	done
