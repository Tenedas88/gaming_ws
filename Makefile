#locations
TOPDIR = .
MODULESDIR = $(TOPDIR)/modules
GAMESDIR = $(TOPDIR)/games
OLCDIR = $(TOPDIR)/external/olc
OLCDIR += $(TOPDIR)/external/olc/Extensions
BUILDDIR = $(TOPDIR)/build

#names for relative locations
OBJDIRNAME = gamefiles
SPRITEDIRNAME = gamesprites

#compiler and flags
CPP = g++
CFLAGS = -g
LDFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs
RM = rm -rf

#output environment
EXT = .x
FASTLINKNAME = play$(EXT)

#default goal manipulation
MAKECMDGOALS = all
CLEANTARGET = $(BUILDDIR)/$(NAME)

#specific game build
#INPUT: 
#NAME: name of the game folder
CPPFILES := 
CPPDIRS := $(MODULESDIR)
USERINCDIR  :=
INCDIR += $(foreach olc_inc, $(OLCDIR), -I$(olc_inc))

sinclude $(GAMESDIR)/$(NAME)/$(NAME).mk
#OUTPUT:
#CPPFILES: Add single game files here
#CPPDIRS: entire folders containing source files
#USERINCDIR: entire folders containing header files

CPPFILES += $(sort $(foreach src_dir, $(CPPDIRS), $(shell find $(src_dir) -name "*.cpp")))
INCDIR += $(foreach inc_dir, $(USERINCDIR), $(foreach usr_dir, $(shell find $(inc_dir) -name "*.h"),-I$(dir $usr_dir)))
OBJFILES := $(patsubst $(TOPDIR)%,$(BUILDDIR)/$(NAME)/$(OBJDIRNAME)%,$(CPPFILES:.cpp=.o))

$(info $(OBJFILES))
all: $(NAME)

$(NAME): $(OBJFILES)
	@mkdir -p $(BUILDDIR)/$(NAME)
	@cp -r $(GAMESDIR)/$(NAME)/$(SPRITEDIRNAME) $(BUILDDIR)/$(NAME)/ 2> /dev/null
	@$(CPP) -o $(BUILDDIR)/$@/$@$(EXT) $^ $(LDFLAGS)
	@rm -f $(FASTLINKNAME) 2> /dev/null
	@ln -s $(BUILDDIR)/$@/$@$(EXT) $(FASTLINKNAME)

$(BUILDDIR)/$(NAME)/$(OBJDIRNAME)/%.o: $(TOPDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CPP) $(CFLAGS) -c -o $@ $< $(LDFLAGS) $(INCDIR)

clean:
	@$(RM) $(CLEANTARGET) $(FASTLINKNAME)