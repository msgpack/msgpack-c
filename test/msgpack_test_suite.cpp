/*
 * Source: msgpack-test-suite/dist/msgpack-test-suite.json at commit
 * e04f6edeaae589c768d6b70fcce80aa786b7800e
 * SHA-256: 8ea4d7aea19f7cf447ffe1031a4818bf5fd8b99dc28baf2b4a33fe9d8e5a5874.
 * Copyright (c) 2017-2018 Yusuke Kawasaki; see msgpack-test-suite-LICENSE.
 */

#include "msgpack.h"
#include <msgpack/timestamp.h>

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif //defined(__GNUC__)

#include <gtest/gtest.h>

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif //defined(__GNUC__)

#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Value {
    enum Kind {
        NIL,
        BOOLEAN,
        SIGNED_INTEGER,
        UNSIGNED_INTEGER,
        FLOAT,
        STRING,
        BINARY,
        ARRAY,
        MAP,
        EXTENSION,
        TIMESTAMP
    };

    Value() : kind(NIL), boolean(false), signed_integer(0), unsigned_integer(0),
              floating_point(0.0), ext_type(0), seconds(0), nanoseconds(0) {}

    Kind kind;
    bool boolean;
    int64_t signed_integer;
    uint64_t unsigned_integer;
    double floating_point;
    int8_t ext_type;
    int64_t seconds;
    uint32_t nanoseconds;
    std::string bytes;
    std::vector<Value> elements;
    std::vector<std::pair<Value, Value> > entries;
};

struct TestCase {
    TestCase(const char* group_name, bool is_bignum, const Value& test_value,
             const std::vector<std::string>& accepted_encodings)
        : group(group_name), bignum(is_bignum), value(test_value), encodings(accepted_encodings) {}

    const char* group;
    bool bignum;
    Value value;
    std::vector<std::string> encodings;
};

Value nilValue() {
    return Value();
}

Value booleanValue(bool value) {
    Value result;
    result.kind = Value::BOOLEAN;
    result.boolean = value;
    return result;
}

Value signedValue(int64_t value) {
    Value result;
    result.kind = Value::SIGNED_INTEGER;
    result.signed_integer = value;
    return result;
}

Value unsignedValue(uint64_t value) {
    Value result;
    result.kind = Value::UNSIGNED_INTEGER;
    result.unsigned_integer = value;
    return result;
}

Value floatValue(double value) {
    Value result;
    result.kind = Value::FLOAT;
    result.floating_point = value;
    return result;
}

std::string hexBytes(const char* hex) {
    std::string result;
    int high = -1;
    for (const char* p = hex; *p != '\0'; ++p) {
        if (*p == '-') continue;
        const char c = *p;
        int digit = -1;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'f') digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') digit = c - 'A' + 10;
        if (digit < 0) return std::string();
        if (high < 0) high = digit;
        else {
            result.push_back(static_cast<char>((high << 4) | digit));
            high = -1;
        }
    }
    return high < 0 ? result : std::string();
}

Value stringValue(const char* hex) {
    Value result;
    result.kind = Value::STRING;
    result.bytes = hexBytes(hex);
    return result;
}

Value binaryValue(const char* hex) {
    Value result;
    result.kind = Value::BINARY;
    result.bytes = hexBytes(hex);
    return result;
}

Value arrayValue(const std::vector<Value>& values) {
    Value result;
    result.kind = Value::ARRAY;
    result.elements = values;
    return result;
}

Value mapValue(const std::vector<std::pair<Value, Value> >& values) {
    Value result;
    result.kind = Value::MAP;
    result.entries = values;
    return result;
}

Value extensionValue(int8_t type, const char* hex) {
    Value result;
    result.kind = Value::EXTENSION;
    result.ext_type = type;
    result.bytes = hexBytes(hex);
    return result;
}

Value timestampValue(int64_t seconds, uint32_t nanoseconds) {
    Value result;
    result.kind = Value::TIMESTAMP;
    result.seconds = seconds;
    result.nanoseconds = nanoseconds;
    return result;
}

