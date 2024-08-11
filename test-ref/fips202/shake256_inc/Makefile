# -*- Makefile -*-

# --------------------------------------------------------------------------------------------------

AS       ?= as
CC       ?= clang
CFLAGS   ?= -Wall -g -O3 -Wextra -Wpedantic

JASMIN   ?= jasminc
JAZZCT   ?= jazzct
JFLAGS   ?= ${JADDFLAGS}
JINCLUDE ?= -I Sphincs:../../../ref-jasmin/ -I Stdlib:../../../ref-jasmin/stdlib/

JPP          ?= ../../../submodules/jasmin-preprocessor/jpp
PREPROCESSOR ?= ../../../submodules/jasmin-preprocessor/preprocessor

# --------------------------------------------------------------------------------------------------

OUTLEN_LIST := 33 34 63 64 65 
INLEN_LIST  := 135 136 137 271 272 273 407 408 409

TLIST := $(foreach O,$(OUTLEN_LIST), $(foreach I,$(INLEN_LIST), $(O)_$(I)))

# --------------------------------------------------------------------------------------------------

GET_OUTLEN  = $(word 1, $(subst _, ,$*))
GET_INLEN   = $(word 2, $(subst _, ,$*))

# --------------------------------------------------------------------------------------------------

TESTS := $(addprefix bin/test_shake256_inc_, $(TLIST))
OUT   := $(addsuffix .out, $(TESTS))

JPP_FILES := $(addsuffix .jpp, $(TESTS))
ASM_FILES := $(JPP_FILES:.jpp=.s)
OBJ_FILES := $(ASM_FILES:.s=.o)
CT_FILES  := $(JPP_FILES:.jpp=.ct)

# --------------------------------------------------------------------------------------------------

default: $(TESTS)
all: $(ASM_FILES) $(OBJ_FILES) $(CT_FILES) $(TESTS)
run: $(OUT)

jpp_files: $(JPP_FILES)
asm_files: $(ASM_FILES)
obj_files: $(OBJ_FILES)
check_ct:  $(CT_FILES)

# --------------------------------------------------------------------------------------------------

$(OBJ_FILES):
%.o: %.s
	$(AS) $< -o $@

$(JPP_FILES):
.PRECIOUS: bin/test_shake256_inc_%.jpp
bin/test_shake256_inc_%.jpp: test_shake256_inc.jazz | bin/
	cp $< $@
	$(JPP) $(JINCLUDE) -in $@ -out $@
	$(PREPROCESSOR) --input_file $@ --output_file $@ --task "fn:shake256_inc_absorb_template_jazz p:INLEN:$(GET_INLEN)" "fn:shake256_inc_squeeze_template_jazz p:OUTLEN:$(GET_OUTLEN)"

$(ASM_FILES):
.PRECIOUS: bin/test_shake256_inc_%.s
bin/test_shake256_inc_%.s: bin/test_shake256_inc_%.jpp | bin/
	$(JASMIN) $(JFLAGS) $(JINCLUDE) $< -o $@

$(CT_FILES):
.PRECIOUS: bin/test_shake256_inc_%.s
bin/test_shake256_inc_%.ct: bin/test_shake256_inc_%.jpp
	$(JAZZCT) --infer $< > $@

$(TESTS):
bin/test_shake256_inc_%: bin/test_shake256_inc_%.s | bin/
	$(CC) $(CFLAGS) -o $@ -DOUTLEN=$(GET_OUTLEN) -DINLEN=$(GET_INLEN) \
	-I../../common -I../ test_shake256_inc.c $< ../../fips202.c

$(OUT):
bin/test_shake256_inc_%.out: bin/test_shake256_inc_%
	@./$<

# --------------------------------------------------------------------------------------------------

bin/:
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r bin/
