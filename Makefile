# GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug_linux
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug_linux)
  ui_custom_controls_config = debug_linux
endif
ifeq ($(config),release_linux)
  ui_custom_controls_config = release_linux
endif

PROJECTS := ui-custom-controls

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

ui-custom-controls:
ifneq (,$(ui_custom_controls_config))
	@echo "==== Building ui-custom-controls ($(ui_custom_controls_config)) ===="
	@${MAKE} --no-print-directory -C build/ui-custom-controls -f Makefile config=$(ui_custom_controls_config)
endif

clean:
	@${MAKE} --no-print-directory -C build/ui-custom-controls -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug_linux"
	@echo "  release_linux"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   ui-custom-controls"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"