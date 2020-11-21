ifeq ($(OS),)
OS = $(shell uname -s)
endif
PREFIX = /usr/local
CC   = gcc
CPP  = g++
AR   = ar
LIBPREFIX = lib
LIBEXT = .a
ifeq ($(OS),Windows_NT)
BINEXT = .exe
SOLIBPREFIX =
SOEXT = .dll
else ifeq ($(OS),Darwin)
BINEXT =
SOLIBPREFIX = lib
SOEXT = .dylib
else
BINEXT =
SOLIBPREFIX = lib
SOEXT = .so
endif
INCS = -Iinclude
CFLAGS = $(INCS) -Os
CPPFLAGS = $(INCS) -Os
STATIC_CFLAGS = -DBUILD_VERSIONCMP_STATIC
SHARED_CFLAGS = -DBUILD_VERSIONCMP_DLL
LIBS =
LDFLAGS =
ifeq ($(OS),Darwin)
STRIPFLAG =
else
STRIPFLAG = -s
endif
MKDIR = mkdir -p
RM = rm -f
RMDIR = rm -rf
CP = cp -f
CPDIR = cp -rf
DOXYGEN = $(shell which doxygen)

OSALIAS := $(OS)
ifeq ($(OS),Windows_NT)
ifneq (,$(findstring x86_64,$(shell gcc --version)))
OSALIAS := win64
else
OSALIAS := win32
endif
endif

LIBVERSIONCMP_OBJ = lib/versioncmp.o
LIBVERSIONCMP_LDFLAGS = 
LIBVERSIONCMP_SHARED_LDFLAGS =
ifneq ($(OS),Windows_NT)
SHARED_CFLAGS += -fPIC
endif
ifeq ($(OS),Windows_NT)
LIBVERSIONCMP_SHARED_LDFLAGS += -Wl,--out-implib,$(LIBPREFIX)$@$(LIBEXT) -Wl,--output-def,$(@:%$(SOEXT)=%.def)
endif
ifeq ($(OS),Darwin)
OS_LINK_FLAGS = -dynamiclib -o $@
else
OS_LINK_FLAGS = -shared -Wl,-soname,$@ $(STRIPFLAG)
endif

UTILS_BIN = src/versioncmp_test$(BINEXT)

COMMON_PACKAGE_FILES = README.md LICENSE Changelog.txt
SOURCE_PACKAGE_FILES = $(COMMON_PACKAGE_FILES) Makefile doc/Doxyfile include/*.h lib/*.c src/*.c build/*.cbp build/*.depend build/*.workspace

default: all

all: static-lib shared-lib utils

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

%.static.o: %.c
	$(CC) -c -o $@ $< $(STATIC_CFLAGS) $(CFLAGS) 

%.shared.o: %.c
	$(CC) -c -o $@ $< $(SHARED_CFLAGS) $(CFLAGS)

static-lib: $(LIBPREFIX)versioncmp$(LIBEXT)

shared-lib: $(SOLIBPREFIX)versioncmp$(SOEXT)

$(LIBPREFIX)versioncmp$(LIBEXT): $(LIBVERSIONCMP_OBJ:%.o=%.static.o)
	$(AR) cr $@ $^

$(SOLIBPREFIX)versioncmp$(SOEXT): $(LIBVERSIONCMP_OBJ:%.o=%.shared.o)
	$(CC) -o $@ $(OS_LINK_FLAGS) $^ $(LIBVERSIONCMP_SHARED_LDFLAGS) $(LIBVERSIONCMP_LDFLAGS) $(LDFLAGS) $(LIBS)

utils: $(UTILS_BIN)

#src/versioncmp_test_s$(BINEXT): %$(BINEXT): %.static.o $(LIBPREFIX)versioncmp$(LIBEXT)
#	$(CC) $(STRIPFLAG) -o $@ $^ $(LIBVERSIONCMP_LDFLAGS) $(LDFLAGS)

src/versioncmp_test$(BINEXT): %$(BINEXT): %.shared.o $(SOLIBPREFIX)versioncmp$(SOEXT)
	$(CC) $(STRIPFLAG) -o $@ $^ $(LIBVERSIONCMP_LDFLAGS) $(LDFLAGS)

.PHONY: doc
doc:
ifdef DOXYGEN
	$(DOXYGEN) doc/Doxyfile
endif

install: all doc
	$(MKDIR) $(PREFIX)/include $(PREFIX)/lib $(PREFIX)/bin
	$(CP) include/*.h $(PREFIX)/include/
	$(CP) *$(LIBEXT) $(PREFIX)/lib/
	$(CP) $(UTILS_BIN) $(PREFIX)/bin/
ifeq ($(OS),Windows_NT)
	$(CP) *$(SOEXT) $(PREFIX)/bin/
	$(CP) *.def $(PREFIX)/lib/
else
	$(CP) *$(SOEXT) $(PREFIX)/lib/
endif
ifdef DOXYGEN
	$(CPDIR) doc/man $(PREFIX)/
endif

test: src/versioncmp_test$(BINEXT)
	LD_LIBRARY_PATH=. src/versioncmp_test

.PHONY: version
version:
	sed -ne "s/^#define\s*VERSIONCMP_VERSION_[A-Z]*\s*\([0-9]*\)\s*$$/\1./p" include/versioncmp.h | tr -d "\n" | sed -e "s/\.$$//" > version

.PHONY: package
package: version
	tar cfJ versioncmp-$(shell cat version).tar.xz --transform="s?^?versioncmp-$(shell cat version)/?" $(SOURCE_PACKAGE_FILES)

.PHONY: package
binarypackage: version
ifneq ($(OS),Windows_NT)
	$(MAKE) PREFIX=binarypackage_temp_$(OSALIAS) install
	tar cfJ versioncmp-$(shell cat version)-$(OSALIAS).tar.xz --transform="s?^binarypackage_temp_$(OSALIAS)/??" $(COMMON_PACKAGE_FILES) binarypackage_temp_$(OSALIAS)/*
else
	$(MAKE) PREFIX=binarypackage_temp_$(OSALIAS) install DOXYGEN=
	cp -f $(COMMON_PACKAGE_FILES) binarypackage_temp_$(OSALIAS)
	rm -f versioncmp-$(shell cat version)-$(OSALIAS).zip
	cd binarypackage_temp_$(OSALIAS) && zip -r9 ../versioncmp-$(shell cat version)-$(OSALIAS).zip $(COMMON_PACKAGE_FILES) * && cd ..
endif
	rm -rf binarypackage_temp_$(OSALIAS)

.PHONY: clean
clean:
	$(RM) lib/*.o src/*.o *$(LIBEXT) *$(SOEXT) $(UTILS_BIN) version versioncmp-*.tar.xz doc/doxygen_sqlite3.db
ifeq ($(OS),Windows_NT)
	$(RM) *.def
endif
	$(RMDIR) doc/html doc/man

