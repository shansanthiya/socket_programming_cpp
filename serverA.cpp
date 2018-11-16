//serverA

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#include <fstream>
#include <string.h>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;

int prefix(string);
int search( string );
int hamming_dist( string , string );
int suffix (string );

int similar;
string search_str;
string prefix_str;
string suffix_str;

int main()
{
	int serverA_fd;
    	int portNumA = 21152;
	//int portNumA_aws = 24152;
    	int bufsize = 5000;
    	char buffer[bufsize];
	char buf_ser_aws[bufsize];
	string func;
	string word;
	string similar_str;
	string match_str;

    	struct sockaddr_in serverA;
	struct sockaddr_in serverA_aws;
    	socklen_t size;

   	serverA_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
   	if (serverA_fd < 0) 
	{
       		cout << "\nError creating socket" << endl;
        	exit(1);
	}

    	serverA.sin_family = AF_INET;
    	serverA.sin_addr.s_addr = htons(INADDR_ANY);
    	serverA.sin_port = htons(portNumA);

	serverA_aws.sin_family = AF_INET;
    	//serverA_aws.sin_addr.s_addr = htons(INADDR_ANY);
    	//serverA_aws.sin_port = htons(portNumA);
	socklen_t len = sizeof(serverA_aws);
	

    	if ((bind(serverA_fd, (struct sockaddr*)&serverA,sizeof(serverA))) < 0) 
    	{
        	cout << "\nError binding connection" << endl;
        	return -1;
    	}

	cout << "\nThe ServerA is up and running using UDP on port <" << portNumA << ">" << endl;

	while(1)
	{

		recvfrom ( serverA_fd, buffer, bufsize, 0,(struct sockaddr*)&serverA_aws,&len);

		string str(buffer);
		func = str.substr(0, str.find(" "));
		int l1 = func.length();
		//cout << "\nlength: " << l1 << endl;
		int match = 0;
		stringstream ss_m;
		stringstream ss_s;

			
		word = str.substr(l1+1);

		cout << "\nThe ServerA recieved input <" << word << "> and operation <" << func << ">" << endl;

		transform(func.begin(), func.end(), func.begin(), ::tolower);

		if(func.compare("prefix") == 0)
		{

			//cout << "\nprefix" << endl;

			match =  prefix(word);
			ss_m << match;
			match_str = ss_m.str();
			strcpy( buf_ser_aws, match_str.c_str());
			sendto(serverA_fd, buf_ser_aws, bufsize, 0,(struct sockaddr*)&serverA_aws,sizeof(serverA_aws));
			cout << "\nThe ServerA has found <" << buf_ser_aws << "> matches" << endl;
			
			strcpy( buf_ser_aws, prefix_str.c_str());
			sendto(serverA_fd, buf_ser_aws, bufsize, 0,(struct sockaddr*)&serverA_aws,sizeof(serverA_aws));
			//cout << "\nThe ServerA sent <" << buf_ser_aws << "> to AWS" << endl;
			
			
		}

		else if(func.compare("suffix") == 0)
		{
			//cout << "\nsuffix" << endl;

			match =  suffix(word);
			ss_m << match;
			match_str = ss_m.str();
			strcpy( buf_ser_aws, match_str.c_str());
			sendto(serverA_fd, buf_ser_aws, bufsize, 0,(struct sockaddr*)&serverA_aws,sizeof(serverA_aws));
			cout << "\nThe ServerA has found <" << buf_ser_aws << "> matches" << endl;
			
			strcpy( buf_ser_aws, suffix_str.c_str());
			sendto(serverA_fd, buf_ser_aws, bufsize, 0,(struct sockaddr*)&serverA_aws,sizeof(serverA_aws));
			//cout << "\nThe ServerA sent <" << buf_ser_aws << "> to AWS" << endl;
		}

		else if(func.compare("search") == 0)
		{
			//cout << "\nsearch" << endl;
			
			match = search(word);
			ss_m << match;
			match_str = ss_m.str();
			ss_s << similar;
			similar_str = ss_s.str();
			//cout << "\n match : " << match_str << endl;
			//cout << "\n similar : " << similar_str << endl;

			strcpy( buf_ser_aws, similar_str.c_str());
			sendto(serverA_fd, buf_ser_aws, bufsize, 0,(struct sockaddr*)&serverA_aws,sizeof(serverA_aws));
			if (match > 0 )
			cout << "\nThe ServerA has found <1> match and <" << buf_ser_aws << "> similar words" << endl;
			else
			cout << "\nThe ServerA has found <0> match and <" << buf_ser_aws << "> similar words" << endl;

			strcpy( buf_ser_aws, search_str.c_str());
			sendto(serverA_fd, buf_ser_aws, bufsize, 0,(struct sockaddr*)&serverA_aws,sizeof(serverA_aws));
			//cout << "\nThe ServerA sent <" << buf_ser_aws << "> to AWS" << endl;

			match = 0; similar = 0;
			match_str = '\0'; similar_str = '\0'; 
		}

		else
		{
			cout << "\nInvalid input " << endl;
			exit(0);
		}

		cout << "\nThe ServerA finished sending the output to AWS" << endl;
	}
	
	return 0;
}

