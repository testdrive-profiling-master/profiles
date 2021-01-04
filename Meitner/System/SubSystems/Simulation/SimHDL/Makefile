all:
	@$(TESTDRIVE_DIR)bin/make_list verilog_desc.txt -output Makefile.HDL
	rm -f SimHDL.o
	mingw32-make -j $(NUMBER_OF_PROCESSORS) -f Makefile.HDL

clean:
	mingw32-make -f Makefile.HDL clean

static:
	mingw32-make -f Makefile.HDL static
