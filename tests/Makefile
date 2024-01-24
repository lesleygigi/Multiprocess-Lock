SRC_DIR := tests/src
BIN_DIR := tests/bin

SRC := $(wildcard $(SRC_DIR)/*.c)
BIN := $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%)

CFLAGS  := -g -Wall -Werror
CPPFLAGS := -I.
LDFLAGS := -L. -Wl,-rpath=.
# each source file links all libraries for simplicity, even if not used
# LDLIBS  := -lpthread -lcounter -llist -lhash -lmq
LDLIBS  := -lpthread -lmq

.PHONY: all clean

all: $(BIN)

$(BIN_DIR)/%: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

$(BIN_DIR):
	mkdir -p $@

clean:
	$(RM) -rv $(BIN_DIR) $(OBJ_DIR)
