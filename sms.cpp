


#include<iostream>
#include<sstream>
#include<wininet.h>
#include<windows.h>

using namespace std;

string encode(string url)
{
	const char *hex = "0123456789abcdef";
	stringstream s;

	for(unsigned int i = 0; i < url.length(); i++)
	{
		byte c = (char)url.c_str()[i];
		if( ('a' <= c && c <= 'z')
				|| ('A' <= c && c <= 'Z')
				|| ('0' <= c && c <= '9') ){
			s << c;
		} else {
			if(c == ' ') s << "%20";
			else
				s << '%' << (hex[c >> 4]) << (hex[c & 15]);
		}
	}

	return s.str();
}




int main(int argc, char** argv)
{

	string host       = "localhost";     
	int port          = 9501;

	string username   = "admin";         
	string password   = "abc123";

	string message    = "This is a test SMS."; 

	string originator = "+919966320464";    
	string recipient  = "+919620354720";    

	stringstream url;					 
	url << "/api?action=sendmessage&username=" << encode(username);
	url << "&password=" << encode(password);
	url << "&recipient=" << encode(recipient);
	url << "&messagetype=SMS:TEXT&messagedata=" << encode(message);
	url << "&originator=" << encode(originator);
	url << "&responseformat=xml";

	HINTERNET inet = InternetOpen("Ozeki", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	HINTERNET conn = InternetConnect(inet, host.c_str() , port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

	HINTERNET sess=HttpOpenRequest(conn,"GET",url.str().c_str(),"HTTP/1.1",NULL,NULL, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD,0);

	int error = GetLastError();
	if(error == NO_ERROR)
	{
		HttpSendRequest(sess, NULL, 0, NULL,0);

		int size = 1024;
		char *buffer = new char[size + 1];
		DWORD read;
		int rsize = InternetReadFile(sess, (void *)buffer, size, &read);
		string s = buffer;
		s = s.substr(0, read);

		int pos = s.find("<statuscode>0</statuscode>");

		if(pos > 0) cout << "Message sent." << endl;
		else cout << "Error." << endl;
	}

//	system("pause");


}

