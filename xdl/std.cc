#include "xdl/std.hh"
#include "util/Buffer.hh"
#include "util/datatype.hh"

using namespace std;

vector<string> XDLType::typeNames;

// this must be called before XDLType is used (class initialization of static vars)
void XDLType::classInit() {
	typeNames.reserve(1024);
}

DataType U8::getDataType() const          { return DataType::U8;}
uint32_t U8::size() const                 { return 1; }
void U8::write(Buffer& buf) const         { buf.write(val); }
void U8::display(Buffer& binaryIn, Buffer& asciiOut) const {
  uint8_t u8 = binaryIn.readUint8();
  //asciiOut.print(u8);  any Buffer function for ascii cache?
}

DataType U16::getDataType() const         { return DataType::U16;}
uint32_t U16::size() const                { return 2; }
void U16::write(Buffer& buf) const        { buf.write(val); }
void U16::display(Buffer& binaryIn, Buffer& asciiOut) const {
  uint16_t u16 = binaryIn.readUint16();
}

DataType U24::getDataType() const         { return DataType::U24;}
uint32_t U24::size() const                { return 3; }
void U24::write(Buffer& buf) const        { buf.write(val); }
void U24::display(Buffer& binaryIn, Buffer& asciiOut) const {
  // uint32_t u24 = binaryIn.readUint24(); 
}

DataType U32::getDataType() const         { return DataType::U32;}
uint32_t U32::size() const                { return 4; }
void U32::write(Buffer& buf) const        { buf.write(val); }
void U32::display(Buffer& binaryIn, Buffer& asciiOut) const {
  uint32_t u32 = binaryIn.readUint32();
}

DataType U64::getDataType() const         { return DataType::U64;}
uint32_t U64::size() const                { return 8; }
void U64::write(Buffer& buf) const        { buf.write(val); }
void U64::display(Buffer& binaryIn, Buffer& asciiOut) const {
  uint64_t u64 = binaryIn.readUint64();
}

DataType U128::getDataType() const        { return DataType::U128;}
uint32_t U128::size() const               { return 16; }
void U128::write(Buffer& buf) const       { buf.write(a); buf.write(b); }
void U128::display(Buffer& binaryIn, Buffer& asciiOut) const {
  uint64_t u64_a = binaryIn.readUint64();
  uint64_t u64_b = binaryIn.readUint64();
}

DataType U256::getDataType() const        { return DataType::U256;}
uint32_t U256::size() const               { return 32; }
void U256::write(Buffer& buf) const       {
	buf.write(a); buf.write(b); buf.write(c); buf.write(d);
}
void U256::display(Buffer& binaryIn, Buffer& asciiOut) const {
  uint64_t u64_a = binaryIn.readUint64();
  uint64_t u64_b = binaryIn.readUint64();
  uint64_t u64_c = binaryIn.readUint64();
  uint64_t u64_d = binaryIn.readUint64();
}

DataType I8::getDataType() const          { return DataType::I8;}
uint32_t I8::size() const                 { return 1; }
void I8::write(Buffer& buf) const         { buf.write(val); }
void I8::display(Buffer& binaryIn, Buffer& asciiOut) const {
  int8_t I8 = binaryIn.readI8();
}

DataType I16::getDataType() const         { return DataType::I16;}
uint32_t I16::size() const                { return 2; }
void I16::write(Buffer& buf) const        { buf.write(val); }
void I16::display(Buffer& binaryIn, Buffer& asciiOut) const {
  int16_t I16 = binaryIn.readI16();
}

DataType I24::getDataType() const         { return DataType::I24;}
uint32_t I24::size() const                { return 3; }
void I24::write(Buffer& buf) const        { buf.write(val); }
void I24::display(Buffer& binaryIn, Buffer& asciiOut) const {
  //int32_t I24 = binaryIn.read24();
}

DataType I32::getDataType() const         { return DataType::I32;}
uint32_t I32::size() const                { return 4; }
void I32::write(Buffer& buf) const        { buf.write(val); }
void I32::display(Buffer& binaryIn, Buffer& asciiOut) const {
  int32_t I32 = binaryIn.readI32();
}

DataType I64::getDataType() const         { return DataType::I64;}
uint32_t I64::size() const                { return 8; }
void I64::write(Buffer& buf) const        { buf.write(val); }
void I64::display(Buffer& binaryIn, Buffer& asciiOut) const {
  int64_t I64 = binaryIn.readI64();
}

DataType I128::getDataType() const        { return DataType::I128;}
uint32_t I128::size() const               { return 16; }
void I128::write(Buffer& buf) const       { buf.write(a); buf.write(b); }
void I128::display(Buffer& binaryIn, Buffer& asciiOut) const {
  int64_t I64_a = binaryIn.readI64();
  int64_t I64_b = binaryIn.readI64();
}

