# ZUMBO Instrument 3

ZUMBO is an original professional-oriented psychedelic synthesizer codebase.

## Included

- 8 oscillator polyphonic engine
- PolyBLEP anti-aliasing foundation
- 32 voices
- granular engine with 256 grain slots
- user sample buffer API
- reverse grains
- oversampled nonlinear drive
- state-variable filters
- LP/HP/BP/Notch architecture with 24/48 dB extension points
- 4 LFO parameter architecture
- 16-slot modulation matrix parameter architecture
- reverse-delay engine
- reverb + shimmer processing path
- parameter attachments for visible controls
- APVTS state/preset serialization
- factory preset catalog
- CPU-oriented release flags
- DSP unit tests
- Windows/macOS validation scripts
- custom psychedelic GUI

## Sample loading

The DSP exposes `Granular::load(AudioBuffer<float>)`. The next integration layer should use JUCE's `AudioFormatManager` from a background thread, then atomically hand the prepared buffer to the audio engine. Never decode WAV/AIFF/FLAC on the real-time audio thread.

## Build

Install JUCE 8.x:

```bash
cmake -S . -B build -DJUCE_PATH=/path/to/JUCE -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

For a professional release, run JUCE `pluginval` on the resulting VST3 bundle on every supported OS/architecture.

## Production note

The repository is intentionally self-contained and extensible, but audio plugins still require host-specific validation. DSP correctness, CPU load, latency, denormal handling, sample loading and GUI behavior should be tested on actual Windows/macOS DAWs before distribution.

## GitHub

Recommended repository name:

`ZUMBO`

Suggested branches:

- `main` — stable
- `develop` — integration
- `feature/*` — DSP/UI features

