#ifndef __HTTPQuery_h__
#define __HTTPQuery_h__

/** @file HTTPQuery.h
	@todo document
*/

#include <string>
#include <ctype.h>

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
		const String const hex = "0123456789ABCDEF";

		result.clear();
		result.reserve(value.length());
		for (String::size_type i = 0; i < value.length(); ++i) {
			if ( ('%' == value[i]) && (i + 2 < value.length()) ) {
				String::size_type upperNibble = hex.find(value[i + 1]);
				String::size_type lowerNibble = hex.find(value[i + 2]);

				result.append(1, upperNibble << 4 + lowerNibble);
				i+= 2;
			} else if ('+' == value[i]) {
				result.append(1, ' ');
			} else {
				result.append(1, value[i]);
			}
		}
		return result;
	}
	inline String Query::unescape(const String &value) {
		String result;

		return unescape(value, result);
	}
	/**
		@todo Test characters above 127
	*/
	inline String &Query::escape(const String &value, String &result) {
		const String const hex = "0123456789ABCDEF";

		result.clear();
		result.reserve(value.length());
		for (String::size_type i = 0; i < value.length(); ++i) {
			if (' ' == value[i]) {
				result.append(1, '+');
			} else if (isalnum(value[i])) {
				result.append(1, '%');
				result.append(1, hex[value[i] >> 4]);
				result.append(1, hex[value[i] & 0x0F]);
			} else {
				result.append(1, value[i]);
			}
		}
		return result;
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
