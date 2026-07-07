CC = g++
CFLAGS = -Iinclude -Wall -Wextra -g

BUILD = build
SRC = src

TARGET = $(BUILD)/assembler

BISON_SRC = $(BUILD)/parser.tab.c
BISON_HDR = $(BUILD)/parser.tab.h
FLEX_SRC  = $(BUILD)/lex.yy.c

# Automatically pick up every .c file in src/
SRC_FILES = $(wildcard $(SRC)/*.c)
SRC_OBJS  = $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(SRC_FILES))

OBJS = \
	$(BUILD)/parser.tab.o \
	$(BUILD)/lex.yy.o \
	$(SRC_OBJS)

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
# your code (pattern rule covers every src/*.c -> build/*.o)
# ------------------------
$(BUILD)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

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