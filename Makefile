rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
reverse = $(if $(1),$(call reverse,$(wordlist 2,$(words $(1)),$(1)))) $(firstword $(1))

CPP = g++
CC = gcc

SRCDIR := rmold3D
OBJDIR := obj

CFLAGS = -I $(shell pwd)/ -I $(shell pwd)/rmold3D -I $(shell pwd)/rmold3D/3rd-Party/glm -Ofast

SRC = $(call rwildcard,$(SRCDIR),*.cpp)  
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

LIB = engrmold3D.a

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ mkdir -p $(@D)
	$(CPP) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: build
build: lib

clean:
	rm -rf $(OBJS)

lib: $(OBJS)
	ar -crs $(LIB) $(OBJS)

game: lib 
	$(CPP) game.cpp $(LIB) -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -o game $(CFLAGS)