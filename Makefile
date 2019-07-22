CPPFLAGS=
LDFLAGS=-g
LDLIBS=
RM=rm -fr
GPP=g++
OBJS=Source.o tinyxml2.o
SYMS=*.dSYM

all: Source

Source: $(OBJS)
	$(GPP) $(LDFLAGS) -o Source $(OBJS)  $(LDLIBS)

Source.o: src/Source.cpp
	$(GPP) $(CPPFLAGS) -c src/Source.cpp

tinyxml2.o: src/tinyxml2.cpp
	$(GPP) $(CPPFLAGS) -c src/tinyxml2.cpp
   
clean:
	$(RM) $(OBJS) $(SYMS)

distclean:
	$(RM) Source $(OBJS) $(SYMS)

