# SpecSync
**Zero-Impact Performance Optimization for Handhelds & PC**

SpecSync is a high-performance C++ middleware designed to monitor hardware telemetry in real-time and dynamically adjust game settings to maintain stability, preserve battery life, and prevent thermal throttling on devices like the **Steam Deck**.

## Current Status: v0.1.0-alpha
This is a developer-focused alpha. The core C++ engine and Python communication bridge are functional and verified on Windows-based environments.

### What's Working:
* **Real-time Telemetry:** Polling for FPS, CPU Usage, GPU Temperature, and Available VRAM.
* **Optimization Engine:** Logic-based "Thermal Guardrail" and "VRAM Safety" triggers.
* **Python Bridge:** High-speed .pyd module for building external dashboards and QA tools.
* **Auto-Build:** CMake automatically handles pybind11 dependencies.

### In Progress:
* **Linux/SteamOS Native Support:** Finalizing LinuxShim for non-Windows SteamOS environments.
* **Engine Wrappers:** Integration headers for Unreal Engine 5 and Unity.

## Prerequisites
Before building, ensure you have the following installed:
* **Python 3.13.x** 
* **CMake 3.15+**
* **C++20 Compliant Compiler**