DataType I256::getDataType() const        { return DataType::I256;}
uint32_t I256::size() const               { return 32; }
void I256::write(Buffer& buf) const       {
	buf.write(a); buf.write(b); buf.write(c); buf.write(d);
}
void I256::display(Buffer& binaryIn, Buffer& asciiOut) const {
   int64_t I64_a = binaryIn.readI64();
   int64_t I64_b = binaryIn.readI64();
   int64_t I64_c = binaryIn.readI64();
   int64_t I64_d = binaryIn.readI64();
}

DataType BOOL::getDataType() const        { return DataType::BOOL;}
uint32_t BOOL::size() const               { return 1; }
void BOOL::write(Buffer& buf) const       { buf.write(val); }

DataType FLOAT::getDataType() const       { return DataType::FLOAT;}
uint32_t FLOAT::size() const              { return 8; }
void FLOAT::write(Buffer& buf) const      { buf.write(val); }
void FLOAT::display(Buffer& binaryIn, Buffer& asciiOut) const {
  float fl = binaryIn.readFloat();
}

DataType DOUBLE::getDataType() const      { return DataType::DOUBLE;}
uint32_t DOUBLE::size() const             { return 8; }
void DOUBLE::write(Buffer& buf) const     { buf.write(val); }
void DOUBLE::display(Buffer& binaryIn, Buffer& asciiOut) const {
  double db = binaryIn.readDouble();
}

DataType Date::getDataType() const        { return DataType::DATE;}
uint32_t Date::size() const               { return 4; }
void Date::write(Buffer& buf) const       { buf.write(val); }
// int32_t

DataType JulianDate::getDataType() const     { return DataType::JULDATE;}
uint32_t JulianDate::size() const            { return 8; }
void JulianDate::write(Buffer& buf) const    { buf.write(val); }


DataType STRING8::getDataType() const        { return DataType::STRING8;}
uint32_t STRING8::size() const               { return  1 + val.length(); }
void STRING8::write(Buffer& buf) const       { buf.write(val); }
 
void STRING8::display(Buffer& binaryIn, Buffer& asciiOut) const {
  string s8 = binaryIn.readstring1();
}

DataType STRING16::getDataType() const       { return DataType::STRING16;}
uint32_t STRING16::size() const              { return 2 + val.length(); }
void STRING16::write(Buffer& buf) const      { buf.write(val); }
void STRING16::display(Buffer& binaryIn, Buffer& asciiOut) const {
  string s16 = binaryIn.readstring2();
}

DataType STRING32::getDataType() const       { return DataType::STRING32;}
uint32_t STRING32::size() const              { return 4 + val.length(); }
void STRING32::write(Buffer& buf) const      { buf.write(val); }
void STRING32::display(Buffer& binaryIn, Buffer& asciiOut) const {
  string s32 = binaryIn.readstring4();
}

DataType STRING64::getDataType() const       { return DataType::STRING64;}
uint32_t STRING64::size() const              { return 8 + val.length(); }
void STRING64::write(Buffer& buf) const      { buf.write(val); }
// no readstring8();



/*
	All code below here will eventually be generated by the compiler
and then this will be deleted.
*/

void UserId::writeMeta(Buffer& buf) const {
    buf.write(getDataType());
    buf.write(typeName);
}

void Address::writeMeta(Buffer& buf) const {
	buf.write(getDataType());
	buf.write(typeName);
}

void Phone::writeMeta(Buffer& buf) const {
	buf.write(getDataType());
	buf.write(typeName);
}

DataType Email::getDataType() const {
	return DataType::STRING8;
}

uint32_t Email::size() const {
	return 16; //TODO: compute size
}

void Email::write(Buffer& buf) const {
	buf.write(val);
}
void Email::writeMeta(Buffer& buf) const {
	buf.write(getDataType());
	buf.write(getTypeName());
}

DataType ContactInfo::getDataType() const {
	return DataType::STRUCT8;
}

uint32_t ContactInfo::size() const {
	return address.size() + phone.size() + email.size();
}

void ContactInfo::write(Buffer& buf) const {
  address.write(buf);
  phone.write(buf);
  email.write(buf);
}

void ContactInfo::writeMeta(Buffer& buf) const {
  buf.write(getDataType());
  buf.write(getTypeName());
  buf.write(uint8_t(3));
  address.writeMeta(buf);
  phone.writeMeta(buf);
  email.writeMeta(buf);
}

DataType User::getDataType() const {
	return DataType::STRUCT8;
}

uint32_t User::size() const {
	return 12; //TODO: constant + sizeof component strings 
}

void User::write(Buffer& buf) const {
  buf.checkSpace(sizeof(*this));
  userid.write(buf);
  buf.write(firstname);
  buf.write(lastname);
  contact.write(buf);
}

void User::writeMeta(Buffer& buf) const {
  buf.write(getDataType());
  buf.write(getTypeName());
  buf.write(uint8_t(4));
  userid.writeMeta(buf);
  buf.write(DataType::STRING8);
  string fn = "FirstName";
  buf.write(fn);
  buf.write(DataType::STRING8);
  string ln = "LastName";
  buf.write(ln);
  contact.writeMeta(buf);
}
