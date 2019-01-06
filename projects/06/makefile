CC := gcc
CFLAGS += -Wall
all_sources := *.c
input_files := $(shell find . -type f -name *.asm)

.PHONY: all build assemble clean

all: build assemble

build: assembler debug

assemble:
	@for file in ${input_files}; do\
		echo "ASSEMBLE: $${file}" ;\
		basename $$file | xargs -I{} sh -c './assembler $$1 > ./actual/$${2%.*}.hack' -- $$file {};\
	done

clean:
	@rm -f assembler debug

define build-bin
  $(strip $(LINK.c)) $(OUTPUT_OPTION) $^
endef

assembler: $(all_sources)
	$(build-bin)

debug: CFLAGS += -g
debug: CPPFLAGS += -DDEBUG
debug: $(all_sources)
	$(build-bin)
