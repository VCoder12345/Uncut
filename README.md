# Uncut

Multithreaded video editor implemented in C++ using Qt, FFmpeg, SDL and CMake.  
Implements multi-track timeline playback, FFmpeg-based video/audio decoding, SDL audio playback and a producer–consumer pipeline for asynchronous streaming across multiple tracks.

## Key features
- FFmpeg decoding pipeline for video and audio
- SDL-based audio playback
- Multithreaded media processing (decoding, buffering, rendering)
- Producer–consumer shared queues for asynchronous streaming across tracks

## Architecture (high level)
- Decoder layer: FFmpeg-based decoders produce decoded frames/packets.
- Buffering layer: thread-safe shared queues hold decoded frames (producer–consumer).
- Playback layer: scheduler pulls frames from buffers and feeds renderers; SDL plays audio.
- UI: Qt-based timeline and preview controls.

## Dependencies
- C++17 toolchain (MSVC, clang or GCC)
- Qt (version used in project)
- FFmpeg (libavformat/libavcodec/libswscale, etc.)
- SDL2 (for audio playback)
- CMake (project uses __Ninja Multi-Config__ generator; tested with CMake >= 3.0)
- Optional: __vcpkg__ for dependency management on Windows

## Build (recommended)
1. Install dependencies (Qt, FFmpeg, SDL2). Using __vcpkg__ is recommended on Windows.
2. Configure and generate build files:
   - Example (out-of-source):
     - cmake -S . -B build -G "Ninja Multi-Config"
3. Build:
   - cmake --build build --config Release
4. Run the produced executable from the build output (for multi-config generators the binary will be under the chosen configuration folder, e.g., `build/Release/`).
## Contact
- See repository for author and contact details.
