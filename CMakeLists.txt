cmake_minimum_required(VERSION 3.22)

# Ορισμός του project
project(ZUMBO VERSION 3.0.0 LANGUAGES CXX C)

# Προσθήκη του JUCE framework
if(NOT JUCE_PATH)
    set(JUCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/JUCE_FRAMEWORK" CACHE PATH "Path to JUCE")
endif()

add_subdirectory("${JUCE_PATH}" JUCE)

# Δημιουργία του Plugin
juce_add_plugin(ZUMBO
    PRODUCT_NAME "ZUMBO"
    VERSION "3.0.0"
    BUNDLE_ID "com.zumbo.synth"
    PLUGIN_MANUFACTURER_CODE "Zumb"
    PLUGIN_CODE "Zumb"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    JUCE_CHANNELS_PROPERTY "2 2"
    FORMATS VST3
)

# Αρχεία πηγής
target_sources(ZUMBO PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/ZumboDSP.cpp
)

# Σύνδεση βιβλιοθηκών
target_link_libraries(ZUMBO PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_audio_formats
    juce::juce_audio_devices
    juce::juce_audio_basics
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_dsp
    juce::juce_data_structures
    juce::juce_core
)

# ΔΙΟΡΘΩΣΗ: Αναγκάζουμε το JUCE να αγνοήσει το σφάλμα αυτοματισμού για να τερματίσει το build επιτυχώς
target_compile_definitions(ZUMBO PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)
