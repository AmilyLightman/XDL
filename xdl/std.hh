/*
	This file contains all the classes for built-in XDL types that are
	pregenerated manually as well as some example classes that will eventually
	be generated from the xdl compiler which at the moment does not exist.

	When that happens, this comment will change and those classes will be
	removed from std.hh

  what should be generated:
	if you declare        to send                 metadata should be
	u8 x;                 buf.write(x);           buf.write(DataType::U8);
	u16 x;                buf.write(x);           buf.write(DataType::U16);

*/
#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include "util/Buffer.hh"
#include "grail/Errors.hh"
#include "util/datatype.hh"

/*
	XDLType is the base class of all XDL-generated code.
	Because any information in the class will increase the runtime size of all children
	instead of storing a string typename inside XDLType, we will use a table
	At constructor time, the built-in types look up the offset in the table and store that
	(a 32-bit quantity)
	For a user-defined type, it registers the name in a HashMap in SymbolTable, and then
	quickly turns the string into an offset into the type table.
	This means the overhead of XDLType is:

	  1. a pointer to the VFT since there are virtual functions
		2. the offset to the name in the array

		This is a total of 12 bytes, or 8 for a 32-bit platform.

		For speed, all methods that return a string object should return const string&
		The string must already exist so that nothing is allocated
 */
class XDLType {
protected:
  std::string typeName; //TODO: this should be replaced by a single offset into a typename list
	static std::vector<std::string> typeNames;
public:
	static void classInit();
  XDLType(){}
  XDLType(const std::string& typeName) : typeName(typeName) {}
  virtual void write(Buffer& b) const = 0;
  virtual void writeMeta(Buffer& buf) const { buf.write(getDataType()); }
  virtual uint32_t size() const = 0;
  //TODO: virtual void generateCode() = 0;
  const std::string& getTypeName() const { return typeName; }
  virtual DataType getDataType() const = 0;
  
  // using this XDL type as ASCII text, not formatted
  void display(Buffer& binaryIn, Buffer& asciiOut) const;

