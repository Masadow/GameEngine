.PHONY : clean all install

CC=g++
CXX=g++
CXXFLAGS= -Wall -fPIC -g -Iengine -I../steam -MMD -MP
LDFLAGS= -shared
UNAME= $(shell uname -s)

ifeq ($(UNAME), Linux)
LDFLAGS+= -ltinyxml2 -lallegro -lallegro_font -lallegro_ttf -lallegro_primitives
CXXFLAGS+= -Werror
else
CXXFLAGS+= -I../dependencies/includes
LDFLAGS+= -L../dependencies -ltinyxml2 -lallegro_monolith.dll
endif

SOURCES = $(shell find $(SOURCEDIR) -name '*.cpp')
DEPENDENCIES = $(shell find $(SOURCEDIR) -name '*.d')
HEADERS = $(shell find $(SOURCEDIR) -name '*.h')
OBJECTS=$(SOURCES:.cpp=.o)

TARGET=libengine.so
INC_FOLDER=engine/include
CUR_DIR = $(shell pwd)

all: $(TARGET)

install:
	sudo rm -rf /usr/local/lib/libengine.so && sudo ln -s $(CUR_DIR)/$(TARGET) /usr/local/lib/libengine.so
	sudo rm -rf /usr/local/include/engine && sudo ln -s $(CUR_DIR)/$(INC_FOLDER) /usr/local/include/engine

clean:
	rm -f $(OBJECTS) $(DEPENDENCIES) $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)
-include $(OBJECTS:.o=.d)
