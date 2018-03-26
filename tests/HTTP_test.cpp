#include "protocol/HTTP.h"
#include <stdio.h>

#define dotest(condition) \
	if(!(condition)) { \
		fprintf(stderr, "FAIL(%s:%d): %s\n",__FILE__, __LINE__, #condition); \
	}

void testHeaders() {
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

	fprintf(stderr, "expected\n--------\n%s\n---------\nproduced\n---------\n%s\n--------\n", "Accept: None\r\n\r\n", std::string(http::Headers("Accept: None\r\n\r\n")).c_str());
	dotest(std::string(http::Headers("Accept: None\r\n\r\n")) == "Accept: None\r\n\r\n");
}

void testQuery() {
	http::Query test1;
	http::Query test2;
	http::Query test4("key4", http::Query::IsQuery);
	http::Query	test5("http://server.com/action.asp?test=1&test=2&time=now&help#NoStoppingUs", http::Query::SearchForQuery);

	dotest(test5.getOne("test") == "1");
	dotest(test5.getOne("time") == "now");
	dotest(test5.has("help"));
	dotest(!test5.hasValue("help"));
	test1["key1"] = "value1";
	test2["key2"] = "value2";
	dotest(std::string(test1) == "?key1=value1");
	dotest(std::string(test2) == "?key2=value2");
	test2 = test1;
	dotest(std::string(test2) == "?key1=value1");

	http::Query test3(test1);

	dotest(std::string(test3) == "?key1=value1");
	try {
		dotest(test1.getOne("key") != "value");
		dotest(false/* We should have thrown*/);
	} catch(const std::out_of_range &exception) {
		dotest(std::string(exception.what()) == "key");
	}
	dotest(test1.has("key1"));
	dotest(test2.has("key1"));
	dotest(test3.has("key1"));
	dotest(test1.hasValue("key1"));
	dotest(test2.hasValue("key1"));
	dotest(test3.hasValue("key1"));
	dotest(test4.has("key4"));
	dotest(!test4.hasValue("key4"));
	dotest(test1.getOne("key1") == "value1");
	dotest(test2.getOne("key1") == "value1");
	dotest(test3.getOne("key1") == "value1");
	dotest(test4.getOne("key4") == "");
	dotest(http::Query::unescape(http::Query::escape("Hello World!")) == "Hello World!");
	dotest(http::Query::escape("Hello World!") != "Hello World!");
	dotest(http::Query::unescape(http::Query::escape("What %?")) == "What %?");
	dotest(http::Query::unescape("Test%") == "Test%");
	dotest(http::Query::unescape("Test%0") == "Test%0");
	dotest(http::Query::unescape("Test%F") == "Test%F");
	dotest(http::Query::unescape("Test%20") == "Test ");
	dotest(http::Query::unescape("Test%3d") == "Test=");
	dotest(http::Query::unescape("Test%3D") == "Test=");
	dotest(http::Query::unescape("Test%3Z") == "Test%3Z");
	dotest(http::Query::escape(http::Query::unescape("%FF")) == "%FF")
	dotest(http::Query::escape(http::Query::unescape("%00")) == "%00")
	dotest(std::string(http::Query("key=value", http::Query::IsQuery)) == "?key=value");
	dotest(std::string(http::Query("key=value", http::Query::SearchForQuery)) == "?key=value");
	dotest(std::string(http::Query("test?key=value", http::Query::IsQuery)) == "?test%3Fkey=value");
	dotest(std::string(http::Query("test?key=value", http::Query::SearchForQuery)) == "?key=value");
	dotest(std::string(http::Query("key", http::Query::IsQuery)) == "?key");
	dotest(std::string(http::Query("key", http::Query::SearchForQuery)) == "?key");
	dotest(std::string(http::Query("key=value#hammerTime", http::Query::IsQuery)) == "?key=value");
	dotest(std::string(http::Query("key=value#hammerTime", http::Query::SearchForQuery)) == "?key=value");
	dotest(std::string(http::Query("test?key=value#hammerTime", http::Query::IsQuery)) == "?test%3Fkey=value");
	dotest(std::string(http::Query("test?key=value#hammerTime", http::Query::SearchForQuery)) == "?key=value");
	dotest(std::string(http::Query("key#hammerTime", http::Query::IsQuery)) == "?key");
	dotest(std::string(http::Query("key#hammerTime", http::Query::SearchForQuery)) == "?key");
}

void testRequestLine() {
	dotest(http::RequestLine("GET /docs/index.html HTTP/1.1").method() == "GET");
	dotest(http::RequestLine("GET /docs/index.html HTTP/1.1").path() == "/docs/index.html");
	dotest(http::RequestLine("GET /docs/index.html HTTPS/1.1").protocol() == "HTTPS");
	dotest(http::RequestLine("GET /docs/index.html HTTP/1.1").version() == "1.1");
	dotest(http::RequestLine("GET /docs/index.html?key=value HTTP/1.1").query().getOne("key") == "value");
	dotest(http::RequestLine("GET /docs/index.html HTTPS").protocol() == "HTTPS");

	http::RequestLine	request;

	request.method() = "PUT";
	request.path() = "/index.html";
	request.protocol() = "HTTPS";
	request.version() = "1.2";
	request.query()["key"] = "value";
	dotest(std::string(request) == "PUT /index.html?key=value HTTPS/1.2");

	try {
		request.query().getOne("zathura");
		fprintf(stderr, "FAILED: We should have thrown\n");
	} catch(const std::out_of_range &) {
	} catch(const std::exception &exception) {
		fprintf(stderr, "FAILED: Exception: %s\n", exception.what());
	}
}

void testRequest() {
	const char * const fullRequestRaw= "PUT /index.html?key=value HTTPS/1.2\r\n"
										"Agent: Protocol\r\n"
										"\r\n";
	http::Request	fullRequest(fullRequestRaw);
	http::Request	requestCopy(fullRequest);

	printf("-=- raw -=-\n%s\n-=- processed -=-\n%s\n", fullRequestRaw, std::string(fullRequest).c_str());
	dotest(std::string(fullRequest) == fullRequestRaw);
	fullRequest.info().method()= "GET";
	fullRequest.fields()["Agent"]= "Testing";
	dotest(std::string(fullRequest) != fullRequestRaw);
	fullRequest= requestCopy;
	dotest(std::string(fullRequest) == fullRequestRaw);
}

void testResponseLine() {
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
}

void testResponse() {
	const char * const fullResponseRaw= "HTTPS/1.9 200 OK\r\n"
										"Agent: Protocol\r\n"
										"\r\n";
	http::Response	fullResponse(fullResponseRaw);
	http::Response	responseCopy(fullResponse);

	printf("-=- raw response -=-\n%s\n-=- processed response -=-\n%s\n", fullResponseRaw, std::string(fullResponse).c_str());
	printf("-=- processed line -=-\n%s\n", std::string(fullResponse.info()).c_str());
	printf("-=- processed line message -=-\n%s\n", std::string(fullResponse.info().message()).c_str());
	dotest(std::string(fullResponse) == fullResponseRaw);
	fullResponse.info().protocol()= "FTP";
	fullResponse.fields()["Agent"]= "Testing";
	dotest(std::string(fullResponse) != fullResponseRaw);
	fullResponse= responseCopy;
	dotest(std::string(fullResponse) == fullResponseRaw);
}

int main(int /*argc*/, char * /*argv*/[]) {
	int	iterations= 3500;
#ifdef __Tracer_h__
	iterations= 1;
#endif

	for(int i= 0; i < iterations; ++i) {
		try {
			testHeaders();
			testQuery();
			testRequestLine();
			testRequest();
			testResponseLine();
			testResponse();
		} catch(const std::exception &exception) {
			fprintf(stderr, "FAILED: Exception: %s\n", exception.what());
		}
	}
	return 0;
}