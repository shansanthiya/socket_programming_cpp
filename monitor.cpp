//monitor

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <bits/stdc++.h>

using namespace std;

int main()
{
    	int monitor;
    	int portNumM = 26152; 
    	int bufsize = 5000;
    	char buffer[bufsize];
	//char buffer2[bufsize];
	string search_out;
	string match_out;
	string func;
	string word;

    	struct sockaddr_in server_addr;
    	inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr));

    	monitor = socket(AF_INET, SOCK_STREAM, 0);

    	if (monitor < 0) 
    	{
        	cout << "\nError creating socket" << endl;
        	exit(1);
    	}
    
    	server_addr.sin_family = AF_INET;
    	server_addr.sin_port = htons(portNumM);

    	if (connect(monitor,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
	{
		cout << "\nThe Monitor is up and running" << endl;
		
		while(1)
		{
		
			recv(monitor,buffer,bufsize,0);
			string str0(buffer);
		
			func = str0.substr(0, str0.find(" "));
			int lx = func.length();
			//cout << "\nlength: " << l1 << endl;
			word = str0.substr(lx+1);
			//cout << "\nThe monitor recieved input <" << word << "> and operation <" << func << ">" << endl;
			transform(func.begin(), func.end(), func.begin(), ::tolower);
			
			recv(monitor,buffer,bufsize,0);
			//cout << buffer << endl;

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

			recv(monitor,buffer,bufsize,0);
			//cout << buffer << endl;

	
			string str(buffer);

			string word1, word2; string input1; string input2; int l1 =0 ; int l4 =0 ;

			if(func.compare("search") == 0)
			{
				string wordx = str.substr(0, str.find("*"));
				l1 = wordx.length();
				if ( l1 == 0 )
				{
					cout << "\nFound no matches for <" << word << ">" << endl;
					//exit(0);
				}
				else
				{
					word1 = wordx.substr(0, wordx.find(" ::"));
					int l2 = word1.length();
					input1 = wordx.substr(l2+4);
				}
			
				string wordy = str.substr(l1+1);
				l4 = wordy.length();
				if ( l4 != 0)
				{
				word2 = wordy.substr(0, wordy.find(" ::"));
				int l3 = word2.length();
				input2 = wordy.substr(l3+4);
				}
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
			 		//cout << "\n n = " << n;
					//cout << "\n f = " << f;
					print_p = str.substr(f, n-f);
					f = n+1;
					cout << "<" << print_p << ">" << endl;
					i++;
				}
			}
		}
			
		else if(func.compare("search") == 0)
		{
			if ( l1 != 0)
				cout << "\nFound a match for <" << word1 << ">:\n<" << input1 << ">" << endl;
			if (l4 != 0)
				cout << "\nOne edit distance match is <" << word2 << ">:\n<" << input2 << ">" << endl;
		}
		else if(func.compare("suffix") == 0)
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

		}
	}

	close(monitor);
    	return 0;
}
