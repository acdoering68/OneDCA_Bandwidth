
CPPFLAGS =  -std=c++11
ICFLAGS	= -g -O6 -march=x86-64

CPP	= g++

INCLUDE = -I ../../xerces-c/src -I../../cudd/cplusplus/ -I../../cudd/cudd/

OBJECTS = algorithm.o ca.o Cone.o configurator.o conf_set.o Enum_BW.o Enum_exs.o ncabw.o TopoIndexer.o top_test.o    linux_monitor.o  

CUDDLIBS = ../../cudd/cplusplus/libobj_la-cuddObj.o \
           ../../cudd/cudd/libcudd.la \
	   ../../cudd/mtr/cudd_libcudd_la-mtrBasic.lo \
           ../../cudd/st/cudd_libcudd_la-st.lo \
           ../../cudd/util/cudd_libcudd_la-safe_mem.lo \
           ../../cudd/epd/cudd_libcudd_la-epd.lo

objects: $(OBJECTS)

%.o : %.cpp
	$(CPP) -c $(CPPFLAGS) $(ICFLAGS) $(INCLUDE) $< -o $@

ncabw: $(OBJECTS)
	g++ $(OBJECTS)     -lcudd ../../xerces-c/src/libxerces-c-4.0.so  -o ncabw

