=========================
Please note the information below is included mainly for purposes of attribution.
To build this project on the Organelle synth, you only need to run "make all" on 
the command line in this folder, and pd-lib-builder takes care of the configuration.

You can find the upstream, cross-platform, complete source code for [readdir] in the
pd-extended collection: https://puredata.info/downloads/pd-extended

Mike Spears - Feb. 2024
=========================


    README for pd external 'readdir'

    Last updated for readdir v0.02

DESCRIPTION
    The 'readdir' object lets you read the contents of a directory.

INSTALLATION
    Issue the following commands to the shell:

       cd readdir-X.YY  (or wherever you extracted the distribution)
       ./configure
       make
       make install

BUILD OPTIONS
    The 'configure' script supports the following options, among others:

    * --enable-debug , --disable-debug
        Whether to enable verbose debugging messages. Default=no.

ACKNOWLEDGEMENTS
    PD by Miller Puckette and others.

    Ideas, black magic, and other nuggets of information drawn from code by
    Guenter Geiger, Larry Troxler, and iohannes m zmoelnig.

KNOWN BUGS
    None known.

AUTHOR
    Bryan Jurish <moocow@ling.uni-potsdam.de>

