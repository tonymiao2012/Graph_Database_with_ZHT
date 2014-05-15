INC_DIR=include
SRC_DIR=src
TEST_DIR=test
LIB_DIR=lib
OBJ_DIR=obj
PROTOBUF_LIB_DIR=/usr/local/lib
PROTOBUF_INC_DIR=/usr/local/include
LINUX_GNU_LIB_DIR=/lib/x86_64-linux-gnu

CC=g++
CXXFLAGS=-c -g3 -O2 -Wall -I$(INC_DIR) -I$(PROTOBUF_INC_DIR) -std=c++0x
LDFLAGS=-L$(LIB_DIR) -lzht -L$(PROTOBUF_LIB_DIR) -lprotobuf-c -lprotobuf -L$(LINUX_GNU_LIB_DIR) -lpthread

SRC=$(foreach x, $(SRC_DIR), $(wildcard $(addprefix ${x}/, *.cpp */*.cpp */*/*.cpp)))
TEST=$(foreach x, $(TEST_DIR), $(wildcard $(addprefix ${x}/, *.cpp */*.cpp */*/*.cpp)))
HEADERS=$(foreach x, $(INC_DIR), $(wildcard $(addprefix ${x}/, *.h */*.h */*/*.h)))

SRC_OBJS=$(addprefix $(OBJ_DIR)/, $(SRC:%.cpp=%.o))
TEST_OBJS=$(addprefix $(OBJ_DIR)/, $(TEST:%.cpp=%.o))
ALL_OBJS=$(SRC_OBJS) $(TEST_OBJS)
OBJ_SUB_DIRS=$(dir $(ALL_OBJS))

EXE=graphdb
TEXE=test_graphdb

all: $(EXE) 
test: $(TEXE)

$(EXE): $(SRC_OBJS) $(HEADERS)
	$(CC) $(SRC_OBJS) $(LDFLAGS) -o $@ 

$(TEXE): $(filter-out $(OBJ_DIR)/src/command_line_main.o, $(ALL_OBJS)) $(HEADERS)
	$(CC) $(filter-out $(OBJ_DIR)/src/command_line_main.o, $(ALL_OBJS)) $(LDFLAGS) -o $@ 

$(ALL_OBJS): $(OBJ_DIR)/%.o : %.cpp $(HEADERS)
	mkdir -p $(OBJ_SUB_DIRS)
	$(CC) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean: 
	rm -rf $(OBJ_DIR) $(EXE) $(TEXE) *.log
