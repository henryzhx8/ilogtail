# Copyright 2022 iLogtail Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.22)
project(self_monitor_unittest)

add_executable(alarm_manager_unittest AlarmManagerUnittest.cpp)
target_link_libraries(alarm_manager_unittest ${UT_BASE_TARGET})

add_executable(metric_manager_unittest MetricManagerUnittest.cpp)
target_link_libraries(metric_manager_unittest ${UT_BASE_TARGET})

add_executable(plugin_metric_manager_unittest PluginMetricManagerUnittest.cpp)
target_link_libraries(plugin_metric_manager_unittest ${UT_BASE_TARGET})

add_executable(self_monitor_metric_event_unittest SelfMonitorMetricEventUnittest.cpp)
target_link_libraries(self_monitor_metric_event_unittest ${UT_BASE_TARGET})

include(GoogleTest)
gtest_discover_tests(alarm_manager_unittest)
gtest_discover_tests(metric_manager_unittest)
gtest_discover_tests(plugin_metric_manager_unittest)
gtest_discover_tests(self_monitor_metric_event_unittest)
