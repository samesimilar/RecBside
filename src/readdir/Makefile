

# change this to match the name of the external you would like to generate

lib.name = readdir_organelle


$(lib.name).class.sources = readdir.c


suppress-wunused = yes

define forDarwin
  cflags += -DMSP_ON_CLANG -DGENLIB_USE_FLOAT32 -mmacosx-version-min=10.9
endef


include ./pd-lib-builder/Makefile.pdlibbuilder