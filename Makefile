CC ?= gcc
CFLAGS := -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2
CXXFLAGS :=
DBGFLAGS := -g
COBJFLAGS := $(CFLAGS) -c

BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug
 
TARGET_NAME := apheleia
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif

C_SUFFIX := .c
O_SUFFIX := .o

TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,$(C_SUFFIX)*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix $(O_SUFFIX), $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix $(O_SUFFIX), $(notdir $(basename $(SRC)))))

CLEAN_LIST := $(OBJ)			\
			  $(OBJ_DEBUG)		\
			  $(TARGET)			\
			  $(TARGET_DEBUG) 	\
			  $(DISTCLEAN_LIST)	\

default: makedir all

$(TARGET): $(OBJ)
	$(CC) $(SRC_PATH)/main.c $(OBJ) $(CFLAGS) -o $(TARGET_NAME)
$(OBJ_PATH)/%$(O_SUFFIX): $(SRC_PATH)/%$(C_SUFFIX)*
	$(CC) $(COBJFLAGS) -o $@ $<

$(DBG_PATH)/%$(O_SUFFIX): $(SRC_PATH)/%$(C_SUFFIX)*
	$(CC) $(COBJFLAGS) $(DBGFLAGS) -o $@ $<

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(SRC_PATH)/main_apheleia.c $(CFLAGS) $(OBJ_DEBUG) $(DBGFLAGS) -o $@

.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)