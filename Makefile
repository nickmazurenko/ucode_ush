.PHONY: all ush uninstall reinstall

SRCS := $(wildcard src/*.c)
OBJS := $(wildcard obj/*.o)

WFLGS := #$(addprefix -W, all extra error pedantic)

all: ush

ush:
	@$(MAKE) -sC libmx/
	@clang -c $(WFLGS) $(SRCS)
	@mkdir obj
	@mv *.o obj
	@clang -std=c11 obj/*.o libmx/libmx.a -I ./inc/ush.h
	@mv a.out $@

clean:
	@rm -rdf obj/
	@$(MAKE) -sC libmx/ clean

uninstall: clean
	@rm -rf ush
	@$(MAKE) -sC libmx/ uninstall

reinstall: uninstall ush
