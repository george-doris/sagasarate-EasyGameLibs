#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = -std=c++14 -W -fPIC -DLUA_COMPAT_BITLIB -DLUA_COMPAT_APIINTCASTS -DLUA_USE_POSIX
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = 

SRC_CPP_FILES=$(shell find ./ -name "*.cpp")
SRC_C_FILES=$(shell find ./ -name "*.c" -a ! -name "wsocket.c")
HEADER_FILES=$(shell find ./ -name "*.h" -a ! -name "wsocket.h")

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g -W -O0 -fPIC -D_DEBUG
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = linux/debug
DEP_DEBUG = 
OUT_DEBUG = ../Bin/LuaD.a

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -g -W -fPIC -D_RELEASE
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS)
OBJDIR_RELEASE = linux/release
DEP_RELEASE = 
OUT_RELEASE = ../Bin/Lua.a

OBJ_C_DEBUG = $(SRC_C_FILES:%.c=$(OBJDIR_DEBUG)/%.o)
OBJ_CPP_DEBUG = $(SRC_CPP_FILES:%.cpp=$(OBJDIR_DEBUG)/%.o)

OBJ_C_RELEASE = $(SRC_C_FILES:%.c=$(OBJDIR_RELEASE)/%.o)
OBJ_CPP_RELEASE = $(SRC_CPP_FILES:%.cpp=$(OBJDIR_RELEASE)/%.o)

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d ../Bin || mkdir -p ../Bin
	test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src
	test -d $(OBJDIR_DEBUG)/luasocket || mkdir -p $(OBJDIR_DEBUG)/luasocket
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_C_DEBUG) $(OBJ_CPP_DEBUG) $(DEP_DEBUG)
	$(AR) rcs $(OUT_DEBUG) $(OBJ_C_DEBUG) $(OBJ_CPP_DEBUG)

stdafx.h.gch: stdafx.h
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $<

$(OBJ_C_DEBUG):$(OBJDIR_DEBUG)/%.o:%.c stdafx.h.gch
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $< -o $@

$(OBJ_CPP_DEBUG):$(OBJDIR_DEBUG)/%.o:%.cpp stdafx.h.gch
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c $< -o $@


clean_debug: 
	-rm -f stdafx.h.gch
	-rm -f $(OBJ_C_DEBUG) $(OBJ_CPP_DEBUG) $(OUT_DEBUG)

before_release: 
	test -d ../Bin || mkdir -p ../Bin
	test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src
	test -d $(OBJDIR_RELEASE)/luasocket || mkdir -p $(OBJDIR_RELEASE)/luasocket
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_C_RELEASE) $(OBJ_CPP_RELEASE) $(DEP_RELEASE)
	$(AR) rcs $(OUT_RELEASE) $(OBJ_C_RELEASE) $(OBJ_CPP_RELEASE)

stdafx.h.gch: stdafx.h
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $<

$(OBJ_C_RELEASE):$(OBJDIR_RELEASE)/%.o:%.c stdafx.h.gch
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $< -o $@

$(OBJ_CPP_RELEASE):$(OBJDIR_RELEASE)/%.o:%.cpp stdafx.h.gch
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c $< -o $@

clean_release: 
	-rm -f stdafx.h.gch
	-rm -f $(OBJ_C_RELEASE) $(OBJ_CPP_RELEASE) $(OUT_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

