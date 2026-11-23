CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# Layout B: each module owns include/<module>/ (public headers),
# src/ (private implementation), and test/ (unit tests).
# One root Makefile discovers all of them; no per-module Makefiles needed.
MODULE_SRC_DIRS     = $(patsubst ./%,%,$(shell find . -path ./build -prune -o -type d -name src     -print | sort))
MODULE_INCLUDE_DIRS = $(patsubst ./%,%,$(shell find . -path ./build -prune -o -type d -name include -print | sort))
MODULE_TEST_DIRS    = $(patsubst ./%,%,$(shell find . -path ./build -prune -o -type d -name test    -print | sort))

# Public headers are included as <module/header.hpp>; the src/ roots are kept
# so a module can still have private headers included as "header.hpp".
INCLUDES = $(addprefix -I,$(MODULE_INCLUDE_DIRS) $(MODULE_SRC_DIRS))

SOURCES = $(shell find $(MODULE_SRC_DIRS)     -name '*.cpp' | sort)
HEADERS = $(shell find $(MODULE_INCLUDE_DIRS) $(MODULE_SRC_DIRS) -name '*.hpp' | sort)
OBJECTS = $(SOURCES:%.cpp=build/%.o)

# Every *_test.cpp becomes its own binary; link all production sources except
# the application entry point, so each test file can define its own main().
TEST_SOURCES = $(shell find $(MODULE_TEST_DIRS) -name '*_test.cpp' | sort)
TEST_BINARIES = $(TEST_SOURCES:%.cpp=build/%.out)
LIB_SOURCES = $(filter-out app/src/main.cpp,$(SOURCES))

EXECUTABLE = build/database

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Mirror the module layout inside build/ so object names never collide.
# Any header change rebuilds all objects (fine at this size).
build/%.o: %.cpp $(HEADERS)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

build/%.out: %.cpp $(SOURCES) $(HEADERS)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I$(dir $<) $< $(LIB_SOURCES) -o $@

# ANSI colors for the test runner (POSIX printf interprets \033).
GREEN := \033[0;32m
RED   := \033[0;31m
RESET := \033[0m

# Modules come from the test directories (app/test -> app).
MODULES = $(sort $(patsubst %/test,%,$(MODULE_TEST_DIRS)))

# Build and run one module's tests right away instead of building everything
# first. Every file prints a "building" line, then its live output, then PASS/FAIL.
# $(MAKE) is expanded by the outer make; "$$t" stays a shell variable.
define RUN_MODULE
printf '\n============================================================\n'; \
printf '  Making check in %s\n' '$(1)'; \
printf '============================================================\n'; \
module_passed=0; \
module_failed=0; \
for t in $(filter build/$(1)/test/%.out,$(TEST_BINARIES)); do \
	src=$${t#build/}; \
	src=$${src%.out}.cpp; \
	printf '  [%s] building %s\n' '$(1)' "$$src"; \
	if ! $(MAKE) --no-print-directory -s "$$t"; then \
		printf '  $(RED)[BUILD FAIL]$(RESET) %s\n' "$$src"; \
		module_failed=$$((module_failed + 1)); \
		continue; \
	fi; \
	printf '  [%s] running %s\n' '$(1)' "$$src"; \
	"./$$t"; \
	status=$$?; \
	if [ $$status -eq 0 ]; then \
		printf '  $(GREEN)[PASS]$(RESET) %s\n' "$$src"; \
		module_passed=$$((module_passed + 1)); \
	else \
		printf '  $(RED)[FAIL]$(RESET) %s (exit %d)\n' "$$src" "$$status"; \
		module_failed=$$((module_failed + 1)); \
	fi; \
done; \
printf '  ------------------------------------------------------------\n'; \
printf '  %s: %d test(s) passed, %d failed\n' '$(1)' "$$module_passed" "$$module_failed"; \
total_passed=$$((total_passed + module_passed)); \
total_failed=$$((total_failed + module_failed));
endef

# Shared final summary for the full check and for a single-module check.
define PRINT_SUMMARY
printf '\n============================================================\n'; \
if [ $$total_failed -eq 0 ]; then \
	printf '$(GREEN)%d test(s) passed, 0 failed$(RESET)\n' "$$total_passed"; \
else \
	printf '$(RED)%d test(s) passed, %d failed$(RESET)\n' "$$total_passed" "$$total_failed"; \
	exit 1; \
fi
endef

# `make check -s`: each module is built, run and reported before the next one.
check:
	@total_passed=0; \
	total_failed=0; \
	$(foreach m,$(MODULES),$(call RUN_MODULE,$(m))) \
	$(PRINT_SUMMARY)

test: check

# ---------------------------------------------------------------------------
# Per-module entry points. The Makefile inside each module calls these with
# MODULE=<name>, so `make`, `make check` and `make clean` work in that module.
# ---------------------------------------------------------------------------
MODULE ?=

ifneq ($(MODULE),)
MODULE_OBJECTS       = $(filter build/$(MODULE)/%.o,$(OBJECTS))
MODULE_TEST_BINARIES = $(filter build/$(MODULE)/test/%.out,$(TEST_BINARIES))

module-all: $(MODULE_OBJECTS) $(MODULE_TEST_BINARIES)
	@printf 'Module %s built (%d object(s), %d test binary(ies))\n' '$(MODULE)' '$(words $(MODULE_OBJECTS))' '$(words $(MODULE_TEST_BINARIES))'

module-check:
	@total_passed=0; \
	total_failed=0; \
	$(call RUN_MODULE,$(MODULE)) \
	$(PRINT_SUMMARY)

module-clean:
	rm -rf build/$(MODULE)
endif

clean:
	rm -rf build

run: $(EXECUTABLE)
	./$(EXECUTABLE)

list-modules:
	@echo "Modules (each owns include/<module>/, src/ and test/):"
	@for dir in $(MODULE_SRC_DIRS); do echo "  $$dir"; done
	@echo "Includes: $(words $(MODULE_INCLUDE_DIRS)) | Sources: $(words $(SOURCES)) | Headers: $(words $(HEADERS)) | Tests: $(words $(TEST_SOURCES))"

.PHONY: all clean run test check module-all module-check module-clean list-modules
