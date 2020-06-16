# Makefile for header reading code.

CFLAGS += -Wall -Werror -Wextra --std=c++14 -pthread
LFLAGS += -pthread -lboost_system
TARGETS_REGULAR = testa testb testreadelf
TARGETS = ${TARGETS_REGULAR} ${TARGETS_EXTRA}
TARGETSOURCES = ${TARGETS_REGULAR:=.cc}
LIBSOURCES = HeaderReader.cc HeaderWriter.cc ElfHeaderReader.cc
SOURCES = ${TARGETSOURCES} ${LIBSOURCES}
LIBOBJS = ${LIBSOURCES:.cc=.o}
OBJS_REGULAR = ${SOURCES:.cc=.o}
OBJS = ${OBJS_REGULAR} ${LIBOBJS}
DEPFILE = make.dep

-include ${DEPFILE}

clean :
	rm -f *.o ${TARGETS} ${DEPFILE}

deps :
	g++ --std=c++11 -MM ${SOURCES} >${DEPFILE}

all :  ${LIBOBJS} ${TARGETS}

echo :
	@echo "CFLAGS:  ${CFLAGS}"
	@echo "TARGETS:  ${TARGETS}"
	@echo "TARGETSOURCES:  ${TARGETSOURCES}"
	@echo "LIBSOURCES:  ${LIBSOURCES}"
	@echo "SOURCES:  ${SOURCES}"
	@echo "LIBOBJS:  ${LIBOBJS}"
	@echo "OBJS:  ${OBJS}"
	@echo "DEPFILE:  ${DEPFILE}"

testa: testa.o ${LIBOBJS}
	g++ $(LFLAGS) -o $@ $@.o ${LIBOBJS} $(LFLAGS)

testb: testb.o ${LIBOBJS}
	g++ $(LFLAGS) -o $@ $@.o ${LIBOBJS} $(LFLAGS)

testreadelf: testreadelf.o ${LIBOBJS}
	g++ $(LFLAGS) -o $@ $@.o ${LIBOBJS} $(LFLAGS)

.cc.o:
	g++ $(CFLAGS) -c $<

