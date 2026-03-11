# Compiler and flags
CC = gcc

# Tells the compiler to show warnings and treat warnings as errors (preventing compilation)
# Note, the static flag is necessary!
CFLAGS ?= -static -Wall -Werror 

# Causes the compiler to generate dependency files (.d) for each source file
# Make uses these to determine which files need to be recompiled when a header changes
DEPFLAGS = -MMD -MP

# The directory where build artifacts will be placed
BUILD_DIR = build

# Define targets here (without their .out extension)
# Each TARGET will be built from TARGET.c into build/TARGET.out
# So, if you have hello.c, then you would add "hello" to the TARGETS variable.
# You should add any new test files you create here.
TARGETS = 

# Derive executables, sources, objects, and dependencies
EXECUTABLES = $(addprefix $(BUILD_DIR)/, $(TARGETS:=.out))
ALL_SOURCES = $(TARGETS:=.c)
ALL_OBJECTS = $(ALL_SOURCES:%.c=$(BUILD_DIR)/%.o)
ALL_DEPS = $(ALL_OBJECTS:.o=.d)

# Phony targets
.PHONY: all clean compile_commands

# Secondary targets tell make about files that don't need to be kept around.
.SECONDARY: $(ALL_OBJECTS)

# Default target
all: $(EXECUTABLES)

# Link rule for all executables
$(BUILD_DIR)/%.out: $(BUILD_DIR)/%.o
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build complete: $@"

# Compile source files to object files and generate dependencies
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned build artifacts"

# Generate compile_commands.json for IDE tooling
compile_commands:
	bear -- $(MAKE) all

# Print variables for debugging
print-%:
	@echo $* = $($*)

# Include generated dependency files for rebuilds
-include $(ALL_DEPS)
