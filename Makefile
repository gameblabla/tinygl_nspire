PRGNAME     = gears

# define regarding OS, which compiler to use
EXESUFFIX = 
TOOLCHAIN = 
CC          = nspire-gcc
CCP         = nspire-g++
LD          = nspire-gcc

# add SDL dependencies
SDL_LIB     = 
SDL_INCLUDE = 

# change compilation / linking flag options
F_OPTS		= -I./include -Ilib_nsp -DTGL_FEATURE_RENDER_BITS=16
CC_OPTS		= -Ofast -fomit-frame-pointer -fdata-sections -ffunction-sections -march=armv5te $(F_OPTS)
CFLAGS		= -I$(SDL_INCLUDE) $(CC_OPTS)
CXXFLAGS	=$(CFLAGS) 
LDFLAGS     = -Wl,--as-needed -Wl,--gc-sections -flto -lm

# Files to be compiled
SRCDIR    = ./src ./lib_nsp ./gears_nsp
VPATH     = $(SRCDIR)
SRC_C   = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
SRC_CP   = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.cpp))
OBJ_C   = $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJ_CP   = $(notdir $(patsubst %.cpp, %.o, $(SRC_CP)))
OBJS     = $(OBJ_C) $(OBJ_CP)

# Rules to make executable
$(PRGNAME)$(EXESUFFIX): $(OBJS)  
	$(LD) $(CFLAGS) -o $(PRGNAME)$(EXESUFFIX) $^ $(LDFLAGS)
	genzehn --input $(PRGNAME) --output $(PRGNAME).tns --compress

$(OBJ_C) : %.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_CP) : %.o : %.cpp
	$(CCP) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(PRGNAME)$(EXESUFFIX) *.o
