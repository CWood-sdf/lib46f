# VEXcode makefile 2019_03_26_01

# show compiler output
VERBOSE = 0

# include toolchain options
include vex/mkenv.mk

# location of the project source cpp and c files
SRC_C  = $(wildcard src/*.cpp) 
SRC_C += $(wildcard src/*/*.cpp) 
SRC_C += $(wildcard src/*/*/*.cpp) 
SRC_C += $(wildcard src/*.c) 
SRC_C += $(wildcard src/*/*.c) 
SRC_C += $(wildcard src/*/*/*.c) 

OBJA = $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(SRC_C))) )
OBJ = $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(SRC_C))) )

# location of include files that c and cpp files depend on
SRC_H  = $(wildcard include/*.h)
SRC_H += $(wildcard include/*/*.h)
SRC_H += $(wildcard include/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*/*/*/*.h)
SRC_H += $(wildcard build-lvgl/*.h)
SRC_H += $(wildcard build-lvgl/*/*.h)
SRC_H += $(wildcard build-lvgl/*/*/*.h)
SRC_H += $(wildcard build-lvgl/*/*/*/*.h)
SRC_H += $(wildcard build-lvgl/*/*/*/*/*.h)
SRC_H += $(wildcard build-lvgl/*/*/*/*/*/*.h)
SRC_H += $(wildcard build-lvgl/*/*/*/*/*/*/*.h)
SRC_H += $(wildcard build-lvgl/*/*/*/*/*/*/*/*.h)
SRC_H += $(wildcard Libs/*.h)


# additional dependancies
SRC_A  = makefile

# project header file locations
INC_F  = include . build-lvgl/include/ build-lvgl/include/lvgl build-lvgl/include/lvgl/src include/* Libs/

# headers needed to use library
# LV_SRC_H += $(wildcard lvgl/lvgl.h) 
# LV_SRC_H += $(wildcard lvgl/src/*.h) 
# LV_SRC_H += $(wildcard lvgl/src/*/*.h)
# LV_SRC_H += lvgl/lv_conf.h
# LV_SRC_H += lvgl/lvgl_inc.h

# LV_DST_H = $(addprefix build-lvgl/, $(LV_SRC_H) )
# LV_DST_H += build-lvgl/lvgl/lv_conf.h build-lvgl/lvgl/lvgl_inc.h

LIB_SRC_H = $(wildcard Libs/*.h)
LIB_SRC_H += $(wildcard include/*.h)
LIB_SRC_H += $(wildcard include/*/*.h)
LIB_SRC_H += $(wildcard include/*/*/*.h)
LIB_SRC_H += $(wildcard include/*/*/*/*.h)

LIB_DST_H = $(addprefix build/, $(LIB_SRC_H) )

$(BUILD)/include/%: %
	$(Q)$(MKDIR)
	$(Q) $(call copyfile,$^, $@)



# override proj name
PROJECTLIB = lib46f

# build targets
all: $(BUILD)/$(PROJECT).bin inc

# copy lvgl header files
.PHONY: inc
inc: $(LIB_DST_H)
	$(ECHO) "Copy headers to build folder"

# include build rules
include vex/mkrules.mk
