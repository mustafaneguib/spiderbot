/*
 *  
 *SpiderBot is a web crawler written in C/C++ using the Berkely Sockets and HTTP/1.1 Protocol
 *Copyright (C) 2012  Mustafa Neguib, MN Tech Solutions
 *  
 *This file is part of SpiderBot.
 *
 *SpiderBot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *You can contact the developer/company at the following:
 *
 *Phone: 00923224138957
 *Website: www.mntechsolutions.net
 *Email: support@mntechsolutions.net , mustafaneguib@mntechsolutions.net
 *
 * 
 * */
 
#ifndef Queue
#include "Node.h"//include Node.h because i am using Node from Node.h
#include <string>
using namespace std;



class Queue{
	
private:

int numOfNodes;
Node * head;
Node * last;

public:

Queue();
Queue(int numOfNodes);
void setNumOfNodes(int num);
void setHead(Node * node);
void setLast(Node * node);
int getNumOfNodes();
Node * getHead();
Node * getLast();
bool isEmpty();//tell if the queue is empty
Node * ifLinkExists(string link);//if the link is in the queue then return the node's address, else return null
Node * enqueue(Node * node);//add to the queue
Node * dequeue();//remove from the queue


};


#endif

