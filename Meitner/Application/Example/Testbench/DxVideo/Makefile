#-------------------------------------------------
# Project
#-------------------------------------------------
#TD_DESCRIPTION	= Camera test
#TD_TYPE		= profile
#TD_ERROR_TOKEN	= "*E: ", "*I: ":0, "*W: ":W
SRC_PATH		:= .
#TD_WORK_FOLDER :=
TARGETPATH		:= ../../../../Program
#TD_FILE_NAME	:= %%PROJECT%%Program\%s.exe
TARGETNAME		:= Testbench_DxVideo

SRCS			:= \
	$(SRC_PATH)/VideoSource/VideoSource.cpp \
	$(SRC_PATH)/Ge/GeUtil.cpp \
	$(SRC_PATH)/Ge/GeBlockWinKey.cpp \
	$(SRC_PATH)/Ge/GePrimitive.cpp \
	$(SRC_PATH)/Ge/GeTexture.cpp \
	$(SRC_PATH)/Ge/GeImage.cpp \
	$(SRC_PATH)/Ge/GeStateBlock.cpp \
	$(SRC_PATH)/Ge/GeFont.cpp \
	$(SRC_PATH)/Ge/GeRenderView.cpp \
	$(SRC_PATH)/Ge/GeSystemMessage.cpp \
	$(SRC_PATH)/Ge/GeCamera.cpp \
	$(SRC_PATH)/Ge/GeCameraPostProcessing.cpp \
	$(SRC_PATH)/Ge/Ge.cpp \
	$(SRC_PATH)/../framework/common.cpp \
	$(SRC_PATH)/main.cpp

SRCS_RES		:= \
	$(SRC_PATH)/res/Resource.rc

INC				:= -I../framework -I./Ge -I./VideoSource -I.
LIBDIR			:= -ld3d9 -ldxerr9 -ld3dx9 -ld3dxof -ldxguid -lwinmm -lole32 -loleaut32 -luuid
CDEFS			:= -D_WIN32_IE=0x0500 -DWINVER=0x500
LDFLAGS			:= -mwindows

#-------------------------------------------------
# common document config
#-------------------------------------------------
include ../../../include/common.mk
