# -*- Makefile -*-

# --------------------------------------------------------------------------------------------------

AS       ?= as
CC       ?= clang
CFLAGS   ?= -Wall -g -O3 -Wextra -Wpedantic

JASMIN   ?= jasminc
JAZZCT   ?= jazzct
JFLAGS   ?= ${JADDFLAGS}
JINCLUDE ?= -I Sphincs:../../ref-jasmin/

JPP          ?= ../../submodules/jasmin-preprocessor/jpp
PREPROCESSOR ?= ../../../submodules/jasmin-preprocessor/preprocessor

# --------------------------------------------------------------------------------------------------

HASH_LIST     := shake
PARAMS_LIST   := 128f 128s 192f 192s 256f 256s
THASH_LIST    := simple robust

# note on THASH
# - address.c requires code from utils.c which requires code from thash_*.c
# - even though address.c only uses code from utils that does not depend on thash
#   to keep things simple we include it in testing this way. In principle, there
#   should be no need to add robust to the THASH_LIST when available as it would
#   only repeat the ~same tests
TLIST := $(foreach H,$(HASH_LIST), $(foreach P,$(PARAMS_LIST), $(foreach T,$(THASH_LIST), $(H)_$(P)_$(T))))

TESTS := $(addprefix bin/test_address_, $(TLIST))
OUT   := $(addsuffix .out, $(TESTS))

JPP_FILES := $(addsuffix .jpp, $(TESTS))
ASM_FILES := $(JPP_FILES:.jpp=.s)
OBJ_FILES := $(ASM_FILES:.s=.o)
CT_FILES  := $(JPP_FILES:.jpp=.ct)

# --------------------------------------------------------------------------------------------------

GET_HASH  = $(word 1, $(subst _, ,$*))
GET_PARAM = $(word 2, $(subst _, ,$*))
GET_THASH = $(word 3, $(subst _, ,$*))

# --------------------------------------------------------------------------------------------------

SOURCES = $(filter-out ../thash_%.c, $(wildcard ../*.c)) ../thash_$(GET_HASH)_$(GET_THASH).c

TEST_FLAGS := -DTEST_ADDRESS

# --------------------------------------------------------------------------------------------------

default: $(TESTS)
all: $(JPP_FILES) $(ASM_FILES) $(OBJ_FILES) $(CT_FILES) $(TESTS)
run: $(OUT)

jpp_files: $(JPP_FILES)
asm_files: $(ASM_FILES)
obj_files: $(OBJ_FILES)
check_ct:  $(CT_FILES)

# --------------------------------------------------------------------------------------------------

%.o: %.s
	$(AS) $< -o $@

$(JPP_FILES):
.PRECIOUS: bin/test_address_%.jpp
bin/test_address_%.jpp: | bin/
	cp test_address.jazz $@
	sed -i "1 i\from Sphincs require \"params/params-sphincs-$(GET_HASH)-$(GET_PARAM).jinc\"" $@
	$(JPP) $(JINCLUDE) -in $@ -out $@

.PRECIOUS: bin/test_address_%.s
bin/test_address_%.s: bin/test_address_%.jpp
	$(JASMIN) $(JFLAGS) $< -o $@

$(TESTS):
bin/test_address_%: bin/test_address_%.s | bin/
	$(CC) $(CFLAGS) -o $@ -DPARAMS=sphincs-$(GET_HASH)-$(GET_PARAM) $(TEST_FLAGS) \
	-I../common -I../ -I../params test_address.c $< $(SOURCES)

.PRECIOUS: bin/test_address_%.ct
bin/test_address_%.ct: bin/test_address_%.jpp
	$(JAZZCT) --infer $< > $@

bin/test_address_%.out: bin/test_address_%
	@./$<

# --------------------------------------------------------------------------------------------------

bin/:
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r bin/
