# VEXcode makefile 2019_03_26_01

# show compiler output
VERBOSE = 0

# include toolchain options
include vex/mkenv.mk

# location of the project source cpp and c files
SRC_C  = $(wildcard src/*.cpp) 
SRC_C += $(wildcard src/*/*.cpp) 
SRC_C += $(wildcard src/*/*/*.cpp) 

OBJA = $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(SRC_C))) )
OBJ  = $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(SRC_C))) )

# location of include files that c and cpp files depend on
SRC_H  = $(wildcard include/*.h)
SRC_H += $(wildcard include/*/*.h)
SRC_H += $(wildcard include/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*/*/*/*.h)
SRC_H += $(wildcard Libs/*.h)


# additional dependancies
SRC_A  = makefile

# project header file locations
INC_F  = include . include/lvgl include/lvgl/src include/Odometry Libs/

LIB_SRC_H  = $(wildcard include/*.h)
LIB_SRC_H += $(wildcard include/*/*.h)
LIB_SRC_H += $(wildcard include/*/*/*.h)
LIB_SRC_H += $(wildcard include/*/*/*/*.h)
LIB_SRC_H += $(wildcard Libs/*.h)

LIB_DST_H = $(addprefix $(BUILD)/include/, $(LIB_SRC_H) )

$(BUILD)/include/%: %
	$(Q)$(MKDIR)
	$(Q) $(call copyfile,$^, $@)

vpath %.h include/lvgl/ include/ Libs/

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
