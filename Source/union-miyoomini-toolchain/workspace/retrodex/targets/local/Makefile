BUILD_DIR = build
BINARY_OUTPUT = bin
TARGET_NAME = retrodex

rebuild: clean cmake_configure build

clean:
	rm -rfv $(BUILD_DIR)/*  # Or just rm -rf build if the build dir is dedicated

cmake_configure: clean
	cd $(BUILD_DIR) && cmake ..

build: cmake_configure
	ls && \
	cd $(BUILD_DIR) && make

run:
	cp -v $(BUILD_DIR)/$(BINARY_OUTPUT)/$(TARGET_NAME) . && \
	./$(TARGET_NAME)
