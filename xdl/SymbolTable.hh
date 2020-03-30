#pragma once

#include <iostream>
#include <string>
#include <vector>
#include<unordered_map>
#include "util/datatype.hh"
#include "util/Buffer.hh"
#include "grail/Errors.hh"
#include "util/Ex.hh"
#include "xdl/std.hh"

class XDLCompiler;

class BuiltinType : public XDLType {
private:
  DataType t;
public:
  BuiltinType(const std::string& name, const DataType& t) : XDLType(name), t(t) {}
	uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const override { return t; }
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};
		  
class TypeDef: public XDLType {
private:
  XDLType* type;
  bool strict;
public:
  TypeDef(const std::string& name, XDLType* t) : XDLType(name), type(t), strict(true) {}
	uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const  { return type->getDataType(); }
  string getTypeName() const {return typeName;}
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Member: public XDLType {
private:
  XDLType* type;
public:
  Member(const std::string& name, XDLType* t) : XDLType(name), type(t) {}
	uint32_t size() const override; // not really needed for members, but must be overridden
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const  { return type->getDataType(); }
  string getTypeName() const {return typeName;}
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Struct: public XDLType {
private:
  vector<Member*> memberList;
	uint32_t packedSize;
public:
  Struct(const std::string& name) : XDLType(name), packedSize(0) {}
  void addMember(XDLCompiler& compiler, XDLType* t, const char name[]);
  vector<Member*>& getMember() {
    return memberList;
  }
	uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const  { return DataType::STRUCT8; }
  string getTypeName() const {return typeName;}
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Regex: public XDLType {
private:
  std::regex rexp;
public:
  Regex(const std::string& name, const std::string& exp) : XDLType(name), rexp(exp) {}
  // return true if this regex object matches the text
  bool match(const std::string& text) const {
    return regex_match(text, rexp);
  }
	uint32_t size() const override { return 0; } //TODO: implement in .cc
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const  { return DataType::REGEX; }
  string getTypeName() const {return typeName;}
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};
  


class SymbolTable {
protected:
  unordered_map<std::string, XDLType*> sym;
  unordered_map<std::string, DataType> metabuf;
  vector<string> typeName;
  XDLCompiler& compiler;
public:
  SymbolTable(XDLCompiler& c);
  XDLType* getSymbol(const std::string& name) {
    unordered_map<std::string, XDLType*>::iterator i = sym.find(name);
    return (i != sym.end()) ? i->second : nullptr;
  }
  DataType getDataType(XDLType* t) { return t->getDataType(); }
  void addBuiltin(const char name[], DataType typeId);
  Struct* addStruct(const char name[]);  
  void addMember(Struct* s, const char type[], const char name[]);
  void addTypedef(const char name[], const char type[]);
  void addRegex(const std::string& name, const std::string& regex);
  void addValueXdl(const std::string& name,XDLType* xdltype);
  
  // read in metadata from buffer and return pointer to the type being added to the symbol table
  XDLType* add(Buffer& metadataBuf);

  // dump a specific type as text
  void displayText(const std::string& name) const;
};

