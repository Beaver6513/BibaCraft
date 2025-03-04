# Variables
CXX       = g++
CXXFLAGS  = -std=c++17 -Wno-deprecated-declarations -Wno-unused-result
# Additional include directories (e.g., -I./include)
INCLUDES  = 
# Additional library directories (e.g., -L./lib)
LIBDIRS   = 
# Libraries to link (e.g., -lmylib)
LIBS      = -lvulkan -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lX11 -lrt -ldl -lpthread -lshaderc_combined

# Directories and file settings
SRC_DIR   = src
BUILD_DIR = build
SRCS			= $(shell find . -name '*.cpp')
OBJS      = $(addprefix build/,$(notdir $(SRCS:.cpp=.o)))
TARGET    = $(BUILD_DIR)/main

VPATH = $(sort $(dir $(SRCS)))

# Default target
all: $(TARGET)

# Link the object files to create the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBDIRS) -o $@ $^ $(LIBS) 

# Compile each .cpp file into an object file inside the build directory.
# The pipe symbol '|' before '$(BUILD_DIR)' is an order-only dependency that ensures the build directory exists.
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET)
	@./$(TARGET)

# Clean up generated files by removing the build directory
clean:
	rm -rf $(BUILD_DIR)
	

