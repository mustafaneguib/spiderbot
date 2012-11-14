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

#include "Node.h"
#include <string>

/**
* Addition in Version 0.3
* I have added new function definitions for the new data member which will contain the text value of the individual words
* in the div tag.
*/

using namespace std;


Node::Node()
{
	this->id=0;
	this->next=0;
	this->link="";
	this->text="";

}


Node::Node(int id, string link,string text)
{
	this->id=id;
	this->next=0;
	this->link=link;
	this->text=text;

}


void Node::setId(int id)
{
		this->id=id;

}

void Node::setNext(Node *node)
{
		this->next=node;

}


void Node::setLink(string link)
{
		this->link=link;

}

void Node::setText(string text)
{

        this->text=text;

}


int Node::getId()
{
		return this->id;
}

Node * Node:: getNext()
{
		return this->next;

}

string Node::getLink()
{

	return this->link;
}

string Node::getText()
{
    return this->text;

}
