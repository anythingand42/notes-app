INCLUDES    = `fltk-config --use-images --cxxflags`\
              `pkg-config --cflags cairo`
LIBRARY     = `fltk-config --use-images --ldstaticflags`\
              `pkg-config --libs cairo`
TARGET      =  main
.PHONY   : clean install

VPATH = src

${TARGET} : main.o CairoBox.o 
	g++ -g -Wall -L/opt/lib $^ ${LIBRARY}  -o $@

CairoBox.o : CairoBox.cpp CairoBox.h
	g++ -g -Wall ${INCLUDES} -c $< -o $@

main.o : main.cpp CairoBox.h
	g++ -g -Wall ${INCLUDES} -c $< -o $@

clean:
	rm *.o ${TARGET}