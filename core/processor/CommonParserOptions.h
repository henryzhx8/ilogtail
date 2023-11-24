/*
 * Copyright 2023 iLogtail Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <json/json.h>

#include <string>

#include "common/Constants.h"
#include "pipeline/PipelineContext.h"
#ifdef APSARA_UNIT_TEST_MAIN
#include "models/PipelineEventGroup.h"
#endif
namespace logtail {
struct CommonParserOptions {
    bool Init(const Json::Value& config, const PipelineContext& ctx, const std::string& pluginName);
    bool mKeepingSourceWhenParseFail = false;
    bool mKeepingSourceWhenParseSucceed = false;
    std::string mRenamedSourceKey;
    bool mCopingRawLog = false;
    const std::string UNMATCH_LOG_KEY = "__raw_log__";

    bool ShouldAddUnmatchLog(bool parseSuccess);
    // Parsing successful and original logs are retained or parsing failed and original logs are retained.
    bool ShouldAddRenamedSourceLog(bool parseSuccess);
    bool ShouldEraseEvent(bool parseSuccess, const LogEvent& sourceEvent);
};

} // namespace logtail