const std::vector<TestCase>& msgpackTestCases() {
    static const TestCase cases[] = {
        TestCase("10.nil", false, nilValue(), {"c0"}),
        TestCase("11.bool", false, booleanValue(false), {"c2"}),
        TestCase("11.bool", false, booleanValue(true), {"c3"}),
        TestCase("12.binary", false, binaryValue(""), {"c4-00", "c5-00-00", "c6-00-00-00-00"}),
        TestCase("12.binary", false, binaryValue("01"), {"c4-01-01", "c5-00-01-01", "c6-00-00-00-01-01"}),
        TestCase("12.binary", false, binaryValue("00ff"), {"c4-02-00-ff", "c5-00-02-00-ff", "c6-00-00-00-02-00-ff"}),
        TestCase("20.number-positive", false, unsignedValue(0ULL), {"00", "cc-00", "cd-00-00", "ce-00-00-00-00", "cf-00-00-00-00-00-00-00-00", "d0-00", "d1-00-00", "d2-00-00-00-00", "d3-00-00-00-00-00-00-00-00", "ca-00-00-00-00", "cb-00-00-00-00-00-00-00-00"}),
        TestCase("20.number-positive", false, unsignedValue(1ULL), {"01", "cc-01", "cd-00-01", "ce-00-00-00-01", "cf-00-00-00-00-00-00-00-01", "d0-01", "d1-00-01", "d2-00-00-00-01", "d3-00-00-00-00-00-00-00-01", "ca-3f-80-00-00", "cb-3f-f0-00-00-00-00-00-00"}),
        TestCase("20.number-positive", false, unsignedValue(127ULL), {"7f", "cc-7f", "cd-00-7f", "ce-00-00-00-7f", "cf-00-00-00-00-00-00-00-7f", "d0-7f", "d1-00-7f", "d2-00-00-00-7f", "d3-00-00-00-00-00-00-00-7f"}),
        TestCase("20.number-positive", false, unsignedValue(128ULL), {"cc-80", "cd-00-80", "ce-00-00-00-80", "cf-00-00-00-00-00-00-00-80", "d1-00-80", "d2-00-00-00-80", "d3-00-00-00-00-00-00-00-80"}),
        TestCase("20.number-positive", false, unsignedValue(255ULL), {"cc-ff", "cd-00-ff", "ce-00-00-00-ff", "cf-00-00-00-00-00-00-00-ff", "d1-00-ff", "d2-00-00-00-ff", "d3-00-00-00-00-00-00-00-ff"}),
        TestCase("20.number-positive", false, unsignedValue(256ULL), {"cd-01-00", "ce-00-00-01-00", "cf-00-00-00-00-00-00-01-00", "d1-01-00", "d2-00-00-01-00", "d3-00-00-00-00-00-00-01-00"}),
        TestCase("20.number-positive", false, unsignedValue(65535ULL), {"cd-ff-ff", "ce-00-00-ff-ff", "cf-00-00-00-00-00-00-ff-ff", "d2-00-00-ff-ff", "d3-00-00-00-00-00-00-ff-ff"}),
        TestCase("20.number-positive", false, unsignedValue(65536ULL), {"ce-00-01-00-00", "cf-00-00-00-00-00-01-00-00", "d2-00-01-00-00", "d3-00-00-00-00-00-01-00-00"}),
        TestCase("20.number-positive", false, unsignedValue(2147483647ULL), {"ce-7f-ff-ff-ff", "cf-00-00-00-00-7f-ff-ff-ff", "d2-7f-ff-ff-ff", "d3-00-00-00-00-7f-ff-ff-ff"}),
        TestCase("20.number-positive", false, unsignedValue(2147483648ULL), {"ce-80-00-00-00", "cf-00-00-00-00-80-00-00-00", "d3-00-00-00-00-80-00-00-00", "ca-4f-00-00-00", "cb-41-e0-00-00-00-00-00-00"}),
        TestCase("20.number-positive", false, unsignedValue(4294967295ULL), {"ce-ff-ff-ff-ff", "cf-00-00-00-00-ff-ff-ff-ff", "d3-00-00-00-00-ff-ff-ff-ff", "cb-41-ef-ff-ff-ff-e0-00-00"}),
        TestCase("21.number-negative", false, signedValue(-1LL), {"ff", "d0-ff", "d1-ff-ff", "d2-ff-ff-ff-ff", "d3-ff-ff-ff-ff-ff-ff-ff-ff", "ca-bf-80-00-00", "cb-bf-f0-00-00-00-00-00-00"}),
        TestCase("21.number-negative", false, signedValue(-32LL), {"e0", "d0-e0", "d1-ff-e0", "d2-ff-ff-ff-e0", "d3-ff-ff-ff-ff-ff-ff-ff-e0", "ca-c2-00-00-00", "cb-c0-40-00-00-00-00-00-00"}),
        TestCase("21.number-negative", false, signedValue(-33LL), {"d0-df", "d1-ff-df", "d2-ff-ff-ff-df", "d3-ff-ff-ff-ff-ff-ff-ff-df"}),
        TestCase("21.number-negative", false, signedValue(-128LL), {"d0-80", "d1-ff-80", "d2-ff-ff-ff-80", "d3-ff-ff-ff-ff-ff-ff-ff-80"}),
        TestCase("21.number-negative", false, signedValue(-256LL), {"d1-ff-00", "d2-ff-ff-ff-00", "d3-ff-ff-ff-ff-ff-ff-ff-00"}),
        TestCase("21.number-negative", false, signedValue(-32768LL), {"d1-80-00", "d2-ff-ff-80-00", "d3-ff-ff-ff-ff-ff-ff-80-00"}),
        TestCase("21.number-negative", false, signedValue(-65536LL), {"d2-ff-ff-00-00", "d3-ff-ff-ff-ff-ff-ff-00-00"}),
        TestCase("21.number-negative", false, signedValue(-2147483648LL), {"d2-80-00-00-00", "d3-ff-ff-ff-ff-80-00-00-00", "cb-c1-e0-00-00-00-00-00-00"}),
        TestCase("22.number-float", false, floatValue(0.5), {"ca-3f-00-00-00", "cb-3f-e0-00-00-00-00-00-00"}),
        TestCase("22.number-float", false, floatValue(-0.5), {"ca-bf-00-00-00", "cb-bf-e0-00-00-00-00-00-00"}),
        TestCase("23.number-bignum", true, unsignedValue(4294967296ULL), {"cf-00-00-00-01-00-00-00-00", "d3-00-00-00-01-00-00-00-00", "ca-4f-80-00-00", "cb-41-f0-00-00-00-00-00-00"}),
        TestCase("23.number-bignum", true, signedValue(-4294967296LL), {"d3-ff-ff-ff-ff-00-00-00-00", "cb-c1-f0-00-00-00-00-00-00"}),
        TestCase("23.number-bignum", true, unsignedValue(281474976710656ULL), {"cf-00-01-00-00-00-00-00-00", "d3-00-01-00-00-00-00-00-00", "ca-57-80-00-00", "cb-42-f0-00-00-00-00-00-00"}),
        TestCase("23.number-bignum", true, signedValue(-281474976710656LL), {"d3-ff-ff-00-00-00-00-00-00", "ca-d7-80-00-00", "cb-c2-f0-00-00-00-00-00-00"}),
        TestCase("23.number-bignum", true, unsignedValue(9223372036854775807ULL), {"d3-7f-ff-ff-ff-ff-ff-ff-ff", "cf-7f-ff-ff-ff-ff-ff-ff-ff"}),
        TestCase("23.number-bignum", true, signedValue(-9223372036854775807LL), {"d3-80-00-00-00-00-00-00-01"}),
        TestCase("23.number-bignum", true, unsignedValue(9223372036854775808ULL), {"cf-80-00-00-00-00-00-00-00"}),
        TestCase("23.number-bignum", true, signedValue(std::numeric_limits<int64_t>::min()), {"d3-80-00-00-00-00-00-00-00"}),
        TestCase("23.number-bignum", true, unsignedValue(18446744073709551615ULL), {"cf-ff-ff-ff-ff-ff-ff-ff-ff"}),
        TestCase("30.string-ascii", false, stringValue(""), {"a0", "d9-00", "da-00-00", "db-00-00-00-00"}),
        TestCase("30.string-ascii", false, stringValue("61"), {"a1-61", "d9-01-61", "da-00-01-61", "db-00-00-00-01-61"}),
        TestCase("30.string-ascii", false, stringValue("31323334353637383930313233343536373839303132333435363738393031"), {"bf-31-32-33-34-35-36-37-38-39-30-31-32-33-34-35-36-37-38-39-30-31-32-33-34-35-36-37-38-39-30-31", "d9-1f-31-32-33-34-35-36-37-38-39-30-31-32-33-34-35-36-37-38-39-30-31-32-33-34-35-36-37-38-39-30-31", "da-00-1f-31-32-33-34-35-36-37-38-39-30-31-32-33-34-35-36-37-38-39-30-31-32-33-34-35-36-37-38-39-30-31"}),
        TestCase("30.string-ascii", false, stringValue("3132333435363738393031323334353637383930313233343536373839303132"), {"d9-20-31-32-33-34-35-36-37-38-39-30-31-32-33-34-35-36-37-38-39-30-31-32-33-34-35-36-37-38-39-30-31-32", "da-00-20-31-32-33-34-35-36-37-38-39-30-31-32-33-34-35-36-37-38-39-30-31-32-33-34-35-36-37-38-39-30-31-32"}),
        TestCase("31.string-utf8", false, stringValue("d09ad0b8d180d0b8d0bbd0bbd0b8d186d0b0"), {"b2-d0-9a-d0-b8-d1-80-d0-b8-d0-bb-d0-bb-d0-b8-d1-86-d0-b0", "d9-12-d0-9a-d0-b8-d1-80-d0-b8-d0-bb-d0-bb-d0-b8-d1-86-d0-b0"}),
        TestCase("31.string-utf8", false, stringValue("e381b2e38289e3818ce381aa"), {"ac-e3-81-b2-e3-82-89-e3-81-8c-e3-81-aa", "d9-0c-e3-81-b2-e3-82-89-e3-81-8c-e3-81-aa"}),
        TestCase("31.string-utf8", false, stringValue("ed959ceab880"), {"a6-ed-95-9c-ea-b8-80", "d9-06-ed-95-9c-ea-b8-80"}),
        TestCase("31.string-utf8", false, stringValue("e6b189e5ad97"), {"a6-e6-b1-89-e5-ad-97", "d9-06-e6-b1-89-e5-ad-97"}),
        TestCase("31.string-utf8", false, stringValue("e6bca2e5ad97"), {"a6-e6-bc-a2-e5-ad-97", "d9-06-e6-bc-a2-e5-ad-97"}),
        TestCase("32.string-emoji", false, stringValue("e29da4"), {"a3-e2-9d-a4", "d9-03-e2-9d-a4"}),
        TestCase("32.string-emoji", false, stringValue("f09f8dba"), {"a4-f0-9f-8d-ba", "d9-04-f0-9f-8d-ba"}),
        TestCase("40.array", false, arrayValue({}), {"90", "dc-00-00", "dd-00-00-00-00"}),
        TestCase("40.array", false, arrayValue({unsignedValue(1ULL)}), {"91-01", "dc-00-01-01", "dd-00-00-00-01-01"}),
        TestCase("40.array", false, arrayValue({unsignedValue(1ULL), unsignedValue(2ULL), unsignedValue(3ULL), unsignedValue(4ULL), unsignedValue(5ULL), unsignedValue(6ULL), unsignedValue(7ULL), unsignedValue(8ULL), unsignedValue(9ULL), unsignedValue(10ULL), unsignedValue(11ULL), unsignedValue(12ULL), unsignedValue(13ULL), unsignedValue(14ULL), unsignedValue(15ULL)}), {"9f-01-02-03-04-05-06-07-08-09-0a-0b-0c-0d-0e-0f", "dc-00-0f-01-02-03-04-05-06-07-08-09-0a-0b-0c-0d-0e-0f", "dd-00-00-00-0f-01-02-03-04-05-06-07-08-09-0a-0b-0c-0d-0e-0f"}),
        TestCase("40.array", false, arrayValue({unsignedValue(1ULL), unsignedValue(2ULL), unsignedValue(3ULL), unsignedValue(4ULL), unsignedValue(5ULL), unsignedValue(6ULL), unsignedValue(7ULL), unsignedValue(8ULL), unsignedValue(9ULL), unsignedValue(10ULL), unsignedValue(11ULL), unsignedValue(12ULL), unsignedValue(13ULL), unsignedValue(14ULL), unsignedValue(15ULL), unsignedValue(16ULL)}), {"dc-00-10-01-02-03-04-05-06-07-08-09-0a-0b-0c-0d-0e-0f-10", "dd-00-00-00-10-01-02-03-04-05-06-07-08-09-0a-0b-0c-0d-0e-0f-10"}),
        TestCase("40.array", false, arrayValue({stringValue("61")}), {"91-a1-61", "dc-00-01-a1-61", "dd-00-00-00-01-a1-61"}),
        TestCase("41.map", false, mapValue({}), {"80", "de-00-00", "df-00-00-00-00"}),
        TestCase("41.map", false, mapValue({std::make_pair(stringValue("61"), unsignedValue(1ULL))}), {"81-a1-61-01", "de-00-01-a1-61-01", "df-00-00-00-01-a1-61-01"}),
        TestCase("41.map", false, mapValue({std::make_pair(stringValue("61"), stringValue("41"))}), {"81-a1-61-a1-41", "de-00-01-a1-61-a1-41", "df-00-00-00-01-a1-61-a1-41"}),
        TestCase("42.nested", false, arrayValue({arrayValue({})}), {"91-90", "dc-00-01-dc-00-00", "dd-00-00-00-01-dd-00-00-00-00"}),
        TestCase("42.nested", false, arrayValue({mapValue({})}), {"91-80", "dc-00-01-80", "dd-00-00-00-01-80"}),
        TestCase("42.nested", false, mapValue({std::make_pair(stringValue("61"), mapValue({}))}), {"81-a1-61-80", "de-00-01-a1-61-de-00-00", "df-00-00-00-01-a1-61-df-00-00-00-00"}),
        TestCase("42.nested", false, mapValue({std::make_pair(stringValue("61"), arrayValue({}))}), {"81-a1-61-90", "de-00-01-a1-61-90", "df-00-00-00-01-a1-61-90"}),
        TestCase("50.timestamp", false, timestampValue(1514862245LL, 0u), {"d6-ff-5a-4a-f6-a5"}),
        TestCase("50.timestamp", false, timestampValue(1514862245LL, 678901234u), {"d7-ff-a1-dc-d7-c8-5a-4a-f6-a5"}),
        TestCase("50.timestamp", false, timestampValue(2147483647LL, 999999999u), {"d7-ff-ee-6b-27-fc-7f-ff-ff-ff"}),
        TestCase("50.timestamp", false, timestampValue(2147483648LL, 0u), {"d6-ff-80-00-00-00"}),
        TestCase("50.timestamp", false, timestampValue(2147483648LL, 1u), {"d7-ff-00-00-00-04-80-00-00-00"}),
        TestCase("50.timestamp", false, timestampValue(4294967295LL, 0u), {"d6-ff-ff-ff-ff-ff"}),
        TestCase("50.timestamp", false, timestampValue(4294967295LL, 999999999u), {"d7-ff-ee-6b-27-fc-ff-ff-ff-ff"}),
        TestCase("50.timestamp", false, timestampValue(4294967296LL, 0u), {"d7-ff-00-00-00-01-00-00-00-00"}),
        TestCase("50.timestamp", false, timestampValue(17179869183LL, 999999999u), {"d7-ff-ee-6b-27-ff-ff-ff-ff-ff"}),
        TestCase("50.timestamp", false, timestampValue(17179869184LL, 0u), {"c7-0c-ff-00-00-00-00-00-00-00-04-00-00-00-00"}),
        TestCase("50.timestamp", false, timestampValue(-1LL, 0u), {"c7-0c-ff-00-00-00-00-ff-ff-ff-ff-ff-ff-ff-ff"}),
        TestCase("50.timestamp", false, timestampValue(-1LL, 999999999u), {"c7-0c-ff-3b-9a-c9-ff-ff-ff-ff-ff-ff-ff-ff-ff"}),
        TestCase("50.timestamp", false, timestampValue(0LL, 0u), {"d6-ff-00-00-00-00"}),
        TestCase("50.timestamp", false, timestampValue(0LL, 1u), {"d7-ff-00-00-00-04-00-00-00-00"}),
        TestCase("50.timestamp", false, timestampValue(1LL, 0u), {"d6-ff-00-00-00-01"}),
        TestCase("50.timestamp", false, timestampValue(-2208988801LL, 999999999u), {"c7-0c-ff-3b-9a-c9-ff-ff-ff-ff-ff-7c-55-81-7f"}),
        TestCase("50.timestamp", false, timestampValue(-2208988800LL, 0u), {"c7-0c-ff-00-00-00-00-ff-ff-ff-ff-7c-55-81-80"}),
        TestCase("50.timestamp", false, timestampValue(-62167219200LL, 0u), {"c7-0c-ff-00-00-00-00-ff-ff-ff-f1-86-8b-84-00"}),
        TestCase("50.timestamp", false, timestampValue(253402300799LL, 999999999u), {"c7-0c-ff-3b-9a-c9-ff-00-00-00-3a-ff-f4-41-7f"}),
        TestCase("60.ext", false, extensionValue(static_cast<int8_t>(1), "10"), {"d4-01-10"}),
        TestCase("60.ext", false, extensionValue(static_cast<int8_t>(2), "2021"), {"d5-02-20-21"}),
        TestCase("60.ext", false, extensionValue(static_cast<int8_t>(3), "30313233"), {"d6-03-30-31-32-33"}),
        TestCase("60.ext", false, extensionValue(static_cast<int8_t>(4), "4041424344454647"), {"d7-04-40-41-42-43-44-45-46-47"}),
        TestCase("60.ext", false, extensionValue(static_cast<int8_t>(5), "505152535455565758595a5b5c5d5e5f"), {"d8-05-50-51-52-53-54-55-56-57-58-59-5a-5b-5c-5d-5e-5f"}),
        TestCase("60.ext", false, extensionValue(static_cast<int8_t>(6), ""), {"c7-00-06", "c8-00-00-06", "c9-00-00-00-00-06"}),
        TestCase("60.ext", false, extensionValue(static_cast<int8_t>(7), "707172"), {"c7-03-07-70-71-72", "c8-00-03-07-70-71-72", "c9-00-00-00-03-07-70-71-72"})
    };
    static const std::vector<TestCase> result(cases, cases + sizeof(cases) / sizeof(cases[0]));
    return result;
}

