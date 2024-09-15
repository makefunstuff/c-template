CFLAGS = -Wall -Wextra

# example how to include lib paths
# $(shell pkg-config --libs <LIBS>)
LIBS =

# same for include but different flag
# $(shell pkg-config --cflags <LIBS>)
INCLUDE =


SRC = $(wildcard src/*.c)
OUT_DIR = build
OUT_FILE = $(OUT_DIR)/<PROJECT_NAME>

# object file in the build directory
OBJ = $(OUT_DIR)/$(notdir $(SRC:.c=.o))

# the main target
<PROJECT_NAME>: $(OUT_FILE)

# rule to create the output file from the object file
$(OUT_FILE): $(OBJ)
	mkdir -p $(OUT_DIR)
	clang $(CFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)

# rule to compile the source file into the object file in build directory
$(OBJ): $(SRC)
	mkdir -p $(OUT_DIR)
	clang $(CFLAGS) $(INCLUDE) -c -o $@ $<

# run the program
run: <PROJECT_NAME>
	./$(OUT_FILE)
