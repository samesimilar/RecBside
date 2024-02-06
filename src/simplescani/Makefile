
# To build, run `make all`

lib.name = simplescani


$(lib.name).class.sources = simplescani.c


suppress-wunused = yes

define forDarwin
  cflags += -DMSP_ON_CLANG -DGENLIB_USE_FLOAT32 -mmacosx-version-min=10.9
endef




include ./pd-lib-builder/Makefile.pdlibbuilder