cmake_minimum_required(VERSION 3.22)

# Ενεργοποίηση των γλωσσών CXX και C και ορισμός VERSION για το JUCE
project(ZUMBO VERSION 3.0.0 LANGUAGES CXX C)

# Προσθήκη του JUCE framework στο project
if(NOT JUCE_PATH)
    set(JUCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/JUCE_FRAMEWORK" CACHE PATH "Path to JUCE")
endif()

add_subdirectory("${JUCE_PATH}" JUCE)

# Δημιουργία του Plugin και αυτόματη παραγωγή του JuceHeader
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

# ΕΓΓΥΗΜΕΝΗ ΔΙΟΡΘΩΣΗ: Εντολή στο JUCE να σταματήσει τη σύγκρουση αυτοματισμών VST2/VST3
set_target_properties(ZUMBO PROPERTIES JUCE_DISABLE_VST2_COMPATIBILITY_WITH_VST3 TRUE)

# Ορισμός των αρχείων πηγής του project
target_sources(ZUMBO PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/ZumboDSP.cpp
)

# Σύνδεση με τις βιβλιοθήκες του JUCE
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

# Ενεργοποίηση των απαραίτητων ρυθμίσεων του compiler
target_compile_definitions(ZUMBO PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)

