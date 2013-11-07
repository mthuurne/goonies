ifeq ($(TARGET),pandora)
PREFIX = /mnt/utmp/goonies
CROSS = arm-angstrom-linux-gnueabi-
else ifeq ($(TARGET),gcw0)
PREFIX = install
CROSS = mipsel-gcw0-linux-uclibc-
else ifeq ($(TARGET),)
$(error Please specify TARGET)
else
$(error Unsupported TARGET: $(TARGET))
endif
$(info Target: $(TARGET))

FLAVOR ?= release
ifeq ($(filter $(FLAVOR),release debug),)
$(error Unknown FLAVOR: $(FLAVOR))
endif
$(info Flavor: $(FLAVOR))

EXE = goonies
SRC = src
EXTRADIR = ./build/linux

GAMEDIR = $(PREFIX)/games
STARTUP = $(GAMEDIR)/$(EXE)
BINDIR = $(PREFIX)/share/games/$(EXE)
ICNDIR = $(PREFIX)/share/pixmaps
APPDIR = $(PREFIX)/share/applications

DATA = graphics maps sound
ICON = goonies.png
DESKTOP = goonies.desktop

OUTDIR := output/$(TARGET)/$(FLAVOR)
OBJDIR := $(OUTDIR)/obj
EXEDIR := $(OUTDIR)/bin

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJS = $(SOURCES:$(SRC)/%.cpp=$(OBJDIR)/%.o)

CXX = $(CROSS)g++
RM = rm -f
CP = cp -r
MD = mkdir -p
ECHO = echo
CHMOD = chmod
ifeq ($(FLAVOR),release)
STRIP = $(CROSS)strip
else
STRIP = echo "Not stripping:"
endif

SYSROOT := $(shell $(CXX) -print-sysroot)

CXXFLAGS := -O3 -pipe -ftree-vectorize -ffast-math -fsingle-precision-constant -fsigned-char
ifeq ($(FLAVOR),release)
CXXFLAGS += -DNDEBUG
endif
ifeq ($(FLAVOR),debug)
CXXFLAGS += -g
endif
CXXFLAGS += -Wno-write-strings
CXXFLAGS += -DHAVE_GLES -DUSE_GLES1
ifeq ($(TARGET),pandora)
CXXFLAGS += -DPANDORA -march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp
endif
ifeq ($(TARGET),gcw0)
CXXFLAGS += -DGCW
endif
CXXFLAGS += $(shell $(SYSROOT)/usr/bin/sdl-config --cflags)
ifeq ($(TARGET),pandora)
CXXFLAGS += -I$(PREFIX)/include -L$(PREFIX)/lib
CXXFLAGS += -I/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr/X11R6/include -I/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr/include
endif

LDFLAGS := $(shell $(SYSROOT)/usr/bin/sdl-config --libs)
ifeq ($(TARGET),pandora)
LDFLAGS += -L/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr/X11R6/lib/
endif
LDFLAGS += -lSDL_image -lSDL_mixer
ifeq ($(TARGET),pandora)
LDFLAGS += -lGLES_CM -lEGL -lX11
endif
ifeq ($(TARGET),gcw0)
LDFLAGS += -lGLESv1_CM -lEGL
endif

.PHONY: all clean opk install uninstall

ifeq ($(TARGET),gcw0)
all: opk
else
all: $(EXEDIR)/$(EXE)
	@$(ECHO) " o If there are no errors, the game compiled succesfully"
endif

$(OBJS): $(OBJDIR)/%.o: $(SRC)/%.cpp $(wildcard $(SRC)/*.h)
	@$(MD) $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEDIR)/$(EXE): $(OBJS)
	@$(MD) $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@
	@$(STRIP) $@

clean:
	@$(ECHO) " o Cleaning up"
	@$(RM) -r $(OUTDIR)
	@$(RM) $(SRC)/*.bak core

opk: $(EXEDIR)/$(EXE)
	@$(ECHO) " o Creating OPK file"
	@mksquashfs \
		"$(EXTRADIR)/default.gcw0.desktop" \
		"$(EXTRADIR)/goonies-32x32.png" \
		$(EXEDIR)/$(EXE) \
		$(DATA) \
		$(OUTDIR)/goonies.opk \
		-noappend -comp gzip -all-root

install: all
	@$(ECHO) " o Creating install directory $(BINDIR)"
	@$(MD) "$(BINDIR)"
	@$(ECHO) " o Installing game and data to $(BINDIR)"
	@$(CP) "$(EXE)" $(DATA) "$(BINDIR)"
	@$(ECHO) " o Creating startup script $(STARTUP)"
	@$(MD) "$(GAMEDIR)"
	@$(ECHO) "#!/bin/sh" >"$(STARTUP)"
	@$(ECHO) "cd \"$(BINDIR)\" && ./$(EXE)" >>"$(STARTUP)"
	@$(CHMOD) 755 "$(STARTUP)"
	@$(ECHO) " o Creating application menu entry"
	@$(MD) "$(ICNDIR)"
	@$(CP) "$(EXTRADIR)/$(ICON)" "$(ICNDIR)"
	@$(MD) "$(APPDIR)"
	@$(CP) "$(EXTRADIR)/$(DESKTOP)" "$(APPDIR)"
	@$(ECHO) ""

uninstall:
	@$(ECHO) " o Removing game and data from $(BINDIR)"
	@$(RM) -r "$(BINDIR)"
	@$(ECHO) " o Removing startup script $(STARTUP)"
	@$(RM) "$(STARTUP)"
	@$(ECHO) " o Removing application menu entry"
	@$(RM) "$(ICNDIR)/$(ICON)" "$(APPDIR)/$(DESKTOP)"
	@$(ECHO) ""
