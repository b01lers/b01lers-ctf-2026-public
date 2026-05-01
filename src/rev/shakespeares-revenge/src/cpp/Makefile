CXX := g++
CXXFLAGS := -std=c++17 -g -Wall -Wextra -I. -I./src -I./build -MMD -MP -static
LDFLAGS := -lfl

CHARACTER_STATE_DEBUG ?= 0
SPL_ENABLE_IO_STYLES ?= 0

CXXFLAGS += -DSPL_ENABLE_IO_STYLES=$(SPL_ENABLE_IO_STYLES)

ifeq ($(CHARACTER_STATE_DEBUG),1)
CXXFLAGS += -DCHARACTER_STATE_DEBUG
endif

BUILD_DIR := build
TARGET := shakespeare_cpp

PARSER_SRC := $(BUILD_DIR)/parser.cpp
PARSER_HDR := $(BUILD_DIR)/parser.hpp
LEXER_SRC := $(BUILD_DIR)/lexer.cpp

SOURCES := \
	$(PARSER_SRC) \
	$(LEXER_SRC) \
	src/parser_api.cpp \
	src/interpreter.cpp \
	src/main.cpp

OBJECTS := $(SOURCES:.cpp=.o)
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Bison parser generation
$(PARSER_SRC) $(PARSER_HDR): parser.y | $(BUILD_DIR)
	bison -d -Wall -Wcounterexamples -o $(PARSER_SRC) parser.y

# Flex lexer generation
$(LEXER_SRC): lexer.l $(PARSER_HDR) | $(BUILD_DIR)
	flex -o $(LEXER_SRC) lexer.l

# Explicit dependency: parser object needs generated header
$(BUILD_DIR)/parser.o: $(PARSER_HDR)

# Explicit dependency: lexer object needs generated header
$(BUILD_DIR)/lexer.o: $(PARSER_HDR)

# Link target
$(TARGET): $(PARSER_SRC) $(PARSER_HDR) $(LEXER_SRC) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Compile rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Include auto-generated dependency files
-include $(DEPS)

clean:
	rm -f $(TARGET)
	rm -rf $(BUILD_DIR)
	rm -f src/*.o src/*.d