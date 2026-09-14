# Uncut

A work-in-progress video editor written in C++17 using Qt, FFmpeg, SDL, and CMake. The project explores the architecture and implementation of a multithreaded media-processing pipeline, with a focus on asynchronous decoding, buffering, and playback.

## Current Features

* **Video preview:** FFmpeg-based video decoding and playback.
* **Media pipeline:** Separate components for decoding, buffering, and playback.
* **Multithreaded processing:** Asynchronous media processing using a producer–consumer architecture.
* **Qt interface:** Initial editor interface and preview controls.
* **SDL audio:** Audio playback infrastructure.

## Architecture

The project is structured around a multithreaded media pipeline. Decoder threads produce decoded media, thread-safe queues buffer the results, and the playback system consumes them for rendering and audio output.

Multi-track playback is currently **under development**. Its architecture and scheduling approach have been started, but the implementation is not yet complete.

## Building

The project uses CMake and requires a C++17-compatible compiler.

### Dependencies

* [Qt](https://www.qt.io/)
* [FFmpeg](https://ffmpeg.org/)
* [SDL2](https://www.libsdl.org/)
* [CMake](https://cmake.org/)

On Windows, [vcpkg](https://github.com/microsoft/vcpkg) can be used to manage dependencies.

### Build

For example, using the Ninja Multi-Config generator:

```sh
cmake -S . -B build -G "Ninja Multi-Config"
cmake --build build --config Release
```

The resulting executable can be found in the corresponding build configuration directory.
