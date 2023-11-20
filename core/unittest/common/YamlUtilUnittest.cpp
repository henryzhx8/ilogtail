// Copyright 2023 iLogtail Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "common/YamlUtil.h"
#include "unittest/Unittest.h"
#include <string>

namespace logtail {

class YamlUtilUnittest : public ::testing::Test {
public:
    void TestYamlToJson();
    void TestEmptyYaml();
    void TestInvalidYaml();
    void TestNestedYaml();
    void TestDifferentTypesYaml();
    void TestSpecialCharactersYaml();
    void TestComplexNestedYaml();
    void TestMultiLevelListYaml();
    void TestEmptyListAndDictYaml();
    void TestSpecialValuesYaml();
    void TestNestedNonScalarYaml();
    void TestSpecialCharsYaml();
    void TestCommentYaml();
    void TestYamlSpecialMeaningChars();
    void TestMultiLineStringYaml();
    void TestDifferentStyleStringsYaml();
    void TestSpecialKeyValueYaml();
    void TestSpecialBooleanYaml();
};

void YamlUtilUnittest::TestYamlToJson() {
    std::string yaml = R"(
            a: 1
            b: 2
            c: 3
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asInt(), 1);
    APSARA_TEST_EQUAL_FATAL(json["b"].asInt(), 2);
    APSARA_TEST_EQUAL_FATAL(json["c"].asInt(), 3);
}

void YamlUtilUnittest::TestEmptyYaml() {
    std::string yaml = "";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_TRUE_FATAL(json.size() == 0);
}

void YamlUtilUnittest::TestInvalidYaml() {
    std::string yaml = R"(
            a: 1
            b: 2
            c: 3
            d: e: f
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    EXPECT_FALSE(ret);
    APSARA_TEST_EQUAL_FATAL(errorMsg, "parse yaml failed: yaml-cpp: error at line 5, column 17: illegal map value");
}

void YamlUtilUnittest::TestNestedYaml() {
    std::string yaml = R"(
            a: 
                b: 2
                c: 3
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"]["b"].asInt(), 2);
    APSARA_TEST_EQUAL_FATAL(json["a"]["c"].asInt(), 3);
}

void YamlUtilUnittest::TestDifferentTypesYaml() {
    std::string yaml = R"(
            a: 1
            b: 2.2
            c: "3"
            d: true
            e: [1, 2, 3]
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asInt(), 1);
    APSARA_TEST_EQUAL_FATAL(json["b"].asDouble(), 2.2);
    APSARA_TEST_EQUAL_FATAL(json["c"].asString(), "3");
    APSARA_TEST_EQUAL_FATAL(json["d"].asBool(), true);
    APSARA_TEST_TRUE_FATAL(json["e"].isArray());
    APSARA_TEST_EQUAL_FATAL(json["e"][0].asInt(), 1);
    APSARA_TEST_EQUAL_FATAL(json["e"][1].asInt(), 2);
    APSARA_TEST_EQUAL_FATAL(json["e"][2].asInt(), 3);
}

void YamlUtilUnittest::TestSpecialCharactersYaml() {
    {
        std::string yaml = R"(
            a: "1\n2"
            b: "3\t4"
            c: "\u0001"
        )";
        Json::Value json;
        std::string errorMsg;
        YAML::Node yamlRoot;
        bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
        if (ret) {
            json = CovertYamlToJson(yamlRoot);
        }
        APSARA_TEST_TRUE_FATAL(ret);
        APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "1\n2");
        APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "3\t4");
        APSARA_TEST_EQUAL_FATAL(json["c"].asString(), "\u0001");
    }
    {
        std::string yaml = R"(
            a: 1\n2
            b: 3\t4
            c: \u0001
        )";
        Json::Value json;
        std::string errorMsg;
        YAML::Node yamlRoot;
        bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
        if (ret) {
            json = CovertYamlToJson(yamlRoot);
        }
        APSARA_TEST_TRUE_FATAL(ret);
        APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "1\\n2");
        APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "3\\t4");
        APSARA_TEST_EQUAL_FATAL(json["c"].asString(), "\\u0001");
    }
    {
        std::string yaml = R"(
            a: '1\n2'
            b: '3\t4'
            c: '\u0001'
        )";
        Json::Value json;
        std::string errorMsg;
        YAML::Node yamlRoot;
        bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
        if (ret) {
            json = CovertYamlToJson(yamlRoot);
        }
        APSARA_TEST_TRUE_FATAL(ret);
        APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "1\\n2");
        APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "3\\t4");
        APSARA_TEST_EQUAL_FATAL(json["c"].asString(), "\\u0001");
    }
}

void YamlUtilUnittest::TestComplexNestedYaml() {
    std::string yaml = R"(
            a: 
                b: 
                    c: 3
                    d: 4
                e: 5
            f:
                g: 6
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"]["b"]["c"].asInt(), 3);
    APSARA_TEST_EQUAL_FATAL(json["a"]["b"]["d"].asInt(), 4);
    APSARA_TEST_EQUAL_FATAL(json["a"]["e"].asInt(), 5);
    APSARA_TEST_EQUAL_FATAL(json["f"]["g"].asInt(), 6);
}

