BUILD_DIR = obj
ISO_DIR = iso

all:
	cd $(BUILD_DIR)&&make
	cd $(ISO_DIR)&&make