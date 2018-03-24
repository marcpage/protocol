#ifndef __HTTPHeaders_h__
#define __HTTPHeaders_h__

/** @file HTTPHeaders.h
	@todo document
*/

#include <string>
#include <map>
#include <ctype.h>
#include <stdexcept>

namespace http {

	class Headers {
		public:
			typedef std::string String;
			Headers();
			Headers(const String &text);
			Headers(const String &text, String::size_type &offset);
			Headers(const Headers &other);
			~Headers() {}
			Headers &operator=(const Headers &other);
			operator String() const;
			const String &operator[](const String &name) const;
			String &operator[](const String &name);
			void remove(const String &name);
			bool has(const String &name) const;
			bool empty() const;
		private:
			typedef std::map<String, String> _Headers;
			struct _Range {
				String::size_type start;
				String::size_type end;
				_Range():start(0), end(0) {}
				_Range(String::size_type s, String::size_type e):start(s), end(e) {}
				bool empty() {return start == end;}
				String &value(const String &text, String &result) {result.assign(text, start, end - start);return result;}
				void nextLine(const String &text);
				_Range consumeKey(const String &text);
				_Range trimmed(const String &text) const;
			};
			_Headers	_headers;
			String::size_type _init(const String &text);
	};

	inline Headers::Headers():_headers() {}
	inline Headers::Headers(const String &text):_headers() {
		_init(text);
	}
	inline Headers::Headers(const String &text, String::size_type &offset):_headers() {
		offset = _init(text);
	}
	inline Headers::Headers(const Headers &other):_headers() {
		*this= other;
	}
	inline Headers &Headers::operator=(const Headers &other) {
		_headers.clear();
		for (_Headers::const_iterator i= other._headers.begin(); i != other._headers.end(); ++i) {
			_headers[i->first] = i->second;
		}
		return *this;
	}
	inline Headers::operator String() const {
		String	results("");

		for (_Headers::const_iterator i= _headers.begin(); i != _headers.end(); ++i) {
			results+= i->first + ": " + i->second + "\r\n";
		}
		return results + "\r\n";
	}
	inline const Headers::String &Headers::operator[](const String &name) const {
		_Headers::const_iterator found= _headers.find(name);

		if (found == _headers.end()) {
			throw std::out_of_range(name);
		}
		return found->second;
	}
	inline Headers::String &Headers::operator[](const String &name) {
		return _headers[name];
	}
	inline void Headers::remove(const String &name) {
		_headers.erase(name);
	}
	inline bool Headers::has(const String &name) const {
		_Headers::const_iterator found= _headers.find(name);

		return (found != _headers.end());
	}
	inline bool Headers::empty() const {
		return _headers.empty();
	}
	inline Headers::String::size_type Headers::_init(const String &text) {
		_Range	line;
		String	lastKey;
		String	value;
		String	prefix;

		do {
			line.nextLine(text);
			if (::isspace(text[line.start])) {
				prefix = " ";
			} else {
				line.consumeKey(text).trimmed(text).value(text, lastKey);
				prefix.clear();
			}
			if (!line.trimmed(text).empty()) {
				_headers[lastKey]+= prefix + line.trimmed(text).value(text, value);
			}
		} while(!line.trimmed(text).empty());
		return line.end;
	}
	inline void Headers::_Range::nextLine(const String &text) {
		String::size_type cr= text.find('\r', end);
		String::size_type lf= text.find('\n', end);

		start = end;
		if ( (cr != String::npos) && (lf != String::npos) ) {
			end = ( ((cr + 1 == lf) || (lf < cr)) ? lf : cr) + 1;
		} else if ( (cr != String::npos) || (lf != String::npos) ) {
			end = ( lf != String::npos ? lf : cr) + 1;
		} else {
			end = text.length();
		}
	}
	inline Headers::_Range Headers::_Range::consumeKey(const String &text) {
		String::size_type colonPos = text.find(':', start);
		String::size_type keyStart = start;

		start= colonPos + 1;
		return _Range(keyStart, colonPos);
	}
	inline Headers::_Range Headers::_Range::trimmed(const String &text) const {
		_Range range(start, end);

		while ( (range.start < range.end) && ::isspace(text[range.start]) ) {
			++range.start;
		}
		while ( (range.start < range.end) && ::isspace(text[range.end - 1]) ) {
			--range.end;
		}
		return range;
	}

}

#endif // __HTTPHeaders_h__