int search (string s1) 
{
	string line; string lineout; string line_HD;
	ifstream myfile ("backendA.txt");
	//string s1;
	int len1, len2;
	string s2;
	int HD;
	int match_count = 0;
	//int HD_count = 0;

	len1 = s1.length();

	transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
	//cout << "\ntest: " << s1 << endl; 

  	if (myfile.is_open())
  	{
    		while ( getline (myfile,line) )
    		{
			HD = 0;
     	
			s2 = line.substr(0, line.find(" ::"));
			len2 = s2.length();
			
			transform(s2.begin(), s2.end(), s2.begin(), ::toupper);
			
			if (len1 == len2 )
			HD = hamming_dist(s1,s2);

			if (HD == 1)
			{
				similar += 1;
				line_HD = line;
			}
			
			if ( s1.compare(s2) == 0)
			{
				match_count += 1;
      				lineout = line;
				//break;
			}
			
    		}

		search_str = lineout + "*" + line_HD;
    		myfile.close();
  	}

  	else cout << "\nError : Unable to open file" << endl; 

  	return match_count;
}

int hamming_dist( string str1, string str2)
{
    int i = 0, count = 0;
    while (str1[i] != '\0')
    {
        if (str1[i] != str2[i])
            count++;
        i++;
    }
    return count;
}

int prefix(string s1) 
{
	string line;
	ifstream myfile ("backendA.txt");
	//string s1;
	string s2;
	int count = 0;
	prefix_str.clear();
	//string prefix_str;

	transform(s1.begin(), s1.end(), s1.begin(), ::tolower);

  	if (myfile.is_open())
  	{
    		while ( getline (myfile,line) )
    		{
			
			s2 = line.substr(0, line.find(" ::")); 

			transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
			    			
			int found = s2.find(s1,0);

			if ( found == 0)
			{	
				count = count + 1;
				prefix_str = prefix_str + s2 + "#";
      				
			}
    		}	
    		myfile.close();
		//cout << prefix_str << '\n';
  	}

  	else 
		cout << "\nUnable to open file"; 

  	return count;
}

int suffix(string s1)
{
	string line;
	ifstream myfile ("backendA.txt");
	//string s1;
	string s2;
	string s3;
	int count = 0;
	suffix_str.clear();

	int l1 = s1.length();
	transform(s1.begin(), s1.end(), s1.begin(), ::tolower);

  	if (myfile.is_open())
  	{
    		while ( getline (myfile,line) )
    		{
     	
			s2 = line.substr(0, line.find(" ::"));
			//cout << s2 << endl;
			int l2 = s2.length();
			transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
			//cout << l2 << endl;
			int l3 = l2 - l1;
			//cout << s2 << endl;

			if ( l2 > l1)
			{
	
				s3 = s2.substr((l3) ,(l2 - 1));
		
				if ( s3.compare(s1) == 0)
				{
					count = count + 1;
      					suffix_str = suffix_str + s2 + "#";
				//break;
				}
			}
			else continue;
    		}	
    		myfile.close();
		//cout << suffix_str << '\n';
  	}

  	else cout << "Unable to open file"; 

  	return count;
}

