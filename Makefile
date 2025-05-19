APP_NAME = demo
LIB_NAME = lib
OPT_NAME = loop

DEBUG_FILES = bitrgbled data- gdb.txt head- q1.txt q2.bin

CFLAGS = -Wall -Wextra -Werror -g
CPPFLAGS = -I src -MP -MMD
VALFLAGS = --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src

APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a
OPT_PATH = $(BIN_DIR)/$(OPT_NAME)

SRC_EXT = c

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

LIB_SOURCES = $(shell find $(SRC_DIR)/$(LIB_NAME) -name '*.$(SRC_EXT)')
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

OPT_SOURCES = $(shell find $(SRC_DIR)/$(OPT_NAME) -name '*.$(SRC_EXT)')
OPT_OBJECTS = $(OPT_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

DEPS = $(APP_OBJECTS:.o=.d) $(LIB_OBJECTS:.o=.d)

.PHONY: all
all: $(APP_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(APP_PATH) $(LIB_PATH) $(DEBUG_FILES)
	find $(OBJ_DIR) -name '*.o' -exec $(RM) '{}' \;
	find $(OBJ_DIR) -name '*.d' -exec $(RM) '{}' \;

.PHONY: run
run: $(APP_PATH)
	$(APP_PATH)

.PHONY: valgrind
valgrind: $(APP_PATH)
	valgrind $(VALFLAGS) $(APP_PATH)

.PHONY: gdb
gdb: $(APP_PATH)
	gdb $(APP_PATH) <gdb_input.txt

.PHONY: opt_0
opt: $(LIB_PATH)
	$(CC) $(CFLAGS) -O0 -I src $(OPT_SOURCES) $< -o $(OPT_PATH)_$@_0
	$(CC) $(CFLAGS) -O1 -I src $(OPT_SOURCES) $< -o $(OPT_PATH)_$@_1
	$(CC) $(CFLAGS) -O2 -I src $(OPT_SOURCES) $< -o $(OPT_PATH)_$@_2
	$(CC) $(CFLAGS) -O3 -I src $(OPT_SOURCES) $< -o $(OPT_PATH)_$@_3
