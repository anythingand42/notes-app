INCLUDES    = `fltk-config --use-images --cxxflags`\
              `pkg-config --cflags cairo`
LIBRARY     = `fltk-config --use-images --ldstaticflags`\
              `pkg-config --libs cairo`
TARGET      =  main
.PHONY   : clean install

VPATH = src

${TARGET} : main.o CanvasBox.o OffscreenDrawer.o
	g++ -g -Wall -L/opt/lib $^ ${LIBRARY}  -o $@

OffscreenDrawer.o : OffscreenDrawer.cpp OffscreenDrawer.hpp
	g++ -g -Wall ${INCLUDES} -c $< -o $@

CanvasBox.o : CanvasBox.cpp CanvasBox.hpp
	g++ -g -Wall ${INCLUDES} -c $< -o $@

main.o : main.cpp CanvasBox.hpp
	g++ -g -Wall ${INCLUDES} -c $< -o $@

clean:
	rm *.o ${TARGET}