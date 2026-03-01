
# Contributing to SpecSync

First off, thank you for taking the time to contribute! By helping with SpecSync, you are helping game developers ensure their game runs smoothly on any gamer's device, from high-end PCs to handhelds like the Steam Deck.

As an embedded framework, our priority is Zero Impact. SpecSync should never be the reason a game stutters.

## Strategic Areas for Contribution

We are currently prioritizing the following tracks:

 1. Platform Providers (C++)

SpecSync needs to talk to hardware. We need Shims that can safely poll telemetry across different environments:

* **Windows:** Refining NVAPI and ADL (AMD Display Library) integrations.
* **Linux/Proton:** Improving sensors for Steam Deck (MangoHud-style telemetry).
* **Consoles:** Abstracting safe hooks for current-gen hardware.

### 2. Engine Shims (Unreal & Unity)

We need experts to deepen the integration with modern pipelines:

* **Unreal Engine:** Expanding support for Nanite/Lumen scalability settings and the new Scriptable Render Pipeline.
* **Unity:** Enhancing the **Volume Profile** overrides for HDRP and URP.

### 3. The Logic Engine (Data Science)

Helping us refine the Optimization Decision Tree. If $FPS < Target$, which setting should be dropped first? We need data-driven insights to determine which settings provide the most Performance-per-Visual-Pixel.

## Development Workflow

### 1. Local Setup

1. **Clone the Repo:** git clone [https://github.com/SpecSync/SpecSync-Embedded.git](https://github.com/GGH117/SpecSync.git)
2. **Submodules:** This project uses submodules for certain hardware APIs. Run git submodule update --init --recursive.
3. **Build:** We use **CMake**. Ensure you have a C++20 compliant compiler.

### 2. Coding Standards

Because we live inside a game's process, we follow strict rules:

* **No Heavy Allocations:** Avoid new/delete or std::vector resizing inside the Tick() or Update() loop. Use pre-allocated buffers.
* **Thread Safety:** The Telemetry thread must never block the Game/Render thread. Use atomic flags for communication.
* **Minimal Dependencies:** Keep the core header-only where possible. Do not add heavy third-party libraries.

### 3. Submitting a Pull Request

1. **Branch:** Create a feature branch (feature/your-feature-name).
2. **Test:** If you are modifying the Unreal plugin, test it in at least a Blank project and the Lyra sample if possible.
3. **Document:** Update the inline Doxygen comments for any new API functions.
4. **PR Template:** Fill out the PR template, specifically noting the **Performance Impact** of your change.



## Code of Conduct

We are committed to fostering a welcoming and collaborative community.

* **Be Respectful:** We are all here to build cool tech.
* **Constructive Feedback:** Critique the code, not the programmer.
* **Safety First:** Any code that could potentially cause hardware instability (e.g., uncontrolled overvolting) must be reported and will be rejected immediately.



## Need Help?

* **Issues:** For bug reports or feature requests, use the GitHub Issues tab.
* **Discussions:** Join our Discord for real-time architecture chats.
* **Security:** If you find a vulnerability, please email me at atharvswaroop@gmail.com instead of opening a public issue.
If your ready start please join the discord for announcements!
