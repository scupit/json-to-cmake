#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <fstream>
#include <string>
#include "JsonValue.hpp"

enum class ParseType {
  JSTRING,
  JOBJECT,
  JARRAY,
  JNUMBER,
  JTRUE,
  JFALSE,
  JNULL,
  JNEXTITEM,
  JENDITEM,
  JENDFILE,
  JERROR
};

class JsonParser {
  private:
    std::ifstream fin;
    JsonValue json;
    std::string line;
    unsigned int currentParseIndex;

    inline bool isDigit(char c) { return c >= '0' && c <= '9'; }
    bool seqEqLineAtCurrentIndex(const std::string&);
    void parseInto(JsonValue&, const ParseType);
    const std::string parseKey();
    void traverseParseIndexToEndingQuote();

  public:
    JsonParser(const std::string&);
    JsonParser(const std::string&, JsonValue&, const bool = false);
    void parseNewFile(const std::string&);
    void parseNewFile(const std::string&, JsonValue&, const bool = false);
    inline JsonValue getJsonCopy() { return json; }
    inline JsonValue& getJsonReference() { return json; }
    
  protected:
    bool currentPositionValid();
    ParseType determineNextType(const char = '\\');
    void parseArray(JsonValue&);
    void parseTrue(JsonValue&);
    void parseFalse(JsonValue&);
    void parseNull(JsonValue&);
    void parseNumber(JsonValue&);
    void parseObject(JsonValue&);
    void parseString(JsonValue&);

    void setFloat(JsonValue&, const unsigned int, const unsigned int);
    void setInt(JsonValue&, const unsigned int, const unsigned int);
};

#endif