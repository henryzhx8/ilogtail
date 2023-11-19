// Copyright 2023 iLogtail Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "common/YamlUtil.h"

#include "common/ExceptionBase.h"
#include "common/StringTools.h"
#include "logger/Logger.h"

using namespace std;

namespace logtail {

bool ParseYamlConfig(const string& config, Json::Value& res, string& errorMsg) {
    try {
        YAML::Node yamlRoot = YAML::Load(config); // 加载YAML配置
        res = ChangeYamlToJson(yamlRoot); // 将YAML转换为JSON
        return true;
    } catch (const YAML::ParserException& e) {
        errorMsg = "parse yaml failed: " + string(e.what());
        return false;
    } catch (const exception& e) {
        errorMsg = e.what();
        return false;
    } catch (...) {
        return false;
    }
}

Json::Value ParseScalar(const YAML::Node& node) {
    // yaml-cpp automatically discards quotes in quoted values,
    // so to differentiate strings and integer for purely-digits value,
    // we can tell from node.Tag(), which will return "!" for quoted values and "?" for others
    if (node.Tag() == "!") {
        return node.as<string>();
    }

    int i;
    if (YAML::convert<int>::decode(node, i))
        return i;

    double d;
    if (YAML::convert<double>::decode(node, d))
        return d;

    bool b;
    if (YAML::convert<bool>::decode(node, b))
        return b;

    string s;
    if (YAML::convert<string>::decode(node, s))
        return s;

    return Json::Value();
}

Json::Value ChangeYamlToJson(const YAML::Node& rootNode) {
    Json::Value resultJson;

    switch (rootNode.Type()) {
        case YAML::NodeType::Null:
            return Json::Value();

        case YAML::NodeType::Scalar:
            return ParseScalar(rootNode);

        case YAML::NodeType::Sequence: {
            resultJson = Json::Value(Json::arrayValue);  // 明确地创建一个空数组
            int i = 0;
            for (auto&& node : rootNode) {
                resultJson[i] = ChangeYamlToJson(node);
                i++;
            }
            break;
        }

        case YAML::NodeType::Map: {
            resultJson = Json::Value(Json::objectValue);  // 明确地创建一个空对象
            for (auto&& it : rootNode) {
                resultJson[it.first.as<string>()] = ChangeYamlToJson(it.second);
            }
            break;
        }

        default:
            return Json::Value();
    }

    return resultJson;
}
} // namespace logtail
