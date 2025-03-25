all: install

install: /usr/include/Spektral/Arenas/MemoryArena.hpp

/usr/include/Spektral/Arenas/MemoryArena.hpp: includes/Spektral/Arenas/MemoryArena.hpp
	@echo "Installing header at $@"
	@sudo mkdir -p /usr/include/Spektral/Arenas/
	@sudo cp $^ $@

docs: doxygenfile includes/
	@echo "Generating docs"
	@mkdir -p docs/html
	@doxygen $<
