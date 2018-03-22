#ifndef __HTTPRequest_h__
#define __HTTPRequest_h__

/** @file HTTPRequest.h
	@todo document
*/

#include <string>
#include <vector>
#include <map>
#include <ctype.h>
#include <stdexcept>
#include "protocol/HTTPHeaders.h"

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

	class RequestLine {
		public:
			typedef std::string String;
			RequestLine();
			RequestLine(const String &line);
			RequestLine(const String &line, String::size_type &after);
			RequestLine(const RequestLine &other);
			~RequestLine() {}
			RequestLine &operator=(const RequestLine &other);
			operator String() const;
			const String &method() const;
			const String &path() const;
			const String &protocol() const;
			const String &version() const;
			const Query &query() const;
			String &method();
			String &path();
			String &protocol();
			String &version();
			Query &query();
		private:
			String	_method;
			String	_path;
			String	_protocol;
			String	_version;
			Query	_query;
			String::size_type _find(bool whitespace, const String &text, String::size_type start, String::size_type end);
			String::size_type _init(const String &line);
	};

	class Request {
		public:
			typedef std::string String;
			Request();
			Request(const String &headers);
			Request(const String &headers, String::size_type &after);
			Request(const Request &other);
			~Request() {}
			Request &operator=(const Request &other);
			operator String() const;
			const RequestLine &request() const;
			RequestLine &request();
			const Headers &fields() const;
			Headers &fields();
		private:
			RequestLine	_request;
			Headers		_headers;
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

	inline RequestLine::RequestLine():_method("GET"), _path("/"), _protocol("HTTP"), _version("1.1"), _query() {}
	inline RequestLine::RequestLine(const String &line):_method(), _path(), _protocol(), _version("1.1"), _query() {
		_init(line);
	}
	inline RequestLine::RequestLine(const String &line, String::size_type &after):_method(), _path(), _protocol(), _version("1.1"), _query() {
		after = _init(line);
	}
	inline RequestLine::RequestLine(const RequestLine &other) {
		*this= other;
	}
	inline RequestLine &RequestLine::operator=(const RequestLine &other) {
		_method = other._method;
		_path = other._path;
		_protocol = other._protocol;
		_version = other._version;
		_query = other._query;
		return *this;
	}
	inline RequestLine::operator String() const {
		return _method + " " + _path + std::string(_query) + " " + _protocol + "/" + _version;
	}
	inline const RequestLine::String &RequestLine::method() const {
		return _method;
	}
	inline const RequestLine::String &RequestLine::path() const {
		return _path;
	}
	inline const RequestLine::String &RequestLine::protocol() const {
		return _protocol;
	}
	inline const RequestLine::String &RequestLine::version() const {
		return _version;
	}
	inline const Query &RequestLine::query() const {
		return _query;
	}
	inline RequestLine::String &RequestLine::method() {
		return _method;
	}
	inline RequestLine::String &RequestLine::path() {
		return _path;
	}
	inline RequestLine::String &RequestLine::protocol() {
		return _protocol;
	}
	inline RequestLine::String &RequestLine::version() {
		return _version;
	}
	inline Query &RequestLine::query() {
		return _query;
	}
	inline RequestLine::String::size_type RequestLine::_find(bool whitespace, const String &text, String::size_type start, String::size_type end) {
		while ( ((::isspace(text[start]) ? true : false) != whitespace) && (start < end) ) {
			++start;
		}
		return start;
	}
	inline RequestLine::String::size_type RequestLine::_init(const String &line) {
		String::size_type	start = 0;
		String::size_type	end;
		String				protocol;
		String::size_type	queryPos;
		String::size_type	slashPos;
		String::size_type	lineEnd = line.find('\r');
		String::size_type	after;

		if (String::npos == lineEnd) {
			lineEnd = line.find('\n');
		}
		after = lineEnd + 1;
		if (String::npos == lineEnd) {
			lineEnd = line.length();
			after = lineEnd;
		}
		end = _find(true, line, start, lineEnd);
		_method = line.substr(start, end - start);
		start = _find(false, line, end, lineEnd);
		end = _find(true, line, start, lineEnd);
		queryPos = line.find('?', start);
		if ( (queryPos == String::npos) || (queryPos > end) ) {
			_path = line.substr(start, end - start);
		} else {
			_path = line.substr(start, queryPos - start);
			_query = Query(line.substr(queryPos + 1, end - queryPos - 1), Query::IsQuery);
		}
		start = _find(false, line, end, lineEnd);
		end = _find(true, line, start, lineEnd);
		slashPos = line.find('/', start);
		if ( (slashPos == String::npos) || (slashPos > end) ) {
			_protocol = line.substr(start, end - start);
		} else {
			_protocol = line.substr(start, slashPos - start);
			_version = line.substr(slashPos + 1, end - slashPos - 1);
		}
		return after + 1;
	}

	inline Request::Request():_request(), _headers() {}
	inline Request::Request(const String &headers):_request(), _headers() {
		String::size_type	offset= 0;
		_request= RequestLine(headers, offset);
		printf("FAIL: offset=%lu\n", offset);
		_headers= Headers(headers.substr(offset));
	}
	inline Request::Request(const String &headers, String::size_type &after):_request(), _headers() {
		_request= RequestLine(headers, after);
		printf("FAIL: offset=%lu\n", after);
		_headers= Headers(headers.substr(after), after);
	}
	inline Request::Request(const Request &other):_request(other._request), _headers(other._headers) {}
	inline Request &Request::operator=(const Request &other) {
		_request= other._request;
		_headers= other._headers;
		return *this;
	}
	inline Request::operator String() const {
		return String(_request) + "\r\n" + String(_headers);
	}
	inline const RequestLine &Request::request() const {
		return _request;
	}
	inline RequestLine &Request::request() {
		return _request;
	}
	inline const Headers &Request::fields() const {
		return _headers;
	}
	inline Headers &Request::fields() {
		return _headers;
	}

}

#endif // __HTTPRequest_h__
