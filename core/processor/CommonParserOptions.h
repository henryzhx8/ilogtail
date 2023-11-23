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

    bool ShouldAddUnmatchLog(bool parseSuccess) {
        return !parseSuccess && mKeepingSourceWhenParseFail && mCopingRawLog;
    }

    // Parsing successful and original logs are retained or parsing failed and original logs are retained.
    bool ShouldAddRenamedSourceLog(bool parseSuccess, const std::string& sourceKey) {
        return (((parseSuccess && mKeepingSourceWhenParseSucceed) || (!parseSuccess && mKeepingSourceWhenParseFail))
                && sourceKey != mRenamedSourceKey);
    }
    // Parsing successful but original logs are not retained or parsing failed but original logs are not retained.
    bool ShouldAddEarseSourceLog(bool parseSuccess) {
        return (((parseSuccess && !mKeepingSourceWhenParseSucceed) || (!parseSuccess && !mKeepingSourceWhenParseFail)));
    }

    bool ShouldEraseEvent(bool parseSuccess, const LogEvent& sourceEvent) {
        if (!parseSuccess && !mKeepingSourceWhenParseFail) {
            const auto& contents = sourceEvent.GetContents();
            if (contents.empty()) {
                return true;
            }
#ifdef APSARA_UNIT_TEST_MAIN
            // "__file_offset__"  or "log.file.offset"
            if (contents.size() == 1
                && (contents.begin()->first == LOG_RESERVED_KEY_FILE_OFFSET
                    || contents.begin()->first == EVENT_GROUP_META_LOG_FILE_OFFSET)) {
                return true;
            }
#else
            // "__file_offset__"
            if (contents.size() == 1 && (contents.begin()->first == LOG_RESERVED_KEY_FILE_OFFSET)) {
                return true;
            }
#endif
        }
        return false;
    }
};

} // namespace logtail
