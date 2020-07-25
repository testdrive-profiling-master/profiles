all:
	@$(TESTDRIVE_DIR)bin/make_list verilog_dpi_preload.txt -output verilog_dpi_preload.inl
	@$(TESTDRIVE_DIR)bin/make_list verilog_desc.txt -output Makefile.HDL
	make -f Makefile.HDL clean
ifdef DEBUG
	make -j -f Makefile.HDL DEBUG=1
else
	make -j -f Makefile.HDL
endif

clean:
	make -f Makefile.HDL clean
