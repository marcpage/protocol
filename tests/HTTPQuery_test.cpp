#include "protocol/HTTPQuery.h"
#include <stdio.h>

#define dotest(condition) \
	if(!(condition)) { \
		fprintf(stderr, "FAIL(%s:%d): %s\n",__FILE__, __LINE__, #condition); \
	}

int main(int /*argc*/, char * /*argv*/[]) {
	int	iterations= 2;
#ifdef __Tracer_h__
	iterations= 1;
#endif

	for(int i= 0; i < iterations; ++i) {
		dotest(http::Query::unescape(http::Query::escape("Hello World!")) == "Hello World!");
		dotest(http::Query::escape("Hello World!") != "Hello World!");
		dotest(http::Query::unescape(http::Query::escape("What %?")) == "What %?");
		dotest(http::Query::unescape("Test%") == "Test%");
		dotest(http::Query::unescape("Test%0") == "Test%0");
		dotest(http::Query::unescape("Test%F") == "Test%F");
		dotest(http::Query::unescape("Test%20") == "Test ");
	}
	return 0;
}
