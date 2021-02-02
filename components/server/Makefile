PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BIN_DIR := bin
SRC_DIR := src

OBJ_DIR := $(BIN_DIR)/objs

SRC_DIRS := ${shell find ${SRC_DIR} -type d -print}
OBJ_DIRS := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SRC_DIRS))

SRC_FILES := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

INCL_PATHS = /home/afelsher/boost/boost_1_75_0
LIB_PATHS = /home/afelsher/boost/boost_1_75_0/stage/lib

#LIBS = -pthread -lpqxx -lpq
LIBS = -lbacktrace -lboost_system -ldl -lpq -lpqxx

CPPFLAGS += -std=c++11
#LDFLAGS += someflag

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
	CFLAGS += -D YAMO_DEBUG
else
	CFLAGS += -O2 -Wall -Werror
endif

CFLAGS += -DBOOST_STACKTRACE_USE_BACKTRACE

all:	build $(BIN_DIR)/server

build:
#	$(info $$SRC_DIRS is [${SRC_DIRS}])
#	$(info $$SRC_FILES is [${SRC_FILES}])
#	$(info $$OBJ_DIR is [${OBJ_DIR}])
#	$(info $$OBJ_DIRS is [${OBJ_DIRS}])
#	$(info $$OBJ_FILES is [${OBJ_FILES}])
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

$(BIN_DIR)/server: $(OBJ_FILES)
	$(CXX) $(LD_FLAGS) -o ../../$@ $^ -L $(LIB_PATHS) $(LIBS)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -I $(INCL_PATHS) -L $(LIB_PATHS) -o $@ $< $(LIBS)

clean:
	rm -rf $(BIN_DIR)