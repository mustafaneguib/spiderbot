cd spiderbot/
################compiling header files with the program
g++ -c Queue.cpp
g++ -c Node.cpp
g++ spider.cpp Queue.o Node.o -o spidercpp
##################
./spidercpp

