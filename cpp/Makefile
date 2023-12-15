.PHONY: build del rebuild
build:
	mkdir build
	cd build && cmake ../
	cd build && make
del:
	rm -rf build
rebuild:
	make del
	make build
lint:
	cppcheck --enable=all --error-exitcode=1 --cppcheck-build-dir=build ./notes/*.cpp ./database/*.cpp ./*.cpp
