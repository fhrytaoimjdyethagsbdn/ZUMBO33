cmake_minimum_required(VERSION 3.22)
project(ZUMBO VERSION 3.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(JUCE_PATH "" CACHE PATH "JUCE 8.x checkout")
if(NOT EXISTS "${JUCE_PATH}/CMakeLists.txt")
  message(FATAL_ERROR "Use -DJUCE_PATH=/path/to/JUCE")
endif()
add_subdirectory(${JUCE_PATH} JUCE)

juce_add_plugin(ZUMBO
  COMPANY_NAME "ZUMBO Audio"
  COMPANY_WEBSITE "https://github.com/yourname/ZUMBO"
  BUNDLE_ID "com.zumbo.audio.zumbo"
  PLUGIN_MANUFACTURER_CODE Zmbo
  PLUGIN_CODE Zm03
  FORMATS VST3
  PRODUCT_NAME "ZUMBO"
  DESCRIPTION "ZUMBO psychedelic granular synthesizer"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT FALSE
  IS_MIDI_EFFECT FALSE
  COPY_PLUGIN_AFTER_BUILD TRUE
)

target_sources(ZUMBO PRIVATE
 Source/PluginProcessor.cpp
 Source/PluginEditor.cpp
 Source/ZumboDSP.h
 Source/ZumboDSP.cpp
 Source/ZumboParameters.h
 Source/ZumboLookAndFeel.h
)

target_compile_definitions(ZUMBO PRIVATE
 JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(ZUMBO PRIVATE
 juce::juce_audio_utils juce::juce_dsp juce::juce_gui_extra
)

if(MSVC)
 target_compile_options(ZUMBO PRIVATE /O2 /fp:fast)
else()
 target_compile_options(ZUMBO PRIVATE -O3 -ffast-math)
endif()

option(ZUMBO_BUILD_TESTS "Build DSP tests" ON)
if(ZUMBO_BUILD_TESTS)
 enable_testing()
 add_executable(ZUMBO_DSP_TESTS Tests/DSPTests.cpp Source/ZumboDSP.cpp)
 target_include_directories(ZUMBO_DSP_TESTS PRIVATE Source)
 target_link_libraries(ZUMBO_DSP_TESTS PRIVATE juce::juce_dsp juce::juce_audio_basics juce::juce_core)
 add_test(NAME ZUMBO_DSP_TESTS COMMAND ZUMBO_DSP_TESTS)
endif()
