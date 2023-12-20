BASE = ..

include $(BASE)/Makefile.shared
include $(BASE)/gen/Makefile.env

all: $(GENERATED_OBJ)

clean:
	rm -f $(GENERATED_OBJ)

$(GENERATED_OBJ): %.o: %.cc
	$(CXX) -c $(CFLAGS) -Wno-error $< -o $@
	