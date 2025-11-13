/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2025 Sky UK
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "firebolt.h"
#include "json_engine.h"
#include "gtest/gtest.h"
#include "json_types/jsondata_accessibility_types.h"

class AccessibilityTest : public ::testing::Test
{
protected:
    void SetUp() override { eventReceived = false; }

    // Create a condition variable and mutex to wait for the events to be received
    std::condition_variable cv;
    std::mutex mtx;
    bool eventReceived;

    JsonEngine jsonEngine;
};

TEST_F(AccessibilityTest, audioDescription)
{
    auto result = Firebolt::IFireboltAccessor::Instance().AccessibilityInterface().audioDescription();
    ASSERT_TRUE(result) << "AccessibilityImpl::audioDescription() returned an error";

    auto expectedValue = jsonEngine.get_value("Accessibility.audioDescription").get<bool>();
    EXPECT_EQ(*result, expectedValue);
}
TEST_F(AccessibilityTest, closedCaptionsSettings)
{
    auto result = Firebolt::IFireboltAccessor::Instance().AccessibilityInterface().closedCaptionsSettings();
    ASSERT_TRUE(result) << "AccessibilityImpl::closedCaptionsSettings() returned an error";

    auto expectedValue = jsonEngine.get_value("Accessibility.closedCaptionsSettings");
    EXPECT_EQ(result->enabled, expectedValue["enabled"].get<bool>());
    EXPECT_EQ(result->preferredLanguages, expectedValue["preferredLanguages"].get<std::vector<std::string>>());
}

TEST_F(AccessibilityTest, subscribeOnAudioDescriptionChanged)
{
    auto subscriptionId = Firebolt::IFireboltAccessor::Instance().AccessibilityInterface()
                              .subscribeOnAudioDescriptionChanged([&](bool value) {
                                  {
                                      std::lock_guard<std::mutex> lock(mtx);
                                      eventReceived = true;
                                  }
                                  cv.notify_one();
                              });

    ASSERT_TRUE(subscriptionId) << "subscribeOnAudioDescriptionChanged returned an error";

    // Wait for the event to be received or timeout after 5 seconds
    std::unique_lock<std::mutex> lock(mtx);
    if (!cv.wait_for(lock, std::chrono::seconds(5), [&] { return eventReceived; })) {
        FAIL() << "Timeout waiting for audioDescription change event";
    }
}   

TEST_F(AccessibilityTest, subscribeOnClosedCaptionsSettingsChanged)
{
    auto subscriptionId = Firebolt::IFireboltAccessor::Instance().AccessibilityInterface()
                              .subscribeOnClosedCaptionsSettingsChanged(
                                  [&](const Firebolt::Accessibility::ClosedCaptionsSettings &settings) {
                                      {
                                          std::lock_guard<std::mutex> lock(mtx);
                                          eventReceived = true;
                                      }
                                      cv.notify_one();
                                  });

    ASSERT_TRUE(subscriptionId) << "subscribeOnClosedCaptionsSettingsChanged returned an error";

    // Wait for the event to be received or timeout after 5 seconds
    std::unique_lock<std::mutex> lock(mtx);
    if (!cv.wait_for(lock, std::chrono::seconds(5), [&] { return eventReceived; })) {
        FAIL() << "Timeout waiting for closedCaptionsSettings change event";
    }
} 

TEST_F(AccessibilityTest, highContrastUI)
{
    auto result = Firebolt::IFireboltAccessor::Instance().AccessibilityInterface().highContrastUI();
    ASSERT_TRUE(result) << "AccessibilityImpl::highContrastUI() returned an error";
    auto expectedValue = jsonEngine.get_value("Accessibility.highContrastUI").get<bool>();
    EXPECT_EQ(*result, expectedValue);
}
TEST_F(AccessibilityTest, subscribeOnHighContrastUIChanged)
{
    auto subscriptionId = Firebolt::IFireboltAccessor::Instance().AccessibilityInterface()
                              .subscribeOnHighContrastUIChanged([&](bool value) {
                                  {
                                      std::lock_guard<std::mutex> lock(mtx);
                                      eventReceived = true;
                                  }
                                  cv.notify_one();
                              });

    ASSERT_TRUE(subscriptionId) << "subscribeOnHighContrastUIChanged returned an error";

    // Wait for the event to be received or timeout after 5 seconds
    std::unique_lock<std::mutex> lock(mtx);
    if (!cv.wait_for(lock, std::chrono::seconds(5), [&] { return eventReceived; })) {
        FAIL() << "Timeout waiting for highContrastUI change event";
    }
}
TEST_F(AccessibilityTest, voiceGuidanceSettings)
{
    auto result = Firebolt::IFireboltAccessor::Instance().AccessibilityInterface().voiceGuidanceSettings();
    ASSERT_TRUE(result) << "AccessibilityImpl::voiceGuidanceSettings() returned an error";

    auto expectedValue = jsonEngine.get_value("Accessibility.voiceGuidanceSettings");
    EXPECT_EQ(result->enabled, expectedValue["enabled"].get<bool>());
    EXPECT_EQ(result->rate, expectedValue["rate"].get<double>());
    EXPECT_EQ(result->navigationHints, expectedValue["navigationHints"].get<bool>());
} 
TEST_F(AccessibilityTest, subscribeOnVoiceGuidanceSettingsChanged)
{
    auto subscriptionId = Firebolt::IFireboltAccessor::Instance().AccessibilityInterface()
                              .subscribeOnVoiceGuidanceSettingsChanged(
                                  [&](const Firebolt::Accessibility::VoiceGuidanceSettings &settings) {
                                      {
                                          std::lock_guard<std::mutex> lock(mtx);
                                          eventReceived = true;
                                      }
                                      cv.notify_one();
                                  });

    ASSERT_TRUE(subscriptionId) << "subscribeOnVoiceGuidanceSettingsChanged returned an error";

    // Wait for the event to be received or timeout after 5 seconds
    std::unique_lock<std::mutex> lock(mtx);
    if (!cv.wait_for(lock, std::chrono::seconds(5), [&] { return eventReceived; })) {
        FAIL() << "Timeout waiting for voiceGuidanceSettings change event";
    }
}  


