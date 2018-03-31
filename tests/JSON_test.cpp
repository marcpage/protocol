#include "protocol/JSON.h"
#include <stdio.h>
#include <math.h>

#define dotest(condition) \
	if(!(condition)) { \
		fprintf(stderr, "FAIL(%s:%d): %s\n",__FILE__, __LINE__, #condition); \
	}

int main(int /*argc*/, char * /*argv*/[]) {
	int	iterations= 1000;
#ifdef __Tracer_h__
	iterations= 1;
#endif
	for(int i= 0; i < iterations; ++i) {
		try {
			const char *j1= "{\"test \\\"me\\\"\":[1,2,{\"go\\/now\":3},{\"eol\":\"\\r\\n\"},{\"ht\":\"\\t\"},{\"vt\":\"\\f\"},true,false,null,\"C:\\\\\"]}";

			printf("-=- original -=-\n'%s'\n-=- parsed -=-\n'%s'\n", j1, std::string(json::Value().parse(j1)).c_str());
			dotest(std::string(json::Value().parse(j1)) == j1);
			dotest(json::Value().parse(j1).is(json::ObjectType));
			dotest(json::Value().parse(j1)["test \"me\""][9].count() == 3);
			dotest(json::Value().parse(j1)["test \"me\""].is(json::ArrayType));
			dotest(json::Value().parse(j1)["test \"me\""][0].is(json::IntegerType));
			dotest(json::Value().parse(j1)["test \"me\""][1].is(json::IntegerType));
			dotest(json::Value().parse(j1)["test \"me\""][2].is(json::ObjectType));
			dotest(json::Value().parse(j1)["test \"me\""][2]["go/now"].is(json::IntegerType));
			dotest(json::Value().parse(j1)["test \"me\""][3].is(json::ObjectType));
			dotest(json::Value().parse(j1)["test \"me\""][3]["eol"].is(json::StringType));
			dotest(json::Value().parse(j1)["test \"me\""][4].is(json::ObjectType));
			dotest(json::Value().parse(j1)["test \"me\""][4]["ht"].is(json::StringType));
			dotest(json::Value().parse(j1)["test \"me\""][5].is(json::ObjectType));
			dotest(json::Value().parse(j1)["test \"me\""][5]["vt"].is(json::StringType));
			dotest(json::Value().parse(j1)["test \"me\""][6].is(json::BooleanType));
			dotest(json::Value().parse(j1)["test \"me\""][7].is(json::BooleanType));
			dotest(json::Value().parse(j1)["test \"me\""][8].is(json::NullType));
			dotest(json::Value().parse(j1)["test \"me\""][9].is(json::StringType));
			try {
				json::Value().parse(j1)["test \"me\""][0].count();
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().parse(j1).boolean();
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().parse(j1).real();
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().parse(j1).integer();
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().parse(j1).string();
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().string();
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().parse(j1)["test \"me\""].string();
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().parse("\"\\u123k\"");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().parse("\"\\u{1\"");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().parse("\"\\u{}\"");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().parse("\"\\u{123k}\"");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().parse("=");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value().parse("....");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				std::string(json::Value().parse("\"\xF8\""));
				dotest(false);
			} catch(const std::invalid_argument&) {}
			printf("--------------------------------\n");
			try {
				json::Value().parse("\"\\u{110000}\"");
				dotest(false);
			} catch(const std::invalid_argument&) {}
			printf("--------------------------------\n");
			try {
				std::string(json::Value().parse("\"\xc0\x80\""));
				dotest(false);
			} catch(const msg::Exception&) {}
			try {
				std::string(json::Value().parse("\"\xc0\x00\""));
				dotest(false);
			} catch(const msg::Exception&) {}
			try {
				std::string(json::Value().parse("\"\xe0\x80\x80\""));
				dotest(false);
			} catch(const msg::Exception&) {}
			try {
				std::string(json::Value().parse("\"\xe0\x00\x80\""));
				dotest(false);
			} catch(const msg::Exception&) {}
			try {
				std::string(json::Value().parse("\"\xe0\x80\x00\""));
				dotest(false);
			} catch(const msg::Exception&) {}
			try {
				std::string(json::Value().parse("\"\xf0\x00\x80\x80\""));
				dotest(false);
			} catch(const msg::Exception&) {}
			try {
				std::string(json::Value().parse("\"\xf0\x80\x00\x80\""));
				dotest(false);
			} catch(const msg::Exception&) {}
			try {
				std::string(json::Value().parse("\"\xf0\x80\x80\x00\""));
				dotest(false);
			} catch(const msg::Exception&) {}
			dotest(json::Value().parse("\"\\u0041\"").string() == "A");
			dotest(json::Value().parse("\"\\u00Ae\"").string() == "\xc2\xae");
			dotest(json::Value().parse("\"\\u2211\"").string() == "\xe2\x88\x91");
			dotest(json::Value().parse("\"\\u{1f605}\"").string() == "\xf0\x9f\x98\x85");
			printf("Here we go\n");
			dotest(json::Value().parse("\"\xc2\xae\"").string() == "\xc2\xae");
			dotest(json::Value().parse("\"\xe2\x88\x91\"").string() == "\xe2\x88\x91");
			dotest(json::Value().parse("\"\xf0\x9f\x98\x85\"").string() == "\xf0\x9f\x98\x85");
			printf("%s\n", std::string(json::Value().parse("\"\xc2\xae\"")).c_str());
			printf("%s\n", std::string(json::Value().parse("\"\xe2\x88\x91\"")).c_str());
			dotest(std::string(json::Value().parse("\"\xc2\xae\"")) == "\"\\u00ae\"");
			dotest(std::string(json::Value().parse("\"\xe2\x88\x91\"")) == "\"\\u2211\"");
			dotest(std::string(json::Value().parse("\"\xf0\x9f\x98\x85\"")) == "\"\xf0\x9f\x98\x85\"");

			const char 	*j2= " \t{\t \"real\"\t: 3.14159265, \"true\": true, \"false\": false, \"array\" : [ 1, 2, 3, 4, 5 ]} ";
			json::Value	json2(j2);

			printf("-=- original -=-\n'%s'\n-=- parsed -=-\n'%s'\n", j2, std::string(json2).c_str());
			dotest(::fabs(json2["real"].real() - 3.14159265) < 0.0000001);
			dotest(json2["true"].boolean());
			dotest(!json2["false"].boolean());
			dotest(json2["array"].count() == 5);
			dotest(json2["array"][0].integer() == 1);
			dotest(json2["array"][1].integer() == 2);
			dotest(json2["array"][2].integer() == 3);
			dotest(json2["array"][3].integer() == 4);
			dotest(json2["array"][4].integer() == 5);
			json2["array"].erase(4, 5);
			dotest(json2["array"].count() == 4);
			dotest(json2["array"][0].integer() == 1);
			dotest(json2["array"][1].integer() == 2);
			dotest(json2["array"][2].integer() == 3);
			dotest(json2["array"][3].integer() == 4);
			json2["array"].erase(0, 1);
			dotest(json2["array"].count() == 3);
			dotest(json2["array"][0].integer() == 2);
			dotest(json2["array"][1].integer() == 3);
			dotest(json2["array"][2].integer() == 4);
			json2["array"].erase(1, 2);
			dotest(json2["array"].count() == 2);
			dotest(json2["array"][0].integer() == 2);
			dotest(json2["array"][1].integer() == 4);
			json2["array"].clear();
			dotest(json2["array"].count() == 0);
			dotest(json2.count() == 4);
			json2.erase("false");
			dotest(json2.count() == 3);
			json2.clear();
			dotest(json2.count() == 0);
			json2["hello"]= std::string("yes");
			dotest(json2["hello"].is(json::StringType));
			dotest(json2["hello"].string() == "yes");
			dotest(json2["hello"].count() == 3);
			json2["hello"].erase(1,3);
			printf("should be y but is '%s'\n", json2["hello"].string().c_str());
			dotest(json2["hello"].string() == "y");
			dotest(json2["hello"].count() == 1);
			json2["hello"].clear();
			dotest(json2["hello"].is(json::StringType));
			dotest(json2["hello"].count() == 0);
			json2.makeObject();
			dotest(json2.count() == 0);
			try {
				json::Value("{\"test\":1");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("{  ");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("{\"test\"");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("{\"test\"  ");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("{\"test\" , 5}");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("\"hello");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("[");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("{");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("{ 5");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("-");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("[ 1");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("[ 1, 2, ");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("\"\\5\"");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("test");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("nope");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("fun");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("0.0.0.0");
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("0").clear();
				dotest(false);
			} catch(const json::WrongType&) {}
			try {
				json::Value("0").erase(0,1);
				dotest(false);
			} catch(const json::WrongType&) {}
			dotest(std::string(json::Value("\"\\b\"")) == "\"\\b\"");

			json::Value json3("[1,2,3]");

			dotest(json3.count() == 3);
			json3.makeArray();
			dotest(json3.is(json::ArrayType));
			dotest(json3.count() == 0);

			json3= 3;
			dotest(json3.is(json::IntegerType));
			dotest(json3.integer() == 3);
			json3= 4;
			dotest(json3.is(json::IntegerType));
			dotest(json3.integer() == 4);

			json3= 3.14159265;
			dotest(json3.is(json::RealType));
			dotest(::fabs(json3.real() - 3.14159265) < 0.00000001);
			json3= 2.7;
			dotest(json3.is(json::RealType));
			dotest(::fabs(json3.real() - 2.7) < 0.01);

			json3= true;
			dotest(json3.is(json::BooleanType));
			dotest(json3.boolean());
			json3= false;
			dotest(json3.is(json::BooleanType));
			dotest(!json3.boolean());
		} catch(const std::exception &exception) {
			fprintf(stderr, "FAILED: Exception: %s\n", exception.what());
		}
	}
	return 0;
}
