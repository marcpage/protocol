#include "protocol/HTTPResponse.h"
#include <stdio.h>

#define dotest(condition) \
	if(!(condition)) { \
		fprintf(stderr, "FAIL(%s:%d): %s\n",__FILE__, __LINE__, #condition); \
	}

int main(int /*argc*/, char * /*argv*/[]) {
	int	iterations= 90000;
#ifdef __Tracer_h__
	iterations= 1;
#endif

	for(int i= 0; i < iterations; ++i) {
		try {
			fprintf(stderr, "protocol='%s'\n", http::ResponseLine("FTP/5.1 200 OK\r\n").protocol().c_str());
			fprintf(stderr, "version='%s'\n", http::ResponseLine("FTP/5.1 200 OK\r\n").version().c_str());
			dotest(http::ResponseLine("FTP/5.1 200 OK\r\n").protocol() == "FTP");
			dotest(http::ResponseLine("FTP/5.1 200 OK\r\n").version() == "5.1");
			dotest(http::ResponseLine("FTP/5.1 200 OK\r\n").code() == "200");
			dotest(http::ResponseLine("FTP/5.1 200 OK\r\n").message() == "OK");

			http::ResponseLine response;

			response.protocol()= "HTTPS";
			response.version()= "3.1";
			response.code()= "301";
			response.message()= "Moved Permanently";
			dotest(std::string(response) == "HTTPS/3.1 301 Moved Permanently");

		} catch(const std::exception &exception) {
			fprintf(stderr, "FAILED: Exception: %s\n", exception.what());
		}
	}
	return 0;
}