bool isAcceptedEncoding(const TestCase& test_case, const std::string& encoded) {
    for (size_t i = 0; i < test_case.encodings.size(); ++i) {
        if (hexBytes(test_case.encodings[i].c_str()) == encoded) return true;
    }
    return false;
}

bool isInteger(const msgpack_object& object) {
    return object.type == MSGPACK_OBJECT_POSITIVE_INTEGER ||
           object.type == MSGPACK_OBJECT_NEGATIVE_INTEGER;
}

bool integerEquals(const msgpack_object& object, const Value& expected) {
    if (expected.kind == Value::SIGNED_INTEGER) {
        return object.type == MSGPACK_OBJECT_NEGATIVE_INTEGER &&
               object.via.i64 == expected.signed_integer;
    }
    if (expected.kind == Value::UNSIGNED_INTEGER) {
        return object.type == MSGPACK_OBJECT_POSITIVE_INTEGER &&
               object.via.u64 == expected.unsigned_integer;
    }
    return false;
}

double numberAsDouble(const Value& value) {
    if (value.kind == Value::SIGNED_INTEGER) return static_cast<double>(value.signed_integer);
    if (value.kind == Value::UNSIGNED_INTEGER) return static_cast<double>(value.unsigned_integer);
    return value.floating_point;
}

