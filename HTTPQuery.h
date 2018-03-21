#ifndef __HTTPQuery_h__
#define __HTTPQuery_h__

/** @file HTTPQuery.h
	@todo document
*/

#include <string>
#include <vector>
#include <map>
#include <ctype.h>
#include <stdexcept>

namespace http {

	class Query {
		public:
			enum SearchMode {SearchForQuery,IsQuery};
			typedef std::string String;
			typedef std::vector<String> StringList;
			static String &unescape(const String &value, String &result);
			static String unescape(const String &value);
			static String &escape(const String &value, String &result);
			static String escape(const String &value);
			Query();
			Query(const String &query, SearchMode mode);
			Query(const Query &other);
			~Query() {}
			Query &operator=(const Query &other);
			const String &operator[](const String &name) const;
			String &operator[](const String &name);
			operator String() const;
			bool hasValue(const String &name) const;
			bool has(const String &name) const;
			const StringList &get(const String &name) const;
			const String &getOne(const String &name) const;
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
		String::size_type	queryPos = SearchForQuery == mode ? query.find('?') : String::npos;
		String::size_type	position = queryPos == String::npos ? 0 : queryPos + 1;
		String::size_type	hashPos = query.find('#', position);
		String::size_type	end = hashPos == String::npos ? query.length() : hashPos;

		while (position < end) {
			String::size_type	separatorPos = query.find('&', position);
			String::size_type	positionEnd = (separatorPos == String::npos || separatorPos > end) ? end : separatorPos;

			if (position < positionEnd) {
				String::size_type	equalsPos = query.find('=', position);
				String::size_type	keyEndPos = equalsPos == String::npos ? positionEnd : equalsPos;
				String				key = unescape(query.substr(position, keyEndPos - position));
				String				value = keyEndPos < positionEnd ? unescape(query.substr(keyEndPos + 1, positionEnd - keyEndPos - 1)) : String();

				_keyValues[key].push_back(value);
			}
			position = positionEnd + 1;
		}
	}
	inline Query::Query():_keyValues() {}
	inline Query::Query(const Query &other):_keyValues() {
		*this= other;
	}
	inline Query &Query::operator=(const Query &other) {
		_keyValues.clear();
		if (this != &other) {
			for (_KeyValues::const_iterator key = other._keyValues.begin(); key != other._keyValues.end(); ++key) {
				for (StringList::const_iterator value = key->second.begin(); value != key->second.end(); ++value) {
					_keyValues[key->first].push_back(*value);
				}
			}
		}
		return *this;
	}
	inline const Query::String &Query::operator[](const String &name) const {
		return getOne(name);
	}
	inline Query::String &Query::operator[](const String &name) {
		StringList &values= _keyValues[name];

		if (values.size() == 0) {
			values.push_back("");
		}
		return values[0];
	}
	inline bool Query::hasValue(const String &name) const {
		_KeyValues::const_iterator found= _keyValues.find(name);

		return (found != _keyValues.end()) && (found->second.size() > 0) && (found->second[0].length() > 0);
	}
	inline bool Query::has(const String &name) const {
		_KeyValues::const_iterator found= _keyValues.find(name);
		return found != _keyValues.end();
	}
	inline Query::operator String() const {
		String	result = empty() ? "" : "?";
		String	prefix = "";

		for (_KeyValues::const_iterator key = _keyValues.begin(); key != _keyValues.end(); ++key) {
			if (key->second.size() > 0) {
				for (StringList::const_iterator value = key->second.begin(); value != key->second.end(); ++value) {
					result += prefix + escape(key->first);
					if (value->length() > 0) {
						result += "=" + escape(*value);
					} else {
					}
					prefix = "&";
				}
			} else {
				result += prefix + escape(key->first);
				prefix = "&";
			}
		}
		return result;
	}
	inline const Query::StringList &Query::get(const String &name) const {
		_KeyValues::const_iterator found= _keyValues.find(name);

		if (found == _keyValues.end()) {
			throw std::out_of_range(name);
		}
		return found->second;
	}
	inline const Query::String &Query::getOne(const String &name) const {
		const StringList &found= get(name);

		if (0 == found.size()) {
			throw std::out_of_range(name);
		}
		return found[0];
	}
	inline void Query::remove(const String &name) {
		_keyValues.erase(name);
	}
	inline void Query::add(const String &name, const String &value) {
		_keyValues[name].push_back(value);
	}
	inline bool Query::empty() const {
		return _keyValues.empty();
	}

}

#endif // __HTTPQuery_h__
