#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGET_NAME		:=TITLE_userguide

SRCS			:= \
	$(wildcard $(SRC_PATH)/src/*) \
	$(wildcard $(SRC_PATH)/media/*) \
	$(wildcard $(SRC_PATH)/template/*) \
	$(SRC_PATH)/*.lua
	
#-------------------------------------------------
# Build commands
#-------------------------------------------------
all: $(TARGET_NAME).docx
	@echo Document Generation is done!

clean:
	@rm -f $(TARGET_NAME).docx
	@rm -f $(TARGET_NAME).pdf
	@echo Done!

$(TARGET_NAME).docx:$(SRCS)
	@echo '*** DocGen Build *** (Please wait a seconds!!!)'
	@docgen -t template/docgen_template.docx -f pdf main.lua $(TARGET_NAME).docx
