NAME = ush
CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic) -g
COMP = clang

SRCDIR = src
INCDIR = inc
OBJDIR = obj

LMXDIR = libmx
LMXA := $(LMXDIR)/libmx.a
LMXINC := $(LMXDIR)/inc


INCS = $(wildcard $(INCDIR)/*.h)
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))

all: install

install: $(LMXA) $(NAME)

$(NAME): $(OBJS)
	@clang $(CFLAGS) $(OBJS) -L$(LMXDIR) -lmx -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCS)
	@clang $(CFLAGS) -c $< -o $@ -I$(INCDIR) -I$(LMXINC)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
		@mkdir -p $@

$(LMXA):
		@make -sC $(LMXDIR)

clean:
	@rm -rf $(OBJDIR)

uninstall: clean
	@make -sC $(LMXDIR) $@
	@rm -rf $(OBJDIR)
	@rm -rf $(NAME)

reinstall: uninstall install
