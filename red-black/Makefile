# Executable
NAME := a.out


# Directories
SDIR := src
ODIR := obj
DDIR := dep
IDIR := includes
LDIR := libs

# Files
SRC := $(shell find $(SDIR) -name "*.cpp" -type f)
OBJ := $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRC))
DEP := $(patsubst $(SDIR)/%.cpp,$(DDIR)/%.d,$(SRC))
HDF := $(shell find $(IDIR) -name "*.hpp" -type f)

# Compilation
CC = clang++
IFLAGS = -I$(IDIR)
LFLAGS =
CFLAGS = -Wall -Wextra -Werror -Wno-unused -Wno-unused-parameter -std=c++14 -pedantic

ifeq ($(shell uname -s),Linux)
	DFLAGS := -O0 -g -fsanitize=address -fsanitize=leak
else
	DFLAGS := -O0 -g -fsanitize=address
endif

# ifdef DEBUG
	CFLAGS += $(DFLAGS)
# endif

RM = /bin/rm -f

# Rules
.PHONY: all
all:
	$(MAKE) -j4 $(NAME)

# Compilation
$(NAME): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LFLAGS)
$(OBJ): $(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)
$(DDIR)/%.d: $(SDIR)/%.cpp
	@mkdir -p $(@D)
	@$(CC) $< -MM -MF $@ -MT $(ODIR)/$*.o $(IFLAGS)

# Cleanup
.PHONY: clean fclean re cleanall
clean:
	$(RM) -r $(ODIR)
	$(RM) -r $(DDIR)
fclean: clean
	$(RM) $(NAME)
re: fclean
	$(MAKE) all

# Debug
.PHONY: debug
debug: fclean
	$(MAKE) all DEBUG=1


-include $(DEP)
