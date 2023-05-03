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

.PHONY: test

all: libchecksieve.a check-sieve

codegen:
	$(MAKE) -C $(BASE)/src codegen

check-sieve: $(BINARY_SRC) libchecksieve.a
	$(MAKE) -C $(BASE)/src binary
	$(CXX) $(CFLAGS) -o check-sieve $(BINARY_OBJ) libchecksieve.a

libchecksieve.a: $(GENERATED_SRC) $(LIBCHECKSIEVE_SRC) $(AST_SRC) $(SERVER_SRC)
	$(MAKE) -C $(BASE)/gen
	$(MAKE) -C $(BASE)/src lib
	ar rc libchecksieve.a $(GENERATED_OBJ) $(LIBCHECKSIEVE_OBJ) $(AST_OBJ) $(SERVER_OBJ)

test: libchecksieve.a check-sieve
	rm -Rf checksieve.*.so build || true
	python3 $(BASE)/test/setup.py build_ext -i
	python3 -m unittest discover -s test -p '*_test.py'

install: all
	mkdir -p $(INSTALL_PREFIX)/bin
	mkdir -p $(INSTALL_PREFIX)/lib
	mkdir -p $(INSTALL_PREFIX)/share/man/man1
	install check-sieve $(INSTALL_PREFIX)/bin
	install libchecksieve.a $(INSTALL_PREFIX)/lib
	install ./docs/man1/check-sieve.1 $(INSTALL_PREFIX)/share/man/man1

wasm:
	emcc --bind -o checksieve.js $(CFLAGS) $(GENERATED_SRC) $(LIBCHECKSIEVE_SRC) $(AST_SRC) $(BASE)/src/webchecksieve.cc

clean:
	$(MAKE) -C $(BASE)/gen clean
	$(MAKE) -C $(BASE)/src clean
	rm -f libchecksieve.a checksieve*.so check-sieve
	rm -Rf build
