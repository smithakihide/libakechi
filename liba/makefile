#MakeFile

FLAGS=-O3
cpu:.cpuset .main
amp:.ampset .main
hcc:.hccset .main
debug:.debugset .main
	
.ampset:
	$(eval CXX=hcc `clamp-config --cxxflags` $(FLAGS) )
	$(eval CLINK=hcc `clamp-config --ldflags`  -o exeamp)
.hccset:
	$(eval CXX=hcc `hcc-config --cxxflags`   $(FLAGS) )
	$(eval CLINK=hcc `hcc-config --ldflags` -o exehcc)
.cpuset:
	$(eval CXX=g++  $(FLAGS) -DAMP_CPU_EMULATE -std=c++14 -fopenmp)
	$(eval CLINK=g++ -fopenmp -o execpu)
.debugset:
	$(eval CXX=g++ -g -DAMP_CPU_EMULATE -D_DEBUG -std=c++14 )
	$(eval CLINK=g++  -pthread -o exedebug)

.main: strlib_akechi.h Vector.h
	$(CXX) -c strlib_akechi.cpp
	ar rcs libakechi.a strlib_akechi.o 

install:
	cp libakechi.a /usr/local/lib
	cp strlib_akechi.h /usr/local/include
	cp Vector.h /usr/local/include

uninstall:
	rm /usr/local/lib/libakechi.a
	rm /usr/local/include/strlib_akechi.h 
	rm /usr/local/include/Vector.h
