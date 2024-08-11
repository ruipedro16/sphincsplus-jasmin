# -*- Makefile -*-

# --------------------------------------------------------------------------------------------------

AS       ?= as
CC       ?= clang
CFLAGS   ?= -Wall -g -O3 -Wextra -Wpedantic

JASMIN   ?= jasminc
JAZZCT   ?= jazzct
JFLAGS   ?= ${JADDFLAGS}
JINCLUDE ?= -I Sphincs:../../../ref-jasmin/

JPP          ?= ../../../submodules/jasmin-preprocessor/jpp
PREPROCESSOR ?= ../../../submodules/jasmin-preprocessor/preprocessor

# --------------------------------------------------------------------------------------------------

OUTLEN_LIST ?= 1 31 32 33 63 64 65 95 96 97 127 128 129 135 136 137 159 160\
161 191 192 193 223 224 225 255 256 257 270 271 272

INLEN_LIST  ?= 1 31 32 33 63 64 65 95 96 97 127 128 129 135 136 137 159 160\
161 191 192 193 223 224 225 255 256 257 270 271 272

TLIST := $(foreach O,$(OUTLEN_LIST), $(foreach I,$(OUTLEN_LIST), $(O)_$(I)))

# --------------------------------------------------------------------------------------------------

GET_OUTLEN  = $(word 1, $(subst _, ,$*))
GET_INLEN   = $(word 2, $(subst _, ,$*))

# --------------------------------------------------------------------------------------------------

TESTS := $(addprefix bin/test_shake256_array_, $(TLIST))
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

%.o: %.s
	$(AS) $< -o $@

$(JPP_FILES):
.PRECIOUS: bin/test_shake256_array_%.jpp
bin/test_shake256_array_%.jpp: test_shake256_array.jazz | bin/
	cp $< $@
	$(JPP) $(JINCLUDE) -in $@ -out $@
	$(PREPROCESSOR) --input_file $@ --output_file $@ --task "fn:shake256  p:OUTLEN:$(GET_OUTLEN) p:INLEN:$(GET_INLEN)"

$(ASM_FILES):
.PRECIOUS: bin/test_shake256_array_%.s
bin/test_shake256_array_%.s: bin/test_shake256_array_%.jpp
	$(JASMIN) $(JFLAGS) $< -o $@

$(CT_FILES):
.PRECIOUS: bin/test_shake256_array_%.s
bin/test_shake256_array_%.ct: bin/test_shake256_array_%.jpp
	$(JAZZCT) --infer $< > $@

$(TESTS):
bin/test_shake256_array_%: bin/test_shake256_array_%.s | bin/
	$(CC) $(CFLAGS) -o $@ -DOUTLEN=$(GET_OUTLEN) -DINLEN=$(GET_INLEN) \
	-I../../common -I../ test_shake256_array.c $< ../../fips202.c

bin/test_shake256_array_%.out: bin/test_shake256_array_%
	@./$<


# --------------------------------------------------------------------------------------------------

bin/:
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r bin/

debug:
	@echo $(JPP_FILES)