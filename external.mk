################################################################################
#
# libuio_helper
#
################################################################################

LIBUIO_HELPER_VERSION = 1.0.0
LIBUIO_HELPER_SITE_METHOD = git
LIBUIO_HELPER_SITE = $(call github,byates,uio_helper,$(LIBUIO_HELPER_VERSION))
LIBUIO_HELPER_LICENSE = GPL-2.0
LIBUIO_HELPER_INSTALL_TARGET = NO
LIBUIO_HELPER_INSTALL_STAGING = YES

define LIBUIO_HELPER_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

# Files that may exists on target and SDK. Typically all libraries (static and
# shared), all config files, etc.
define LIBUIO_HELPER_INSTALL_STAGING_CMDS
	$(INSTALL) -D -m 0755 $(@D)/libuio_helper.a $(STAGING_DIR)/usr/lib/libuio_helper.a
	$(INSTALL) -D -m 0644 $(@D)/src/uio_helper.h $(STAGING_DIR)/usr/include/uio_helper.h
endef

# Files that only need to be on the target. Compared to staging/,
# target/ contains only the files and libraries needed to run the
# selected target applications: the development files (headers,
# etc.) are not present, the binaries are stripped.
define LIBUIO_HELPER_INSTALL_TARGET_CMDS
endef

$(eval $(generic-package))
