targets = banner
toclean = $(targets)
RM ?= rm -f

all: $(targets)
clean:
	$(RM) $(toclean)

banner_objs = banner.o
toclean += $(banner_objs)
banner: $(banner_objs)
	$(CC) $(LDFLAGS) -o $@ $($@_srcs) $($@_objs) $($@_ldflags) $($@_libs) $(LIBS)
