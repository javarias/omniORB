MYSRCS = include.c main.c parse.c pr.c cppsetup.c ifparser.c

DIR_CPPFLAGS = -DDEBUG

ifdef NTArchitecture
MYSRCS += gnuwin32.c
LIBS = advapi32.lib
DIR_CPPFLAGS += -DWIN32
endif

OBJS = $(MYSRCS:.c=.o)

prog = $(patsubst %,$(BinPattern),omkdepend)

all:: $(prog)

$(prog): $(OBJS)
	@(libs="$(LIBS)"; $(CExecutable))

export:: $(prog)
	@$(ExportExecutable)

clean::
	$(RM) $(prog)
