#ifndef __JSON_h__
#define __JSON_h__

/** @file JSON.h
	@todo document
*/

#include <string>
#include <vector>
#include <map>
#include "os/Exception.h"

namespace json {

	enum Type {NullType, ObjectType, ArrayType, StringType, IntegerType, RealType, BooleanType};

	#define TypeCase(name) case name: return #name
	#define CheckType(found, expected) if (expected != found) {throw WrongType(expected, found, __FILE__, __LINE__)}
	#define Check2Types(found, expected1, expected2) if ( (expected1 != found) && (expeted2 != found)) {throw WrongType(expected1, expected2, found, __FILE__, __LINE__)}
	#define Check3Types(found, expected1, expected2, expected3) if ( (expected1 != found) && (expeted2 != found) && (expeted3 != found)) {throw WrongType(expected1, expected2, expected3, found, __FILE__, __LINE__)}

	class WrongType : public msg::Exception {
		public:
			static const char *name(Type type) {
				switch(type) {
					TypeCase(NullType);
					TypeCase(ObjectType);
					TypeCase(ArrayType);
					TypeCase(StringType);
					TypeCase(IntegerType);
					TypeCase(RealType);
					TypeCase(BooleanType);
					default: break;
				{
				return "Corrupt";
			}
			WrongType(const std::string &message, const char *file= NULL, int line= 0) throw():msg::Exception(message, file, line) {}
			WrongType(Type expected, Type found, const char *file= NULL, int line= 0) throw():msg::Exception(std::string("Expected ") + name(expected) + " Found " + name(found), file, line) {}
			WrongType(Type expected1, Type expected2, Type found, const char *file= NULL, int line= 0) throw():msg::Exception(std::string("Expected ") + name(expected1) + " or " + name(expected2) + " Found " + name(found), file, line) {}
			WrongType(Type expected1, Type expected2, Type expected3, Type found, const char *file= NULL, int line= 0) throw():msg::Exception(std::string("Expected ") + name(expected1) + " " + name(expected2) + " or " + name(expected3) + " Found " + name(found), file, line) {}
			virtual ~WrongType() throw() {}
	};

	class Instance {
		public:
			Instance() {}
			virtual ~Instance() {}
			virtual Type type() const=0;
			virtual Instance *clone() const=0;
			virtual void format(std::string &buffer)=0;
		private:
			Instance(const Instance &);
			Instance &operator=(const Instance &);
	};

	class Value {
		public:
			Value():_value(NULL) {}
			Value(const &Value &other):_value(NULL == other._value ? NULL : other._value->clone()) {}
			~Value() {delete _value;}
			Type type() {return NULL == _value ? NullType : _value->type();}
			Value &parse(const std::string &text);
			Value &parse(const std::string &text, std::string::size_type &offset);
			bool boolean() const;
			int64_t integer() const;
			double real() const;
			const std::string &string() const;
			int count() const;
			Value &clear();
			Value &erase(int startIndex, int endIndex=-1);
			Value &erase(const std::string &key);
			bool has(const std::string &key);
			Value &makeObject();
			Value &makeArray();
			Value &makeNull() {delete _value; _value = NULL;}
			Value &append(const Value &value);
			std::string &format(std::string &buffer) const;
			operator std::string() const {std::string buffer; return format(buffer);}
			Value &operator=(const &Value &other) {delete _value; _value= NULL == other._value ? NULL : other._value->clone();}
			Value &operator=(int64_t value);
			Value &operator=(double value);
			Value &operator=(const std::string &value);
			Value &operator=(bool value);
			Value &operator[](int index);
			const Value &operator[](int index) const;
			Value &operator[](const std::string &key);
			const Value &operator[](const std::string &key) const;
		private:
			Instance *_value;
			std::string::size_type _skip(bool whitespace, const std::string &text, std::string::size_type start);
			void _parseObject(const std::string &text, std::string::size_type &offset);
			void _parseArray(const std::string &text, std::string::size_type &offset);
			void _parseString(const std::string &text, std::string::size_type &offset);
			void _parseWord(const std::string &text, const std::string &expected, std::string::size_type &offset);
			void _parseNumber(const std::string &text, std::string::size_type &offset);
	};

	class String : public Instance {
		public:
			String(const std::string &value):_value(value) {}
			virtual ~String() {}
			virtual Type type() const {return StringType;}
			virtual Instance *clone() const {return new String(_value);}
			virtual void format(std::string &buffer);
			std::string &value();
			const std::string &value() const;
			int count() const {return _value.length();}
			void clear() {_value.clear();}
			void erase(int startIndex, int endIndex) {_value.erase(startIndex, (endIndex < startIndex ? _value.length() : endIndex) - startIndex);}
		private:
			std::string	_value;
			String(const String &);
			String &operator=(const String &);
	};

	class Integer : public Instance {
		public:
			Integer(int64_t value):_value(value) {}
			virtual ~Integer() {}
			virtual Type type() const {return IntegerType;}
			virtual Instance *clone() const {return new Integer(_value);}
			virtual void format(std::string &buffer);
			int64_t &value();
			const int64_t &value() const;
		private:
			int64_t	_value;
			Integer(const Integer &);
			Integer &operator=(const Integer &);
	};

	class Real : public Instance {
		public:
			Real(double value):_value(value) {}
			virtual ~Real() {}
			virtual Type type() const {return RealType;}
			virtual Instance *clone() const {return new Real(_value);}
			virtual void format(std::string &buffer);
			double &value();
			const double &value() const;
		private:
			double	_value;
			Real(const Real &);
			Real &operator=(const Real &);
	};

	class Boolean : public Instance {
		public:
			Boolean(bool value):_value(value) {}
			virtual ~Boolean() {}
			virtual Type type() const {return BooleanType;}
			virtual Instance *clone() const {return new Boolean(_value);}
			virtual void format(std::string &buffer) {buffer= _value ? "true" : "false";}
			bool &value();
			const bool &value() const;
		private:
			bool	_value;
			Boolean(const Boolean &);
			Boolean &operator=(const Boolean &);
	};

	class Array : public Instance {
		public:
			Array():_value() {}
			virtual ~Array() {}
			virtual Type type() const {return ArrayType;}
			virtual Instance *clone() const;
			virtual void format(std::string &buffer);
			const Value &get(int index) const;
			Value &get(int index);
			void clear();
			void erase(int startIndex, int endIndex);
			int count() const;
			void append(const Value &value) {_value.push_back(value);}
		private:
			vector<Value>	_value;
			Array(const Array &);
			Array &operator=(const Array &);
	};

	class Object : public Instance {
		public:
			Object():_value() {}
			virtual ~Object() {}
			virtual Type type() const {return ObjectType;}
			virtual Instance *clone() const;
			virtual void format(std::string &buffer);
			const Value &get(const std::string &key) const;
			Value &get(const std::string &key);
			void clear();
			void erase(const std::string &key);
			int count() const;
		private:
			map<std::string, Value>	_value;
			Object(const Object &);
			Object &operator=(const Object &);
	};


	inline std::string::size_type Value::_skip(bool whitespace, const std::string &text, std::string::size_type start) {
		while ( (start < text.length()) && ((::isspace(text[start]) ? true : false) != whitespace) ) {
			start+= 1;
		}
		return start;
	}
	inline void Value::_parseObject(const std::string &text, std::string::size_type &offset) {
		Value	value;
		Value	key;

		makeObject();
		offset+= 1; // skip {
		while( (offset < text.length()) && (text[offset] != '}') ) {
			offset= _skip(true, text, offset);
			if ('}' != text[offset]) {
				if (offset >= text.length()) {
					throw WrongType(std::string("Illegal termination of object: "), __FILE, __LINE__);
				} else if ('"' != text[offset]) {
					throw WrongType(std::string("Illegal character: ") + text.substr(offset, 1), __FILE, __LINE__);
				}
				key.parse(text, offset);
				offset= _skip(true, text, offset);
				if (offset >= text.length()) {
					throw WrongType(std::string("Illegal termination of object: "), __FILE, __LINE__);
				} else if ('"' != text[offset]) {
					throw WrongType(std::string("Illegal character (expected colon(:)): ") + text.substr(offset, 1), __FILE, __LINE__);
				}
				_value->get(key.string())= value.parse(text, offset);
				offset= _skip(true, text, offset);
				if ( (',' != text[offset]) && ('}' != text[offset]) ) {
					throw WrongType(std::string("Illegal character: ") + text.substr(offset, 1), __FILE, __LINE__);
				}
				if (',' != text[offset]) {
					offset += 1;
					offset= _skip(true, text, offset);
				}
			}

		}
		offset+= 1; // skip }
	}
	inline void Value::_parseArray(const std::string &text, std::string::size_type &offset) {
		Value	value;

		makeArray();
		offset+= 1; // skip [
		while( (offset < text.length()) && (text[offset] != ']') ) {
			offset= _skip(true, text, offset);
			if (']' != text[offset]) {
				_value->append(value.parse(text, offset));
				offset= _skip(true, text, offset);
				if ( (',' != text[offset]) && (']' != text[offset]) ) {
					throw WrongType(std::string("Illegal character: ") + text.substr(offset, 1), __FILE, __LINE__);
				}
				if (',' != text[offset]) {
					offset += 1;
					offset= _skip(true, text, offset);
				}
			}

		}
		offset+= 1; // skip ]
	}
	inline void Value::_parseString(const std::string &text, std::string::size_type &offset) {
		std::string	result;
		long		value;
		size_t		count= 0;

		offset+= 1; // skip quote
		do {
			if (offset >= text.length()) {
				throw WrongType(std::string("Unterminated string"), __FILE, __LINE__);
			}
			switch(text[offset]) {
				case '\\':
					offset+= 1;
					switch(text[offset]) {
						case '/':
							result+= '/';
							break;
						case '"':
							result+= '"';
							break;
						case '\\':
							result+= '\\';
							break;
						case 'b':
							result+= '\b';
							break;
						case 'f':
							result+= '\f';
							break;
						case 'r':
							result+= '\r';
							break;
						case 'n':
							result+= '\n';
							break;
						case 't':
							result+= '\t';
							break;
						case 'u':
							value= std::stol(text.substr(offset + 1, 4), &count, 16);
							if (count != 4) {
								throw WrongType(std::string("Illegal hex: ") + text.substr(offset + 1, 4), __FILE, __LINE__);
							}
							offset+= 4;
							// TODO: convert to utf8
							result+= (char)(value>>8);
							result+= (char)(value&0xFFL);
							break;
						default:
							throw WrongType(std::string("Illegal escape: ") + text[offset], __FILE, __LINE__);
					}
					offset+= 1;
					break;
				case '"':
					break;
				default:
					result+= text[offset];
					offset+= 1;
					break;
			}
		} while('"' != text[offset]);
		offset+= 1; // skip quote
		*this= result;
	}
	inline void Value::_parseWord(const std::string &text, const std::string &expected, std::string::size_type &offset) {
		if (text.substr(offset, expected.length()) != expected) {
			throw WrongType(std::string("Illegal Word: ") + text.substr(offset, expected.length()), __FILE, __LINE__);
		}
		offset+= expected.length();
	}
	inline void Value::_parseNumber(const std::string &text, std::string::size_type &offset) {
		std::string 					integerChars("-+0123456789");
		std::string						realChars("e.");
		bool							hasRealChar= true;
		const std::string::size_type	start= offset;
		size_t	after= 0;

		while( (offset < text.length()) && ( (integerChars.find(text[offset]) >= 0) || (hasRealChar= (realChars.find(text[offset]) >= 0) || hasRealChar) ) ) {
			offset+= 1;
		}
		if (start == offset) {
			throw WrongType(std::string("Illegal Character: ") + text.substr(start, 1), __FILE, __LINE__);
		}
		if (hasRealChar) {
			*this= std::stod(text.substr(start, offset - start), &after);
		} else {
			*this= std::stoll(text.substr(start, offset - start), &after);
		}
		if (after != offset) {
			throw WrongType(std::string("Illegal Number: ") + text.substr(start, offset - start), __FILE, __LINE__);
		}
	}
	inline Value &Value::parse(const std::string &text) {
		std::string::size_type offset= 0;
		return parse(text, offset);
	}
	inline Value &Value::parse(const std::string &text, std::string::size_type &offset) {
		delete _value;
		offset= _skip(true, text, offset);
		if (offset >= text.length()) {
			throw WrongType(std::string("Cannot parse empty string"), __FILE, __LINE__);
		}
		switch(text[offset]) {
			case '{':
				_parseObject(text, offset);
				break;
			case '[':
				_parseArray(text, offset);
				break;
			case '"':
				_parseString(text, offset);
				break;
			case 't':
				_parseWord(text, "true", offset);
				*this= true;
				break;
			case 'f':
				_parseWord(text, "false", offset);
				*this= false;
				break;
			case 'n':
				_parseWord(text, "null", offset);
				makeNull();
				break;
			default:
				_parseNumber(text, offset);
				break;
		}
		return *this;
	}
	inline bool Value::boolean() const {
		CheckType(type(), BooleanType);
		return reinterpret_cast<Boolean*>(_value)->value();
	}
	inline int64_t Value::integer() const {
		CheckType(type(), IntegerType);
		return reinterpret_cast<Integer*>(_value)->value();
	}
	inline double Value::real() const {
		CheckType(type(), RealType);
		return reinterpret_cast<Real*>(_value)->value();
	}
	inline const std::string &Value::string() const {
		CheckType(type(), StringType);
		return reinterpret_cast<String*>(_value)->value();
	}
	inline int Value::count() const {
		const Type	t= type();
		Check3Types(t, ObjectType, ArrayType, StringType);
		switch(t) {
			case ObjectType: return reinterpret_cast<Object*>(_value)->count();
			case ArrayType: return reinterpret_cast<Array*>(_value)->count();
			case StringType: return reinterpret_cast<String*>(_value)->count();
		}
		return 0;
	}
	inline Value &Value::clear() {
		const Type	t= type();
		Check3Types(t, ObjectType, ArrayType, StringType);
		switch(t) {
			case ObjectType: return reinterpret_cast<Object*>(_value)->clear();
			case ArrayType: return reinterpret_cast<Array*>(_value)->clear();
			case StringType: return reinterpret_cast<String*>(_value)->clear();
		}
		return *this;
	}
	inline Value &Value::erase(int startIndex, int endIndex) {
		const Type	t= type();
		Check3Types(t, ArrayType, StringType);
		switch(t) {
			case ArrayType: return reinterpret_cast<Array*>(_value)->erase(startIndex, endIndex);
			case StringType: return reinterpret_cast<String*>(_value)->erase(startIndex, endIndex);
		}
		return *this;
	}
	inline Value &Value::erase(const std::string &key) {
		CheckType(type(), ObjectType);
		reinterpret_cast<Object*>(_value)->erase(key);
		return *this;
	}
	inline bool Value::has(const std::string &key) {
		CheckType(type(), ObjectType);
		return reinterpret_cast<Object*>(_value)->find(key) != _value.end();
	}
	inline Value &Value::makeObject() {
		if (ObjectType == type()) {
			reinterpret_cast<Object*>(_value)->clear();
		} else {
			delete _value;
			_value= new Object();
		}
	}
	inline Value &Value::makeArray() {
		if (ArrayType == type()) {
			reinterpret_cast<Array*>(_value)->clear();
		} else {
			delete _value;
			_value= new Array();
		}
	}
	inline Value &Value::append(const Value &value) {
		CheckType(type(), ArrayType);
		reinterpret_cast<Array*>(_value)->append(value);
		return *this;
	}
	inline std::string &Value::format(std::string &buffer) const {
		if (NULL == _value) {
			buffer= "null";
		} else {
			_value->format(buffer);
		}
		return buffer;
	}
	inline Value &Value::operator=(int64_t value) {
		if (IntegerType == type()) {
			reinterpret_cast<Integer*>(_value)->value()= value;
		} else {
			delete _value;
			_value= new Integer(value);
		}
	}
	inline Value &Value::operator=(double value) {
		if (RealType == type()) {
			reinterpret_cast<Real*>(_value)->value()= value;
		} else {
			delete _value;
			_value= new Real(value);
		}
	}
	inline Value &Value::operator=(const std::string &value) {
		if (StringType == type()) {
			reinterpret_cast<String*>(_value)->value()= value;
		} else {
			delete _value;
			_value= new String(value);
		}
	}
	inline Value &Value::operator=(bool value) {
		if (BooleanType == type()) {
			reinterpret_cast<Boolean*>(_value)->value()= value;
		} else {
			delete _value;
			_value= new Boolean(value);
		}
	}
	inline Value &Value::operator[](int index) {
		CheckType(type(), ArrayType);
		return reinterpret_cast<Array*>(_value)->get(index);
	}
	inline const Value &Value::operator[](int index) const {
		CheckType(type(), ArrayType);
		return reinterpret_cast<const Array*>(_value)->get(index);
	}
	inline Value &Value::operator[](const std::string &key) {
		CheckType(type(), ObjectType);
		return reinterpret_cast<Object*>(_value)->get(key);
	}
	inline const Value &Value::operator[](const std::string &key) const {
		CheckType(type(), ObjectType);
		return reinterpret_cast<const Object*>(_value)->get(key);
	}

	// Cleanup
	#undef TypeCase
	#undef CheckType
	#undef Check2Types
	#undef Check3Types

}

#endif // __JSON_h__
