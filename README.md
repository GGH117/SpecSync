SpecSync

SpecSync is an open-source, AI-driven performance optimization agent for PC games. It helps to bridge the gap between the user's PC and a game's rendering reqiurments by adjusting settings in real-time to maintain stable frame rates and prevent thermal throttling.

Core Features
* Hardware & Bottleneck Detection: Scans CPU, GPU, RAM, and SSD/Hard drive speeds to generate a baseline hardware profile.
* Real-Time Telemetry: Monitors FPS, CPU and GPU utilization, and hardware temps.
* Dynamic Optimization Engine: Automatically scales texture pools, volumetric fog, shadow resolution, and AI upscaling like Nvidia's DLSS and AMD's FSR based on real-time VRAM availability and FPS targets.
* Thermal-Safe Overrides: Hardcoded safety layer that immediately reduces post-processing if GPU temperatures exceed safe thresholds set by the user.
* Natural Language Chatbot: Built-in conversational UI allowing players to query why settings were changed or request specific optimizations without navigating complex menus.
* Drop-in Engine Plugins: Native integrations for Unreal Engine in C++ and Unity C#, allowing game devs expose their rendering settings to the SpecSync Agent with minimal setup.



System Architecture

SpecSync is mode based which means it separates the monitoring logic from the game engine hooks.

1. SpecSync-Core: A lightweight background process that polls the OS for hardware metrics and executes the optimization decision tree.
2. SpecSync-UI: The frontend interface where users interact with the Agent.
3. Plugins: Standardized APIs that sit inside the game engine, receiving commands from the Agent and modifying UGameUserSettings in Unreal or QualitySettings in Unity.



For Game Devs: Engine Integration

Integrating SpecSync into your game allows your it to dynamically scale to any user's PC automatically.

Unreal Engine Quickstart

1. Drop the SpecSyncPlugin folder into your project's Plugins/ directory.
2. Enable the plugin in your .uproject file.
3. SpecSync automatically hooks into URendererSettings. To add custom game-specific overrides, implement the ISpecSyncReceiver interface on your GameMode or PlayerController.

Unity Quickstart

1. Import the SpecSync.unitypackage.
2. Add the SpecSyncManager MonoBehaviour to your persistent boot scene.
3. SpecSync will automatically interface with Unity's QualitySettings and GraphicsSettings APIs. You can preview SpecSync's AI decisions directly inside the Unity Editor via the custom Inspector window.


The Strategy Pattern

SpecSync uses a Strategy Pattern for its optimization logic. Modders and developers can write custom "Optimization Strategies" without modifying the core telemetry loop.

Example of a custom strategy checking VRAM:

python
 Pseudocode representation of a SpecSync Strategy
if available_vram < VRAM_SAFETY_BUFFER_MB:
    Log("VRAM Warning. Reducing Texture Streaming Pool.")
    GameConnector.set_parameter("TextureQuality", "Medium")
    GameConnector.flush_render_cache()


Contributing

SpecSync is 100% Open Source and community-driven. We are looking for contributors in the following areas:

* Hardware Profiling: Adding accurate baseline profiles for legacy GPUs (Like GTX 10 series cards and RX 500 series cards).
* Engine Programmers: Expanding the Unreal and Unity plugins to support specific rendering pipelines (e.g., Unity HDRP vs URP).
* Data Scientists: Helping refine the default optimization decision trees based on community telemetry data.

Please read the [CONTRIBUTING.md]() for details on the code of conduct, and the process for submitting pull requests.