void YamlUtilUnittest::TestMultiLevelListYaml() {
    std::string yaml = R"(
            - a
            - b
            - 
                - c
                - d
            - e
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json[0].asString(), "a");
    APSARA_TEST_EQUAL_FATAL(json[1].asString(), "b");
    APSARA_TEST_EQUAL_FATAL(json[2][0].asString(), "c");
    APSARA_TEST_EQUAL_FATAL(json[2][1].asString(), "d");
    APSARA_TEST_EQUAL_FATAL(json[3].asString(), "e");
}

void YamlUtilUnittest::TestEmptyListAndDictYaml() {
    std::string yaml = R"(
            a: []
            b: {}
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_TRUE_FATAL(json["a"].empty());
    APSARA_TEST_TRUE_FATAL(json["a"].isArray());
    APSARA_TEST_TRUE_FATAL(json["b"].empty());
    APSARA_TEST_TRUE_FATAL(json["b"].isObject());
}

void YamlUtilUnittest::TestSpecialValuesYaml() {
    std::string yaml = R"(
            a: null
            b: .NaN
            c: .inf
            d: -.inf
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_TRUE_FATAL(json["a"].isNull());
    APSARA_TEST_TRUE_FATAL(std::isnan(json["b"].asDouble()));
    APSARA_TEST_TRUE_FATAL(std::isinf(json["c"].asDouble()) && json["c"].asDouble() > 0);
    APSARA_TEST_TRUE_FATAL(std::isinf(json["d"].asDouble()) && json["d"].asDouble() < 0);
}

void YamlUtilUnittest::TestNestedNonScalarYaml() {
    std::string yaml = R"(
            a:
                - b
                - c:
                    - d
                    - e
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"][0].asString(), "b");
    APSARA_TEST_EQUAL_FATAL(json["a"][1]["c"][0].asString(), "d");
    APSARA_TEST_EQUAL_FATAL(json["a"][1]["c"][1].asString(), "e");
}

void YamlUtilUnittest::TestSpecialCharsYaml() {
    std::string yaml = R"(
            a: "\t\n\r\b\f"
            b: "\"\'\\"
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "\t\n\r\b\f");
    APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "\"\'\\");
}

void YamlUtilUnittest::TestCommentYaml() {
    std::string yaml = R"(
            a: 1 # this is a comment
            b: 2
            # this is another comment
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asInt(), 1);
    APSARA_TEST_EQUAL_FATAL(json["b"].asInt(), 2);
}

void YamlUtilUnittest::TestYamlSpecialMeaningChars() {
    std::string yaml = R"(
            a: "---"
            b: "..."
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "---");
    APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "...");
}

void YamlUtilUnittest::TestMultiLineStringYaml() {
    std::string yaml = R"(
            a: |
                This is a
                multi-line std::string.
            b: >
                This is another
                multi-line std::string.
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "This is a\nmulti-line std::string.\n");
    APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "This is another multi-line std::string.\n");
}

void YamlUtilUnittest::TestDifferentStyleStringsYaml() {
    std::string yaml = R"(
            a: bare std::string
            b: 'single quoted std::string'
            c: "double quoted std::string"
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "bare std::string");
    APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "single quoted std::string");
    APSARA_TEST_EQUAL_FATAL(json["c"].asString(), "double quoted std::string");
}

void YamlUtilUnittest::TestSpecialKeyValueYaml() {
    std::string yaml = R"(
            "a:b": "c,d"
            "[e]": "{f}"
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a:b"].asString(), "c,d");
    APSARA_TEST_EQUAL_FATAL(json["[e]"].asString(), "{f}");
}

void YamlUtilUnittest::TestSpecialBooleanYaml() {
    std::string yaml = R"(
            a: yes
            b: no
            c: on
            d: off
        )";
    Json::Value json;
    std::string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        json = CovertYamlToJson(yamlRoot);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asBool(), true);
    APSARA_TEST_EQUAL_FATAL(json["b"].asBool(), false);
    APSARA_TEST_EQUAL_FATAL(json["c"].asBool(), true);
    APSARA_TEST_EQUAL_FATAL(json["d"].asBool(), false);
}

UNIT_TEST_CASE(YamlUtilUnittest, TestYamlToJson);
UNIT_TEST_CASE(YamlUtilUnittest, TestEmptyYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestInvalidYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestNestedYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestDifferentTypesYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestSpecialCharactersYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestComplexNestedYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestMultiLevelListYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestEmptyListAndDictYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestSpecialValuesYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestNestedNonScalarYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestSpecialCharsYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestCommentYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestYamlSpecialMeaningChars);
UNIT_TEST_CASE(YamlUtilUnittest, TestMultiLineStringYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestDifferentStyleStringsYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestSpecialKeyValueYaml);
UNIT_TEST_CASE(YamlUtilUnittest, TestSpecialBooleanYaml);
} // namespace logtail

UNIT_TEST_MAIN
