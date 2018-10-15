
SOURCES := $(shell ls *.cc)
cc := g++
CFLAGS := 
PROGRAMS = $(SOURCES:.cc=)
default:
	@echo make \<program\> [ program can be \"$(PROGRAMS)\"]
all: ${PROGRAMS}
.cc:
	$(cc) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o  $(PROGRAMS)


