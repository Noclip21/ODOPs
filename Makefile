################################################################################
#                                   Makefile                                   #
################################################################################
#                                    Config                                    #

SOURCES = main.cpp
#SOURCES = main_bvh.cpp
#SOURCES = main_bvh_gpu.cpp
#SOURCES = main_bvh_only.cpp
#SOURCES = main_kdop.cpp
#SOURCES = main_obb.cpp
#SOURCES = main_bvh_print.cpp
#SOURCES = main_bezier.cpp
#SOURCES = main_dummy.cpp
#SOURCES = main_dummy_join.cpp
#SOURCES = main_hair.cpp
#SOURCES = main_heatmap_primaryRay.cpp
#SOURCES = main_obj.cpp

BINARY     = test
SOURCE_DIR = src/
BINARY_DIR = bin/

#==

CXX       = clang++
CXXFLAGS := -Wall -std=c++20 -O3
CXXFLAGS += -Isrc/cgmath
CXXFLAGS += -Isrc/cgasset
CXXFLAGS += -Isrc/cgtexture
CXXFLAGS += -Isrc/cgbvh
CXXFLAGS += -Isrc/cgthread
CXXFLAGS += -Isrc/cgtrace
CXXFLAGS += -Isrc/cgcompute
LDFLAGS  := -ltbb
LDFLAGS  += -lfreeimage
LDFLAGS  += -lassimp
LDFLAGS  += -lpthread
LDFLAGS  += -lGLEW
LDFLAGS  += -lGL
LDFLAGS  += -lglfw

#==

OBJECT_DIR  = obj/
OBJECTS     = $(SOURCES:.cpp=.o)
DEPENDS     = $(SOURCES:.cpp=.d)
SOURCE_FULL = $(addprefix $(SOURCE_DIR),$(SOURCES))
OBJECT_FULL = $(addprefix $(OBJECT_DIR),$(OBJECTS))
DEPEND_FULL = $(addprefix $(OBJECT_DIR),$(DEPENDS))
BINARY_FULL = $(BINARY_DIR)$(BINARY)

################################################################################
#                                   Recipes                                    #

all: build

run: build
	@$(BINARY_DIR)$(BINARY)

build: $(OBJECT_DIR) $(BINARY_DIR) $(BINARY_FULL)

$(OBJECT_DIR):
	mkdir -p $@
	mkdir -p $(dir $(OBJECT_FULL))

$(BINARY_DIR):
	mkdir -p $@

$(BINARY_FULL): $(OBJECT_FULL)
	$(CXX) -o $@ $^ $(LDFLAGS)

-include $(DEPEND_FULL)

$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.cpp Makefile
	$(CXX) -MMD -MP -c -o $@ $< $(CXXFLAGS)

clean:
	rm -fr $(OBJECT_DIR) $(BINARY_DIR)

.PHONY: run build clean
