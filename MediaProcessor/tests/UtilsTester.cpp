#include <gtest/gtest.h>

#include <filesystem>

#include "../src/Utils.h"

namespace fs = std::filesystem;
namespace MediaProcessor::Tests {

TEST(UtilsTester, checkPreparedOutputPaths) {
    fs::path videoPath = "/Tests/Video.mp4";
    fs::path expectedVocalsPath = "/Tests/Video_isolated_audio.wav",
             expectedProcessedVideoPath = "/Tests/Video_processed_video.mp4";

    auto [outputVocalsPath, outputProcessedVideoPath] = Utils::prepareOutputPaths(videoPath);
    EXPECT_EQ(expectedVocalsPath, outputVocalsPath);
    EXPECT_EQ(expectedProcessedVideoPath, outputProcessedVideoPath);
}

TEST(UtilsTester, EnsureDirectoryExists) {
    fs::path tempPath = fs::temp_directory_path() / "test_dir";

    EXPECT_FALSE(fs::exists(tempPath));

    EXPECT_TRUE(Utils::ensureDirectoryExists(tempPath));
    EXPECT_TRUE(fs::exists(tempPath));

    EXPECT_FALSE(Utils::ensureDirectoryExists(tempPath));

    fs::remove_all(tempPath);
}

TEST(UtilsTester, trimTrailingSpaces_Success) {
    std::string inputWithTrailingSpace = "Hello, World! ";
    std::string inputWithoutTrailingSpace = "Hello, World!";

    EXPECT_EQ(inputWithoutTrailingSpace, Utils::trimTrailingSpace(inputWithTrailingSpace));
    EXPECT_EQ(inputWithoutTrailingSpace, Utils::trimTrailingSpace(inputWithoutTrailingSpace));
}

TEST(UtilsTester, runCommand_Success) {
    std::string command = "echo \"Hello\"";
    EXPECT_TRUE(Utils::runCommand(command));

    auto output = Utils::runCommand(command, true);
    EXPECT_TRUE(output.has_value());
    EXPECT_EQ(*output, "Hello\n");
}

TEST(UtilsTester, runCommand_Fail) {
    std::string invalid_command = "invalid_command\nanything/@&h";
    EXPECT_FALSE(Utils::runCommand(invalid_command));

    auto output = Utils::runCommand(invalid_command, true);
    EXPECT_FALSE(output.has_value());
}

TEST(UtilsTester, getMediaDuration_validFile_Success) {
    fs::path testMediaPath(TEST_MEDIA_DIR);
    testMediaPath.make_preferred();
    fs::path audioPath = testMediaPath / "test_audio_processed.wav";
    // precomupted length of test_audio_processed.wav
    EXPECT_EQ(Utils::getMediaDuration(audioPath), 1.9635);
}

TEST(UtilsTester, getMediaDuration_invalidFile_Fail) {
    fs::path invalidAudioPath("This path doesn't exist");
    EXPECT_EQ(Utils::getMediaDuration(invalidAudioPath), -1);
}

}  // namespace MediaProcessor::Tests
