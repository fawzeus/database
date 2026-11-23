# Per-module entry point: builds/tests only this module via the root Makefile.
MODULE := common
ROOT   := ..

.DEFAULT_GOAL := all

all:
	@$(MAKE) --no-print-directory -C $(ROOT) module-all MODULE=$(MODULE)

check:
	@$(MAKE) --no-print-directory -C $(ROOT) module-check MODULE=$(MODULE)

clean:
	@$(MAKE) --no-print-directory -C $(ROOT) module-clean MODULE=$(MODULE)

.PHONY: all check clean
