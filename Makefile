# Name: Gilad Bitton
# RedID: 130621085
# Makefile for Restaurant Flow Control

CXX = g++
CXXFLAGS = -Wall -Wextra -g -pthread

# Paths
SRC_DIR = code_files/cpp_files
INC_DIR = code_files/header_files
OBJ_DIR = object_files

# Files
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/log.o $(OBJ_DIR)/producer.o $(OBJ_DIR)/consumer.o
DEPS = $(INC_DIR)/log.h $(INC_DIR)/seating.h $(INC_DIR)/restaurant.h \
       $(INC_DIR)/producer.h $(INC_DIR)/consumer.h
TARGET = dineseating

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile step
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)