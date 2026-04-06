XZ_DIR=xz-5.2.2
XZ_LIB_DIR=$(XZ_DIR)/src/liblzma/.libs
XZ_LIBLZMA=$(XZ_LIB_DIR)/liblzma.a
SQUASHTOOL_DIR=squashfs-tools

all: xz_lzma mksquashfs_xz

xz_lzma:
	@echo "Build XZ lzma library"
	@if [ ! -e $(XZ_DIR)/Makefile ]; then \
		tar zxvf xz-5.2.2.tar.gz; \
		cd $(XZ_DIR); \
		configure; \
		make; \
	else \
		cd $(XZ_DIR); \
		make ; \
	fi
	@cp $(XZ_LIBLZMA) $(SQUASHTOOL_DIR)

mksquashfs_xz:
	@echo "Build mksquash tool"
	@if [ -e $(XZ_LIBLZMA) ]; then \
		cd $(SQUASHTOOL_DIR); \
		make; \
	else \
		echo "Build Fail"; \
	fi
	@cp $(SQUASHTOOL_DIR)/mksquashfs mksquashfs_xz

clean:
	@rm -f mksquashfs_xz
	@rm -rf $(XZ_DIR);
	@cd $(SQUASHTOOL_DIR); make clean;
