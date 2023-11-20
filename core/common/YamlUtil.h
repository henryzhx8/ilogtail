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

#pragma once

#include "json/json.h"
#include "yaml-cpp/yaml.h"

namespace logtail {

bool ParseYamlConfig(const std::string& config, YAML::Node& yamlRoot, std::string& errorMsg);
bool CovertYamlToJson(const YAML::Node& yamlRoot, Json::Value& res, std::string& errorMsg);
Json::Value CovertYamlToJson(const YAML::Node& rootNode);
Json::Value parseScalar(const YAML::Node& node);

} // namespace logtail
