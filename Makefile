CPPFLAGS= 
LDFLAGS=-g
LDLIBS=-ltinyxml2
RM=rm -fr
GPP=g++
OBJS=Source.o
SYMS=*.dSYM

all: Source

Source: $(OBJS)
	$(GPP) $(LDFLAGS) -o Source $(OBJS)  $(LDLIBS) 

Source.o: Source.cpp
	$(GPP) $(CPPFLAGS) -c Source.cpp

clean:
	$(RM) $(OBJS) $(SYMS)

distclean: 
	$(RM) Source $(OBJS) $(SYMS)
