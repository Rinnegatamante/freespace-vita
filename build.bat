make -f Makefile.vita clean
make -f Makefile.vita -j16 FS1=true
make -f Makefile.vita FS1=true clean
make -f Makefile.vita -j16 DEMO=true
make -f Makefile.vita DEMO=true clean
make -f Makefile.vita -j16