bool numberEqualsForEncoding(const msgpack_object& object, const Value& expected,
                             const std::string& encoded) {
    const unsigned char marker = static_cast<unsigned char>(encoded[0]);
    if (marker == 0xca) {
        const double rounded = static_cast<double>(static_cast<float>(numberAsDouble(expected)));
        return object.type == MSGPACK_OBJECT_FLOAT32 && object.via.f64 == rounded;
    }
    if (marker == 0xcb) {
        return object.type == MSGPACK_OBJECT_FLOAT64 && object.via.f64 == numberAsDouble(expected);
    }
    return integerEquals(object, expected);
}

bool bytesEqual(const char* actual, const std::string& expected) {
    return expected.empty() || std::memcmp(actual, expected.data(), expected.size()) == 0;
}

bool bignumEquals(const msgpack_object& object, const Value& expected) {
    if (isInteger(object)) return integerEquals(object, expected);
    if (object.type != MSGPACK_OBJECT_FLOAT32 && object.type != MSGPACK_OBJECT_FLOAT64) return false;
    const double value = object.via.f64;
    if (!std::isfinite(value) || std::floor(value) != value) return false;
    if (expected.kind == Value::SIGNED_INTEGER) {
        const double lower_limit = -9223372036854775808.0;
        const double upper_limit = 9223372036854775808.0;
        if (value < lower_limit || value >= upper_limit) return false;
        return static_cast<int64_t>(value) == expected.signed_integer;
    }
    if (expected.kind == Value::UNSIGNED_INTEGER) {
        if (value < 0.0 || value >= 18446744073709551616.0) return false;
        return static_cast<uint64_t>(value) == expected.unsigned_integer;
    }
    return false;
}

