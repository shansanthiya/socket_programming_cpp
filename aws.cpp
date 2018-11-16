//aws

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sstream>

using namespace std;

int main()
{
	int client_fd, monitor_fd, child1_fd, child2_fd;
	int server_fd;
    	int portNumCl = 25152;
	int portNumM = 26152;
	int portNumA = 21152;
	int portNumB = 22152;
	int portNumC = 23152;
	int portNumUDP= 24152;
    	int bufsize = 5000;
    	char buf_cl_aws[bufsize];
	string func;
	string word;

    	struct sockaddr_in client;
	struct sockaddr_in monitor;
	struct sockaddr_in serverA;
	struct sockaddr_in serverB;
	struct sockaddr_in serverC;
	struct sockaddr_in server;

    	socklen_t size1, size2;
	//cout << "test" << endl;

   	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	
   	if (client_fd < 0) 
	{
       		cout << "\nError creating socket" << endl;
        	exit(1);
	}

    	client.sin_family = AF_INET;
    	client.sin_addr.s_addr = htons(INADDR_ANY);
    	client.sin_port = htons(portNumCl);

    	if ((bind(client_fd, (struct sockaddr*)&client,sizeof(client))) < 0) 
    	{
        	cout << "\nError binding connection" << endl;
        	return -1;
    	}

	monitor_fd = socket(AF_INET, SOCK_STREAM, 0);
	
   	if (monitor_fd < 0) 
	{
       		cout << "\nError creating socket" << endl;
        	exit(1);
	}

    	monitor.sin_family = AF_INET;
    	monitor.sin_addr.s_addr = htons(INADDR_ANY);
    	monitor.sin_port = htons(portNumM);

    	if ((bind(monitor_fd, (struct sockaddr*)&monitor,sizeof(monitor))) < 0) 
    	{
        	cout << "\nError binding connection" << endl;
        	return -1;
    	}


   	server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
   	if (server_fd < 0) 
	{
       		cout << "\nError creating socket" << endl;
        	exit(1);
	}

    	serverA.sin_family = AF_INET;
    	serverA.sin_addr.s_addr = htons(INADDR_ANY);
    	serverA.sin_port = htons(portNumA);
	socklen_t len = sizeof(serverA);

	serverB.sin_family = AF_INET;
    	serverB.sin_addr.s_addr = htons(INADDR_ANY);
    	serverB.sin_port = htons(portNumB);
	socklen_t len1 = sizeof(serverB);
	
	serverC.sin_family = AF_INET;
    	serverC.sin_addr.s_addr = htons(INADDR_ANY);
    	serverC.sin_port = htons(portNumC);
	socklen_t len2 = sizeof(serverC);

	server.sin_family = AF_INET;
    	server.sin_addr.s_addr = htons(INADDR_ANY);
    	server.sin_port = htons(portNumUDP);
	socklen_t len0 = sizeof(server);

    	if ((bind(server_fd, (struct sockaddr*)&server,sizeof(server))) < 0) 
    	{
        	cout << "\nError binding connection" << endl;
        	return -1;
    	}
	
    	size1 = sizeof(client);
	listen(client_fd, 10);
	//cout << "\nListening for clients" << endl;

	size2 = sizeof(monitor);
	listen(monitor_fd, 10);
	//cout << "\nListening for monitor" << endl;
	
	cout << "\nThe AWS is up and running" << endl;

	int i = 0;

	child2_fd = accept(monitor_fd,(struct sockaddr *)&monitor,&size2);

    		if (child2_fd < 0) 
        		cout << "\nError accepting" << endl;
    		else 
			cout << "\nConnected to Monitor" << endl ;

    	while (1)
	{
		i = i+1;
		//cout << i << endl;

    		child1_fd = accept(client_fd,(struct sockaddr *)&client,&size1);

    		if (child1_fd < 0) 
        		cout << "\nError accepting" << endl;
    		else 
			cout << "\nConnected to Client" << endl ;


		recv(child1_fd, buf_cl_aws,bufsize,0);
		//cout << buffer << endl;
		
		string str(buf_cl_aws);
		func = str.substr(0, str.find(" "));
		int l1 = func.length();
		//cout << "\nlength: " << l1 << endl;
		
		word = str.substr(l1+1);

		cout << "\nThe AWS recieved input = <" << word << "> and function = <" << func
			<< "> from the client using TCP over port <" << portNumCl << ">" << endl;
			
		char buf_aws_ser1[bufsize];
		char buf_aws_ser2[bufsize];
		
		serverA.sin_family = AF_INET;
    	        serverA.sin_addr.s_addr = htons(INADDR_ANY);
    	        serverA.sin_port = htons(portNumA);
	        socklen_t len = sizeof(serverA);

		sendto(server_fd, buf_cl_aws, bufsize, 0,(struct sockaddr*)&serverA,sizeof(serverA));
		cout << "\nThe AWS sent <" << word << "> and <" << func << "> to Backend-Server A" << endl;

		sendto(server_fd, buf_cl_aws, bufsize, 0,(struct sockaddr*)&serverB,sizeof(serverB));
		cout << "The AWS sent <" << word << "> and <" << func << "> to Backend-Server B" << endl;
		
		sendto(server_fd, buf_cl_aws, bufsize, 0,(struct sockaddr*)&serverC,sizeof(serverC));
		cout << "The AWS sent <" << word << "> and <" << func << "> to Backend-Server C" << endl;
		string str1, str2, str3, str4, str5,str6;
		
		for ( int k = 0 ; k < 6; k++)
		{	
		
			recvfrom ( server_fd, buf_aws_ser1, bufsize, 0,(struct sockaddr*)&serverA,&len);
			//cout << "\nThe AWS recieved <" << buf_aws_ser1 << "> similar words from Backend-Server A " << endl;
			string strx(buf_aws_ser1);
			
			int port = ntohs(serverA.sin_port);
			//cout << "\n port = " << port << endl;

			if (isdigit(strx[0]))
			{	
				if ( port == 21152 )	
					str1 = strx;
				else if ( port == 22152 )
					str3 = strx;
				else if ( port == 23152 )
					str5 = strx;
					
			}
			else
			{
				if ( port == 21152 )
					str2 = strx;
				else if ( port == 22152 )
					str4 = strx;
				else if ( port == 23152 )
					str6 = strx;
			}

		}

		string str7 = str1 + "#" + str3 + "#" +  str5 + "#";
		//cout << str7 << endl;
		strcpy( buf_aws_ser1, str7.c_str());

		string str8;
		str8.clear();

		stringstream ss0(str1);
		int a = 0;
		ss0 >> a;
		//cout << "\ntest: " << a << endl;

		stringstream ss1(str3);
		int b = 0;
		ss1 >> b;
		//cout << "\ntest: " << b << endl;

		stringstream ss2(str5);
		int c = 0;
		ss2 >> c;
		//cout << "\ntest: " << c << endl;

		int final_count = a+b+c;


		if(func.compare("prefix") == 0)
		{

			cout << "\nThe AWS recieved <" << str1 << "> matches from Backend-Server <A> using UDP over port <" << portNumUDP << ">" << endl;
			cout << "The AWS recieved <" << str3 << "> matches from Backend-Server <B> using UDP over port <" << portNumUDP << ">" << endl;
			cout << "The AWS recieved <" << str5 << "> matches from Backend-Server <C> using UDP over port <" << portNumUDP << ">" << endl;

				str8 = str2+str4+str6;
				//cout << "test: " << str8 <<endl;
				strcpy( buf_aws_ser2, str8.c_str());
			 
		}

		else if(func.compare("search") == 0)
		{
			string word1; string word2; 
			
			//string final_word;
			
			/*if ( final_count == 0 )
			{
				cout << "\nThe AWS recieved <0> similar words from Backend-Server <A> using UDP over port <" << portNumUDP << ">" << endl;
				cout << "The AWS recieved <0> similar words from Backend-Server <B> using UDP over port <" << portNumUDP << ">" << endl;
				cout << "The AWS recieved <0> similar words from Backend-Server <C> using UDP over port <" << portNumUDP << ">" << endl;	
			}*/


			int f0 = str2.find("*");
			string t1 = str2.substr(0,f0);
			string t2 = str2.substr(f0+1);
			int f1 = t1.length();
			int f2 = t2.length();

			if ( f2 == 0)
			cout << "\nThe AWS recieved <0> similar words from Backend-Server <A> using UDP over port <" << portNumUDP << ">" << endl;	
			else if ( f2 > 0)
			cout << "\nThe AWS recieved <1> similar words from Backend-Server <A> using UDP over port <" << portNumUDP << ">" << endl;
			
			int f01 = str4.find("*");
			string t11 = str4.substr(0,f01);
			string t21 = str4.substr(f01+1);
			int f11 = t11.length();
			int f21 = t21.length();

			if ( f21 == 0)
			cout << "\nThe AWS recieved <0> similar words from Backend-Server <B> using UDP over port <" << portNumUDP << ">" << endl;	
			else if ( f21 > 0)
			cout << "\nThe AWS recieved <1> similar words from Backend-Server <B> using UDP over port <" << portNumUDP << ">" << endl;
			
			int f011 = str6.find("*");
			string t111 = str6.substr(0,f011);
			string t211 = str6.substr(f011+1);
			int f111 = t111.length();
			int f211 = t211.length();

			if ( f211 == 0)
			cout << "\nThe AWS recieved <0> similar words from Backend-Server <C> using UDP over port <" << portNumUDP << ">" << endl;	
			else if ( f211 > 0)
			cout << "\nThe AWS recieved <1> similar words from Backend-Server <C> using UDP over port <" << portNumUDP << ">" << endl;

			if( f1 != 0)
				word1 = t1;
			else if (f11 != 0)
				word1 = t11;
			else if (f111 !=0)
				word1 = t111;

			if( f2 != 0)
				word2 = t2;
			else if (f21 != 0)
				word2 = t21;
			else if (f211 !=0)
				word2 = t211;

			str8 = word1+"*"+word2;
				//cout << "test: " << str8 <<endl;

			int length_w = word2.length();

			strcpy( buf_aws_ser2, str8.c_str());
		}

		else if(func.compare("suffix") == 0)
		{

			cout << "\nThe AWS recieved <" << str1 << "> matches from Backend-Server <A> using UDP over port <" << portNumUDP << ">" << endl;
			cout << "The AWS recieved <" << str3 << "> matches from Backend-Server <B> using UDP over port <" << portNumUDP << ">" << endl;
			cout << "The AWS recieved <" << str5 << "> matches from Backend-Server <C> using UDP over port <" << portNumUDP << ">" << endl;
			str8 = str2+str4+str6;
				//cout << "test: " << str8 <<endl;
				strcpy( buf_aws_ser2, str8.c_str());
		}

		send(child1_fd,buf_aws_ser1,bufsize,0);
		//cout << "\nThe AWS sent n matches <" << buf_aws_ser1 << "> to client " << endl;

		send(child1_fd,buf_aws_ser2,bufsize,0);
		if(func.compare("prefix") == 0 || func.compare("suffix") == 0 )	
		cout << "\nThe AWS sent <" << final_count << "> matches to client" << endl;
		
		send(child2_fd, buf_cl_aws,bufsize,0);

		send(child2_fd, buf_aws_ser1,bufsize,0);
		//cout << "\nThe AWS sent <" << buf_aws_ser1 << "> to monitor via TCP port <" << portNumM << ">" << endl;
			
		send(child2_fd, buf_aws_ser2,bufsize,0);
		
		if(func.compare("prefix") == 0 || func.compare("suffix") == 0 )		
		cout << "\nThe AWS sent <" << final_count << "> matches to the monitor via TCP port <" << portNumM << ">" << endl;
		//cout << num;
		string word1, word2;

		if(func.compare("search") == 0)
		{
			string wordx = str8.substr(0, str8.find("*"));
			word1 = wordx.substr(0, wordx.find(" ::"));
		
			int l1 = wordx.length();
			//cout << "\nlength: " << l1 << endl;
		
			string wordy = str8.substr(l1+1);
			word2 = wordy.substr(0, wordy.find(" ::"));
			int l2 = wordy.length();
			//cout << "\nlength: " << l2 << endl;

			if ( l1 > 0 && l2 > 0 )
			{
				cout << "\nThe AWS sent <1> matches to client" << endl;
				cout << "\nThe AWS sent <" << word1 << "> and <" << word2 << "> to the monitor via TCP port <" << portNumM << ">" << endl;
			}
			else if ( l1 > 0 && l2 == 0 )
			{
				cout << "\nThe AWS sent <1> matches to client" << endl;
				cout << "\nThe AWS sent <" << word1 << "> to the monitor via TCP port <" << portNumM << ">" << endl;
			}		
			else if ( l1 == 0 && l2 > 0 )
			{
				cout << "\nThe AWS sent <0> matches to client" << endl;
				cout << "\nThe AWS sent <" << word2 << "> to the monitor via TCP port <" << portNumM << ">" << endl;
			}
			else
			cout << "\nThe AWS sent <0> matches to client" << endl;
			

		}

		close(child1_fd);
		str2.clear();
		str4.clear();
		str6.clear();
		
	}
	
	close(child2_fd);
	close(client_fd);
	close(monitor_fd);
	close(server_fd);

    	return 0;
}

