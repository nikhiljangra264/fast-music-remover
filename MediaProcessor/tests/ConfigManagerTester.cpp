#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "../src/ConfigManager.h"
#include "TestUtils.h"

namespace fs = std::filesystem;
namespace MediaProcessor::Tests {

// ClassName_MethodName_StateUnderTest_ExpectedBehavior gtest std naming convention
TEST(ConfigManagerTest, ConfigManager_loadConfig_ValidConfigFile_Success) {
    // config file with valid JSON
    auto testConfigFile = TestUtils::TestConfigFile();
    testConfigFile.changeConfigOptions("use_thread_cap", true, "max_threads_if_capped", 1);

    auto configManager = ConfigManager::getInstance();
    bool loadSuccess = configManager.loadConfig(testConfigFile.getFilePath());

    EXPECT_TRUE(loadSuccess);
    EXPECT_EQ(configManager.getDeepFilterPath(),
              testConfigFile.getConfigValue<fs::path>("deep_filter_path"));
    EXPECT_EQ(configManager.getDeepFilterTarballPath(),
              testConfigFile.getConfigValue<fs::path>("deep_filter_tarball_path"));
    EXPECT_EQ(configManager.getDeepFilterEncoderPath(),
              testConfigFile.getConfigValue<fs::path>("deep_filter_encoder_path"));
    EXPECT_EQ(configManager.getDeepFilterDecoderPath(),
              testConfigFile.getConfigValue<fs::path>("deep_filter_decoder_path"));
    EXPECT_EQ(configManager.getFFmpegPath(),
              testConfigFile.getConfigValue<fs::path>("ffmpeg_path"));
    EXPECT_EQ(configManager.getOptimalThreadCount(),
              testConfigFile.getConfigValue<unsigned int>("max_threads_if_capped"));
    EXPECT_EQ(configManager.getFilterAttenuationLimit(),
              testConfigFile.getConfigValue<unsigned int>("filter_attenuation_limit"));
}

TEST(ConfigManagerTest, ConfigManager_loadConfig_InvalidConfigFile_ThrowsRuntimeError) {
    fs::path invalidConfigPath = "invalid_config.json";

    // invalid config file for testing
    std::ofstream(invalidConfigPath) << "not a json";
    auto configManager = ConfigManager::getInstance();
    EXPECT_THROW(configManager.loadConfig(invalidConfigPath), std::runtime_error);
}

TEST(ConfigManagerTest, ConfigManager_loadConfig_InvalidConfigOptions_ThrowsRuntimeError) {
    // config file with valid JSON but with invalid options
    auto testConfigFile = TestUtils::TestConfigFile();
    testConfigFile.changeConfigOptions("use_thread_cap", "true", "max_threads_if_capped", -1,
                                       "deep_filter_path", false, "deep_filter_tarball_path", 1.0,
                                       "deep_filter_encoder_path", -1, "deep_filter_decoder_path",
                                       true, "ffmpeg_path", true, "downloads_path", 0,
                                       "uploads_path", false, "filter_attenuation_limit", "100");

    auto configManager = ConfigManager::getInstance();
    bool loadSuccess = configManager.loadConfig(testConfigFile.getFilePath());

    EXPECT_TRUE(loadSuccess);
    EXPECT_THROW(configManager.getDeepFilterPath(), std::runtime_error);
    EXPECT_THROW(configManager.getDeepFilterTarballPath(), std::runtime_error);
    EXPECT_THROW(configManager.getDeepFilterEncoderPath(), std::runtime_error);
    EXPECT_THROW(configManager.getDeepFilterDecoderPath(), std::runtime_error);
    EXPECT_THROW(configManager.getFFmpegPath(), std::runtime_error);
    EXPECT_THROW(configManager.getOptimalThreadCount(), std::runtime_error);
    EXPECT_THROW(configManager.getFilterAttenuationLimit(), std::runtime_error);
}

}  // namespace MediaProcessor::Tests