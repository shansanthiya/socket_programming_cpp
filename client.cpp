//client

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
	//cout << "argc: " << argc << endl;
    	int client_fd;
    	int portNumC = 25152; 
    	int bufsize = 5000;
    	char buffer[bufsize];
	string temp;
	string word;
	string func;
	string search_out;

	if ( argc < 3)
	{
		cout << "\nInvalid Input" << endl;
		exit(0);
	}

	func = argv[1];

	for (int i = 2; i < argc; ++i)
	{
    		if(i == argc-1)
		{
    			word = word + argv[i];
    		}
		else
		{
    			word = word + argv[i] + " ";
    		}
	}
	temp = func + " " + word;
	strcpy( buffer, temp.c_str());
	
    	struct sockaddr_in client;
    	inet_pton(AF_INET, "127.0.0.1", &(client.sin_addr));

    	client_fd = socket(AF_INET, SOCK_STREAM, 0);

    	if (client_fd < 0) 
    	{
        	cout << "\nError creating socket" << endl;
        	exit(1);
    	}
    
    	client.sin_family = AF_INET;
    	client.sin_port = htons(portNumC);

    	if (connect(client_fd,(struct sockaddr *)&client, sizeof(client)) == 0)
	{
		cout << "\nThe Client is up and running" << endl;
		send(client_fd, buffer,bufsize,0);

		//for (int n = 1; n < argc; n++)
   		cout << "\nThe Client sent <" << word << "> and <" << func << "> to AWS" << endl ;

		recv(client_fd, buffer, bufsize,0);
		//cout << "\nThe Client recieved <" << buffer << "> from AWS " << endl;
		string str1(buffer);
			
		string mn[3];
		int i = 0; 
		int f = 0; 
		int n =0;
			
		while(i < 3)
		{
			n = str1.find("#",f);
			mn[i] = str1.substr(f, n-f);
			f = n+1;
			//cout << "\n" << print_p << endl;
			i++;
		}


		stringstream ss0(mn[0]);
		int a = 0;
		ss0 >> a;
		
		stringstream ss1(mn[1]);
		int b = 0;
		ss1 >> b;
		
		stringstream ss2(mn[2]);
		int c = 0;
		ss2 >> c;
		
		int final_count = a+b+c;

		recv(client_fd, buffer, bufsize,0);

		string str(buffer);

		string word1, word2; int l1; string input;
		if(func.compare("search") == 0)
		{
			string wordx = str.substr(0, str.find("*"));
			int l1 = wordx.length();
			if ( l1 == 0 )
			{
				cout << "\nFound no matches for <" << word << ">" << endl;
				exit(0);
			}

			word1 = wordx.substr(0, wordx.find(" ::"));
			int l2 = word1.length();
			input = wordx.substr(l2+4);
		
			string wordy = str.substr(l1+1);
			word2 = wordy.substr(0, wordy.find(" ::"));
		}

		

		if(func.compare("prefix") == 0)
		{	
			if(final_count == 0)
			{
				cout << "\nFound no matches for <" << word << ">" << endl;
			}
			else
			{
				cout << "\nFound <" << final_count << "> matches for <" << word << ">" << endl;
				string print_p;
				int i = 0; 
				int f = 0; 
				int n =0;
			
				while(i < final_count)
				{
					n = str.find("#",f);
					print_p = str.substr(f, n-f);
					f = n+1;
					cout << "<" << print_p << ">" << endl;
					i++;
				}
			}
		}

		//recv(client_fd, buffer, bufsize,0);
		else if(func.compare("search") == 0)
		{
			{
				cout << "\nFound a match for <" << word << ">:\n<" << input << ">" << endl;
			}
		
		}
		else if(func.compare("suffix") == 0)
		{
			//cout << "\nThe Client recieved input <" << str << "> and <" << str<< "> from AWS " << endl;
			if(final_count == 0)
			{
				cout << "\nFound no matches for <" << word << ">" << endl;
			}
			else
			{
				cout << "\nFound <" << final_count << "> matches for <" << word << ">" << endl;
				string print_p;
					int i = 0; 
					int f = 0; 
					int n =0;
			
				while(i < final_count)
				{
				
					n = str.find("#",f);
					print_p = str.substr(f, n-f);
					f = n+1;
					cout << "<" << print_p << ">" << endl;
					i++;
				}
			}
		}

		
	}

	close(client_fd);

    	return 0;
}
