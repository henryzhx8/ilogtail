/*
 * Copyright 2023 iLogtail Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <string>
#include <utility>

#include "json/json.h"
#include "boost/regex.hpp"

#include "pipeline/PipelineContext.h"

namespace logtail {

class MultilineOptions {
public:
    enum class Mode { CUSTOM, JSON };

    bool Init(const Json::Value& config, const PipelineContext& ctx, const std::string& pluginName);
    const std::shared_ptr<boost::regex>& GetStartPatternReg() const { return mStartPatternRegPtr; }
    const std::shared_ptr<boost::regex>& GetContinuePatternReg() const { return mContinuePatternRegPtr; }
    const std::shared_ptr<boost::regex>& GetEndPatternReg() const { return mEndPatternRegPtr; }
    bool IsMultiline() const { return mIsMultiline; }

    std::shared_ptr<boost::regex> mStartPatternRegPtr;
    std::shared_ptr<boost::regex> mContinuePatternRegPtr;
    std::shared_ptr<boost::regex> mEndPatternRegPtr;
    Mode mMode = Mode::CUSTOM;
    std::string mStartPattern;
    std::string mContinuePattern;
    std::string mEndPattern;

private:
    bool ParseRegex(const std::string& pattern, std::shared_ptr<boost::regex>& reg);

    bool mIsMultiline = false;
};

using MultilineConfig = std::pair<const MultilineOptions*, const PipelineContext*>;

} // namespace logtail