  // using this XDL type, output formatted
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U8 : public XDLType {
private:
  uint8_t val;
public:
  U8(uint8_t val) : XDLType("U8"), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U16 : public XDLType {
private:
  uint16_t val;
public:
  U16(uint16_t val) : XDLType("U16"), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const;
  	friend bool operator ==(const U16& a, const U16& b) {
    return a.val == b.val;
  }
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U24 : public XDLType {
private:
  uint32_t val;
public:
  U24(uint32_t val) : XDLType("U24"), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  	friend bool operator ==(const U24& a, const U24& b) {
    return a.val == b.val;
  }
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U32 : public XDLType {
private:
  uint32_t val;
public:
  U32(uint32_t val) : XDLType("U32"), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  friend bool operator ==(const U32& a, const U32& b) {
    return a.val == b.val;
  }
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U64 : public XDLType {
private:
  uint64_t val;
public:
  U64(uint64_t val) : XDLType("U64"), val(val){}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  friend bool operator ==(const U64& a, const U64& b) {
    return a.val == b.val;
  }
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U128 : public XDLType {
private:
  uint64_t a,b; 	
public:
  U128(uint64_t a,uint64_t b) : XDLType("U128"), a(a), b(b) {}
  U128(uint64_t b) : XDLType("U128"), a(0), b(b) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  friend bool operator ==(const U128& a, const U128& b) {
    return a.a == b.a && a.b == b.b;
  }
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U256 : public XDLType {
private:
  uint64_t a,b,c,d;
public:
  U256(uint64_t a,uint64_t b,uint64_t c,uint64_t d) :
		XDLType("U256"), a(a), b(b), c(c), d(d) {}
  DataType getDataType() const;
  uint32_t size() const override;
  friend bool operator ==(const U256& a, const U256& b) {
    return a.a == b.a && a.b == b.b;
  }
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I8 : public XDLType {
private:
  int8_t val;
public:
  I8(int8_t val) : XDLType("I8"), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I16 : public XDLType {
private:
  int16_t val;
public:
  I16(int16_t val) : XDLType("I16"), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I24 : public XDLType {
private:
  int32_t val;
public:
  I24(int32_t val) : XDLType("I24"), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I32 : public XDLType {
private:
  int32_t val;
public:
  I32(int32_t val):val(val), XDLType("I32"){}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I64 : public XDLType {
private:
  int8_t val;
public:
  I64(int64_t val) : XDLType("I64"), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I128 : public XDLType {
private:
	int64_t a;
  uint64_t b;
public:
  I128(int64_t a, uint64_t b) : XDLType("I128"), a(a), b(b) {}
  I128(int64_t b) : XDLType("U128"), a(b < 0 ? 0xFFFFFFFFFFFFFFFFLL : 0), b(b) {}
  DataType getDataType() const;
  uint32_t size() const override;
  friend bool operator ==(const I128& a, const I128& b) {
    return a.a == b.a && a.b == b.b;
  }
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I256 : public XDLType {
private:
	int64_t a;
  uint64_t b,c,d;
public:
  I256(int64_t a,uint64_t b,uint64_t c,uint64_t d) :
		XDLType("I256"), a(a), b(b), c(c), d(d) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  friend bool operator ==(const I256& a, const I256& b) {
    return a.a == b.a && a.b == b.b;
  }
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class BOOL : public XDLType {
private:
  bool val;
public:
  BOOL(bool val) : val(val), XDLType("BOOL") {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class FLOAT : public XDLType {
private:
  float val;
public:
  FLOAT(float val) : val(val), XDLType("FLOAT") {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class DOUBLE : public XDLType {
private:
  double val;
public:
  DOUBLE(double val):val(val), XDLType("DOUBLE"){}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Date : public XDLType {
private:
  int32_t val;
  Date(int32_t val) : XDLType("Date"), val(val) {}
public:
	Date(int32_t year, uint8_t month, uint8_t day);
	int32_t getYear() const;
	int32_t getMonth() const;
	int32_t getDay() const;
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class JulianDate : public XDLType {
private:
  double val;
  JulianDate(double val) : XDLType("JulDate"), val(val) {}
public:
	JulianDate(int32_t year, uint8_t month, uint8_t day,
						 uint8_t hour, uint8_t min, uint8_t sec);
	int32_t getYear() const;
	int32_t getMonth() const;
	int32_t getDay() const;
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class STRING8 : public XDLType {
private:
  std::string val;
public:
  STRING8(string val) :  XDLType("STRING8"), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class STRING16 : public XDLType {
private:
  std::string val;
public:
  STRING16(string val) : XDLType("STRING16"), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class STRING32 : public XDLType {
private:
  std::string val;
public:
  STRING32(string val) : XDLType("STRING32"), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class STRING64 : public XDLType {
private:
  std::string val;
public:
  STRING64(string val) : XDLType("STRING64"), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};


class UserId : public U64 {
private:
  std::string typeName;
public:
  UserId(uint64_t v) : typeName("UserId"), U64(v) {}
  void writeMeta(Buffer& buf) const override;
  //TODO:    b.write(DataType::DEFINETYPE); b.write("UserId");
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Address : public STRING8 {
private:
  std::string typeName;
public:
  Address(const std::string& s) : typeName("Address"), STRING8(s) {}
  void writeMeta(Buffer& b) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};  
  
class Phone : public U64 {
private:
  std::string typeName;
public:
  Phone(uint64_t v) : typeName("Phone"), U64(v) {}
  void writeMeta(Buffer& b) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Email : public XDLType {
private:
  std::string val;
public:
  Email(const std::string& s) : XDLType("Email"), val(s) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& b) const override;
  void writeMeta(Buffer& b) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};  

class ContactInfo : public XDLType {
private:
  Address address;
  Phone phone;
  Email email;
public:
  ContactInfo(const Address& address,
              const Phone& phone,
              const Email& email) :
		XDLType("ContactInfo"), address(address), phone(phone), email(email) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& b) const override;
  void writeMeta(Buffer& b) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class User : public XDLType {
private:
  UserId userid;
  string firstname;
  string lastname;
  ContactInfo contact;
public:
  User(const UserId& userid,
       const char firstname[],
       const char lastname[],
       const Address& address,
       const Phone& phone,
       const Email& email) :
		XDLType("User"),
    userid(userid), firstname(firstname), lastname(lastname),
    contact( address, phone, email) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

template<typename T>
class List : public XDLType {
private:
  std::vector<T> impl;
public:
  List() : XDLType("LIST8") {}
	DataType getDataType() const   { return DataType::LIST8;}
  void add(const T& e) {
    impl.push_back(e);
  }
#if 0
  template <class... Args>
  void add(Args&&... args) {
    impl.emplace_back(args);
  }
#endif
  uint32_t size() const override {
		return impl.size(); // TODO: * T.size();
	}
  void write(Buffer& buf) const override {
    for (uint32_t i = 0; i < impl.size(); i++)
      impl[i].write(buf);
  }
  void writeMeta(Buffer& buf) const override {
    buf.write(DataType::LIST8);
    buf.write(uint8_t(impl.size()));
    if (impl.size() == 0)
      return;
    impl[0].writeMeta(buf);
  }
};
