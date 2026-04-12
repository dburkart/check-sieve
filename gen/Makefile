BASE = ..

include $(BASE)/Makefile.shared
include $(BASE)/gen/Makefile.env

all: $(GENERATED_OBJ)

clean:
	rm -f $(GENERATED_OBJ)

# Note: $(CFLAGS) is intentionally omitted here. The generated flex/bison sources
# trigger false positives under ASAN, so they are compiled with fixed flags only.
$(GENERATED_OBJ): %.o: %.cc
	$(CXX) -c $(CINCLUDES) -std=c++17 -fPIC -Wno-deprecated-register -DPLATFORM=\"$(ARCH)-$(PLATFORM)\" $< -o $@
	