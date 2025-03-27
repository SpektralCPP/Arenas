all: install

install: /usr/include/Spektral/Arenas/LinearArena.hpp

/usr/include/Spektral/Arenas/LinearArena.hpp: includes/Spektral/Arenas/LinearArena.hpp
	@echo "Installing header at $@"
	@sudo mkdir -p /usr/include/Spektral/Arenas/
	@sudo cp $^ $@

docs: doxygenfile includes/
	@echo "Generating docs"
	@mkdir -p docs/html
	@doxygen $<
