# Makefile for the Allegro 5 artillery game.
#
# Usage:
#   make            Build an optimised release binary (./missile-game)
#   make run        Build (if needed) and run the game from the project root
#   make debug      Build an unoptimised binary with debug symbols (for gdb)
#   make clean      Remove all build artefacts
#
# The game loads its assets through relative "./assets/..." paths, so it must
# be run from the project root. `make run` does that for you.

BIN      ?= missile-game
BUILD    ?= release
OBJDIR   := build/$(BUILD)

SRCS     := $(wildcard *.cpp)
OBJS     := $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS     := $(OBJS:.o=.d)

CXX      ?= g++
CXXFLAGS := -std=c++17 -Wall -MMD -MP

ifeq ($(BUILD),debug)
  CXXFLAGS += -g -O0
else
  CXXFLAGS += -O2
endif

# Allegro 5 and the addons this project links against.
ALLEGRO_MODULES := allegro-5 allegro_audio-5 allegro_acodec-5 allegro_dialog-5 \
                   allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5

# Prefer pkg-config (works across distros). Fall back to plain -l flags if it
# is unavailable or does not know about Allegro.
ifeq ($(shell pkg-config --exists $(ALLEGRO_MODULES) 2>/dev/null && echo yes),yes)
  CXXFLAGS     += $(shell pkg-config --cflags $(ALLEGRO_MODULES))
  ALLEGRO_LIBS := $(shell pkg-config --libs $(ALLEGRO_MODULES))
else
  ALLEGRO_LIBS := -lallegro -lallegro_audio -lallegro_acodec -lallegro_dialog \
                  -lallegro_font -lallegro_image -lallegro_primitives -lallegro_ttf
endif

.PHONY: all debug run clean

all: $(BIN)

# Convenience target: re-enter make with the debug build profile.
debug:
	@$(MAKE) BUILD=debug

$(BIN): $(OBJS)
	$(CXX) $^ -o $@ $(ALLEGRO_LIBS)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

run: $(BIN)
	./$(BIN)

clean:
	rm -rf build $(BIN)

-include $(DEPS)
