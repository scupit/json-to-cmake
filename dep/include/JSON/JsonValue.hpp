#ifndef JSONVALUE_HPP
#define JSONVALUE_HPP

#define JFLOAT_TOLERANCE 0.0001

#include <iostream>
#include <map>
#include <vector>

// Define possible json types
enum class JsonType {
  JARRAY,
  JOBJECT,
  JSTRING,
  JINT,
  JFLOAT,
  JBOOL,
  JNULL,
  JINVALID
};

// Forward declare JsonValue so the typedefs do not break.
class JsonValue;

typedef std::vector<JsonValue> JsonArray;
typedef std::map<std::string, JsonValue> JsonObject;

// Union containing possible data for each type other than JNULL.
// No need to include null item since it can be inferred from the type anyways.
union Value {
  int intValue;
  float floatValue;
  bool boolValue;
  std::string* stringValue;
  JsonArray* arrayValue;
  JsonObject* objectValue;
};

// Class for managing types and value
class JsonValue {
  private:
    JsonType type;
    Value value;

  public:
    JsonValue(const std::nullptr_t = nullptr);
    JsonValue(const JsonType);
    JsonValue(const int);
    JsonValue(const float);
    JsonValue(const bool);
    JsonValue(const char*);
    JsonValue(const std::string&);
    JsonValue(const JsonArray&);
    JsonValue(const JsonObject&);
    JsonValue(const JsonValue&);
    ~JsonValue();

    inline JsonType getType() { return type; }

    void changeType(const JsonType);
    bool boolNot();
    void reset();
    void setNull(void);
    void set(const int);
    void set(const float);
    void set(const bool);
    void set(const char*);
    void set(const std::string&);
    void set(const JsonArray&);
    void set(const JsonObject&);

    std::nullptr_t asNull();
    int asInt();
    float asFloat();
    bool asBool();
    std::string& asString();
    JsonArray& asVector();
    JsonObject& asMap();

    bool hasOwnProperty(const std::string&);

    // This stringify is not recommended.
    std::string stringify();
    
    // Use one of these instead
    void stringify(std::string&);
    void stringify(std::ostream&);

    bool typeMatches(const JsonValue&) const;

    JsonValue& operator=(const JsonValue&);
    int operator=(const int);
    float operator=(const float);
    bool operator=(const bool);
    const char* operator=(const char*);
    std::string& operator=(std::string&);
    const JsonArray& operator=(const JsonArray&);
    const JsonObject& operator=(const JsonObject&);

    bool operator==(const JsonValue&) const;
    bool operator==(const int) const;
    bool operator==(const float) const;
    bool operator==(const bool) const;
    bool operator==(const char*) const;
    bool operator==(const std::string&) const;
    bool operator==(const JsonArray&) const;
    bool operator==(const JsonObject&) const;

    friend bool operator==(const int, const JsonValue&);
    friend bool operator==(const float, const JsonValue&);
    friend bool operator==(const bool, const JsonValue&);
    friend bool operator==(const char*, const JsonValue&);
    friend bool operator==(const std::string&, const JsonValue&);
    friend bool operator==(const JsonArray&, const JsonValue&);
    friend bool operator==(const JsonObject&, const JsonValue&);

    bool operator!=(const JsonValue&) const;
    bool operator!=(const int) const;
    bool operator!=(const float) const;
    bool operator!=(const bool) const;
    bool operator!=(const char*) const;
    bool operator!=(const std::string&) const;
    bool operator!=(const JsonArray&) const;
    bool operator!=(const JsonObject&) const;

    friend bool operator!=(const int, const JsonValue&);
    friend bool operator!=(const float, const JsonValue&);
    friend bool operator!=(const bool, const JsonValue&);
    friend bool operator!=(const char*, const JsonValue&);
    friend bool operator!=(const std::string&, const JsonValue&);
    friend bool operator!=(const JsonArray&, const JsonValue&);
    friend bool operator!=(const JsonObject&, const JsonValue&);

    bool operator<(const JsonValue&) const;
    bool operator<(const int) const;
    bool operator<(const float) const;

    friend bool operator<(const int, const JsonValue&);
    friend bool operator<(const float, const JsonValue&);

    bool operator<=(const JsonValue&) const;
    bool operator<=(const int) const;
    bool operator<=(const float) const;

    friend bool operator<=(const int, const JsonValue&);
    friend bool operator<=(const float, const JsonValue&);

    bool operator>(const JsonValue&) const;
    bool operator>(const int) const;
    bool operator>(const float) const;

    friend bool operator>(const int, const JsonValue&);
    friend bool operator>(const float, const JsonValue&);

    bool operator>=(const JsonValue&) const;
    bool operator>=(const int) const;
    bool operator>=(const float) const;

    friend bool operator>=(const int, const JsonValue&);
    friend bool operator>=(const float, const JsonValue&);

    bool operator!();

    JsonValue& operator++();
    JsonValue operator++(int);

    JsonValue& operator--();
    JsonValue operator--(int);

    JsonValue operator+(const JsonValue&) const;
    JsonValue operator+(const int) const;
    JsonValue operator+(const float) const;
    
    // These two return std::strings because they only have one possible return type, unlike numerical addition.
    std::string operator+(const std::string&) const;
    std::string operator+(const char*) const;

    JsonValue& operator+=(const JsonValue&);
    JsonValue& operator+=(const int);
    JsonValue& operator+=(const float);
    JsonValue& operator+=(const char*);
    JsonValue& operator+=(const std::string&);

    JsonValue operator-(const JsonValue&) const;
    JsonValue operator-(const int) const;
    JsonValue operator-(const float) const;

    JsonValue& operator-=(const JsonValue&);
    JsonValue& operator-=(const int);
    JsonValue& operator-=(const float);

    JsonValue operator*(const JsonValue&) const;
    JsonValue operator*(const int) const;
    JsonValue operator*(const float) const;

    JsonValue& operator*=(const JsonValue&);
    JsonValue& operator*=(const int);
    JsonValue& operator*=(const float);

    JsonValue operator/(const JsonValue&) const;
    JsonValue operator/(const int) const;
    JsonValue operator/(const float) const;

    JsonValue& operator/=(const JsonValue&);
    JsonValue& operator/=(const int);
    JsonValue& operator/=(const float);

    JsonValue operator%(const JsonValue&) const;
    JsonValue operator%(const int) const;
    JsonValue operator%(const float) const;

    JsonValue& operator%=(const JsonValue&);
    JsonValue& operator%=(const int);
    JsonValue& operator%=(const float);

    JsonValue& operator[](const int);
    JsonValue& operator[](const std::string&);

    friend std::ostream& operator<<(std::ostream&, JsonValue&);

  private:
    static bool jfequal(const float, const float);
    void destroyCurrentValue();
    void stringifyHelper(std::ostream&, const unsigned short, const bool = false);
    void typeChangeHelper(const JsonType);
    static void writeTabs(std::ostream&, unsigned short);
};

bool operator==(const JsonArray&, const JsonArray&);
bool operator!=(const JsonArray&, const JsonArray&);

bool operator==(const JsonObject&, const JsonObject&);
bool operator!=(const JsonObject&, const JsonObject&);

#endif