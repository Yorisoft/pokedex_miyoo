BUILD_DIR = build
BINARY_OUTPUT = bin
TARGET_NAME = retrodex

rebuild: clean cmake_configure build

clean:
	rm -rfv $(BUILD_DIR)/*  # Or just rm -rf build if the build dir is dedicated

cmake_configure: clean
	cd $(BUILD_DIR) && cmake .. -DCMAKE_TOOLCHAIN_FILE=../Toolchain.cmake

build: cmake_configure
	cd $(BUILD_DIR) && make

run:
	cp -v $(BUILD_DIR)/$(BINARY_OUTPUT)/$(TARGET_NAME) . && \
	./$(TARGET_NAME)
