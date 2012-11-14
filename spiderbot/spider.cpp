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

#include "Queue.h"//only include Queue.h. Do not include Node.h again because Queue.h already has included it
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <vector>
using namespace std;


/**
 *
 * SpiderBot
 * Mustafa Neguib
 * Version 0.3
 *
 * */


 /**
 *
 *Changes in Version 0.1
 *
 *I have commented out most of the cout statements, as they cause the program to
 *slow down. At the moment i am only showing the current url and the number of node in
 *the list.
 *
 */


 /**
  * Changes in Verison 0.2
  *
  * I have fixed a major bug in the getBase function, but i have to perform more tests to be sure that no such bug appears in the function.
  * Furthermore, i have changed the versioning scheme. Now every push of the code will be a new version such as the curent version is 0.2.
  * The code of the next version will be Version 0.3 , no matter whether new features have been added or bugs have been fixed.
  *
  */


 /**
  * Additions & Changes in Version 0.3
  * 
  * I have added the trees for the div and p tags. Now the text contained within these tags is
  * stored in lists. I have added the required data members to the Node class to store the text 
  * data. Furthermore, i discovered i discovered a bug in the getBase function where the function
  * was crashing at a particular value of the url. The issue has been fixed, but more testing
  * is required.
  * 
  * However due to the addition of the trees of the div and p tags the single threaded spider has
  * slowed down in speed considerably even though i am extracting the required data in one pass of the code. 
  * I am working on a pipelining scheme for the spider which will further be a part of a distributed spider
  * which should speed up things a bit.
  * 
  */ 


string getPage(char domainName[100],char fileName[100]);//this function gets the HTML content from the server and returns the content as a string
void getLinks(string content,string currentURL);//this parses the HTML page and gets all the links
string formatLink(string link,string base);//this formats the link into an absolute link. the input link may be absolute or relative.
char ** parseUrl(string url);//this parses the absolute link and returns an array of strings which contains the domain and file name
string getBase(string url);//this gets the base of the absolute url
bool checkIfAllSpace(string content);//this function checks if the provided string is empty

/**
 * Added in Version 0.0
 * the titles variable contains the list of titles retrieved from the anchor tags
 * the texts variable contains the list of text retrieved from the anchor tags
 * the queue variable contains the queue of links not crawled.
 * the crawledQueue contains teh queue of links that have been crawled. Once the link has been visited it is added to this queue so that it is not
 * visited again.
 * */

vector<string> titles;
vector<string> texts;
Queue * queue=new Queue();
Queue * crawledQueue=new Queue();
Queue * textDivList=new Queue();

int main()
{
	/**
	 * Added in Version 0.0
	 *The arrays domainName and fileName were to get input from the user running the program.
	 * I am not getting the seed link from the usr now.
	 * */

	//char  domainName[100];
	//char  fileName[100];
	string absoluteUrl;
	char ** pointer;

	Node * node;

	/*printf("Enter the domain name: ");
	scanf("%s",domainName);
	printf("\n");
	printf("Enter the file name: ");
	scanf("%s",fileName);*/

/*	//cout<<"Enter the absolute url: ";
	cin>>absoluteUrl;
	////cout<<absoluteUrl<<endl;
	*/

	/**
	 * Added in Version 0.0
	 * At the moment to start the program i am providing a seed link by hardcoding the link
	 * as the first node in the queue
	 * As i will extract more links from the anchor tags in the HTML pages i will be placing them
	 * in the queue after converting and formatting them into absolute urls.
	 * */

/******************************************************************/

	node=new Node(1,"http://www.worldofpakistan.net/","");
	queue->enqueue(node);
	int i=1;
	while(!queue->isEmpty())
	{//run till the queue is not empty

		node=queue->dequeue();
		crawledQueue->enqueue(node);

		absoluteUrl=node->getLink();

		cout<<endl<<"Current Url: "<<endl<<absoluteUrl<<endl;

		if(checkIfAllSpace(absoluteUrl)==false)
		{//the absoluteUrl must not be empty

			pointer=parseUrl(absoluteUrl);//parse the absolute url into domain and path so that i can use them later on

			//cout<<endl<<"domain: "<<pointer[0]<<endl;
			//cout<<endl<<"path: "<<pointer[1]<<endl;

			string content=getPage(pointer[0],pointer[1]);//get the HTML content of the page


			//cout<<content<<endl;

			if(content.compare("--1ERROR")!=0)
			{//if the page was retrieved successfully then look for the data of interest (what we want)
				getLinks(content,absoluteUrl);//extract the anchor tags and save them into to the current queue where the links will wait to be parsed

			}//end if
			else
			{//either the domain is not available or the internet connection is down.
				//the server may not have allowed to crawl

			}//end else

		}//end if

		//cout<<endl<<i<<endl;
		i++;

	    cout<<"Num of nodes in the current queue: "<<queue->getNumOfNodes()<<endl;//get the number of nodes currently enqueued in the current queue
		cout<<"Num of nodes in the crawled queue: "<<crawledQueue->getNumOfNodes()<<endl;//get the number of nodes currently enqueued in the crawled queue


	}//end while




	return 0;
}



