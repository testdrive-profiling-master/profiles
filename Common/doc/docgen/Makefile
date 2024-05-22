#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGET_NAME		:=docgen_userguide

SRCS			:= \
	$(wildcard $(SRC_PATH)/src/*) \
	$(wildcard $(SRC_PATH)/media/*) \
	$(SRC_PATH)/*.lua
	
#-------------------------------------------------
# Build commands
#-------------------------------------------------
all: $(TARGET_NAME)_ko.docx
	@echo Document Generation is done!

clean:
	@rm -f $(TARGET_NAME)_ko.docx
	@rm -f $(TARGET_NAME)_ko.pdf
	@rm -f $(TARGET_NAME)_en.docx
	@rm -f $(TARGET_NAME)_en.pdf
	@echo Done!

$(TARGET_NAME)_ko.docx:$(SRCS)
	@echo '*** DocGen Build (Korean) *** (Please wait a seconds!!!)'
	@docgen -f pdf -t testdrive -l ko main.lua $(TARGET_NAME)_ko.docx
	@docgen -f pdf -t testdrive -l en main.lua $(TARGET_NAME)_en.docx
