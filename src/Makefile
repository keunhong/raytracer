#CC      = g++ -Wall -g -pg -O0 -fprofile-arcs -ftest-coverage
CC      = g++ -Wall -g
CFLAGS  = -c -I/opt/local/include
LDFLAGS = -L/usr/lib -L/opt/local/lib

OPENCV_LDFLAGS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lprofiler

all: raytracer 

raytracer: raytracer.o material.o color.o scene.o vec3.o luminaire.o sphere.o camera.o primitive.o plane.o ray.o rectangle.o
	$(CC) -o $@  $^ $(LDFLAGS) $(OPENCV_LDFLAGS)\

%.o : %.cpp
	$(CC) $(CFLAGS) $<

clean: cleanobj
	rm -f raytracer

cleanobj:
	rm -f *.o
