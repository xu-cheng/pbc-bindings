CMAKE_BUILD_TYPE := RelWithDebInfo

build: cmake
	cd build && $(MAKE)

cmake:
	mkdir -p build
	cd build && cmake .. -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}

clean:
	rm -rf build

test: build
	cd build && $(MAKE) test

test-verbose:
	BOOST_TEST_LOG_LEVEL=message $(MAKE) test ARGS="-VV"

.PHONY: build cmake clean test test-verbose
