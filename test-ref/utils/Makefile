# -*- Makefile -*-

# --------------------------------------------------------------------------------------------------

AS       ?= as
CC       ?= clang
CFLAGS   ?= -Wall -g -O3 -Wextra -Wpedantic

JASMIN   ?= jasminc
JAZZCT   ?= jazzct
JFLAGS   ?= ${JADDFLAGS}
JINCLUDE ?= -I Sphincs:../../ref-jasmin/ -I Stdlib:../../ref-jasmin/stdlib/

JPP          ?= ../../submodules/jasmin-preprocessor/jpp
PREPROCESSOR ?= ../../submodules/jasmin-preprocessor/preprocessor

# --------------------------------------------------------------------------------------------------

HASH_LIST     := shake
PARAMS_LIST   := 128f 128s 192f 192s 256f 256s
THASH_LIST    := simple robust

TLIST := $(foreach H,$(HASH_LIST), $(foreach P,$(PARAMS_LIST), $(foreach T,$(THASH_LIST),$(H)_$(P)_$(T))))

# --------------------------------------------------------------------------------------------------

GET_HASH  = $(word 1, $(subst _, ,$*))
GET_PARAM = $(word 2, $(subst _, ,$*))

GET_PARAMS   = sphincs-$(GET_HASH)-$(GET_PARAM)
GET_THASH    = $(word 3, $(subst _, ,$*))

# --------------------------------------------------------------------------------------------------

TESTS := $(addprefix bin/test_utils_, $(TLIST))
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

SOURCES = $(filter-out ../thash_%.c ../utils.c ../rng.c ../PQCgenKAT_sign.c, $(wildcard ../*.c)) \
           ../thash_$(GET_HASH)_$(GET_THASH).c

TEST_FLAGS = -DTEST_COMPUTE_ROOT

# --------------------------------------------------------------------------------------------------

$(OBJ_FILES):
%.o: %.s
	$(AS) $< -o $@

$(JPP_FILES):
.PRECIOUS: bin/test_utils_%.jpp
bin/test_utils_%.jpp: | bin/
	cp test_utils.jazz $@
	sed -i "1 i\from Sphincs require \"params/params-sphincs-$(GET_HASH)-$(GET_PARAM).jinc\"" $@
	sed -i "2 i\from Sphincs require \"thash/thash_$(GET_HASH)_$(GET_THASH).jtmpl\"" $@
	$(JPP) $(JINCLUDE) -in $@ -out $@
	$(PREPROCESSOR) --input_file $@ --output_file $@

$(ASM_FILES):
.PRECIOUS: bin/test_utils_%.s
bin/test_utils_%.s: bin/test_utils_%.jpp
	$(JASMIN) $(JFLAGS) $< -o $@

$(CT_FILES):
.PRECIOUS: bin/test_utils_%.s
bin/test_utils_%.ct: bin/test_utils_%.jpp
	($(JAZZCT) --infer $< > $@) || true

$(TESTS):
bin/test_utils_%: bin/test_utils_%.s | bin/
	$(CC) $(CFLAGS) -o $@ -DPARAMS=$(GET_PARAMS) -DTHASH=$(GET_THASH) $(TEST_FLAGS) \
	-I../common -I../ -I../params test_utils.c $<  $(SOURCES)

$(OUT):
bin/test_utils_%.out: bin/test_utils_%
	@./$<

# --------------------------------------------------------------------------------------------------


bin/:
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r bin/
