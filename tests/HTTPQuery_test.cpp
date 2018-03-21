#include "protocol/HTTPQuery.h"
#include <stdio.h>

#define dotest(condition) \
	if(!(condition)) { \
		fprintf(stderr, "FAIL(%s:%d): %s\n",__FILE__, __LINE__, #condition); \
	}

int main(int /*argc*/, char * /*argv*/[]) {
	int	iterations= 20000;
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
		} catch(const std::exception &exception) {
			fprintf(stderr, "FAILED: Exception: %s\n", exception.what());
		}
	}
	return 0;
}
