CC = g++
CFLAGS = -Iinclude -Wall -Wextra -g

BUILD = build
SRC = src

# ---------------------------------------------------------------
# Three separate binaries, one per toolchain stage.
# Each one only pulls in the .c files it actually needs, so they
# no longer fight over a single main().
# ---------------------------------------------------------------

ASM_TARGET  = $(BUILD)/asembler
LNK_TARGET  = $(BUILD)/linker
EMU_TARGET  = $(BUILD)/emulator

BISON_SRC = $(BUILD)/parser.tab.c
BISON_HDR = $(BUILD)/parser.tab.h
FLEX_SRC  = $(BUILD)/lex.yy.c

# assembler: lexer/parser + the assembler driver + everything the
# assembler-side code (sections/symbols/literal pool/instruction
# encoding/relocations/ELF writer) needs.
ASM_SRCS = \
	$(SRC)/assembler.c \
	$(SRC)/elf_writer.c \
	$(SRC)/instruction_encoding.c \
	$(SRC)/literal_pool.c \
	$(SRC)/sections.c \
	$(SRC)/symbol_table.c \
	$(SRC)/relocations.c

ASM_OBJS = $(BUILD)/parser.tab.o $(BUILD)/lex.yy.o \
	$(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(ASM_SRCS))

# linker: self-contained module cluster, does not touch the
# assembler's in-memory section/symbol tables at all.
LNK_SRCS = \
	$(SRC)/linker.c \
	$(SRC)/object_reader.c \
	$(SRC)/relocator.c \
	$(SRC)/section_placer.c \
	$(SRC)/symbol_resolver.c \
	$(SRC)/output_writer.c


LNK_OBJS = $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(LNK_SRCS))

# emulator: currently just the one file.
EMU_SRCS = $(SRC)/emulator.c
EMU_OBJS = $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(EMU_SRCS))

.PHONY: all clean rebuild asembler linker emulator

# emulator is left out of the default target on purpose: src/emulator.c
# does not define main() yet in this project, so it can't link into a
# binary until that's added. Run `make emulator` explicitly once it does.
all: $(BUILD) asembler linker

asembler: $(ASM_TARGET)
linker: $(LNK_TARGET)
emulator: $(EMU_TARGET)

# ------------------------
# create build directory
# ------------------------
$(BUILD):
	mkdir -p $(BUILD)

# ------------------------
# Bison
# ------------------------
$(BISON_SRC) $(BISON_HDR): parser.y | $(BUILD)
	bison -d -o $(BISON_SRC) parser.y

# ------------------------
# Flex
# ------------------------
$(FLEX_SRC): lexer.l | $(BUILD)
	flex -o $(FLEX_SRC) lexer.l

$(BUILD)/parser.tab.o: $(BISON_SRC)
	$(CC) $(CFLAGS) -c $(BISON_SRC) -o $@

$(BUILD)/lex.yy.o: $(FLEX_SRC)
	$(CC) $(CFLAGS) -c $(FLEX_SRC) -o $@

# ------------------------
# generic rule for everything else in src/
# ------------------------
$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# ------------------------
# link the three binaries
# ------------------------
$(ASM_TARGET): $(ASM_OBJS)
	$(CC) $(ASM_OBJS) -o $@

$(LNK_TARGET): $(LNK_OBJS)
	$(CC) $(LNK_OBJS) -o $@

# NOTE: src/emulator.c does not define main() yet (per the project's own
# todo.txt), so this target will fail to *link* until that's added -
# it will still compile to build/emulator.o cleanly in the meantime.
$(EMU_TARGET): $(EMU_OBJS)
	$(CC) $(EMU_OBJS) -o $@

# ------------------------
# clean
# ------------------------
clean:
	rm -rf $(BUILD)

rebuild: clean all
