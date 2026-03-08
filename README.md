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
* **Python 3.13.x** The bridge is linked to this version
* **CMake 3.15+**
* **C++20 Compliant Compiler** Visual Studio 2022 recommended for Windows

---

##  Build Instructions

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/your-username/SpecSync.git](https://github.com/your-username/SpecSync.git)
   cd SpecSync
Run the Build:

Bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
Locate the Bridge:
After a successful build, your Python module SpecSyncBridge.cp313-win_amd64.pyd will be located in /build/Release.

# Project Architecture
/src: Core C++ implementation (Telemetry, Engine, Registry).

/include: Public headers for integration.

/ui/bridge: PyBind11 source for the Python module.

/build: (Generated) Compiled binaries and project files.