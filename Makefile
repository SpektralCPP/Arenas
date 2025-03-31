all: install

install: includes/Spektral/Arenas/LinearArena.hpp\
	includes/Spektral/Arenas/BlockArena.hpp includes/Spektral/Arenas/utils.hpp
	@echo "Installing header $@ at /usr/include/Spektral/Arenas"
	@sudo mkdir -p /usr/include/Spektral/Arenas/
	@sudo cp $^ /usr/include/Spektral/Arenas/

docs: doxygenfile includes/Spektral/Arenas/*.hpp
	@echo "Generating docs"
	@mkdir -p docs/html
	@doxygen $<

tests: build/bencharmks build/functionality
benchmark: build/benchmarks
clean:
	@echo "Cleaning"
	@rm -rf build/
	@rm -rf docs/

build/benchmarks: tests/perf/main.cpp
	@echo "Compiling Benchmarks @ $@"
	@mkdir -p build/
	@g++ $^ -o $@ -lbenchmark -O3 --std=c++23

.PHONY: install clean benchmark
