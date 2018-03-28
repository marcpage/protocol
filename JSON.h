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
	#define CheckType(found, expected) if (expected != found) {throw WrongType(expected, found, __FILE__, __LINE__);}
	#define Check2Types(found, expected1, expected2) if ( (expected1 != found) && (expected2 != found)) {throw WrongType(expected1, expected2, found, __FILE__, __LINE__);}
	#define Check3Types(found, expected1, expected2, expected3) if ( (expected1 != found) && (expected2 != found) && (expected3 != found)) {throw WrongType(expected1, expected2, expected3, found, __FILE__, __LINE__);}

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
				}
				return "Corrupt";
			}
			WrongType(const std::string &message, const char *file= NULL, int line= 0) throw():msg::Exception(message, file, line) {}
			WrongType(Type expected, Type found, const char *file= NULL, int line= 0) throw():msg::Exception(std::string("Expected ") + name(expected) + " Found " + name(found), file, line) {}
			WrongType(Type expected1, Type expected2, Type found, const char *file= NULL, int line= 0) throw():msg::Exception(std::string("Expected ") + name(expected1) + " or " + name(expected2) + " Found " + name(found), file, line) {}
			WrongType(Type expected1, Type expected2, Type expected3, Type found, const char *file= NULL, int line= 0) throw():msg::Exception(std::string("Expected ") + name(expected1) + " " + name(expected2) + " or " + name(expected3) + " Found " + name(found), file, line) {}
			virtual ~WrongType() throw() {}
	};

	class Value {
		public:
			Value():_value(NULL) {}
			Value(const std::string &text):_value(NULL) {parse(text);}
			Value(const Value &other):_value( (NULL == other._value) ? reinterpret_cast<Instance*>(NULL) : other._value->clone()) {}
			~Value() {makeNull();}
			Type type() const {return NULL == _value ? NullType : _value->type();}
			bool is(Type t) const {return t == type();}
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
			Value &makeNull() {delete _value; _value = NULL; return *this;}
			Value &append(const Value &value);
			std::string &format(std::string &buffer) const;
			operator std::string() const {std::string buffer; return format(buffer);}
			Value &operator=(const Value &other) {makeNull(); _value= (NULL == other._value) ? reinterpret_cast<Instance*>(NULL) : other._value->clone(); return *this;}
			Value &operator=(int value);
			Value &operator=(int64_t value);
			Value &operator=(double value);
			Value &operator=(const std::string &value);
			Value &operator=(bool value);
			Value &operator[](int index);
			const Value &operator[](int index) const;
			Value &operator[](const std::string &key);
			const Value &operator[](const std::string &key) const;
		private:
			class Instance {
				public:
					Instance() {}
					virtual ~Instance() {}
					virtual Type type() const=0;
					virtual Instance *clone() const=0;
					virtual void format(std::string &buffer) const=0;
				private:
					Instance(const Instance &);
					Instance &operator=(const Instance &);
			};

			class String : public Instance {
				public:
					String(const std::string &value):_value(value) {}
					virtual ~String() {}
					virtual Type type() const {return StringType;}
					virtual Instance *clone() const {return new String(_value);}
					virtual void format(std::string &buffer) const;
					std::string &value() {return _value;}
					const std::string &value() const {return _value;}
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
					virtual void format(std::string &buffer) const {buffer= std::to_string(_value);}
					int64_t &value() {return _value;}
					const int64_t &value() const {return _value;}
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
					virtual void format(std::string &buffer) const {buffer= std::to_string(_value);}
					double &value() {return _value;}
					const double &value() const {return _value;}
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
					virtual void format(std::string &buffer) const {buffer= _value ? "true" : "false";}
					bool &value() {return _value;}
					const bool &value() const {return _value;}
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
					virtual void format(std::string &buffer) const;
					const Value &get(int index) const {return _value[index];}
					Value &get(int index) {return _value[index];}
					void clear() {_value.clear();}
					void erase(int startIndex, int endIndex) {_value.erase(_value.begin() + startIndex, _value.begin() + endIndex);}
					int count() const {return _value.size();}
					void append(const Value &value) {_value.push_back(value);}
				private:
					std::vector<Value>	_value;
					Array(const Array &);
					Array &operator=(const Array &);
			};

			class Object : public Instance {
				public:
					Object():_value() {}
					virtual ~Object() {}
					virtual Type type() const {return ObjectType;}
					virtual Instance *clone() const;
					virtual void format(std::string &buffer) const;
					const Value &get(const std::string &key) const;
					Value &get(const std::string &key) {return _value[key];}
					void clear() {_value.clear();}
					void erase(const std::string &key) {_value.erase(key);}
					int count() const {return _value.size();}
					bool has(const std::string &key) const {return _value.find(key) != _value.end();}
				private:
					std::map<std::string, Value>	_value;
					Object(const Object &);
					Object &operator=(const Object &);
			};
			Instance *_value;
			std::string::size_type _skipWhitespace(const std::string &text, std::string::size_type start);
			void _parseObject(const std::string &text, std::string::size_type &offset);
			void _parseArray(const std::string &text, std::string::size_type &offset);
			void _parseString(const std::string &text, std::string::size_type &offset);
			void _parseWord(const std::string &text, const std::string &expected, std::string::size_type &offset);
			void _parseNumber(const std::string &text, std::string::size_type &offset);
	};

	inline std::string::size_type Value::_skipWhitespace(const std::string &text, std::string::size_type start) {
		while ( (start < text.length()) && ::isspace(text[start]) ) {
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
			offset= _skipWhitespace(text, offset);
			if ('}' != text[offset]) {
				if (offset >= text.length()) {
					throw WrongType(std::string("Illegal termination of object: "), __FILE__, __LINE__);
				} else if ('"' != text[offset]) {
					throw WrongType(std::string("Illegal character: ") + text.substr(offset, 1), __FILE__, __LINE__);
				}
				key.parse(text, offset);
				offset= _skipWhitespace(text, offset);
				if (offset >= text.length()) {
					throw WrongType(std::string("Illegal termination of object: "), __FILE__, __LINE__);
				} else if (':' != text[offset]) {
					throw WrongType(std::string("Illegal character (expected colon(:)): ") + text.substr(offset, 1), __FILE__, __LINE__);
				}
				offset+= 1; // skip :
				reinterpret_cast<Object*>(_value)->get(key.string())= value.parse(text, offset);
				offset= _skipWhitespace(text, offset);
				if ( (',' != text[offset]) && ('}' != text[offset]) ) {
					throw WrongType(std::string("Illegal character: ") + text.substr(offset, 1), __FILE__, __LINE__);
				}
				if (',' == text[offset]) {
					offset += 1; // skip ,
					offset= _skipWhitespace(text, offset);
				}
			}

		}
		offset+= 1; // skip }
		if (offset > text.length()) {
			throw WrongType("Unterminated Object", __FILE__, __LINE__);
		}
	}
	inline void Value::_parseArray(const std::string &text, std::string::size_type &offset) {
		Value	value;

		makeArray();
		offset+= 1; // skip [
		while( (offset < text.length()) && (text[offset] != ']') ) {
			offset= _skipWhitespace(text, offset);
			if (']' != text[offset]) {
				reinterpret_cast<Array*>(_value)->append(value.parse(text, offset));
				offset= _skipWhitespace(text, offset);
				if ( (',' != text[offset]) && (']' != text[offset]) ) {
					throw WrongType(std::string("Illegal character: ") + text.substr(offset, 1), __FILE__, __LINE__);
				}
				if (',' == text[offset]) {
					offset += 1; // skip ,
					offset= _skipWhitespace(text, offset);
				}
			}

		}
		offset+= 1; // skip ]
		if (offset > text.length()) {
			throw WrongType("Unterminated Array", __FILE__, __LINE__);
		}
	}
	inline void Value::_parseString(const std::string &text, std::string::size_type &offset) {
		std::string	result;
		long		value;
		size_t		count= 0;

		offset+= 1; // skip quote
		do {
			if (offset >= text.length()) {
				throw WrongType(std::string("Unterminated string"), __FILE__, __LINE__);
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
								throw WrongType(std::string("Illegal hex: ") + text.substr(offset + 1, 4), __FILE__, __LINE__);
							}
							offset+= 4;
							// TODO: convert to utf8
							result+= (char)(value>>8);
							result+= (char)(value&0xFFL);
							break;
						default:
							throw WrongType(std::string("Illegal escape: ") + text[offset], __FILE__, __LINE__);
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
			throw WrongType(std::string("Illegal Word: ") + text.substr(offset, expected.length()), __FILE__, __LINE__);
		}
		offset+= expected.length();
	}
	inline void Value::_parseNumber(const std::string &text, std::string::size_type &offset) {
		std::string 					integerChars("-+0123456789");
		std::string						realChars("e.");
		bool							hasRealChar= false;
		const std::string::size_type	start= offset;
		size_t	after= 0;

		while( (offset < text.length()) && ( (integerChars.find(text[offset]) != std::string::npos) || ((realChars.find(text[offset]) != std::string::npos)) ) ) {
			hasRealChar= hasRealChar || (realChars.find(text[offset]) != std::string::npos);
			offset+= 1;
		}
		if (start == offset) {
			throw WrongType(std::string("Illegal Character: ") + text.substr(start, 1), __FILE__, __LINE__);
		}
		if (hasRealChar) {
			try {
				*this= std::stod(text.substr(start, offset - start), &after);
			} catch(const std::exception &exception) {
				throw WrongType(std::string("Illegal Number: ") + text.substr(start, offset - start), __FILE__, __LINE__);
			}
		} else {
			try {
				*this= std::stoll(text.substr(start, offset - start), &after);
			} catch(const std::exception &exception) {
				throw WrongType(std::string("Illegal Number: ") + text.substr(start, offset - start), __FILE__, __LINE__);
			}
		}
		if (after != offset - start) {
			throw WrongType(std::string("Illegal Number: ") + text.substr(start, offset - start), __FILE__, __LINE__);
		}
	}
	inline Value &Value::parse(const std::string &text) {
		std::string::size_type offset= 0;
		return parse(text, offset);
	}
	inline Value &Value::parse(const std::string &text, std::string::size_type &offset) {
		makeNull();
		offset= _skipWhitespace(text, offset);
		if (offset >= text.length()) {
			throw WrongType(std::string("Cannot parse empty string."), __FILE__, __LINE__);
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

		switch(t) {
			case ObjectType: return reinterpret_cast<Object*>(_value)->count();
			case ArrayType: return reinterpret_cast<Array*>(_value)->count();
			case StringType: return reinterpret_cast<String*>(_value)->count();
			case NullType:
			case IntegerType:
			case RealType:
			case BooleanType:
			default:
				Check3Types(t, ObjectType, ArrayType, StringType);
				break;
		}
		return 0;
	}
	inline Value &Value::clear() {
		const Type	t= type();

		switch(t) {
			case ObjectType:
				reinterpret_cast<Object*>(_value)->clear();
				break;
			case ArrayType:
				reinterpret_cast<Array*>(_value)->clear();
				break;
			case StringType:
				reinterpret_cast<String*>(_value)->clear();
				break;
			case NullType:
			case IntegerType:
			case RealType:
			case BooleanType:
			default:
				Check3Types(t, ObjectType, ArrayType, StringType);
				break;
		}
		return *this;
	}
	inline Value &Value::erase(int startIndex, int endIndex) {
		const Type	t= type();

		switch(t) {
			case ArrayType:
				reinterpret_cast<Array*>(_value)->erase(startIndex, endIndex);
				break;
			case StringType:
				reinterpret_cast<String*>(_value)->erase(startIndex, endIndex);
				break;
			case NullType:
			case IntegerType:
			case RealType:
			case BooleanType:
			case ObjectType:
			default:
				Check2Types(t, ArrayType, StringType);
				break;
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
		return reinterpret_cast<Object*>(_value)->has(key);
	}
	inline Value &Value::makeObject() {
		if (ObjectType == type()) {
			reinterpret_cast<Object*>(_value)->clear();
		} else {
			makeNull();
			_value= new Object();
		}
		return *this;
	}
	inline Value &Value::makeArray() {
		if (ArrayType == type()) {
			reinterpret_cast<Array*>(_value)->clear();
		} else {
			makeNull();
			_value= new Array();
		}
		return *this;
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
	inline Value &Value::operator=(int value) {
		*this= (int64_t)value;
		return *this;
	}
	inline Value &Value::operator=(int64_t value) {
		if (IntegerType == type()) {
			reinterpret_cast<Integer*>(_value)->value()= value;
		} else {
			makeNull();
			_value= new Integer(value);
		}
		return *this;
	}
	inline Value &Value::operator=(double value) {
		if (RealType == type()) {
			reinterpret_cast<Real*>(_value)->value()= value;
		} else {
			makeNull();
			_value= new Real(value);
		}
		return *this;
	}
	inline Value &Value::operator=(const std::string &value) {
		if (StringType == type()) {
			reinterpret_cast<String*>(_value)->value()= value;
		} else {
			makeNull();
			_value= new String(value);
		}
		return *this;
	}
	inline Value &Value::operator=(bool value) {
		if (BooleanType == type()) {
			reinterpret_cast<Boolean*>(_value)->value()= value;
		} else {
			makeNull();
			_value= new Boolean(value);
		}
		return *this;
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

	inline void Value::String::format(std::string &buffer) const {
		buffer= "\"";
		for (std::string::size_type i= 0; i < _value.length(); ++i) {
			switch(_value[i]) {
				case '\\':
					buffer+= "\\\\";
					break;
				case '"':
					buffer+= "\\\"";
					break;
				case '/':
					buffer+= "\\/";
					break;
				case '\t':
					buffer+= "\\t";
					break;
				case '\r':
					buffer+= "\\r";
					break;
				case '\n':
					buffer+= "\\n";
					break;
				case '\b':
					buffer+= "\\b";
					break;
				case '\f':
					buffer+= "\\f";
					break;
				// TODO: Handle utf8 higher order characters and non-printables
				default:
					buffer+= _value[i];
					break;
			}
		}
		buffer+= "\"";

	}

	inline Value::Instance *Value::Array::clone() const {
		Array	*value= new Array();

		for (std::vector<Value>::const_iterator i= _value.begin(); i != _value.end(); ++i) {
			value->append(*i);
		}
		return value;
	}
	inline void Value::Array::format(std::string &buffer) const {
		std::string	value;
		std::string	prefix= "";

		buffer= "[";
		for (std::vector<Value>::const_iterator i= _value.begin(); i != _value.end(); ++i) {
			i->format(value);
			buffer+= prefix + value;
			if (prefix.length() == 0) {
				prefix= ",";
			}
		}
		buffer+= "]";
	}

	inline Value::Instance *Value::Object::clone() const {
		Object	*value= new Object();

		for (std::map<std::string,Value>::const_iterator i= _value.begin(); i != _value.end(); ++i) {
			value->get(i->first)= i->second;
		}
		return value;
	}
	inline void Value::Object::format(std::string &buffer) const {
		Value	key;
		std::string	keyStr;
		std::string	value;
		std::string	prefix= "";

		buffer= "{";
		for (std::map<std::string,Value>::const_iterator i= _value.begin(); i != _value.end(); ++i) {
			key= i->first;
			key.format(keyStr);
			i->second.format(value);
			buffer+= prefix + keyStr + ":" + value;
			if (prefix.length() == 0) {
				prefix= ",";
			}
		}
		buffer+= "}";
	}
	inline const Value &Value::Object::get(const std::string &key) const {
		std::map<std::string,Value>::const_iterator found= _value.find(key);

		if (found == _value.end()) {
			throw WrongType(std::string("Key not found: ") + key, __FILE__, __LINE__);
		}
		return found->second;
	}

	// Cleanup
	#undef TypeCase
	#undef CheckType
	#undef Check2Types
	#undef Check3Types

}

#endif // __JSON_h__