bool objectEquals(const msgpack_object& object, const Value& expected) {
    switch (expected.kind) {
    case Value::NIL:
        return object.type == MSGPACK_OBJECT_NIL;
    case Value::BOOLEAN:
        return object.type == MSGPACK_OBJECT_BOOLEAN && object.via.boolean == expected.boolean;
    case Value::SIGNED_INTEGER:
    case Value::UNSIGNED_INTEGER:
        return integerEquals(object, expected);
    case Value::FLOAT:
        return (object.type == MSGPACK_OBJECT_FLOAT32 || object.type == MSGPACK_OBJECT_FLOAT64) &&
               object.via.f64 == expected.floating_point;
    case Value::STRING:
        return object.type == MSGPACK_OBJECT_STR && object.via.str.size == expected.bytes.size() &&
               bytesEqual(object.via.str.ptr, expected.bytes);
    case Value::BINARY:
        return object.type == MSGPACK_OBJECT_BIN && object.via.bin.size == expected.bytes.size() &&
               bytesEqual(object.via.bin.ptr, expected.bytes);
    case Value::ARRAY:
        if (object.type != MSGPACK_OBJECT_ARRAY || object.via.array.size != expected.elements.size()) return false;
        for (size_t i = 0; i < expected.elements.size(); ++i) {
            if (!objectEquals(object.via.array.ptr[i], expected.elements[i])) return false;
        }
        return true;
    case Value::MAP:
        if (object.type != MSGPACK_OBJECT_MAP || object.via.map.size != expected.entries.size()) return false;
        for (size_t i = 0; i < expected.entries.size(); ++i) {
            bool found = false;
            for (uint32_t j = 0; j < object.via.map.size; ++j) {
                const msgpack_object_kv& item = object.via.map.ptr[j];
                if (objectEquals(item.key, expected.entries[i].first) &&
                    objectEquals(item.val, expected.entries[i].second)) {
                    found = true;
                    break;
                }
            }
            if (!found) return false;
        }
        return true;
    case Value::EXTENSION:
        return object.type == MSGPACK_OBJECT_EXT && object.via.ext.type == expected.ext_type &&
               object.via.ext.size == expected.bytes.size() &&
               bytesEqual(object.via.ext.ptr, expected.bytes);
    case Value::TIMESTAMP: {
        msgpack_timestamp timestamp;
        return msgpack_object_to_timestamp(&object, &timestamp) &&
               timestamp.tv_sec == expected.seconds && timestamp.tv_nsec == expected.nanoseconds;
    }
    }
    return false;
}

