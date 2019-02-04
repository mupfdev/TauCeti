.PHONY: all clean debug

include Config.mk

all: $(OBJS) $(LIBESZFW) $(LIBTMX)
	$(CC) $(CFLAGS) $(OBJS) $(LIBESZFW) $(LIBTMX) $(LIBS) -o $(OUT)

%: %.c
	$(CC) -c $(CFLAGS) $(LIBS) -o $@ $<

$(LIBESZFW):
	make -C external/eszFW/

$(LIBTMX):
	make -C external/eszFW/

clean:
	make -C external/eszFW/ clean
	rm -f $(OBJS)
	rm -f $(OUT)
