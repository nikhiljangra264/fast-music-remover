#include <gtest/gtest.h>

#include "../src/CommandBuilder.h"
#include "../src/DeepFilterCommandBuilder.h"

namespace MediaProcessor::Testing {

// ClassName_MethodName_StateUnderTest_ExpectedBehavior gtest std naming convention
TEST(CommandBuildingTest, CommandBuilder_build_Constructs_Success) {
    CommandBuilder builder;
    builder.addArgument("arg1");
    builder.addFlag("flag");
    builder.addArgument("arg2");
    builder.addFlag("flag2", "value");
    std::string command = builder.build();
    EXPECT_EQ(command, "arg1 flag arg2 flag2 value");
}

TEST(CommandBuildingTest, DeepFilterCommandBuilder_build_WithoutRequiredParameters_ThrowsRuntimeError) {
    DeepFilterCommandBuilder builder;
    EXPECT_THROW(builder.build(), std::runtime_error);
}

TEST(CommandBuildingTest, DeepFilterCommandBuilder_setNoiseReductionLevel_InvalidInput_ThrowsInvalidArgument) {
    DeepFilterCommandBuilder builder;
    EXPECT_THROW(builder.setNoiseReductionLevel(1.00001), std::invalid_argument);
    EXPECT_THROW(builder.setNoiseReductionLevel(-0.1), std::invalid_argument);
}

TEST(CommandBuildingTest, DeepFilterCommandBuilder_build_InSequence_Success) {
    DeepFilterCommandBuilder builder;
    builder.setInputFile("input.wav")
           .setOutputFile("output.wav")
           .setNoiseReductionLevel(0.8)
           .enableDelayCompensation();
    EXPECT_EQ(builder.build(), "input.wav --output-dir output.wav --noise-reduction 0.800000 --compensate-delay");
}

/// TODO: DeepFilterCommandBuilder_build_RandomSequence_Success

}  // namespace MediaProcessor::Testing