string getPage(char * domainName,char *fileName)
{

	/**
	 * Added in Version 0.0
	 * Typical variables needed for socket programming using Berkely Sockets
	 * This function connects to the domain server and requests for the file using
	 * HTTP/1.1 commands. Read the RFC 2616 for more information on the HTTP/1.1
	 * commands, request headers, and response header sent from the client to the server.
	 * SpiderBot behaves like a client requesting for files from the servers.
	 * Furthermore SpiderBot also handles those domains which are not reachable due to the
	 * some reason or the other, and waits for some time before closing the socket.
	 *
	 * */

int s_id,zero=0, port_no=80, numberOfBytes=0, valopt,res;
long arg;
fd_set myset;
struct timeval tv;
socklen_t lon;

string content;

struct hostent *he;
struct in_addr *addr;


he = gethostbyname(domainName);

if(he==NULL)
{//if he is null either the domain is not available or the internet connection is down.


	return "--1ERROR";

}//end if
else
{


/*
printf("Official name is: %s\n", he->h_name);

printf("IP address: ");
*/
addr=(struct in_addr *)he->h_addr;

//printf("%s\n ", inet_ntoa(*addr));


char *address=(char *)inet_ntoa(*addr);//i am converting to string format


ssize_t bytesRead;
struct sockaddr_in serv_addr,remote_addr;
int sin_size;


s_id=socket(AF_INET,SOCK_STREAM,zero);//this creates a socket and returns a file descriptor
serv_addr.sin_family=AF_INET;//this is assigning the protocol
serv_addr.sin_port=htons(port_no);//i am converting the port number using host to networks conversion
//serv_addr.sin_addr.s_addr = inet_addr("184.154.232.185");

serv_addr.sin_addr.s_addr = inet_addr(address);//i am converting back to the network format

/**
 * Added in Version 0.0
 * In the following code i am checking if the server is available and if i can connect to it.
 * I am performing a number of checks. if they all fail then the domain is unreachable.
 *
 * */


  // Set non-blocking
  if( (arg = fcntl(s_id, F_GETFL, NULL)) < 0)
  {
     //fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
     return "--1ERROR";
  } //end if

  arg |= O_NONBLOCK;
  if( fcntl(s_id, F_SETFL, arg) < 0)
   {
     //fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
     return "--1ERROR";
   } //end if

  // Trying to connect with timeout
  res = connect(s_id, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));

  if (res < 0)
  {
     if (errno == EINPROGRESS)
      {
       // fprintf(stderr, "EINPROGRESS in connect() - selecting\n");
        do {
           tv.tv_sec = 15;
           tv.tv_usec = 0;
           FD_ZERO(&myset);
           FD_SET(s_id, &myset);
           res = select(s_id+1, NULL, &myset, NULL, &tv);
           if (res < 0 && errno != EINTR) {
             // fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
              return "--1ERROR";
           } //end if
           else if (res > 0)
            {
              // Socket selected for write
              lon = sizeof(int);
              if (getsockopt(s_id, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0)
              {
                // fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno));
                 return "--1ERROR";
              }//end if
              // Check the value returned...
              if (valopt)
               {
               //  fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt));
                 return "--1ERROR";
              } //end if
              break;
           }//end else if
           else
           {
              //fprintf(stderr, "Timeout in select() - Cancelling!\n");
              return "--1ERROR";
           } //end while
        } while (1);
     }//end if
     else
     {
       // fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
        return "--1ERROR";
     }//end if
  }//end if
  // Set to blocking mode again...
  if( (arg = fcntl(s_id, F_GETFL, NULL)) < 0) {
     //fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
     return "--1ERROR";
  } //end if
  arg &= (~O_NONBLOCK);
  if( fcntl(s_id, F_SETFL, arg) < 0) {
     //fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
     return "--1ERROR";
  }//end if


	//i have to send the request to the server first of what i want
	/**
	 * Added in Version 0.0
	 * I am sending the HTTP/1.1 commands to the server
	 *
	 * */

	//char *data="GET / HTTP/1.1\nHost:www.mntechsolutions.net\n\n"; //i have to send all the messages as one message
	//char data[1000]="GET ";
	string data="GET ";
	char *charArrayData;//this array will be used to send data to the server

	/**
	 * Change in Version 0.1
	 * I have changed the char arrays containing the HTTP request to c++ string because some data might get lost.
	 */
	data.append(fileName);

	data.append(" HTTP/1.1\nHost:");

	data.append(domainName);//i am appending the domain name

	/**
	 * Change in Version 0.1
	 * I have changed the User-Agent from SpiderBotV0.1 to SpiderBot.
	 * This is the header request which tells the server who the client is.
	 * i have made this change because the char arrays were limited in size, though string datatype
	 * also has a limit but. Initially i had the problem of the buffer overflow error as i was using
	 * the function strcpy function which is insecure if the string is written into the memory
	 * exceeding the allowed size. Furthermore there might have been a problem if i would have gotten an
	 * extremely long string for the HTTP request, then the arrays would have lost the complete string.
	 * Furthermore using the string datatype is alot more easier than using just character arrays.	 *
	 */

	data.append("\nConnection: close\nCache-Control: max-age=0.0\nUser-Agent: SpiderBot\nAccept: link/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8,image/gif,image/jpeg,image/jpg\nAccept-Language: en-US,en;q=0.8\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3\nContent-Transfer-Encoding: base64\n\n");//i am appending the new lines. the command has to end with two new lines \n\n

	//Mozilla/5.0 (X11; Linux i686) AppleWebKit/534.30 (KHTML, like Gecko) Ubuntu/10.10 Chromium/12.0.742.91 Chrome/12.0.742.91 Safari/534.30

	int x=0;
	//if((x=send(s_id,data,1000,0))==-1)

	/**
	 *Change in Version 0.1
	 * i am converting from string to char array so that i can send to the server using
	 * the send function.
	 */

	charArrayData=new char [data.size()+1];
	strcpy (charArrayData, data.c_str());

	if(send(s_id,charArrayData,data.size()+1,0)==-1)
	{//if no data has been sent then return an error
		//perror("send");
		//exit(1);

		return "--1ERROR";
	}//end if



/******************************************
i have to do multiple receives when receiving the complete html data. i can only get 1000 characters per recv.
******************************************/


FILE *p = NULL;
char file[1000];

//strcat(file,domainName);
//strcat(file,fileName);
//strcat(file,".mus");


// p = fopen(file, "a");//a is for appending to the file

 //p = fopen(file, "w");//w is for writing to a new file

char data1[1260]=" ";
int done=0;
char * val;
char *val1="</html>";
int i=0;
char a;
int counterStuck=0;

fcntl(s_id,F_SETFL,O_NONBLOCK);


while(done==0)
{


	while(recv(s_id,data1,1260,0)==-1)
	{
		//perror("recv");
		//exit(1);

		counterStuck++;

		//cout<<endl<<counterStuck<<endl;
		if(counterStuck>10000000)
		{//i have to run this loop for 10 million times so that i can give the program enough time to get data from the server. i am using non blocking and i have to check again and again for the data, if it is ready to be read.
		//this will also give the program enough time to wait for the data. if the data is not available after this time, then i will not get that data and will regard this as failure.
		return "--1ERROR";

		}//end if

		//cout<<endl<<counterStuck<<endl;
	}//end while


	content.append(data1);

	//printf("%s\n",data1);//print the data that has been retreived from the file


	//fwrite(data1,1260, 1, p);//after the write is done, the pointer is advanced by 1260 bytes.


	for(i=0;i<1260;i++)
	{//clearing the data1 array
		data1[i]=' ';

	}//end for


	numberOfBytes=numberOfBytes+1260;

	//to terminate this loop i am checking if i am receiving any message. if the connection does not exist then i am done with the collection of the data.
	//i had first used this technique when i was developing my ams cloud system.
	if(recv(s_id,&a,1,MSG_PEEK | MSG_DONTWAIT)==0)
	{//i am checking if the connection is still alive or is it dead.
		done=1;
	}//end if
	else
	{
		done=0;
	}//end else


}//end while

//fclose(p);


//printf("\nTotal Number of bytes read: %d\n",numberOfBytes);

close(s_id);

////cout<<content;

//int length=content.length();

////cout<<endl<<"length: "<<length<<endl;

return content;

}//end else

}



