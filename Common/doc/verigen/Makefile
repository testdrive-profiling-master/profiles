#-------------------------------------------------
# Project
#-------------------------------------------------
SRC_PATH		:= .
TARGET_NAME		:=verigen_userguide

SRCS			:= \
	$(wildcard $(SRC_PATH)/src/*) \
	$(wildcard $(SRC_PATH)/media/*) \
	$(SRC_PATH)/*.lua
	

#-------------------------------------------------
# Build commands
#-------------------------------------------------
BUILD_TARGET		:= $(TARGET_NAME).docx
BUILD_TARGET_PDF	:= $(TARGET_NAME).pdf

all: $(BUILD_TARGET)
	@echo Document Generation is done!

clean:
	@rm -f $(BUILD_TARGET)
	@rm -f $(BUILD_TARGET_PDF)
	@echo Done!

$(BUILD_TARGET):$(SRCS)
	@echo '*** DocGen Build *** (Please wait a seconds!!!)'
	@docgen main.lua $(BUILD_TARGET)
