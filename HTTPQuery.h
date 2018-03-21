#ifndef __HTTPQuery_h__
#define __HTTPQuery_h__

/** @file HTTPQuery.h
	@todo document
*/

#include <string>

namespace http {

	class Query {
		public:
			enum SearchMode {
				SearchForQuery,
				IsQuery
			};
			typedef std::string String;
			typedef std::vecotr<String> StringList;
			static String &unescape(const String &value, String &result);
			static String unescape(const String &value);
			static String &escape(const String &value, String &result);
			static String escape(const String &value);
			Query(const String &query, SearchMode mode);
			Query(const Query &other);
			~Query() {}
			Query &operator=(const Query &other);
			const String &operator[](const String &name) const;
			String &operator[](const String &name);
			operator String() const;
			bool has(const String &name) const;
			const StringList get(const String &name) const;
			const String getOne(const String &name) const;
			void remove(const String &name);
			void add(const String &name, const String &value);
			bool empty() const;
		private:
			typedef std::map<String, StringList> _KeyValues;
	};

	inline String &Query::unescape(const String &value, String &result) {
		result.clear();

	}
	inline String Query::unescape(const String &value) {
		String result;

		return unescape(value, result);
	}
	inline String &Query::escape(const String &value, String &result) {
	}
	inline String Query::escape(const String &value) {
		String result;

		return escape(value, result);
	}
	inline Query::Query(const String &query, SearchMode mode):_KeyValues() {
	}
	inline Query::Query(const Query &other):_keyValues() {
	}
	inline Query &Query::operator=(const Query &other) {
	}
	inline const String &Query::operator[](const String &name) const {
	}
	inline String &Query::operator[](const String &name) {
	}
	inline bool Query::has(const String &name) const {
	}
	inline Query::operator String() const {
	}
	inline String &Query::getOne(const String &name, String &value) const {
	}
	inline String Query::getOne(const String &name) const {
	}
	inline void Query::remove(const String &name) {
	}
	inline void Query::add(const String &name, const String &value) {
	}
	inline bool Query::empty() const {
	}

}

#endif // __HTTPQuery_h__
