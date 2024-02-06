# Makefile for Organelle Synth

lib.name = hid


$(lib.name).class.sources = hid.c hid_linux.c input_arrays.c

cflags = -DPD_EXT_NAME=$(lib.name)
suppress-wunused = yes


include ./pd-lib-builder/Makefile.pdlibbuilder