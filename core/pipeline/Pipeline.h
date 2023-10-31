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

#include <string>
#include <vector>
#include <memory>

#include "json/json.h"

#include "config/NewConfig.h"
#include "models/PipelineEventGroup.h"
#include "pipeline/PipelineContext.h"
#include "plugin/instance/InputInstance.h"
#include "plugin/instance/FlusherInstance.h"
#include "plugin/instance/ProcessorInstance.h"


namespace logtail {

class Pipeline {
public:
    bool Init(NewConfig&& config);
    void Start();
    void Process(PipelineEventGroup&& logGroup, std::vector<PipelineEventGroup>& logGroupList);
    void Stop(bool isRemoving);

    const std::string& Name() const { return mName; }
    const PipelineContext& GetContext() const { return mContext; }
    const Json::Value& GetConfig() const { return mConfig; }
    const std::vector<std::unique_ptr<FlusherInstance>>& GetFlushers() const { return mFlushers; }
    bool IsFlushingThroughGoPipeline() const { return !mGoPipelineWithoutInput.isNull(); }
    bool LoadGoPipelines() const; // 应当放在private，过渡期间放在public

    // 临时使用
    const Json::Value& GetGoPipelineWithInput() const { return mGoPipelineWithInput; }
    const Json::Value& GetGoPipelineWithoutInput() const { return mGoPipelineWithoutInput; }
    // only for input_observer_network for compatability
    const std::vector<std::unique_ptr<InputInstance>>& GetInputs() const { return mInputs; }

private:
    void MergeGoPipeline(const Json::Value& src, Json::Value& dst);
    void AddPluginToGoPipeline(const Json::Value& plugin, const std::string& module, Json::Value& dst);
    bool ShouldAddPluginToGoPipelineWithInput() const { return mInputs.empty() && mProcessorLine.empty(); }

    std::string mName;
    std::vector<std::unique_ptr<InputInstance>> mInputs;
    std::vector<std::unique_ptr<ProcessorInstance>> mProcessorLine;
    std::vector<std::unique_ptr<FlusherInstance>> mFlushers;
    Json::Value mGoPipelineWithInput;
    Json::Value mGoPipelineWithoutInput;
    PipelineContext mContext;
    Json::Value mConfig;
};

} // namespace logtail