void getLinks(string content,string currentURL)
{
	
	/**
	 * Added in Version 0.0
	 * I am using a technique that i learned in the course Compiler Construction where we design and
	 * develop transition diagrams of the language. Transition diagrams are mainly used to be able to
	 * recognize tokens in the source code. While coding the transition diagram we use a state which
	 * keeps track of what has been read. This allows us to have control over what the parser is doing and
	 * also look for any kind of token that we need.
	 * In this version i am looking for the anchor tag and the attributes href, title and text of the anchor tag.
	 *
	 * */

	int length=content.length(),state=0,i=0;
	string link,title,text, divText;
	Node * nodeLink;
	Queue * links=new Queue();//this stores the links
    Node * nodeDivText;
	/**
	 * Addition in Version 0.1
	 * i am setting the value "null" so that i can check if the base tag exists in the code or not
	 * */

	string baseTagURL="null";

	while(i<length)
	{//run from start to the end of the string
	//	cout<<endl<<state<<"-->";
		switch(state)
		{//the state will keep track of what has been read

			case 0:

			if(content[i]=='<')
			{
				state=1;
			}//end if
			else if(content[i]==' ')
			{//if the character is space
				state=0;
			}//end else if
			else
			{
				state=0;
			}//end else


			break;

			case 1:

			if(content[i]=='a' || content[i]=='A')
			{//start of the <a tag
				state=2;
			}//end if
			else if(content[i]=='b' || content[i]=='B')
			{//start of the <base tag

				/**
				 * Addition in Version 0.1
				 * I am adding the tree for the base tag. States starting from 57 till 72 belong
				 * to the tree for the base tag which tells what the base url of the web page retrieved is.
				 * */

				state=57;

			}//end else if
			else if(content[i]=='d' || content[i]=='D')
			{//start of the div tag

                /**
                * Addition in Version 0.3
                * I have added the tree for extracting the text within the div tag.
                *
                */


                state=72;


			}//end else if
			else if(content[i]=='p' || content[i]=='P')
			{//start of the p tag
			    state=77;

			}//end else if
			else
			{
				state=0;
			}//end else



			break;

			case 2:

			if(content[i]==' ')
			{
				state=3;
			}//end if
			else
			{
				state=0;
			}//end else

			break;

			case 3:

			if(content[i]=='h' || content[i]=='H')
			{
				state=4;
			}//end if
			else if(content[i]=='t')
			{
				state=31;
			}//end else if
			else if(content[i]==' ')
			{
				state=3;
			}//end else if
			else if(content[i]=='>')
			{
				state=0;
			}//end else if
			else
			{
				state=3;
			}//end else

			break;

			case 4:

			if(content[i]=='r' || content[i]=='R')
			{
				state=5;
			}//end if
			else
			{
				state=3;
			}//end else


			break;

			case 5:

			if(content[i]=='e' || content[i]=='E')
			{
				state=6;
			}//end if
			else
			{
				state=3;
			}//end else


			break;

			case 6:

			if(content[i]=='f' || content[i]=='F')
			{
				state=7;
			}//end if
			else
			{
				state=3;
			}//end else


			break;

			case 7:


			if(content[i]==' ')
			{
				state=8;
			}//end if
			else if(content[i]=='=')
			{

				state=9;
			}//end else if
			else
			{
				state=3;
			}//end else


			break;

			case 8:


			if(content[i]==' ')
			{
				state=8;
			}//end if
			else if(content[i]=='=')
			{
				state=9;
			}//end else if
			else
			{
				state=3;
			}//end else


			break;

			case 9:


			if(content[i]==' ')
			{
				state=10;
			}//end if
			else if(content[i]=='"')
			{
				state=11;
			}//end else if
			else if(content[i]=='\'')
			{
				state=11;
			}//end else if
			else
			{
				state=3;
			}//end else

			break;

			case 10:


			if(content[i]==' ')
			{
				state=10;
			}//end if
			else if(content[i]=='"')
			{
				state=11;
			}//end else if
			else if(content[i]=='\'')
			{
				state=11;
			}//end else if
			else
			{
				state=3;
			}//end else


			break;

			case 11:

			if(content[i]=='\'')
			{//no link given

				state=13;
			}//end if
			else if(content[i]=='"')
			{//no link given

				state=13;
			}//end if
			else if(content[i]==' ')
			{//space

				state=11;
			}//end else if
			else
			{//text
				link.append(1,content[i]);
				state=12;
			}//end else



			break;

			case 12:

			if(content[i]=='\'')
			{
				state=13;
			}//end if
			else if(content[i]=='"')
			{
				state=13;
			}//end else if
			else if(content[i]==' ')
			{//space
				state=11;
			}//end else if
			else
			{//text
				link.append(1,content[i]);

				state=12;
			}//end else

			break;

			case 13:
			//cout<<link<<endl;
			//links.push_back(link);

			nodeLink=new Node(1,link,"");

			links->enqueue(nodeLink);

			link="";

			if(content[i]=='t')
			{
				state=15;
			}//end if
			else if(content[i]=='>')
			{
				state=16;
			}//end else if
			else
			{//text,space
				state=14;
			}//end else

			break;

			case 14:

			if(content[i]=='>')
			{
				state=16;
			}//end if
			else if(content[i]=='t')
			{
				state=15;
			}//end else if
			else
			{//text,space
				state=14;
			}//end else

			break;

			case 15:

			if(content[i]=='i')
			{
				state=17;
			}//end if
			else
			{//text
				state=13;
			}//end else


			break;

			case 16:

//append to the text of the link text

			text.append(1,content[i]);

			state=30;
			break;

			case 17:

			if(content[i]=='t')
			{
				state=18;
			}//end if
			else
			{//text
				state=13;
			}//end else

			break;

			case 18:

			if(content[i]=='l')
			{
				state=19;
			}//end if
			else
			{//text
				state=13;
			}//end else

			break;

			case 19:

			if(content[i]=='e')
			{
				state=20;
			}//end if
			else
			{
				state=13;
			}//end else

			break;

			case 20:

			if(content[i]==' ')
			{

				state=21;
			}//end if
			else if(content[i]=='=')
			{
				state=22;
			}//end else if
			else
			{//text
				state=13;

			}//end else

			break;

			case 21:


			if(content[i]==' ')
			{
				state=21;
			}//end if
			else if(content[i]=='=')
			{
				state=22;
			}//end else if
			else
			{//text
				state=13;
			}//end else

			break;


			case 22:

			if(content[i]=='\'')
			{
				state=24;
			}//end if
			else if(content[i]=='"')
			{
				state=24;
			}//end else if
			else if(content[i]==' ')
			{
				state=23;
			}//end else if
			else
			{//text
				state=13;
			}//end else


			break;

			case 23:

			if(content[i]==' ')
			{
				state=23;
			}//end if
			else if(content[i]=='\'')
			{
				state=24;
			}//end else if
			else if(content[i]=='"')
			{
				state=24;
			}//end else if
			else
			{
				state=13;
			}//end else

			break;

			case 24:

	// append to the title of the link

			title.append(1,content[i]);

			state=25;

			break;

			case 25:

			if(content[i]=='\'')
			{

				state=26;

			}//end if
			else if(content[i]=='"')
			{

				state=26;

			}//end else if
			else
			{//text,space

			//append to the title of the link

				title.append(1,content[i]);

				state=25;
			}//end else

			break;

			case 26:

			//cout<<title<<endl;
			titles.push_back(title);
			title="";

			if(content[i]=='>')
			{
				state=27;
			}//end if
			else
			{//text,space
				state=28;
			}//enf else

			break;

			case 27:

			//append to the text of the link text
			text.append(1,content[i]);
			state=29;

			break;

			case 28:

			if(content[i]=='>')
			{
				state=27;
			}//end if
			else
			{//text,space
				state=28;
			}//end else

			break;

			case 29:


			if(content[i]=='<')
			{

				//cout<<text<<endl;
				texts.push_back(text);
				text="";

				state=0;
			}//end if
			else
			{	//append to the text of the link text
				//text,space
				text.append(1,content[i]);
				state=29;
			}//end else


			break;

			case 30:

			if(content[i]=='<')
			{
				//cout<<text<<endl;
				texts.push_back(text);
				text="";

				state=0;
			}//end if
			else
			{//text,space
				//append the text of the link text

				text.append(1,content[i]);

				state=30;
			}//end else

			break;


			case 31:

			if(content[i]=='i')
			{
				state=32;
			}//end if
			else
			{//text,space
				state=3;
			}//end else

			break;

			case 32:

			if(content[i]=='t')
			{
				state=33;
			}//end if
			else
			{//text,space
				state=3;
			}//end else

			break;

			case 33:

			if(content[i]=='l')
			{
				state=34;
			}//end if
			else
			{//text,space
				state=3;
			}//end else

			break;


			case 34:

			if(content[i]=='e')
			{
				state=35;
			}//end if
			else
			{//text,space
				state=3;
			}//end else

			break;


			case 35:

			if(content[i]==' ')
			{
				state=36;
			}//end if
			else if(content[i]=='=')
			{
				state=37;
			}//end else if
			else
			{//text
				state=3;
			}//end else

			break;


			case 36:

			if(content[i]==' ')
			{
				state=36;
			}//end if
			else if(content[i]=='=')
			{
				state=37;
			}//end else if
			else
			{//text
				state=3;
			}//end else

			break;


			case 37:

			if(content[i]==' ')
			{
				state=38;

			}//end if
			else if(content[i]=='\'')
			{
				state=39;
			}//end else if
			else if(content[i]=='"')
			{
				state=39;
			}//end else if
			else
			{//text
				state=3;
			}//end else


			break;

			case 38:

			if(content[i]==' ')
			{
				state=38;
			}//end if
			else if(content[i]=='\'')
			{
				state=39;
			}//end else if
			else if(content[i]=='"')
			{
				state=39;
			}//end else if
			else
			{//text
				state=3;
			}//end else

			break;


			case 39:

			title.append(1,content[i]);

			state=40;

			break;

			case 40:

			if(content[i]=='\'')
			{
				state=41;
			}//end if
			else if(content[i]=='"')
			{
				state=41;
			}//end else if
			else
			{//text,space

				title.append(1,content[i]);
				state=40;

			}//end else

			break;

			case 41:


			//cout<<title<<endl;
			titles.push_back(title);
			title="";

			if(content[i]=='>')
			{
				state=16;
			}//end else if
			else
			{//text,space
				state=42;
			}//end else

			break;

			case 42:

			if(content[i]=='>')
			{
				state=16;
			}//end if
			else
			{//text,space
				state=42;
			}//end else

			break;

			case 43:


			if(content[i]=='h' || content[i]=='H')
			{
				state=44;
			}//end if
			else
			{//text,space
				state=43;
			}//end else

			break;

			case 44:

			if(content[i]=='r' || content[i]=='R')
			{
				state=45;
			}//end if
			else
			{
				state=43;
			}//end else

			case 45:

			if(content[i]=='e' || content[i]=='E')
			{
				state=46;
			}//end if
			else
			{//text,space
				state=43;
			}//end else

			break;

			case 46:

			if(content[i]=='f' || content[i]=='F')
			{
				state=47;
			}//end if
			else
			{//text,space
				state=43;
			}//end else

			break;

			case 47:

			if(content[i]==' ')
			{
				state=48;
			}//end if
			else if(content[i]=='=')
			{
				state=49;
			}//end else if
			else
			{//text
				state=43;
			}//end else

			break;

			case 48:

			if(content[i]==' ')
			{
				state=48;
			}//end if
			else if(content[i]=='=')
			{
				state=49;

			}//end else if
			else
			{
				state=43;
			}//end else

			break;

			case 49:


			if(content[i]==' ')
			{
				state=50;
			}//end if
			else if(content[i]=='\'')
			{
				state=51;
			}//end else if
			else if(content[i]=='"')
			{
				state=51;
			}//end else if
			else
			{//text
				state=43;
			}//end else


			break;


			case 50:

			if(content[i]==' ')
			{
				state=50;
			}//end if
			else if(content[i]=='\'')
			{
				state=51;
			}//end else if
			else if(content[i]=='"')
			{
				state=51;
			}//end else if
			else
			{
				state=43;
			}//end else

			break;


			case 51:

			if(content[i]=='\'')
			{//no link given
				state=53;
			}//end  if
			else if(content[i]=='"')
			{//no link given
				state=53;
			}//end else if
			else if(content[i]==' ')
			{
				state=51;
			}//end else if
			else
			{
				link.append(1,content[i]);
				state=52;
			}//end else


			break;

			case 52:

			if(content[i]=='\'')
			{
				state=53;

			}//end if
			else if(content[i]=='"')
			{
				state=53;
			}//end else if
			else if(content[i]==' ')
			{
				state=51;
			}//end else if
			else
			{
				link.append(1,content[i]);
				state=52;
			}//end else

			break;

			case 53:


			//cout<<link<<endl;
			//links.push_back(link);
			nodeLink=new Node(1,link,"");
			links->enqueue(nodeLink);
			link="";

			if(content[i]=='>')
			{

				state=55;
			}//end if
			else
			{//text,space

				state=54;
			}//end else

			break;

			case 54:


			if(content[i]=='>')
			{

				state=55;
			}//end if
			else
			{//text,space

				state=54;
			}//end else


			break;


			case 55:


			text.append(1,content[i]);

			state=56;

			break;

			case 56:


			if(content[i]=='<')
			{
				//cout<<text<<endl;
				texts.push_back(text);
				text="";

				state=0;
			}//end if
			else
			{//text,space

				text.append(1,content[i]);

				state=56;
			}//end else

			break;

			case 57:
				//cout<<content[i]<<endl;
			if(content[i]=='a' || content[i]=='A')
			{
				state=58;
			}//end if
			else
			{
				state=0;
			}//end else

			break;

			case 58:

			if(content[i]=='s' || content[i]=='S')
			{
				state=59;
			}//end if
			else
			{
				state=0;
			}//end else

			break;

			case 59:

			if(content[i]=='e' || content[i]=='E')
			{
				state= 60;
			}//end if
			else
			{
				state=0;
			}//end else

			break;

			case 60:


			if(content[i]==' ')
			{
				state= 61;
			}//end if
			else
			{
				state=0;
			}//end else


			break;

			case 61:

			if(content[i]==' ')
			{
				state=61;
			}//end if
			else if(content[i]=='h' || content[i]=='H')
			{
				state= 62;
			}//end else if
			else
			{
				state=0;
			}//end else


			break;


			case 62:

			if(content[i]=='r' || content[i]=='R')
			{
				state= 63;
			}//end if
			else
			{
				state=0;
			}//end else


			break;


			case 63:

			if(content[i]=='e' || content[i]=='E')
			{
				state= 64;
			}//end if
			else
			{
				state=0;
			}//end else

			break;


			case 64:

			if(content[i]=='f' || content[i]=='F')
			{
				state= 65;
			}//end if
			else
			{
				state=0;
			}//end else


			break;


			case 65:


			if(content[i]==' ')
			{
				state= 66;
			}//end if
			else if(content[i]=='=')
			{
				state=67;
			}//end else if
			else
			{
				state=0;
			}//end else


			break;


			case 66:

			if(content[i]==' ')
			{
				state= 66;
			}//end if
			else if(content[i]=='=')
			{
				state=67;
			}//end else if
			else
			{
				state=0;
			}//end else

			break;


			case 67:


			if(content[i]==' ')
			{
				state= 68;
			}//end if
			else if(content[i]=='\'' || content[i]=='"')
			{
				state=69;
			}//end else if
			else
			{
				state=0;
			}//end else

			break;


			case 68:


			if(content[i]==' ')
			{
				state= 68;
			}//end if
			else if(content[i]=='\'' || content[i]=='"')
			{
				state=69;
			}//end else if
			else
			{
				state=0;
			}//end else

			break;


			case 69:


			if(content[i]==' ')
			{
				state= 69;
			}//end if
			else
			{
				baseTagURL.clear();
				baseTagURL.append(1,content[i]);
				state=70;
			}//end else


			break;


			case 70:


			if(content[i]==' ')
			{
				state= 69;
			}//end if
			else if(content[i]=='\'' || content[i]=='"')
			{
				state=71;
			}//end else if
			else
			{

				baseTagURL.append(1,content[i]);
				state=70;
			}//end else

			break;


			case 71:

			if(content[i]==' ')
			{
				state= 71;
			}//end if
			else
			{
				state=0;
			}//end else

			break;

            case 72:

            if(content[i]=='i' || content[i]=='I')
            {
                state=73;

            }//end if
            else
            {

                state=0;
            }//end else


            break;

            case 73:

            if(content[i]=='v' || content[i]=='V')
            {
                state=74;

            }//end if
            else
            {

                state=0;
            }//end else

            break;


            case 74:


            if(content[i]=='>')
            {
                state=75;

            }//end if
            else
            {

                state=74;
            }//end else



            break;


            case 75:


            if(content[i]=='<' )
            {
                state=1;

            }//end if
            else
            {

                state=76;
            }//end else


            break;


            case 76:


            if(content[i]=='<')
            {

                state=1;

            }//end if
            else if(content[i]==' ')
            {//the space will mark a new word

                nodeDivText=new Node(1,"",divText);
                textDivList->enqueue(nodeDivText);
                state=76;

            }//end else if
            else
            {
                divText.append(1,content[i]);
                state=76;


            }//end else


            break;


            case 77:

            if(content[i]=='>')
            {
                divText.clear();

                state=78;

            }//end if
            else
            {
                state=77;

            }//end else

            break;


            case 78:

            if(content[i]==' ')
            {
                nodeDivText=new Node(1,"",divText);
                textDivList->enqueue(nodeDivText);
                divText.clear();
                state=78;

            }//end if
            else if(content[i]=='<')
            {
                nodeDivText=new Node(1,"",divText);
                textDivList->enqueue(nodeDivText);
                divText.clear();

                state=1;
            }//end else
            else
            {
                 divText.append(1,content[i]);
                 state=78;

            }//end else

            break;



			default:


			state=0;

			break;

		}//end switch


		//cout<<" "<<content[i]<<" -->"<<state<<endl;
		//cout<<"baseUrl: "<<baseURL<<endl;
		//cout<<endl<<link<<" -->"<<state<<endl;
		//cout<<"state: "<<state<<endl;

        //cout<<content[i];

		i++;

	}//end while

    
    //cout<<nodeLink
    Node *node;

    cout<<"Number of divText:"<<textDivList->getNumOfNodes()<<endl;
    textDivList->clearList();
    cout<<"Number of divText:"<<textDivList->getNumOfNodes()<<endl;
/*
    while(!textDivList->isEmpty())
    {
        node=(textDivList->dequeue());
        cout<<(node->getText())<<endl;
        node=NULL;

    }//end while
*/

	string base,formattedLink;
	//cout<<"current url before base: "<<currentURL<<endl;

	/**
	 * Addition in Version 0.1
	 * i am checking if the base tag was found or not. if it was found then use the
	 * link as the currentURL
	 * */


	if(baseTagURL.compare("null")==0)
	{//no base tag was found in the code
		base=getBase(currentURL);
		
	}//end if
	else
	{
		base=getBase(baseTagURL);
	}//end else

	//cout<<endl<<"base: "<<base<<endl;


	int a=0;
	//cout<<"Number of nodes: "<<links->getNumOfNodes()<<endl;
	Node *node2;
	if(!links->isEmpty())
	{

		string link2;

		while(!links->isEmpty())
		{

			node2=links->dequeue();
			link2=node2->getLink();
			//cout<<link2;

			if(checkIfAllSpace(link2)==false)
			{

				formattedLink=formatLink(link2,base);

			//	cout<<formattedLink<<endl;


				node2=new Node(1,formattedLink,"");

				if(!crawledQueue->ifLinkExists(node2->getLink()) && !queue->ifLinkExists(node2->getLink()))
				{//if the link retrieved from the queue is not in the crawled queue and is not in the current queue then enque the link inthe queue

					queue->enqueue(node2);

				}//end if

			}//end if

		}//end while
	}//end if

	/*cout<<endl<<"titles: "<<endl;

	for(a=0;a<titles.size();a++)
	{
		cout<<titles[a]<<endl;
	}//end for


	cout<<endl<<"text: "<<endl;

	for(a=0;a<texts.size();a++)
	{
		cout<<texts[a]<<endl;
	}//end for
	*/
}

