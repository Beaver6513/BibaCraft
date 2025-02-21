# Variables
CXX       = g++
CXXFLAGS  = -std=c++17 -O2
# Additional include directories (e.g., -I./include)
INCLUDES  = 
# Additional library directories (e.g., -L./lib)
LIBDIRS   = 
# Libraries to link (e.g., -lmylib)
LIBS      = -lvulkan -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl
# -pthread -o test glfwtest.cpp -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl

# Directories and file settings
SRC_DIR   = src
BUILD_DIR = build
SRCS      = $(wildcard $(SRC_DIR)/*.cpp)
OBJS      = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TARGET    = $(BUILD_DIR)/main

# Default target
all: $(TARGET)

# Link the object files to create the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBDIRS) -o $@ $^ $(LIBS) 

# Compile each .cpp file into an object file inside the build directory.
# The pipe symbol '|' before '$(BUILD_DIR)' is an order-only dependency that ensures the build directory exists.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET)
	@./$(TARGET)

# Clean up generated files by removing the build directory
clean:
	rm -rf $(BUILD_DIR)
	

