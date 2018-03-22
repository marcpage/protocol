#include "protocol/HTTPHeaders.h"
#include <stdio.h>

#define dotest(condition) \
	if(!(condition)) { \
		fprintf(stderr, "FAIL(%s:%d): %s\n",__FILE__, __LINE__, #condition); \
	}

int main(int /*argc*/, char * /*argv*/[]) {
	int	iterations= 10000;
#ifdef __Tracer_h__
	iterations= 1;
#endif

	for(int i= 0; i < iterations; ++i) {
		try {
			const char * const header1 =
	"Host: net.tutsplus.com\r\n"
	"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) \r\n"
		"\tGecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r\n"
	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
	"Accept-Language: en-us,en;q=0.5\r\n"
	"Accept-Encoding: gzip,deflate\r\n"
	"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
	"Keep-Alive: 300\r\n"
	"Connection: keep-alive\r\n"
	"Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r\n"
	"Pragma: no-cache\r\n"
	"Cache-Control: no-cache\r\n"
	"\r\n"
	"Body: text\r\n";
			const char * const header2 =
	"Host: net.tutsplus.com\n"
	"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) \n"
		"\tGecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\n"
	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
	"Accept-Language: en-us,en;q=0.5\n"
	"Accept-Encoding: gzip,deflate\n"
	"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\n"
	"Keep-Alive: 300\n"
	"Connection: keep-alive\n"
	"Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\n"
	"Pragma: no-cache\n"
	"Cache-Control: no-cache\n"
	"\n"
	"Body: text\n";
			const char * const header3 =
	"Host: net.tutsplus.com\r"
	"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) \r"
		"\tGecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r"
	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r"
	"Accept-Language: en-us,en;q=0.5\r"
	"Accept-Encoding: gzip,deflate\r"
	"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r"
	"Keep-Alive: 300\r"
	"Connection: keep-alive\r"
	"Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r"
	"Pragma: no-cache\r"
	"Cache-Control: no-cache\r"
	"\r"
	"Body: text\r";
			http::Headers	h1(header1);
			http::Headers	h2(header2);
			http::Headers	h3(header3);
			http::Headers	h4(h1);
			http::Headers	h5;

			dotest(!h1.empty());
			dotest(h1.has("Host"));
			dotest(h1.has("User-Agent"));
			dotest(h1.has("Accept"));
			dotest(h1.has("Accept-Language"));
			dotest(h1.has("Accept-Encoding"));
			dotest(h1.has("Accept-Charset"));
			dotest(h1.has("Keep-Alive"));
			dotest(h1.has("Connection"));
			dotest(h1.has("Cookie"));
			dotest(h1.has("Pragma"));
			dotest(h1.has("Cache-Control"));
			dotest(!h1.has("Body"));
			dotest(h1["User-Agent"].find("Gecko") > 0);
			printf("Header 1\n----------\n%s\n----------\n", std::string(h1).c_str());

			dotest(!h2.empty());
			dotest(h2.has("Host"));
			dotest(h2.has("User-Agent"));
			dotest(h2.has("Accept"));
			dotest(h2.has("Accept-Language"));
			dotest(h2.has("Accept-Encoding"));
			dotest(h2.has("Accept-Charset"));
			dotest(h2.has("Keep-Alive"));
			dotest(h2.has("Connection"));
			dotest(h2.has("Cookie"));
			dotest(h2.has("Pragma"));
			dotest(h2.has("Cache-Control"));
			dotest(!h2.has("Body"));
			dotest(h2["User-Agent"].find("Gecko") > 0);
			printf("Header 2\n----------\n%s\n----------\n", std::string(h2).c_str());

			dotest(!h3.empty());
			dotest(h3.has("Host"));
			dotest(h3.has("User-Agent"));
			dotest(h3.has("Accept"));
			dotest(h3.has("Accept-Language"));
			dotest(h3.has("Accept-Encoding"));
			dotest(h3.has("Accept-Charset"));
			dotest(h3.has("Keep-Alive"));
			dotest(h3.has("Connection"));
			dotest(h3.has("Cookie"));
			dotest(h3.has("Pragma"));
			dotest(h3.has("Cache-Control"));
			dotest(!h3.has("Body"));
			dotest(h3["User-Agent"].find("Gecko") > 0);
			printf("Header 3\n----------\n%s\n----------\n", std::string(h3).c_str());

			dotest(!h4.empty());
			dotest(h4.has("Host"));
			dotest(h4.has("User-Agent"));
			dotest(h4.has("Accept"));
			dotest(h4.has("Accept-Language"));
			dotest(h4.has("Accept-Encoding"));
			dotest(h4.has("Accept-Charset"));
			dotest(h4.has("Keep-Alive"));
			dotest(h4.has("Connection"));
			dotest(h4.has("Cookie"));
			dotest(h4.has("Pragma"));
			dotest(h4.has("Cache-Control"));
			dotest(!h4.has("Body"));
			dotest(h4["User-Agent"].find("Gecko") > 0);
			printf("Header 4\n----------\n%s\n----------\n", std::string(h4).c_str());

			dotest(h5.empty());
			h4.remove("Host");
			h4.remove("User-Agent");
			h4.remove("Accept");
			h4.remove("Accept-Language");
			h4.remove("Accept-Encoding");
			h4.remove("Accept-Charset");
			h4.remove("Keep-Alive");
			h4.remove("Connection");
			h4.remove("Cookie");
			h4.remove("Pragma");
			h4.remove("Cache-Control");
			dotest(h4.empty());

			h3.remove("Cookie");
			dotest(!h3.has("Cookie"));
			h3 = h2;
			dotest(h3.has("Cookie"));

		} catch(const std::exception &exception) {
			fprintf(stderr, "FAILED: Exception: %s\n", exception.what());
		}
	}
	return 0;
}
