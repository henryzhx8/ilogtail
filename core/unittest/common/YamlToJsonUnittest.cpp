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

#include <cstdlib>
#include "unittest/Unittest.h"

#include "common/YamlUtil.h"

namespace logtail {

class YamlToJsonUnittest : public ::testing::Test {
public:
    void SetUp() override {}
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

UNIT_TEST_CASE(YamlToJsonUnittest, TestYamlToJson);
UNIT_TEST_CASE(YamlToJsonUnittest, TestEmptyYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestInvalidYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestNestedYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestDifferentTypesYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestSpecialCharactersYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestComplexNestedYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestMultiLevelListYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestEmptyListAndDictYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestSpecialValuesYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestNestedNonScalarYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestSpecialCharsYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestCommentYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestYamlSpecialMeaningChars);
UNIT_TEST_CASE(YamlToJsonUnittest, TestMultiLineStringYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestDifferentStyleStringsYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestSpecialKeyValueYaml);
UNIT_TEST_CASE(YamlToJsonUnittest, TestSpecialBooleanYaml);

void YamlToJsonUnittest::TestYamlToJson() {
    string yaml = R"(
            a: 1
            b: 2
            c: 3
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asInt(), 1);
    APSARA_TEST_EQUAL_FATAL(json["b"].asInt(), 2);
    APSARA_TEST_EQUAL_FATAL(json["c"].asInt(), 3);

}

void YamlToJsonUnittest::TestEmptyYaml() {
    string yaml = "";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
}

void YamlToJsonUnittest::TestInvalidYaml() {
    string yaml = R"(
            a: 1
            b: 2
            c: 3
            d: e: f
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    EXPECT_FALSE(ret);
    APSARA_TEST_EQUAL_FATAL(errorMsg, "parse yaml failed: yaml-cpp: error at line 5, column 17: illegal map value"); // 你期望的错误信息
}

void YamlToJsonUnittest::TestNestedYaml() {
    string yaml = R"(
            a: 
                b: 2
                c: 3
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"]["b"].asInt(), 2);
    APSARA_TEST_EQUAL_FATAL(json["a"]["c"].asInt(), 3);
}

void YamlToJsonUnittest::TestDifferentTypesYaml() {
    string yaml = R"(
            a: 1
            b: 2.2
            c: "3"
            d: true
            e: [1, 2, 3]
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
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

void YamlToJsonUnittest::TestSpecialCharactersYaml() {
    string yaml = R"(
            a: "1\n2"
            b: "3\t4"
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "1\n2");
    APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "3\t4");
}

void YamlToJsonUnittest::TestComplexNestedYaml() {
    string yaml = R"(
            a: 
                b: 
                    c: 3
                    d: 4
                e: 5
            f:
                g: 6
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"]["b"]["c"].asInt(), 3);
    APSARA_TEST_EQUAL_FATAL(json["a"]["b"]["d"].asInt(), 4);
    APSARA_TEST_EQUAL_FATAL(json["a"]["e"].asInt(), 5);
    APSARA_TEST_EQUAL_FATAL(json["f"]["g"].asInt(), 6);
}

void YamlToJsonUnittest::TestMultiLevelListYaml() {
    string yaml = R"(
            - a
            - b
            - 
                - c
                - d
            - e
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json[0].asString(), "a");
    APSARA_TEST_EQUAL_FATAL(json[1].asString(), "b");
    APSARA_TEST_EQUAL_FATAL(json[2][0].asString(), "c");
    APSARA_TEST_EQUAL_FATAL(json[2][1].asString(), "d");
    APSARA_TEST_EQUAL_FATAL(json[3].asString(), "e");
}

void YamlToJsonUnittest::TestEmptyListAndDictYaml() {
    string yaml = R"(
            a: []
            b: {}
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_TRUE_FATAL(json["a"].empty());
    APSARA_TEST_TRUE_FATAL(json["a"].isArray());
    APSARA_TEST_TRUE_FATAL(json["b"].empty());
    APSARA_TEST_TRUE_FATAL(json["b"].isObject());
}

void YamlToJsonUnittest::TestSpecialValuesYaml() {
    string yaml = R"(
            a: null
            b: .NaN
            c: .inf
            d: -.inf
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_TRUE_FATAL(json["a"].isNull());
    APSARA_TEST_TRUE_FATAL(std::isnan(json["b"].asDouble()));
    APSARA_TEST_TRUE_FATAL(std::isinf(json["c"].asDouble()) && json["c"].asDouble() > 0);
    APSARA_TEST_TRUE_FATAL(std::isinf(json["d"].asDouble()) && json["d"].asDouble() < 0);
}

void YamlToJsonUnittest::TestNestedNonScalarYaml() {
    string yaml = R"(
            a:
                - b
                - c:
                    - d
                    - e
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"][0].asString(), "b");
    APSARA_TEST_EQUAL_FATAL(json["a"][1]["c"][0].asString(), "d");
    APSARA_TEST_EQUAL_FATAL(json["a"][1]["c"][1].asString(), "e");
}

void YamlToJsonUnittest::TestSpecialCharsYaml() {
    string yaml = R"(
            a: "\t\n\r\b\f"
            b: "\"\'\\"
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "\t\n\r\b\f");
    APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "\"\'\\");
}

void YamlToJsonUnittest::TestCommentYaml() {
    string yaml = R"(
            a: 1 # this is a comment
            b: 2
            # this is another comment
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asInt(), 1);
    APSARA_TEST_EQUAL_FATAL(json["b"].asInt(), 2);
}

void YamlToJsonUnittest::TestYamlSpecialMeaningChars() {
    string yaml = R"(
            a: "---"
            b: "..."
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "---");
    APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "...");
}

void YamlToJsonUnittest::TestMultiLineStringYaml() {
    string yaml = R"(
            a: |
                This is a
                multi-line string.
            b: >
                This is another
                multi-line string.
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "This is a\nmulti-line string.\n");
    APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "This is another multi-line string.\n");
}

void YamlToJsonUnittest::TestDifferentStyleStringsYaml() {
    string yaml = R"(
            a: bare string
            b: 'single quoted string'
            c: "double quoted string"
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asString(), "bare string");
    APSARA_TEST_EQUAL_FATAL(json["b"].asString(), "single quoted string");
    APSARA_TEST_EQUAL_FATAL(json["c"].asString(), "double quoted string");
}

void YamlToJsonUnittest::TestSpecialKeyValueYaml() {
    string yaml = R"(
            "a:b": "c,d"
            "[e]": "{f}"
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a:b"].asString(), "c,d");
    APSARA_TEST_EQUAL_FATAL(json["[e]"].asString(), "{f}");
}

void YamlToJsonUnittest::TestSpecialBooleanYaml() {
    string yaml = R"(
            a: yes
            b: no
            c: on
            d: off
        )";
    Json::Value json;
    string errorMsg;
    YAML::Node yamlRoot;
    bool ret = ParseYamlConfig(yaml, yamlRoot, errorMsg);
    if (ret) {
        ret = ParseYamlToJson(yamlRoot, json, errorMsg);
    }
    APSARA_TEST_TRUE_FATAL(ret);
    APSARA_TEST_EQUAL_FATAL(json["a"].asBool(), true);
    APSARA_TEST_EQUAL_FATAL(json["b"].asBool(), false);
    APSARA_TEST_EQUAL_FATAL(json["c"].asBool(), true);
    APSARA_TEST_EQUAL_FATAL(json["d"].asBool(), false);
}

} // namespace logtail

int main(int argc, char** argv) {
    logtail::Logger::Instance().InitGlobalLoggers();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}