BASE = .

ifeq ($(INSTALL_PREFIX),)
INSTALL_PREFIX := /usr/local
endif

include $(BASE)/Makefile.shared
include $(BASE)/gen/Makefile.env
include $(BASE)/src/Makefile.env
include $(BASE)/src/AST/Makefile.env

all: libchecksieve.a check-sieve

codegen:
	make -C $(BASE)/src codegen

check-sieve: $(BINARY_SRC) libchecksieve.a
	make -C $(BASE)/src binary
	$(CXX) $(CFLAGS) -o check-sieve $(BINARY_OBJ) libchecksieve.a

libchecksieve.a: $(GENERATED_SRC) $(LIBCHECKSIEVE_SRC) $(AST_SRC)
	make -C $(BASE)/gen
	make -C $(BASE)/src lib
	ar rc libchecksieve.a $(GENERATED_OBJ) $(LIBCHECKSIEVE_OBJ) $(AST_OBJ)

test: libchecksieve.a check-sieve
	rm checksieve.*.so
	python3 $(BASE)/test/setup.py build_ext -i
	python3 -m unittest discover -s test -p '*_test.py'

install: all
	mkdir -p $(INSTALL_PREFIX)/bin
	mkdir -p $(INSTALL_PREFIX)/lib
	mkdir -p $(INSTALL_PREFIX)/share/man/man1
	install check-sieve $(INSTALL_PREFIX)/bin
	install libchecksieve.a $(INSTALL_PREFIX)/lib
	install ./docs/man1/check-sieve.1 $(INSTALL_PREFIX)/share/man/man1

clean:
	make -C $(BASE)/gen clean
	make -C $(BASE)/src clean
	rm -f libchecksieve.a checksieve.so check-sieve
