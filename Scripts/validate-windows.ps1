param([string]$Build="build")
cmake --build $Build --config Release
ctest --test-dir $Build -C Release --output-on-failure
Write-Host "Run JUCE pluginval against the generated ZUMBO.vst3 bundle for final Windows validation."
