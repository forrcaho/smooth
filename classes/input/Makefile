########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
ifneq ($(BUILD_HAIKU),True)
	MYCCOPTS += $(shell pkg-config --cflags x11)
endif
endif
endif

# Enter object files here:
OBJECTS	  = keyboard.o pointer.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,backends)
CLEANCMD1 = $(call cleanin,backends)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
