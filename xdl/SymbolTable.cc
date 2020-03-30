#include "xdl/SymbolTable.hh"
#include "xdl/XDLCompiler.hh"

//#include "symTable.h"
#include "util/Buffer.hh"
#include <iostream>
using namespace std;

//GrailParser *parser;

SymbolTable::SymbolTable(XDLCompiler& c) : compiler(c) {}


uint32_t TypeDef::size() const {
	return 0;
}

void SymbolTable::addBuiltin(const char name[], DataType typeId) {
  XDLType* t = getSymbol(name);
  if (t == nullptr) {      
    sym[name] = new BuiltinType(name, typeId);
  } else {
    compiler.internalError(std::string("duplicate builtin symbol") + name);
    //throw Ex2(Errcode::Multiply_DEFINED, std::string("Builtin : " + name));
  }
}

uint32_t Struct::size() const {
	return packedSize;
}

Struct* SymbolTable::addStruct(const char name[]) { 
  // unordered_map<std::string, Type*>::iterator i=sym.find(name);
  XDLType* t = getSymbol(name);
  if (t != nullptr) {      // if struct is already there
    compiler.duplicateSymbol(name);
    //      throw Ex2(Errcode::Multiply_DEFINED, std::string(name));
  }
  // create a new struct and return so members may be added
  return static_cast<Struct*>(sym[name] = new Struct(name));
}

void SymbolTable::addMember(Struct* s, const char type[], const char name[]) {
  XDLType* t = getSymbol(type);
  if (t == nullptr) {
    compiler.undefinedSymbol(type);
    return;
  }
  s->addMember(compiler, t, name);
}

//This is not needed, only implemented because Member inherits from XDLType
uint32_t Member::size() const {
	return 0;
}


void Struct::addMember(XDLCompiler& compiler, XDLType* t, const char name[]) { // Type* type){
  // test give a string, but we are adding type
  if (t != nullptr) {
    memberList.push_back(new Member(name, t));
		packedSize += t->size(); // add up the packed byte size of all members
  } else {
    compiler.undefinedSymbol(name);
    //throw Ex2(Errcode::UNDEFINED_TYPE, std::string("Type : " + name));
  }
}

void SymbolTable::addTypedef(const char name[], const char type[]) {
  XDLType* t = getSymbol(name);
  if (t != nullptr) { // if duplicate symbol, register the error and get out
    compiler.duplicateSymbol(name);
    //throw Ex2(Errcode::MULTIPLY_DEFINED, std::string("Typedef : ") + name);
    return;
  }
  t = getSymbol(type);
  if (t == nullptr) { // undefined symbol
     compiler.undefinedSymbol(name);
    //throw Ex2(Errcode::UNDEFINED_TYPE, std::string("Type : ") + name);
    return;
  }
  sym[name] = new TypeDef(name, t);
}

void SymbolTable::addRegex(const std::string& name, const std::string& regex) { 
  XDLType* t = getSymbol(name);
  if (t != nullptr) {
    compiler.duplicateSymbol(name);
    //throw Ex2(Errcode::MULTIPLY_DEFINED, std::string("Regex : " + name));
    return;
  }
  sym[name] = new Regex(name, regex);
}

