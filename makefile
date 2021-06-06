OUTFILE = "bin/DisableVehicleLimit.so"
 
COMPILE_FLAGS=-m32 -c -O2 -w
LINK_FLAGS=-m32 -shared -O3

all:
	gcc $(COMPILE_FLAGS) src/SDK/amx/*.h
	g++ $(COMPILE_FLAGS) -std=c++11 src/SDK/*.cpp
	g++ $(COMPILE_FLAGS) -std=c++11 src/*.cpp
	g++ $(LINK_FLAGS) -o $(OUTFILE) *.o
	rm *.o
	strip -s $(OUTFILE)
