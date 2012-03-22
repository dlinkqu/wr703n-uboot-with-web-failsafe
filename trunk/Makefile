
export BUILD_TOPDIR=$(PWD)
MAKECMD=make ARCH=mips CROSS_COMPILE=mips-openwrt-linux-uclibc-
export PATH:=$(BUILD_TOPDIR)/toolchain/OpenWrt-Toolchain-ar71xx-for-mips_r2-gcc-4.6-linaro_uClibc-0.9.33/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33/bin/:$(PATH)
export STAGING_DIR=$(BUILD_TOPDIR)/tmp
export FLASH_SIZE=4
export COMPRESSED_UBOOT=1

all: decompress_toolchain uboot
	echo $(BUILD_TOPDIR)

decompress_toolchain:
	make -C $(BUILD_TOPDIR)/toolchain/

uboot:
	cd $(BUILD_TOPDIR)/u-boot/ && $(MAKECMD) tl-wr703n_config
	cd $(BUILD_TOPDIR)/u-boot/ && $(MAKECMD) ENDIANNESS=-EB V=1 all
	
clean:

clean_all:
	cd $(BUILD_TOPDIR)/u-boot/ && $(MAKECMD) distclean
	make -C $(BUILD_TOPDIR)/toolchain/ clean

