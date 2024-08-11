# -*- Makefile -*-

# --------------------------------------------------------------------------------------------------

AS       ?= as
CC       ?= clang
CFLAGS   ?= -Wall -g -O3 -Wextra -Wpedantic

JASMIN   ?= jasminc
JAZZCT   ?= jazzct
JFLAGS   ?= ${JADDFLAGS}
JINCLUDE ?= -I Sphincs:../../../ref-jasmin/

# --------------------------------------------------------------------------------------------------

TESTS := bin/test_shake256_ptr
OUT   := $(addsuffix .out, $(TESTS))

ASM_FILES := $(JPP_FILES:.jpp=.s)
OBJ_FILES := $(ASM_FILES:.s=.o)
CT_FILES  := $(JPP_FILES:.jpp=.ct)

# --------------------------------------------------------------------------------------------------

default: $(TESTS)
all: $(ASM_FILES) $(OBJ_FILES) $(CT_FILES) $(TESTS)
run: $(OUT)

asm_files: $(ASM_FILES)
obj_files: $(OBJ_FILES)
check_ct:  $(CT_FILES)

# --------------------------------------------------------------------------------------------------

%.o: %.s
	$(AS) $< -o $@

.PRECIOUS: bin/test_shake256_ptr.s
bin/test_shake256_ptr.s: test_shake256_ptr.jazz | bin/
	$(JASMIN) $(JFLAGS) $(JINCLUDE) $< -o $@

bin/test_shake256_ptr: bin/test_shake256_ptr.s | bin/
	$(CC) $(CFLAGS) -o $@ -I../../common -I../ test_shake256_ptr.c $< ../../fips202.c

bin/test_shake256_ptr.ct: test_shake256_ptr.jazz | bin/
	$(JAZZCT) --infer $< > $@

bin/test_shake256_ptr.out: bin/test_shake256_ptr
	@./$<

# --------------------------------------------------------------------------------------------------

bin/:
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r bin/
