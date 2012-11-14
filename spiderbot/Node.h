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

#include <string>
using namespace std;

/**
* Addition in Version 0.3
* I have added a new text data member and setter and getter functions for it, which will contain the
* text of a single word in the div tag.
*/


#ifndef Node

class Node{

	private:

	int id;
	Node * next;
	string link;
	string text;

	public:

	Node();
	Node(int id,string link,string text);
	void setId(int id);
	void setNext(Node *node);
	void setLink(string link);
	void setText(string text);
	int getId();
	Node * getNext();
	string getLink();
	string getText();

};

#endif