bool packValue(msgpack_packer* packer, const Value& value) {
    switch (value.kind) {
    case Value::NIL:
        return msgpack_pack_nil(packer) == 0;
    case Value::BOOLEAN:
        return (value.boolean ? msgpack_pack_true(packer) : msgpack_pack_false(packer)) == 0;
    case Value::SIGNED_INTEGER:
        return msgpack_pack_int64(packer, value.signed_integer) == 0;
    case Value::UNSIGNED_INTEGER:
        return msgpack_pack_uint64(packer, value.unsigned_integer) == 0;
    case Value::FLOAT:
        return msgpack_pack_double(packer, value.floating_point) == 0;
    case Value::STRING:
        return msgpack_pack_str_with_body(packer, value.bytes.data(),
                                          static_cast<uint32_t>(value.bytes.size())) == 0;
    case Value::BINARY:
        return msgpack_pack_bin_with_body(packer, value.bytes.data(),
                                          static_cast<uint32_t>(value.bytes.size())) == 0;
    case Value::ARRAY:
        if (msgpack_pack_array(packer, static_cast<uint32_t>(value.elements.size())) != 0) return false;
        for (size_t i = 0; i < value.elements.size(); ++i) {
            if (!packValue(packer, value.elements[i])) return false;
        }
        return true;
    case Value::MAP:
        if (msgpack_pack_map(packer, static_cast<uint32_t>(value.entries.size())) != 0) return false;
        for (size_t i = 0; i < value.entries.size(); ++i) {
            if (!packValue(packer, value.entries[i].first) ||
                !packValue(packer, value.entries[i].second)) return false;
        }
        return true;
    case Value::EXTENSION:
        return msgpack_pack_ext_with_body(packer, value.bytes.data(),
                                          static_cast<uint32_t>(value.bytes.size()),
                                          value.ext_type) == 0;
    case Value::TIMESTAMP: {
        msgpack_timestamp timestamp = {value.seconds, value.nanoseconds};
        return msgpack_pack_timestamp(packer, &timestamp) == 0;
    }
    }
    return false;
}

} // namespace