string formatLink(string link,string base)
{//this function will convert any relative link to an absolute link, and will also reformat any absolute links

	
	int state=0,i=0,length=link.length();
	string link1;
	char ** links;
	string formattedLink;
	int repeat=1;


	//cout<<"length: "<<length<<endl;

	while(i<length)
	{
		//cout<<i<<endl;
		//cout<<state<<"--->";

		switch(state)
		{
			case 0:

			if(link[i]=='h')
			{
				state=1;
			}//end if
			else if(link[i]=='/')
			{
				state=12;
			}//end else if
			else if(link[i]=='.')
			{
				state=17;

			}//end else if
			else
			{
				link1.append(1,link[i]);
				state=16;
			}//end else

			break;

			case 1:

			if(link[i]=='t')
			{
				state=2;
			}//end if
			else
			{
				state=16;
			}//end else

			break;

			case 2:


			if(link[i]=='t')
			{
				state=3;
			}//end if
			else
			{
				state=16;
			}//end else

			break;

			case 3:


			if(link[i]=='p')
			{
				state=4;
			}//end if
			else
			{
				state=16;
			}//end else

			break;

			case 4:


			if(link[i]==':')
			{
				state=5;
			}//end if
			else if(link[i]=='s')
			{
					state=6;
			}//end else if
			else
			{
				state=16;
			}//end else

			break;

			case 5:


			if(link[i]=='/')
			{
				state=7;
			}//end if
			else
			{//http:example.com is a valid link.
				state=9;
			}//end else

			break;

			case 6:


			if(link[i]==':')
			{
				state=5;
			}//end if
			else
			{
				state=16;
			}//end else

			break;

			case 7:

			if(link[i]=='/')
			{
				state=8;
			}//end if
			else
			{//http:/example.com is a valid link.
				state=9;
			}//end else


			break;


			case 8:


				link1.append(1,link[i]);
				state=9;


			break;


			case 9:


				link1.append(1,link[i]);

				////cout<<link1<<endl;
				if((i+1)==length)
				{//if . comes
					//links=parseUrl(base);
					string protocol="http://";
					//links[0].append(sizeof(link),link);

					formattedLink.append(protocol);
					formattedLink.append(link1);


				}//end if


				state=9;



			break;

			case 10:
			//file name

				//state=11;

			break;


			case 11:

			//file name

			//state=11;

			break;


			case 12:

			if(link[i]=='/')
			{
				state=14;
			}//end if
			else
			{

				link1.append(1,link[i]);

				state=13;

			}//end else


			break;

			case 13:

			state=13;

			link1.append(1,link[i]);

			if((i+1)==length)
				{//if // comes
					//http://www.domain/filename
					links=parseUrl(base);
					//cout<<endl<<"!!!!ERROR HERE!!!!"<<endl;
					//cout<<endl<<links[0]<<endl;
					//cout<<endl<<link1<<endl;
					string protocol="http://";
					//links[0].append(sizeof(link),link);
					formattedLink.append(protocol);
					formattedLink.append(links[0],strlen(links[0]));
					formattedLink.append("/");
					formattedLink.append(link1.c_str());

					//cout<<endl<<formattedLink<<endl;

				}//end if

			break;

			case 14:

			if(link[i]=='/')
			{

				state=14;
			}//end if
			else
			{
				link1.append(1,link[i]);
				state=15;
			}//end else

			break;

			case 15:


			link1.append(1,link[i]);

			if((i+1)==length)
				{//if // comes
					//http://www.filename
					//links=parseUrl(base);
					string protocol="http://";
					//links[0].append(sizeof(link),link);

					formattedLink.append(protocol.c_str());
					formattedLink.append(link1.c_str());

				}//end if

			state=15;

			break;

			case 16:


			link1.append(1,link[i]);

			if((i+1)==length)
				{//if text comes


					//links=parseUrl(base);

					//string removeParent;
					//removeParent.append(links[1],strlen(links[1]));

				//	string base1=getBase(removeParent);

					//string protocol="http://";
					//links[0].append(sizeof(link),link);
					//links[1];
					formattedLink.append(base);
					formattedLink.append("/");
					formattedLink.append(link1);

				 }//end if



			state=16;

			break;


			case 17:

			if(link[i]=='.')
			{
				state=18;
			}//end if
			else if(link[i]=='/')
			{
				state=20;
			}//end else if
			else
			{

				link1.append(1,link[i]);

				state=23;
			}//end else

			break;

			case 18:

			if(link[i]=='/')
			{
				state=19;
			}//end if
			else
			{
				link1.append(1,link[i]);


				state=24;
			}//end else


			break;


			case 19:
			if(link[i]=='.')
			{
				repeat++;

				state=17;
			}//end if
			else
			{

				link1.append(1,link[i]);
				state=22;
			}//end else



			break;

			case 20:

			link1.append(1,link[i]);

			state=21;

			break;

			case 21:

			link1.append(1,link[i]);

			if((i+1)==length)
				{//if ./ comes


					//links=parseUrl(base);
					//string protocol="http://";
					//links[0].append(sizeof(link),link);

					formattedLink.append(base.c_str());
					formattedLink.append("/");
					formattedLink.append(link1.c_str());

				}//end if

			state=21;

			break;

			case 22:

			link1.append(1,link[i]);
			
			if((i+1)==length)
				{//if ../ comes or more ../ come

					links=parseUrl(base);
					
					string removeParent;
					removeParent.append(links[1],strlen(links[1]));
					int i=0; 
					string base1;
	
					for(i=0;i<repeat;i++)
					{
						base1=getBase(removeParent);
						removeParent=base1;

					}//end for
					
					

					string protocol="http://";
					//links[0].append(sizeof(link),link);
					//links[1];

					formattedLink.append(protocol.c_str());
					formattedLink.append(links[0]);
					formattedLink.append(base1.c_str());
					formattedLink.append("/");
					formattedLink.append(link1.c_str());
					
					

				}//end if

			state=22;

			break;

			case 23:

			link1.append(1,link[i]);

			if((i+1)==length)
				{//if . comes
					//links=parseUrl(base);
					//string protocol="http://";
					//links[0].append(sizeof(link),link);

					formattedLink.append(base.c_str());
					formattedLink.append("/.");
					formattedLink.append(link1.c_str());

				}//end if

			state=23;

			break;

			case 24:

			link1.append(1,link[i]);

			if((i+1)==length)
				{//if . comes
					//links=parseUrl(base);
					//string protocol="http://";
					//links[0].append(sizeof(link),link);

					formattedLink.append(base.c_str());
					formattedLink.append("/..");
					formattedLink.append(link1.c_str());

				}//end if

			state=24;

			break;



			default: break;
		}//end switch


		i++;
	}//end while


	return formattedLink;
}


