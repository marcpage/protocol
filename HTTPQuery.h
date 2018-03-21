#ifndef __HTTPQuery_h__
#define __HTTPQuery_h__

/** @file HTTPQuery.h
	@todo document
*/

#include <string>
#include <vector>
#include <map>
#include <ctype.h>

namespace http {

	class Query {
		public:
			enum SearchMode {
				SearchForQuery,
				IsQuery
			};
			typedef std::string String;
			typedef std::vector<String> StringList;
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
			_KeyValues _keyValues;
	};

	inline Query::String &Query::unescape(const String &value, String &result) {
		const String hex = "0123456789ABCDEF";

		result.clear();
		result.reserve(value.length());
		for (String::size_type i = 0; i < value.length(); ++i) {
			if ( ('%' == value[i]) && (i + 2 < value.length()) ) {
				String::size_type upperNibble = hex.find(::toupper(value[i + 1]));
				String::size_type lowerNibble = hex.find(::toupper(value[i + 2]));

				if ( (upperNibble != String::npos) && (lowerNibble != String::npos) ) {
					result.append(1, (upperNibble << 4) + lowerNibble);
					i+= 2;
				} else {
					result.append(1, value[i]);
				}
			} else if ('+' == value[i]) {
				result.append(1, ' ');
			} else {
				result.append(1, value[i]);
			}
		}
		return result;
	}
	inline Query::String Query::unescape(const String &value) {
		String result;

		return unescape(value, result);
	}
	inline Query::String &Query::escape(const String &value, String &result) {
		const String hex = "0123456789ABCDEF";

		result.clear();
		result.reserve(value.length());
		for (String::size_type i = 0; i < value.length(); ++i) {
			if (' ' == value[i]) {
				result.append(1, '+');
			} else if (isalnum(value[i])) {
				result.append(1, value[i]);
			} else {
				result.append(1, '%');
				result.append(1, hex[reinterpret_cast<const unsigned char&>(value[i]) >> 4]);
				result.append(1, hex[value[i] & 0x0F]);
			}
		}
		return result;
	}
	inline Query::String Query::escape(const String &value) {
		String result;

		return escape(value, result);
	}
	inline Query::Query(const String &query, SearchMode mode):_keyValues() {
	}
	inline Query::Query(const Query &other):_keyValues() {
	}
	inline Query &Query::operator=(const Query &other) {
	}
	inline const Query::String &Query::operator[](const String &name) const {
	}
	inline Query::String &Query::operator[](const String &name) {
	}
	inline bool Query::has(const String &name) const {
	}
	inline Query::operator String() const {
	}
	inline const Query::String Query::getOne(const String &name) const {
	}
	inline void Query::remove(const String &name) {
	}
	inline void Query::add(const String &name, const String &value) {
	}
	inline bool Query::empty() const {
	}

}

#endif // __HTTPQuery_h__
