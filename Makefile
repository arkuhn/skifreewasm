BUILD_DIR=build
WEB_DIR=web
TARGET=skifree

build:
	cd $(BUILD_DIR) && emcmake cmake .. && emmake make

copy:
	cp $(BUILD_DIR)/$(TARGET).html $(WEB_DIR)/
	cp $(BUILD_DIR)/$(TARGET).js $(WEB_DIR)/
	cp $(BUILD_DIR)/$(TARGET).wasm $(WEB_DIR)/
	cp $(BUILD_DIR)/$(TARGET).data $(WEB_DIR)/

serve:
	cd $(WEB_DIR) && python3 -m http.server 8000

all: build copy

watch:
	find src include | entr -c make all

.PHONY: build copy serve all watch 