char ** parseUrl(string url)
{//this function will parse the url and divide into the domain part and the path part

	char ** pointer=new char *[2];
	int length=url.length(),pos=url.find_first_of("://");

	//cout<<endl<<"url in the parseUrl function: "<<url<<endl;

	if(pos==-1)
	{//the string was empty
		pointer=NULL;
	}//end if
	else
	{
		pos=pos+3;
		int pos1=url.find_first_of("/",pos);

		if(pos1==-1)
		{//did not find the trailing slash /


			int lengthOfDomain=length-pos;
			int lengthOfPath=1;
			string domain1,path1;//these are temporary variables


			/*cout<<"pos:"<<pos<<endl;
			cout<<"pos1:"<<pos1<<endl;
			cout<<"length:"<<length<<endl;
			cout<<"length of domain:"<<lengthOfDomain<<endl;
			*/

			domain1=url.substr(pos,lengthOfDomain);
			path1="/";
			char * domain=new char[domain1.size()+1];
			char *path=new char[path1.size()+1];
			strcpy(domain,domain1.c_str());
			strcpy(path,path1.c_str());

			/*cout<<endl<<domain<<endl;
			cout<<endl<<path<<endl;
			*/
			pointer[0]=domain;
			pointer[1]=path;


		}//end if
		else
		{

			int lengthOfDomain=(pos1-pos);
			int lengthOfPath=(length-pos1);
			string domain1,path1;//these are temporary variables

			/*cout<<"pos:"<<pos<<endl;
			cout<<"pos1:"<<pos1<<endl;
			cout<<"length:"<<length<<endl;
			cout<<"length of domain:"<<lengthOfDomain<<endl;
			cout<<"length of path:"<<lengthOfPath<<endl;
*/
			domain1=url.substr(pos,lengthOfDomain);
			
			path1=url.substr(pos1,lengthOfPath);
	/*
			cout<<endl<<"domain1: "<<domain1<<endl;
			cout<<endl<<"path1: "<<path1<<endl;*/
			char * domain=new char[domain1.size()+1];
			char *path=new char[path1.size()+1];

			strcpy(domain,domain1.c_str());
			strcpy(path,path1.c_str());

			/*//cout<<endl<<domain<<endl;
			//cout<<endl<<path<<endl;
			*/
			pointer[0]=domain;
			pointer[1]=path;


		}//end else

	}//end else

	//cout<<endl<<pointer[0]<<endl;
	//cout<<endl<<pointer[1]<<endl;
	return pointer;


}


