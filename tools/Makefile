########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

FOLDERS = designer pciffmanager translator

.PHONY: $(FOLDERS)

all: $(FOLDERS)

$(FOLDERS):
	+ $(call makein,$@)

clean: $(foreach FOLDER,$(FOLDERS),$(FOLDER)##clean)

$(foreach FOLDER,$(FOLDERS),$(FOLDER)##clean):
	$(call cleanin,$(subst ##clean,,$@))
