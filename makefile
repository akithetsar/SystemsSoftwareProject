CC = g++
CFLAGS = -Iinclude -Wall -Wextra -g

BUILD = build
SRC = src

TARGET = $(BUILD)/assembler

BISON_SRC = $(BUILD)/parser.tab.c
BISON_HDR = $(BUILD)/parser.tab.h
FLEX_SRC  = $(BUILD)/lex.yy.c

OBJS = \
	$(BUILD)/parser.tab.o \
	$(BUILD)/lex.yy.o \
	$(BUILD)/assembler.o

all: $(BUILD) $(TARGET)

# ------------------------
# create build directory
# ------------------------
$(BUILD):
	mkdir -p $(BUILD)

# ------------------------
# Bison
# ------------------------
$(BISON_SRC) $(BISON_HDR): parser.y
	bison -d -o $(BISON_SRC) parser.y

# ------------------------
# Flex
# ------------------------
$(FLEX_SRC): lexer.l
	flex -o $(FLEX_SRC) lexer.l

# ------------------------
# compile bison output
# ------------------------
$(BUILD)/parser.tab.o: $(BISON_SRC)
	$(CC) $(CFLAGS) -c $(BISON_SRC) -o $@

# ------------------------
# compile flex output
# ------------------------
$(BUILD)/lex.yy.o: $(FLEX_SRC)
	$(CC) $(CFLAGS) -c $(FLEX_SRC) -o $@

# ------------------------
# your code
# ------------------------
$(BUILD)/assembler.o: $(SRC)/assembler.c
	$(CC) $(CFLAGS) -c $(SRC)/assembler.c -o $@

# ------------------------
# link everything
# ------------------------
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# ------------------------
# clean
# ------------------------
clean:
	rm -rf $(BUILD)

rebuild: clean all