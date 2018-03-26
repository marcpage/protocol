#ifndef __HTTPResponse_h__
#define __HTTPResponse_h__

/** @file HTTPResponse.h
	@todo document
*/

#include <string>
#include <vector>
#include <map>
#include <ctype.h>
#include <stdexcept>
#include "protocol/HTTPHeaders.h"

namespace http {

	class ResponseLine {
		public:
			typedef std::string String;
			ResponseLine();
			ResponseLine(const ResponseLine &other);
			ResponseLine(const String &line);
			~ResponseLine() {}
			ResponseLine &operator=(const ResponseLine &other);
			operator String() const;
			const String &protocol() const;
			const String &version() const;
			const String &code() const;
			const String &message() const;
			String &protocol();
			String &version();
			String &code();
			String &message();
		private:
			String	_protocol;
			String	_version;
			String	_code;
			String	_message;
			String::size_type _find(bool whitespace, const String &text, String::size_type start, String::size_type end);
			String::size_type _init(const String &line);
	};

	inline ResponseLine::ResponseLine():_protocol("HTTP"), _version("1.0"), _code("404"), _message("File Not Found") {}
	inline ResponseLine::ResponseLine(const ResponseLine &other):_protocol(other._protocol), _version(other._version), _code(other._code), _message(other._message) {}
	inline ResponseLine::ResponseLine(const String &line) {
		_init(line);
	}
	inline ResponseLine &ResponseLine::operator=(const ResponseLine &other) {
		_code= other._code;
		_message= other._message;
		_protocol= other._protocol;
		_version= other._version;
		return *this;
	}
	inline ResponseLine::operator String() const {
		return _protocol + "/" + _version + " " + _code + " " + _message;
	}
	inline const ResponseLine::String &ResponseLine::protocol() const {
		return _protocol;
	}
	inline const ResponseLine::String &ResponseLine::version() const {
		return _version;
	}
	inline const ResponseLine::String &ResponseLine::code() const {
		return _code;
	}
	inline const ResponseLine::String &ResponseLine::message() const {
		return _message;
	}
	inline ResponseLine::String &ResponseLine::protocol() {
		return _protocol;
	}
	inline ResponseLine::String &ResponseLine::version() {
		return _version;
	}
	inline ResponseLine::String &ResponseLine::code() {
		return _code;
	}
	inline ResponseLine::String &ResponseLine::message() {
		return _message;
	}
	inline ResponseLine::String::size_type ResponseLine::_find(bool whitespace, const String &text, String::size_type start, String::size_type end) {
		while ( ((::isspace(text[start]) ? true : false) != whitespace) && (start < end) ) {
			++start;
		}
		return start;
	}
	inline ResponseLine::String::size_type ResponseLine::_init(const String &line) {
		String::size_type	start = 0;
		String::size_type	end;
		String				protocol;
		String::size_type	lineEnd = line.find('\r');
		String::size_type	after;

		if (String::npos == lineEnd) {
			lineEnd = line.find('\n');
		}
		after = lineEnd + ( ((line[lineEnd] == '\r') && (lineEnd + 1 < line.length()) && (line[lineEnd + 1] == '\n')) ? 2 : 1 );
		if (String::npos == lineEnd) {
			lineEnd = line.length();
			after = lineEnd;
		}
		end = _find(true, line, start, lineEnd);
		if (String::npos != end) {
			String::size_type	slashPos= line.find('/');

			if (String::npos == slashPos) {
				_protocol= line.substr(start, end - start);
				_version= "1.0";
			} else {
				_protocol= line.substr(start, slashPos - start);
				_version= line.substr(slashPos + 1, end - slashPos - 1);
			}
		}
		start = _find(false, line, end, lineEnd);
		end = _find(true, line, start, lineEnd);
		_code= line.substr(start, end - start);
		start= _find(false, line, end, lineEnd);
		_message= line.substr(start, lineEnd - start);
		return after;
	}

}

#endif // __HTTPResponse_h__