TEST(MSGPACKC, msgpack_test_suite)
{
    const std::vector<TestCase>& cases = msgpackTestCases();
    size_t decode_count = 0;
    size_t encode_count = 0;

    for (size_t i = 0; i < cases.size(); ++i) {
        const TestCase& test_case = cases[i];
        SCOPED_TRACE(test_case.group);
        SCOPED_TRACE(i);

        for (size_t j = 0; j < test_case.encodings.size(); ++j) {
            const std::string encoded = hexBytes(test_case.encodings[j].c_str());
            SCOPED_TRACE(test_case.encodings[j]);
            msgpack_unpacked unpacked;
            msgpack_unpacked_init(&unpacked);
            size_t offset = 0;
            const msgpack_unpack_return result = msgpack_unpack_next(
                &unpacked, encoded.data(), encoded.size(), &offset);
            EXPECT_EQ(MSGPACK_UNPACK_SUCCESS, result);
            EXPECT_EQ(encoded.size(), offset);
            if (result == MSGPACK_UNPACK_SUCCESS && offset == encoded.size()) {
                if (test_case.bignum) {
                    EXPECT_TRUE(bignumEquals(unpacked.data, test_case.value));
                }
                else if (test_case.value.kind == Value::SIGNED_INTEGER ||
                         test_case.value.kind == Value::UNSIGNED_INTEGER ||
                         test_case.value.kind == Value::FLOAT) {
                    EXPECT_TRUE(numberEqualsForEncoding(unpacked.data, test_case.value, encoded));
                }
                else {
                    EXPECT_TRUE(objectEquals(unpacked.data, test_case.value));
                }
            }
            msgpack_unpacked_destroy(&unpacked);
            ++decode_count;
        }

        msgpack_sbuffer buffer;
        msgpack_sbuffer_init(&buffer);
        msgpack_packer packer;
        msgpack_packer_init(&packer, &buffer, msgpack_sbuffer_write);
        EXPECT_TRUE(packValue(&packer, test_case.value));
        const std::string encoded(buffer.data, buffer.size);
        EXPECT_TRUE(isAcceptedEncoding(test_case, encoded));
        msgpack_sbuffer_destroy(&buffer);
        ++encode_count;
    }

    EXPECT_EQ(233u, decode_count);
    EXPECT_EQ(85u, encode_count);
}
