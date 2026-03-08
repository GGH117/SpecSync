import sys
import os

# Tell Python to look inside your build/Release folder for the module
sys.path.append(os.path.join(os.getcwd(), "build", "Release"))

try:
    import SpecSyncBridge
    print("✅ SpecSync Bridge Loaded Successfully!")
    
    # Initialize the C++ classes
    telemetry = SpecSyncBridge.Telemetry()
    engine = SpecSyncBridge.OptimizationEngine()
    
    # Run a quick test tick
    telemetry.Tick(0.016) # Simulate 16ms (60fps)
    data = telemetry.GetLatestData()
    
    print(f"Hardware Polling Check:")
    print(f" - FPS: {data.CurrentFPS:.2f}")
    print(f" - CPU: {data.CpuUsage:.1f}%")
    print(f" - VRAM Available: {data.AvailableVRAM} MB")

except ImportError as e:
    print(f"❌ Failed to load bridge: {e}")