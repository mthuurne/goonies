EXE = goonies
SRC = src
PREFIX = /mnt/utmp/goonies
EXTRADIR = ./build/linux

GAMEDIR = $(PREFIX)/games
STARTUP = $(GAMEDIR)/$(EXE)
BINDIR = $(PREFIX)/share/games/$(EXE)
ICNDIR = $(PREFIX)/share/pixmaps
APPDIR = $(PREFIX)/share/applications

DATA = graphics maps sound
ICON = goonies.png
DESKTOP = goonies.desktop

OBJS = \
	$(SRC)/2DCMC.o			$(SRC)/auxiliar.o \
	$(SRC)/debug.o			$(SRC)/font_extractor.o \
	$(SRC)/geometrics.o		$(SRC)/GinterfaceBrowser.o \
	$(SRC)/GinterfaceButton.o	$(SRC)/GinterfaceConfirmation.o \
	$(SRC)/Ginterface.o		$(SRC)/GinterfaceMessage.o \
	$(SRC)/GinterfaceSButton.o	$(SRC)/GinterfaceText.o \
	$(SRC)/GinterfaceTextInput.o	$(SRC)/GLTile.o \
	$(SRC)/GLTManager.o		$(SRC)/GMap.o \
	$(SRC)/GMapLayer.o		$(SRC)/GO_bat.o \
	$(SRC)/GO_bigrock.o		$(SRC)/GObject.o \
	$(SRC)/GObjectCreator.o		$(SRC)/GO_bone.o \
	$(SRC)/GO_bridge.o		$(SRC)/GO_bullet.o \
	$(SRC)/GO_cagedoor.o		$(SRC)/GO_character.o \
	$(SRC)/GO_closingwall.o		$(SRC)/GO_coin.o \
	$(SRC)/GO_drop.o		$(SRC)/GO_dropgenerator.o \
	$(SRC)/GO_enemy.o		$(SRC)/GO_entrydoor.o \
	$(SRC)/GO_exitdoor.o		$(SRC)/GO_fallingrock.o \
	$(SRC)/GO_fallingwater.o	$(SRC)/GO_flame.o \
	$(SRC)/GO_fratelli.o		$(SRC)/GO_ghost.o \
	$(SRC)/GO_item.o		$(SRC)/GO_jumpingskull.o \
	$(SRC)/GO_key.o			$(SRC)/GO_lava.o \
	$(SRC)/GO_musicalnote.o		$(SRC)/GooniesScript.o \
	$(SRC)/GO_pipe_water.o		$(SRC)/GO_rope.o \
	$(SRC)/GO_skeleton.o		$(SRC)/GO_skull.o \
	$(SRC)/GO_skulldoor.o		$(SRC)/GO_trickyskull.o \
	$(SRC)/GO_water.o		$(SRC)/GO_wateropening.o \
	$(SRC)/GO_watersplash.o		$(SRC)/keyboardstate.o \
	$(SRC)/Level.o			$(SRC)/LevelPack.o \
	$(SRC)/main.o			$(SRC)/MusicManager.o \
	$(SRC)/PlacedGLTile.o		$(SRC)/ranrotb.o \
	$(SRC)/SDL_glutaux.o		$(SRC)/SFXManager.o \
	$(SRC)/sound.o			$(SRC)/SoundManager.o \
	$(SRC)/state_credits.o		$(SRC)/state_editor.o \
	$(SRC)/state_endsequence.o	$(SRC)/state_game.o \
	$(SRC)/state_gameover.o		$(SRC)/state_gamestart.o \
	$(SRC)/state_howtoplay.o	$(SRC)/state_interlevel.o \
	$(SRC)/state_konami.o		$(SRC)/state_mapeditor.o \
	$(SRC)/state_msx.o		$(SRC)/state_splash.o \
	$(SRC)/state_titleanimation.o	$(SRC)/state_title.o \
	$(SRC)/Symbol.o			$(SRC)/TheGooniesApp.o \
	$(SRC)/TheGoonies.o		$(SRC)/Vector.o $(SRC)/eglport.o

CC = arm-angstrom-linux-gnueabi-g++
CFLAGS = -DPANDORA -DHAVE_GLES -O3 -pipe -march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp -ftree-vectorize -ffast-math -fsingle-precision-constant -fsigned-char -O3 -DHAVE_GLES=1 -DPANDORA=1 -Wno-write-strings `sdl-config --cflags` -I/mnt/utmp/goonies/include -L/mnt/utmp/goonies/lib -I/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr/X11R6/include -I/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr/include
LDFLAGS = `sdl-config --libs` -L/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi//usr/X11R6/lib/ -lSDL_image -lSDL_mixer -lGLES_CM -lEGL -lX11 -lGLUES_CM
RM = rm -f
CP = cp -r
MD = mkdir -p
ECHO = echo
CHMOD = chmod
STRIP = arm-angstrom-linux-gnueabi-strip
LD = arm-angstrom-linux-gnueabi-g++

all: $(EXE)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	@$(STRIP) $@
	@$(ECHO) " o If there are no errors, the game compiled succesfully"

clean:
	@$(ECHO) " o Cleaning up source directory"
	@$(RM) $(SRC)/*.o $(SRC)/*.bak core $(EXE)

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