string getBase(string url)
{//this function gets the base from the link

	
	/**
	 * Changes in Version 0.2
	 *
	 * I have fixed a bug that i had discovered while testing. I am calling this function to get the base part of the urls
	 * and also when i get the base from the base tag. In the case of getting the base part from the urls i was getting
	 * fully formed urls as i was converting them into proper fnodeLinkormat, but getting the base from the base tag has problems
	 * because that is not what i have converted, that is what i am getting from the HTML code of the page. One such case
	 * that occured was that in the base tag i had gotten then base as //www.example.com and there was no http: as you can see.
	 * Before the fix was applied this function was looking for http. Since http was not found nothing was being returned due to which
	 * i was getting a segmentation fault error as invalid memory was being accessed.
	 *
	 * I have to do more tests to make sure that no more error will be generated.
	 *
	 */
	 
	 /**
	  * Changes in Version 0.3
	  * In the previous versions i was using the function find_first_of
	  * which caused problems in the code. In reality what this function does
	  * if any character in the search string is found, then a true result is given.
	  * Due to this, for some links where the url is only / (slash meaning the root directory)
	  * a false positive was being found, as i was looking for the existence of // and not / so 
	  * as a result i have replaced that function with the more proper for this case find which
	  * searches for the exact match of the string. This is the behaviour that i am looking for. 
	  * 
	  */ 

	string base;
	int length=url.size();
	int pos=url.find("//");


	if(pos==-1)
	{
		base=url.substr(0,length);

	}//end if
	else
	{
		pos=pos+1;

		int pos1=url.find_last_of("/");

		if(pos==pos1)
		{//no path is attached to the domain name
			base=url.substr(0,length);

		}//end if
		else if(pos1>pos)
		{//if the path name is attached to the domain name
			 base=url.substr(0,pos1);
		}//end else if
		else
		{//this case will never happen
			base=url.substr(pos+1,length);

		}//end else
	}//end else


	//cout<<pos<<endl;


	//cout<<endl<<base<<endl;

	return base;

}



bool checkIfAllSpace(string content)
{
	//this functions checks if the content has only empty spaces

	int state=0;

	int length=content.size();

	//cout<<length;
	int i=0;

	while(i<length+1)
	{

		switch(state)
		{

			case 0:


			if(content[i]==' ')
			{
				state=0;
			}//end if
			else
			{
				state=1;
			}//end else

			break;

			case 1:


			return false;

			break;


			default:  return false; break;

		}//end switch

		i++;
	}//end while


	return true;

}
