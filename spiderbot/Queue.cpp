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
 
#include "Queue.h"
#include <string>
#include <iostream>
using namespace std;


Queue::Queue()
{
	
	this->numOfNodes=0;
	this->head=0;
	this->last=0;
}



Queue::Queue(int numOfNodes)
{
		this->numOfNodes=numOfNodes;
		this->head=0;
		this->last=0;
	
}

void Queue::setNumOfNodes(int num)
{
		this->numOfNodes=num;
}

void Queue::setHead(Node * node)
{
	
		this->head=node;
}

void Queue::setLast(Node * node)
{
	
		this->last=node;
}

int Queue::getNumOfNodes()
{
		return this->numOfNodes;
}

Node * Queue::getHead()
{
	return this->head;
	
	
}

Node * Queue::getLast()
{
	return this->last;
	
}

bool Queue::isEmpty()
{
	if((this->numOfNodes)<=0)
	{//the queue is empty
		return true;
		
	}//end if
	else
	{//the queue is not empty
		return false;
	}//end else
	
}


Node * Queue::ifLinkExists(string link)
{

	Node * ptr=this->getHead();
	
	while(ptr!=0)
	{
		if(ptr->getLink()==link)
		{
			
			break;
		}//end if
		
		ptr=ptr->getNext();
		
	}//end while
	
	return ptr;//if the ptr is 0 then the link does not exist in the enqueue, else it does
	
	
	
}


Node *  Queue::enqueue(Node * node)
{
	
	if(this->isEmpty())
	{//the queue is empty
	
		this->setHead(node);
		this->setLast(node);
	}//end if
	else
	{//more than one nodes are in the queue
	//point the node to the next of the last node, and then point the last node to the node itself, therefore growing the queue.
		this->getLast()->setNext(node);
		this->setLast(node);
	}//end else

	this->setNumOfNodes((this->getNumOfNodes())+1);
	
}

Node *  Queue::dequeue()
{
	
	Node *node=this->getHead();//node is pointing to the head
	
	if(node!=this->getLast())
	{//more than one nodes are left. the head and last are pointing to different nodes
		
		this->setHead(node->getNext());
		node->setNext(0);
		
	
	}//end if
	else
	{//only one node is left. both the head and last will point to 0. the head and last are pointing to the same node.
		this->setHead(node->getNext());
		this->setLast(node->getNext());
		
	}//end else
	
	this->setNumOfNodes((this->getNumOfNodes())-1);
	
	return node;
}






