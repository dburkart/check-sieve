BASE = ../..

include $(BASE)/Makefile.shared
include $(BASE)/src/AST/Makefile.env

all: $(AST_OBJ)

clean:
	rm -f $(AST_OBJ)

$(AST_OBJ): %.o: %.cc
	$(CXX) -c $(CFLAGS) $< -o $@