cd searchengine/
################compiling header files with the program
g++ -c Queue.cpp
g++ -c Node.cpp
g++ spiderv0.0.cpp Queue.o Node.o -o spidercpp
##################
./spidercpp

