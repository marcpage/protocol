#include "protocol/HTTPRequest.h"
#include <stdio.h>

#define dotest(condition) \
	if(!(condition)) { \
		fprintf(stderr, "FAIL(%s:%d): %s\n",__FILE__, __LINE__, #condition); \
	}

int main(int /*argc*/, char * /*argv*/[]) {
	int	iterations= 15000;
#ifdef __Tracer_h__
	iterations= 1;
#endif

	for(int i= 0; i < iterations; ++i) {
		try {
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

			const char * const fullRequestRaw= "PUT /index.html?key=value HTTPS/1.2\r\n"
												"Agent: Protocol\r\n"
												"\r\n";
			http::Request	fullRequest(fullRequestRaw);
			http::Request	requestCopy(fullRequest);

			printf("-=- raw -=-\n%s\n-=- processed -=-\n%s\n", fullRequestRaw, std::string(fullRequest).c_str());
			dotest(std::string(fullRequest) == fullRequestRaw);
			fullRequest.request().method()= "GET";
			fullRequest.fields()["Agent"]= "Testing";
			dotest(std::string(fullRequest) != fullRequestRaw);
			fullRequest= requestCopy;
			dotest(std::string(fullRequest) == fullRequestRaw);
		} catch(const std::exception &exception) {
			fprintf(stderr, "FAILED: Exception: %s\n", exception.what());
		}
	}
	return 0;
}
