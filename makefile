.PHONY: clean

all:
	cmake --build build --config Debug --target all
	prime-run ./src/art_gallery
	# ./src/art_gallery