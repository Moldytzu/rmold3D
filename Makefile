rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
reverse = $(if $(1),$(call reverse,$(wordlist 2,$(words $(1)),$(1)))) $(firstword $(1))

CPP = g++

SRCDIR := rmold3D
OBJDIR := obj

CFLAGS = -I $(shell pwd)/ -Ofast

SRC = $(call rwildcard,$(SRCDIR),*.cpp)  
OBJS = $(call reverse,$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC)))

LIB = engrmold3D.a

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ mkdir -p $(@D)
	$(CPP) $(CFLAGS) -c $^ -o $@

.PHONY: build
build: lib

clean:
	rm -rf $(OBJS)

lib: $(OBJS)
	ar -crs $(LIB) $(OBJS)

game: lib 
	$(CPP) game.cpp $(LIB) -lGL -lGLU -lglut -o game $(CFLAGS)