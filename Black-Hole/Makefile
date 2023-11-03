all: TR

TR: main.o Tracer.o Image.o
  g++ main.o Tracer.o Image.o -o TR

main.o: main.cpp
  g++ -c main.cpp
  
Tracer.o: Tracer.cpp
  g++ -c Tracer.cpp
  
Image.o: Image.cpp
  g++ -c Image.cpp
  
clean:
  rm -rf *.o TR
