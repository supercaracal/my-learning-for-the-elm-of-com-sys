CC := gcc
CFLAGS += -Wall
all_sources := *.h *.c

.PHONY: build clean

build: vm-translater debug

clean:
	@rm -f vm-translater debug

define build-bin
  $(strip $(LINK.c)) $(OUTPUT_OPTION) $^
endef

vm-translater: $(all_sources)
	$(build-bin)

debug: CFLAGS += -g
debug: CPPFLAGS += -DDEBUG
debug: $(all_sources)
	$(build-bin)
