SpecSync

The Native, AI-Driven Performance Framework for Games

SpecSync is an open-source, header-only C++ framework designed to be compiled directly into your game. It acts as an **in-process performance engineer**, bridging the gap between hardware constraints and software demands by adjusting engine parameters in real-time.

By moving SpecSync inside the game binary, it gains low-latency access to the render thread and eliminates the need for users to install external background utilities.


Core Features

* **Integrated Telemetry:** Polls hardware metrics (CPU/GPU load, VRAM, and thermals) directly via OS-native APIs (DXGI, Vulkan, or NVAPI) without external hooks.
* **Predictive Scaling:** Uses an internal decision tree to scale texture streaming pools, volumetric effects, and AI upscaling (DLSS/FSR/XeSS) based on the current frame budget.
* **Zero-Latency Feedback:** Because it lives in the engine, adjustments happen within the same frame or the next, preventing the "stutter-correction-stutter" loop common in external tools.
* **Thermal Guardrails:** A compiled-in safety module that aggressively downscales post-processing if hardware sensors report temperatures exceeding user-defined safe zones.
* **In-Game Performance Chatbot:** An optional UI module (ImGui or UMG/UI Toolkit) that allows players to ask, *"Why is my frame rate dipping?"* and receive real-time explanations from the agent.


System Architecture: The Library Model
1. **SpecSync-Runtime (The Core):** A lightweight C++ library linked to your game. It handles the "Optimization Loop" on a dedicated worker thread to avoid impacting the game's main logic.
2. **Abstraction Layer:** Standardized interfaces that translate SpecSync decisions into engine-specific commands (e.g., r.ScreenPercentage in Unreal or QualitySettings in Unity).
3. **The Registry:** A developer-facing API where you "register" which variables SpecSync is allowed to touch.

Engine Integration

Unreal Engine

SpecSync integrates as an **Unreal Engine Subsystem**, allowing it to initialize automatically with the game engine.

1. Add the SpecSync module to your Source/ folder and include it in your .Build.cs.
2. The USpecSyncSubsystem automatically hooks into URendererSettings.
3. **Custom Hooks:** Implement the ISpecSyncControllable interface on any Actor or Component to let the AI tune custom gameplay parameters (like NPC density or Draw Distance).

Unity
Utilizes a C++ native plugin with a high-performance C# wrapper to minimize garbage collection.

1. Import the SpecSync package into your Packages/ directory.
2. Drop the SpecSyncAgent component into your initial loading scene.
3. SpecSync interfaces directly with **URP/HDRP Volume Profiles**, allowing the AI to adjust effects like Bloom, DOF, and Fog density dynamically.

The Strategy Pattern

Developers can define "Performance Profiles" using a simple Strategy Pattern. This example demonstrates a strategy designed to prevent VRAM overflow:

void VRAMSafetyStrategy::Execute(SpecSyncContext& Context) {
    if (Context.GetAvailableVRAM() < 512) { //MB
        Log("VRAM critical. Downscaling textures.");
        Context.SetSetting("TextureQuality", Quality::Medium);
        Context.SetSetting("VRS_Enabled", true); // Variable Rate Shading
        Context.FlushRenderCommands();
    }
}

Contributing

SpecSync is 100% Open Source. We are currently looking for help in these "Embedded-First" areas:

* **Platform Shims:** Writing telemetry collectors for Linux/Steam Deck and macOS (Metal).
* **Shader Compilers:** Developing logic to delay certain effects until shaders have finished background compilation to prevent "compilation stutter."
* **Math Optimization:** Refining the AI decision tree to run in under $0.5ms$ per frame.
