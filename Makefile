INCLUDES    = `fltk-config --use-images --cxxflags`\
              `pkg-config --cflags cairo`
LIBRARY     = `fltk-config --use-images --ldstaticflags`\
              `pkg-config --libs cairo`
TARGET      =  main
.PHONY   : clean install

VPATH = src

${TARGET} : main.o CanvasBox.o 
	g++ -g -Wall -L/opt/lib $^ ${LIBRARY}  -o $@

CanvasBox.o : CanvasBox.cpp CanvasBox.h
	g++ -g -Wall ${INCLUDES} -c $< -o $@

main.o : main.cpp CanvasBox.h
	g++ -g -Wall ${INCLUDES} -c $< -o $@

clean:
	rm *.o ${TARGET}