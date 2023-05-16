CC       := gcc
CFLAGS   := -Wall -O2
QMAKE    := qmake
BIN      := qtfiledialog
PLUGIN   := libqtfiledialog.so
TESTBINS := test-dlopen test-popen

define do_clean
	test ! -f plugin.mk || $(MAKE) $(MAKEFLAGS) -f plugin.mk $1
	test ! -f cmdline.mk || $(MAKE) $(MAKEFLAGS) -f cmdline.mk $1
endef


all: $(BIN) $(PLUGIN)

clean:
	-rm -f $(BIN) $(PLUGIN) $(TESTBINS)
	$(call do_clean, $@)

distclean: clean
	$(call do_clean, $@)
	-test ! -d obj_cmdline || rmdir obj_cmdline
	-test ! -d obj_plugin || rmdir obj_plugin

test: $(TESTBINS)
.PHONY: $(TESTBINS)

test-dlopen: $(PLUGIN)
	$(CC) $(CFLAGS) -o $@ $@.c -Wl,-rpath,'$$ORIGIN' -ldl $(LDFLAGS)

test-popen: $(BIN)
	$(CC) $(CFLAGS) -o $@ $@.c $(LDFLAGS)

$(PLUGIN): plugin.mk obj_plugin
	$(MAKE) -f $<

$(BIN): cmdline.mk obj_cmdline
	$(MAKE) -f $<

obj_%:
	mkdir -p $@

%.mk: %.pro
	$(QMAKE) $< -o $@

