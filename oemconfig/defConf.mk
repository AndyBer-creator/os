
DEFAULT_CONFIG_DIR := $(SDK_CONFIG_DIR)/defConf

PUBLIC_PROJECTS :=  yfd_8382_4_poe yfd_8382_8 yfd_8382_8_poe yfd_8382_16 yfd_8382_16_poe yfd_8382_24 yfd_8382_24_poe yfd_9311_48_poe yfd_9311_48_poe_l2 yfd_9301_24_poe yfd_8382_4_poe_bt yfd_8382_8_poe_bt yfd_8382_10 yfd_8382_10_poe yfd_8382_10_poe_bt

.PHONY : helpme $(PUBLIC_PROJECTS)

yfd_8382_4: defconfig 
	cp -rf $(ABS_ROOT_DIR)/$(OEM_DIR)/$@/* $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/

	if [ ! -e $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch ]; then \
		ln -snf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch; fi

	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.linux-3.18.24.x $(ABS_ROOT_DIR)/kernel/uClinux/linux-3.18.24.x/.config
	if [ ! -e $(ABS_ROOT_DIR)/kernel/uClinux/config.arch ]; then \
		cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.arch $(ABS_ROOT_DIR)/kernel/uClinux/config.arch; fi
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.uClibc-0.9.33-485-200 $(ABS_ROOT_DIR)/kernel/uClinux/uClibc-0.9.33-485-200/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.root $(ABS_ROOT_DIR)/kernel/uClinux/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.vendor $(ABS_ROOT_DIR)/kernel/uClinux/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.sdk $(ABS_ROOT_DIR)/sdk/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.pswitch $(ABS_ROOT_DIR)/pswitch/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.u-boot $(ABS_ROOT_DIR)/loader/u-boot-2011.12/.config

	make -C $(PSWITCH_DIR) config
	make -C $(PSWITCH_DIR) setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) config
	make -C $(LOADER_DIR)
	make -C $(KERNEL_DIR)/$(OS_TYPE) common
	make pswitch
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_mkimage
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_linux
	rm -rf image_$@
	mkdir image_$@
	cp -rf $(ABS_ROOT_DIR)/image/rom-T532MB.bix $(ABS_ROOT_DIR)/image_$@/rom-T532MB.bix
	cp -rf $(ABS_ROOT_DIR)/image/u-boot.bin $(ABS_ROOT_DIR)/image_$@/u-boot.bin
	cp -rf $(ABS_ROOT_DIR)/image/vmlinux.bix $(ABS_ROOT_DIR)/image_$@/vmlinux.bix

yfd_9311_48: defconfig 
	cp -rf $(ABS_ROOT_DIR)/$(OEM_DIR)/$@/* $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/

	if [ ! -e $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch ]; then \
		ln -snf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch; fi

	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.linux-3.18.24.x $(ABS_ROOT_DIR)/kernel/uClinux/linux-3.18.24.x/.config
	if [ ! -e $(ABS_ROOT_DIR)/kernel/uClinux/config.arch ]; then \
		cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.arch $(ABS_ROOT_DIR)/kernel/uClinux/config.arch; fi
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.uClibc-0.9.33-485-200 $(ABS_ROOT_DIR)/kernel/uClinux/uClibc-0.9.33-485-200/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.root $(ABS_ROOT_DIR)/kernel/uClinux/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.vendor $(ABS_ROOT_DIR)/kernel/uClinux/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.sdk $(ABS_ROOT_DIR)/sdk/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.pswitch $(ABS_ROOT_DIR)/pswitch/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.u-boot $(ABS_ROOT_DIR)/loader/u-boot-2011.12/.config

	make -C $(PSWITCH_DIR) config
	make -C $(PSWITCH_DIR) setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) config
	make -C $(LOADER_DIR)
	make -C $(KERNEL_DIR)/$(OS_TYPE) common
	make pswitch
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_mkimage
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_linux
	rm -rf image_$@
	mkdir image_$@
	cp -rf $(ABS_ROOT_DIR)/image/rom-T532MB.bix $(ABS_ROOT_DIR)/image_$@/rom-T532MB.bix
	cp -rf $(ABS_ROOT_DIR)/image/u-boot.bin $(ABS_ROOT_DIR)/image_$@/u-boot.bin
	cp -rf $(ABS_ROOT_DIR)/image/vmlinux.bix $(ABS_ROOT_DIR)/image_$@/vmlinux.bix

yfd_9311_48_l2: defconfig 
	cp -rf $(ABS_ROOT_DIR)/$(OEM_DIR)/$@/* $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/

	if [ ! -e $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch ]; then \
		ln -snf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch; fi

	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.linux-3.18.24.x $(ABS_ROOT_DIR)/kernel/uClinux/linux-3.18.24.x/.config
	if [ ! -e $(ABS_ROOT_DIR)/kernel/uClinux/config.arch ]; then \
		cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.arch $(ABS_ROOT_DIR)/kernel/uClinux/config.arch; fi
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.uClibc-0.9.33-485-200 $(ABS_ROOT_DIR)/kernel/uClinux/uClibc-0.9.33-485-200/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.root $(ABS_ROOT_DIR)/kernel/uClinux/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.vendor $(ABS_ROOT_DIR)/kernel/uClinux/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.sdk $(ABS_ROOT_DIR)/sdk/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.pswitch $(ABS_ROOT_DIR)/pswitch/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.u-boot $(ABS_ROOT_DIR)/loader/u-boot-2011.12/.config

	make -C $(PSWITCH_DIR) config
	make -C $(PSWITCH_DIR) setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) config
	make -C $(LOADER_DIR)
	make -C $(KERNEL_DIR)/$(OS_TYPE) common
	make pswitch
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_mkimage
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_linux
	rm -rf image_$@
	mkdir image_$@
	cp -rf $(ABS_ROOT_DIR)/image/rom-T532MB.bix $(ABS_ROOT_DIR)/image_$@/rom-T532MB.bix
	cp -rf $(ABS_ROOT_DIR)/image/u-boot.bin $(ABS_ROOT_DIR)/image_$@/u-boot.bin
	cp -rf $(ABS_ROOT_DIR)/image/vmlinux.bix $(ABS_ROOT_DIR)/image_$@/vmlinux.bix

yfd_9301_24: defconfig 
	cp -rf $(ABS_ROOT_DIR)/$(OEM_DIR)/$@/* $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/

	if [ ! -e $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch ]; then \
		ln -snf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch; fi

	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.linux-3.18.24.x $(ABS_ROOT_DIR)/kernel/uClinux/linux-3.18.24.x/.config
	if [ ! -e $(ABS_ROOT_DIR)/kernel/uClinux/config.arch ]; then \
		cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.arch $(ABS_ROOT_DIR)/kernel/uClinux/config.arch; fi
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.uClibc-0.9.33-485-200 $(ABS_ROOT_DIR)/kernel/uClinux/uClibc-0.9.33-485-200/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.root $(ABS_ROOT_DIR)/kernel/uClinux/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.vendor $(ABS_ROOT_DIR)/kernel/uClinux/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.sdk $(ABS_ROOT_DIR)/sdk/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.pswitch $(ABS_ROOT_DIR)/pswitch/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.u-boot $(ABS_ROOT_DIR)/loader/u-boot-2011.12/.config

	make -C $(PSWITCH_DIR) config
	make -C $(PSWITCH_DIR) setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) config
	make -C $(LOADER_DIR)
	make -C $(KERNEL_DIR)/$(OS_TYPE) common
	make pswitch
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_mkimage
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_linux
	rm -rf image_$@
	mkdir image_$@
	cp -rf $(ABS_ROOT_DIR)/image/rom-T532MB.bix $(ABS_ROOT_DIR)/image_$@/rom-T532MB.bix
	cp -rf $(ABS_ROOT_DIR)/image/u-boot.bin $(ABS_ROOT_DIR)/image_$@/u-boot.bin
	cp -rf $(ABS_ROOT_DIR)/image/vmlinux.bix $(ABS_ROOT_DIR)/image_$@/vmlinux.bix

$(PUBLIC_PROJECTS): defconfig 
	cp -rf $(ABS_ROOT_DIR)/$(OEM_DIR)/$@/* $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/

	if [ ! -e $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch ]; then \
		ln -snf $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch; fi

	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.linux-3.18.24.x $(ABS_ROOT_DIR)/kernel/uClinux/linux-3.18.24.x/.config
	if [ ! -e $(ABS_ROOT_DIR)/kernel/uClinux/config.arch ]; then \
		cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.arch $(ABS_ROOT_DIR)/kernel/uClinux/config.arch; fi
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.uClibc-0.9.33-485-200 $(ABS_ROOT_DIR)/kernel/uClinux/uClibc-0.9.33-485-200/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.root $(ABS_ROOT_DIR)/kernel/uClinux/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.vendor $(ABS_ROOT_DIR)/kernel/uClinux/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.sdk $(ABS_ROOT_DIR)/sdk/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.pswitch $(ABS_ROOT_DIR)/pswitch/config/.config
	cp -rf $(KERNEL_DIR)/$(OS_TYPE)/vendors/$(CONFIG_VENDOR_DEFAULT)/Switch/config.u-boot $(ABS_ROOT_DIR)/loader/u-boot-2011.12/.config

	make -C $(PSWITCH_DIR) config
	make -C $(PSWITCH_DIR) setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) config
	make -C $(LOADER_DIR)
	make -C $(KERNEL_DIR)/$(OS_TYPE) common
	make pswitch
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_setup
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_mkimage
	make -C $(KERNEL_DIR)/$(OS_TYPE) post_linux
	rm -rf image_$@
	mkdir image_$@
	cp -rf $(ABS_ROOT_DIR)/image/rom-T532MB.bix $(ABS_ROOT_DIR)/image_$@/rom-T532MB.bix
	cp -rf $(ABS_ROOT_DIR)/image/u-boot.bin $(ABS_ROOT_DIR)/image_$@/u-boot.bin
	cp -rf $(ABS_ROOT_DIR)/image/vmlinux.bix $(ABS_ROOT_DIR)/image_$@/vmlinux.bix

defconfig:
	cp -rf $(ABS_ROOT_DIR)/$(OEM_DIR)/defconfig/* $(ABS_ROOT_DIR)/$(PSWITCH_DIR)/vendor/$(CONFIG_VENDOR_DEFAULT)/Switch/

help: 
	@printf "\nBasic commands:\n"
	@printf "   make menuconfig         (go to config menu to set each option)\n"
	@printf "   make                    (build code)\n"
	@printf "\nBuild by default configuration:\n"
	@printf "   make 8380               (8380 switch)\n"
	@printf "   make 8382               (8382 switch)\n"
	@printf "   make 8393               (8393 switch)\n"
	@printf "   make 9301               (9301 switch)\n"
	@printf "   make 9311               (9311 switch)\n"