void SymbolTable::addValueXdl(const std::string& name,XDLType* xdltype) {
  sym[name] = xdltype;
}
#if 0
XDLType* SymbolTable::add(Buffer& metadataBuf){
  DataType t = DataType(metadataBuf.readUint8());
  switch (t) {
  case DataType::U8:{
    string nameU8 = metadataBuf.readstring1(); 
    typeName.push_back(nameU8);
    XDLType* check_t = getSymbol(nameU8);
    if (check_t == nullptr) {      
      metabuf[nameU8] = t;
    }
    break;}
    
  case DataType::U16:{
    string nameU16 = metadataBuf.readstring1();
    typeName.push_back(nameU16);
    XDLType* check_t = getSymbol(nameU16);
    if (check_t == nullptr) {      
      metabuf[nameU16] = t;
    }
    break;}
    
  case DataType::U32:{
    string nameU32 = metadataBuf.readstring1();
    typeName.push_back(nameU32);
    XDLType* check_t = getSymbol(nameU32);
    if (check_t == nullptr) {      
      metabuf[nameU32] = t;
    }
    break;}
    
  case DataType::U64:{
    string nameU64 = metadataBuf.readstring1();
    typeName.push_back(nameU64);
    XDLType* check_t = getSymbol(nameU64);
    if (check_t == nullptr) {      
      metabuf[nameU64] = t;
    }
    break;}
    
  case DataType::DOUBLE:{
    string nameD = metadataBuf.readstring1();
    typeName.push_back(nameD);
    XDLType* check_t = getSymbol(nameD);
    if (check_t == nullptr) {      
      metabuf[nameD] = t;
    }
    break;}
    
  case DataType::FLOAT:{
    string nameF = metadataBuf.readstring1();
    typeName.push_back(nameF);
    XDLType* check_t = getSymbol(nameF);
    if (check_t == nullptr) {      
      metabuf[nameF] = t;
    }
    break;}
    
  case DataType::STRING8:{
    string nameS8 = metadataBuf.readstring1();
    typeName.push_back(nameS8);
    XDLType* check_t = getSymbol(nameS8);
    if (check_t == nullptr) {      
      metabuf[nameS8] = t;
    }
    break;}
    
  case DataType::STRUCT8:{
    string nameST8 = metadataBuf.readstring1(); //get the Struct name
    typeName.push_back(nameST8);
    XDLType* check_t = getSymbol(nameST8); //if there is no such struct
    if (check_t == nullptr) {      
      metabuf[nameST8] = t; //add element in map
    }
    uint8_t len_struct8 = metadataBuf.readUint8(); //get number of elements in struct
    for (int i = 0; i < len_struct8; i++) {
      nameST8.addMember//add(metadataBuf);
    }
    break;}
    
  case DataType::LIST8:{
    //string nameL8 = data.readstring1(); list does not have the name
    
    uint8_t len_list8 = metadataBuf.readUint8();
    for (int i = 0; i < len_list8; i++) {
      add(metadataBuf);
    }
    break;}
  }
}

//need seperate files for matadata and data
void SymbolTable:: displayText(const std::string& name) const{
  for (int i = 0; i < typeName.size(); i++){
    name = typeName[i];
    DataType t = metabuf[name];
    t.display(binaryIn, asciiOut); 
  } 
}
#endif

uint32_t BuiltinType::size() const {
	return 0;
}

void BuiltinType::write(Buffer& buf) const {
  // buf.write(val);
}

void BuiltinType::writeMeta(Buffer& buf) const {
  buf.write(t); //DataType 
}

void BuiltinType::display(Buffer& binaryIn, Buffer& asciiOut) const {
}

void BuiltinType::format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const {
}


void TypeDef::write(Buffer& buf) const {
  //buf.write(val);
}

void TypeDef::writeMeta(Buffer& buf) const {
  buf.write(type->getDataType()); 
  buf.write(typeName);
}

void TypeDef::display(Buffer& binaryIn, Buffer& asciiOut) const {
}

void TypeDef::format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const {
}


void Member::write(Buffer& buf) const {
  //buf.write(val);
}

void Member::writeMeta(Buffer& buf) const {
  buf.write(type->getDataType());
  buf.write(typeName);
}

//void Member::display(char buf[]) const {}

void Member::display(Buffer& binaryIn, Buffer& asciiOut) const {
}

void Member::format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const {
}

void Struct::write(Buffer& buf) const {
  for(auto m : memberList) {
    m->write(buf); // each member write metadata
  }
}

void Struct::writeMeta(Buffer& buf) const {
  buf.write(getDataType());
  buf.write(typeName); //name
  buf.write(uint8_t(memberList.size())); // number of member
  for(auto m : memberList)
    m->writeMeta(buf); // each member write metadata
}
#if 0
void Struct::display(Buffer& binaryIn, Buffer& asciiOut) const {
  string nameST8 = binaryIn.readstring1();
  XDLType* check_t = getSymbol(nameST8); 
  if (check_t != nullptr) {      
    for (int i = 0; i < memberList.size(); i++){
      MemberName = memberList[i];
      MemberName.display(binaryIn, asciiOut); 
    } 
  }
  break;// add new?
}
#endif
void Struct::format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const {
}

void Regex::write(Buffer& buf) const {
  buf.write(rexp);
}

void Regex::writeMeta(Buffer& buf) const {
  buf.write(getDataType());
  buf.write(typeName);
}

void Regex::display(Buffer& binaryIn, Buffer& asciiOut) const {
}

void Regex::format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const {
}
