#!/usr/bin/env bash
set -e
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
echo "Run JUCE pluginval against the generated ZUMBO.vst3 bundle for final macOS validation."
