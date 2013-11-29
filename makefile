# vim: noet ts=4 sw=4 sts=0

# Copyright (C) 2013, eXerigumo Clanjor (哆啦比猫/兰威举)
# Licensed under The MIT License. See COPYING for more details.


# configurations
DST = jopa-ng
APP = $(DST)
VER = 0.20
ARG =

export CWD = $(CURDIR)
# <<<
# configurations
MAKE = make
MAKEC = make --no-print-directory -C
ifndef CWD
export CWD = $(shell $(MAKEC) .. print-root-dir)
endif
CC = g++
LD = g++
SUFFIX = cc
FLG = -Wall -O3 -march=native -std=gnu++0x -pthread \
			$(shell pkg-config --cflags jack libpulse-simple)
LIB = $(shell pkg-config --libs jack libpulse-simple)

DIR = $(strip $(wildcard */))
SRC = $(strip $(wildcard *.$(SUFFIX)))
OBJ = $(SRC:.$(SUFFIX)=.o)
OBJ_EXIST = $(shell find . -name '*.o')

define DO_SPREAD
echo -e "\e[0;32mspread into \e[1;35m$1\e[0m";
awk '
	/^# <<<$$/{mode=1}
	mode{sub(/#!/,"");print}
	/^# >>>$$/{mode=0;print""}
' makefile > $1makefile
endef

define \n


endef

define SPREAD
$(subst $(\n), ,@$(call DO_SPREAD,$1))$(\n)
endef

define SUBS_MAKE
$(foreach subdir, $(DIR), \
	$(call SPREAD,$(subdir)) \
	@$(MAKEC) $(subdir) $1$(\n) \
)
endef
# >>>
DIR = src/		# override DIR in the root dir




# interfaces
all: current
current: subs-compile
	@$(MAKEC) . link
link: $(DST)
cleanall: clean
	@rm -f $(DST)
spreadall: spread
print-root-dir:
	@echo "$(CWD)"
dist-clean: subs-remove-makefile cleanall
commitall: commit
rebuild: dist-clean all
edit:
	@echo -e "\e[0;32medit \e[1;35mmakefile\e[0m"
	@cp makefile makefile.bak
	@vim makefile
	@diff makefile makefile.bak > /dev/null || $(MAKEC) . spread
	@rm -f makefile.bak
test: all
	@echo -e "\e[0;32mtest \e[1;35m$(DST) \e[0;35m$(ARG)\e[0m"
	./$(DST) $(ARG)
minify: all
	@echo -e "\e[0;32mminify \e[1;35m$(DST)\e[0m"
	strip -s -R .note.ABI-tag -R .note.gnu.build-id -R .comment $(DST)
	upx --best --ultra-brute $(DST)
install: all
	@echo -e "\e[0;32minstall \e[1;35m$(DST)\e[0m -> \e[0;35m/usr/bin/$(DST)\e[0m"
	install -svm 755 ./$(DST) /usr/bin/$(DST)
uninstall:
	@echo -e "\e[0;32muninstall \e[1;35m/usr/bin/$(DST)\e[0m"
	rm -f /usr/bin/$(DST)
commit: dist-clean
	@echo -e "\e[0;32mcommit\e[0m"
	@git add -A .
	@git diff --cached
	@git commit -a || true
	@$(MAKEC) . spread
# <<<
# interfaces
MAKEC_UPWARD_PHONY = all link cleanall commitall spreadall dist-clean \
		rebuild edit test minify install uninstall print-root-dir
.PHONY: current clean spread commit $(MAKEC_UPWARD_PHONY)

define UPWARD_PHONY
#!$1:
#!	@$(MAKEC) .. $1
endef
#!current: compile link
#!compile: subs-compile
#!	@$(MAKEC) . do-compile COMPILE=1
#!do-compile: $(OBJ)
#!remove-makefile: subs-remove-makefile
#!	@rm -f makefile
#!commit: remove-makefile
#!	@echo -e "\e[0;32mcommit\e[0m"
#!	@git add -A .
#!	@git diff --cached
#!	@git commit || true
#!	@$(MAKEC) .. spreadall
$(foreach phony,$(MAKEC_UPWARD_PHONY),$(eval $(call UPWARD_PHONY,$(phony))))
spread: subs-spread
clean: $(OBJ_EXIST)
	@echo -e "\e[1;31mdelete\e[1;35m" $^ "\e[0m"
	@rm -f $^
# >>>




# linkage
$(DST): $(OBJ_EXIST)
	@echo -e "\e[0;32mlink \e[1;35m$@: \e[0;35m$^\e[0m"
	$(LD) -o $@ $^ $(FLG) $(LIB)




# <<<
# sub dir spreading
subs:
	$(foreach subdir, $(DIR), $(call SPREAD,$(subdir)))
subs-compile:
	$(call SUBS_MAKE,compile)
subs-spread:
	$(call SUBS_MAKE,spread)
subs-remove-makefile:
	$(call SUBS_MAKE,remove-makefile)

# concrete compiling process
ifdef COMPILE
.$(SUFFIX).o:
	@echo -e "\e[0;32mcompile \e[1;35m$<\e[0m"
	$(CC) -c -o $@ $< $(FLG) $(LIB)

$(foreach file,$(SRC), \
	$(eval $(strip $(shell $(CC) -MM $(FLG) $(file) | sed 's/ \\//g' | awk '{printf$$$$0}'))$(\n)) \
)
endif
# >>>

