BASE = .

ifeq ($(INSTALL_PREFIX),)
INSTALL_PREFIX := /usr/local
endif

ifdef DEBUG
CFLAGS += -DDEBUG -g
endif

include $(BASE)/Makefile.shared
include $(BASE)/gen/Makefile.env
include $(BASE)/src/Makefile.env
include $(BASE)/src/AST/Makefile.env
include $(BASE)/src/Server/Makefile.env
include $(BASE)/src/Email/Makefile.env

ASAN_LIB     := $(shell ldconfig -p 2>/dev/null | grep 'libasan\.so\b' | head -1 | awk '{print $$NF}')
ASAN_CFLAGS  := -fsanitize=address -fno-omit-frame-pointer -g
ASAN_LDFLAGS := -fsanitize=address

UBSAN_CFLAGS  := -fsanitize=undefined -fno-omit-frame-pointer -g -fno-sanitize-recover=all
UBSAN_LDFLAGS := -fsanitize=undefined

.PHONY: test test-asan test-ubsan rebase

all: libchecksieve.a check-sieve

codegen:
	$(MAKE) -C $(BASE)/src codegen

check-sieve: $(BINARY_SRC) libchecksieve.a
	$(MAKE) -C $(BASE)/src binary
	$(CXX) $(CFLAGS) -o check-sieve $(BINARY_OBJ) libchecksieve.a

libchecksieve.a: $(GENERATED_SRC) $(LIBCHECKSIEVE_SRC) $(AST_SRC) $(SERVER_SRC) $(EMAIL_SRC)
	$(MAKE) -C $(BASE)/gen
	$(MAKE) -C $(BASE)/src lib
	ar rc libchecksieve.a $(GENERATED_OBJ) $(LIBCHECKSIEVE_OBJ) $(AST_OBJ) $(SERVER_OBJ) $(EMAIL_OBJ)

rebase: check-sieve
	python3 $(BASE)/test/simulate/rebase.py

test: libchecksieve.a check-sieve
	rm -Rf checksieve.*.so build || true
	@python3 --version
	python3 $(BASE)/test/setup.py build_ext -i
	python3 -m unittest discover -s test -p '*_test.py'

test-asan:
	$(MAKE) clean
	CFLAGS="$(ASAN_CFLAGS)" $(MAKE) all
	rm -Rf build checksieve*.so
	CFLAGS="$(ASAN_CFLAGS)" LDFLAGS="$(ASAN_LDFLAGS)" python3 $(BASE)/test/setup.py build_ext -i
	ASAN_OPTIONS="halt_on_error=1:detect_leaks=0" LD_PRELOAD="$(ASAN_LIB)" \
		python3 -m unittest discover -s test -p '*_test.py'

test-ubsan:
	$(MAKE) clean
	CFLAGS="$(UBSAN_CFLAGS)" $(MAKE) all
	rm -Rf build checksieve*.so
	CFLAGS="$(UBSAN_CFLAGS)" LDFLAGS="$(UBSAN_LDFLAGS)" python3 $(BASE)/test/setup.py build_ext -i
	UBSAN_OPTIONS="halt_on_error=1:print_stacktrace=1" \
		python3 -m unittest discover -s test -p '*_test.py'

install: all
	mkdir -p $(INSTALL_PREFIX)/bin
	mkdir -p $(INSTALL_PREFIX)/$(LIBDIR)
	mkdir -p $(INSTALL_PREFIX)/share/man/man1
	install check-sieve $(INSTALL_PREFIX)/bin
	install libchecksieve.a $(INSTALL_PREFIX)/$(LIBDIR)
	cp ./docs/man1/check-sieve.1 $(INSTALL_PREFIX)/share/man/man1

wasm:
	emcc --bind -o checksieve.js $(CFLAGS) $(GENERATED_SRC) $(LIBCHECKSIEVE_SRC) $(AST_SRC) $(BASE)/src/webchecksieve.cc

clean:
	$(MAKE) -C $(BASE)/gen clean
	$(MAKE) -C $(BASE)/src clean
	rm -f libchecksieve.a checksieve*.so check-sieve
	rm -Rf build
