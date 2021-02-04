PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BIN_DIR := bin
SRC_DIR := src

BUILD_DIR := $(BIN_DIR)/build

LIB_DIR := $(BIN_DIR)/build/libs
OBJ_DIR := $(BIN_DIR)/build/objs

SRC_DIRS := ${shell find ${SRC_DIR} -type d -print}
OBJ_DIRS := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(patsubst $(SRC_DIR),,$(SRC_DIRS)))

SRC_FILES := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

COMPONENTS := server

SO_FILES := $(patsubst %,$(LIB_DIR)/lib%.so,$(COMPONENTS))

OUT_DIRS := $(BIN_DIR) $(OBJ_DIRS) $(LIB_DIR)

BOOST_PATH := /home/afelsher/boost/boost_1_75_0/

INCL_PATHS := -I $(BOOST_PATH)
LIB_PATHS := -L $(BOOST_PATH)/stage/lib/ -L $(LIB_DIR)

#LIBS := -pthread -lpqxx -lpq
LIBS := -lbacktrace -lboost_system -ldl -lpq -lpqxx

CPPFLAGS += -std=c++17

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
	CFLAGS += -D YAMO_DEBUG
else
	CFLAGS += -O2 -Wall -Werror
endif

CFLAGS += -DBOOST_STACKTRACE_USE_BACKTRACE
LD_FLAGS += -shared -fPIC

all: dirs $(COMPONENTS)

server: $(OBJ_FILES) $(SO_FILES)
	$(CXX) -o $(BIN_DIR)/$@ $^ $(LIB_PATHS) -L $(BIN_DIR) $(LIBS) -lserver

dirs:
# $(info $$SRC_DIRS is [${SRC_DIRS}])
# $(info $$SRC_FILES is [${SRC_FILES}])
# $(info $$OBJ_DIR is [${OBJ_DIR}])
# $(info $$OBJ_DIRS is [${OBJ_DIRS}])
# $(info $$OBJ_FILES is [${OBJ_FILES}])
	mkdir -p $(OUT_DIRS)

$(SO_FILES):
	$(CXX) $(LD_FLAGS) -o $@ $^ $(LIB_PATHS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(INCL_PATHS) $(LIB_PATHS) -o $@ $< $(LIBS)

clean:
	rm -rf $(BIN_DIR)