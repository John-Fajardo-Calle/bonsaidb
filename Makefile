all: build

build:
	@mkdir -p build
	cd build && cmake .. && $(MAKE) -j$(shell nproc)

clean:
	rm -rf build

.PHONY: all build clean
