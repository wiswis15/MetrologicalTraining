TARGET = nrf24l01
OUTPUT = ./artifacts

SOURCE += nrf24l01.cpp

INC = -I$(STAGING_DIR)/usr/include
CFLAGS = -Wall -fPIC
LIBDIR =
LIB = -lrt -ldl -lgenerics -ldevices
LDFLAGS = -L$(STAGING_DIR)/usr/lib

DEBUG_INC = $(INC)
DEBUG_CFLAGS = $(CFLAGS) -g -O0 -DPLATFORM_RPI=1
DEBUG_LIBDIR = $(LIBDIR)
DEBUG_LIB = $(LIB)
DEBUG_LDFLAGS = $(LDFLAGS)

RELEASE_INC = $(INC)
RELEASE_CFLAGS = $(CFLAGS) -O2 -DPLATFORM_RPI=1
RELEASE_LIBDIR = $(LIBDIR)
RELEASE_LIB = $(LIB)
RELEASE_LDFLAGS = $(LDFLAGS) -s

ifeq ($(TYPE), Debug)
    ACTUAL_INC = $(DEBUG_INC)
    ACTUAL_CFLAGS = $(DEBUG_CFLAGS)
    ACTUAL_LIBDIR = $(DEBUG_LIBDIR)
    ACTUAL_LIB = $(DEBUG_LIB)
    ACTUAL_LDFLAGS = $(DEBUG_LDFLAGS)
else
    TYPE = Release
    ACTUAL_INC = $(RELEASE_INC)
    ACTUAL_CFLAGS = $(RELEASE_CFLAGS)
    ACTUAL_LIBDIR = $(RELEASE_LIBDIR)
    ACTUAL_LIB = $(RELEASE_LIB)
    ACTUAL_LDFLAGS = $(RELEASE_LDFLAGS)
endif

OBJECTS=$(join $(addsuffix obj/$(TYPE)/, $(dir $(SOURCE))), $(notdir $(SOURCE:.cpp=.o)))

all: install

install: target staging

target: $(OUTPUT)/$(TYPE)/$(TARGET)
	cp $(OUTPUT)/$(TYPE)/$(TARGET) $(TARGET_DIR)/usr/bin/$(TARGET)

staging: $(OUTPUT)/$(TYPE)/$(TARGET)

build: $(OUTPUT)/$(TYPE)/$(TARGET)

$(OUTPUT)/$(TYPE)/$(TARGET): $(OBJECTS)
	@mkdir -p $(OUTPUT)/$(TYPE)
	$(CXX) $(ACTUAL_LIBDIR) $(ACTUAL_LDFLAGS) $(OBJECTS) $(ACTUAL_LIB) -o $(OUTPUT)/$(TYPE)/$(TARGET)

obj/$(TYPE)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -std=c++0x -c $(ACTUAL_CFLAGS) $(ACTUAL_INC) -o $@ $<

clean:
	rm -f ./obj/$(TYPE)/*
	rm -rf $(OUTPUT)/$(TYPE)/$(TARGET)

.PHONY: all install build staging
