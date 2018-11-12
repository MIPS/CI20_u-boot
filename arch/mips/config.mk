#
# (C) Copyright 2003
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

CROSS_COMPILE ?= mips_4KC-

# Handle special prefix in ELDK 4.0 toolchain
ifneq (,$(findstring 4KCle,$(CROSS_COMPILE)))
ENDIANNESS := -EL
endif

ifdef CONFIG_SYS_LITTLE_ENDIAN
ENDIANNESS := -EL
endif

ifdef CONFIG_SYS_BIG_ENDIAN
ENDIANNESS := -EB
endif

PLATFORM_CPPFLAGS += -DCONFIG_MIPS -D__MIPS__

#
# From Linux arch/mips/Makefile
#
# GCC uses -G 0 -mabicalls -fpic as default.  We don't want PIC in the kernel
# code since it only slows down the whole thing.  At some point we might make
# use of global pointer optimizations but their use of $28 conflicts with
# the current pointer optimization.
#
# The DECStation requires an ECOFF kernel for remote booting, other MIPS
# machines may also.  Since BFD is incredibly buggy with respect to
# crossformat linking we rely on the elf2ecoff tool for format conversion.
#
# cflags-y			+= -G 0 -mno-abicalls -fno-pic -pipe
# cflags-y			+= -msoft-float
# LDFLAGS_vmlinux		+= -G 0 -static -n -nostdlib
# MODFLAGS			+= -mlong-calls
#
# On the other hand, we want PIC in the U-Boot code to relocate it from ROM
# to RAM, unless we're building SPL which doesn't relocate. $28 is always
# used as gp.
#
PLATFORM_CPPFLAGS		+= -G 0 $(ENDIANNESS)
PLATFORM_CPPFLAGS		+= -msoft-float
PLATFORM_LDFLAGS		+= -G 0 -static -n -nostdlib $(ENDIANNESS)
PLATFORM_RELFLAGS		+= -ffunction-sections -fdata-sections
LDFLAGS_FINAL			+= --gc-sections
OBJCFLAGS			+= --remove-section=.dynsym
ifdef CONFIG_SPL_BUILD
PLATFORM_CPPFLAGS		+= -fno-pic -mno-abicalls -fomit-frame-pointer
else
PLATFORM_CPPFLAGS		+= -fpic -mabicalls
LDFLAGS_FINAL			+= -pie
endif
