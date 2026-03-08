import sys
import os
import time
import csv
from datetime import datetime

# ANSI Color Codes for the Terminal
RED = "\033[91m"
YELLOW = "\033[93m"
GREEN = "\033[92m"
RESET = "\033[0m"
BOLD = "\033[1m"

def load_config():
    """Reads user-defined thresholds from config.txt"""
    defaults = {"FPS": 60.0, "TEMP": 85.0, "VRAM": 512, "INT": 0.5}
    if not os.path.exists("config.txt"):
        print(f"{YELLOW}Warning: config.txt not found. Using internal defaults.{RESET}")
        return defaults
    
    try:
        with open("config.txt", "r") as f:
            for line in f:
                if "=" in line and not line.startswith("#"):
                    key, val = line.strip().split("=")
                    if key == "TARGET_FPS": defaults["FPS"] = float(val)
                    if key == "CRITICAL_TEMP": defaults["TEMP"] = float(val)
                    if key == "MIN_VRAM_MB": defaults["VRAM"] = int(val)
                    if key == "POLL_INTERVAL": defaults["INT"] = float(val)
    except Exception as e:
        print(f"{RED}Error reading config: {e}. Using defaults.{RESET}")
    return defaults

def start_logging():
    config = load_config()
    
    # Assuming SpecSyncBridge.pyd is in the build directory relative to this script
    BUILD_PATH = os.path.join(os.getcwd(), "build", "Release")
    sys.path.append(BUILD_PATH)

    try:
        import SpecSyncBridge
    except ImportError:
        print(f"{RED}Error: SpecSyncBridge.pyd not found in {BUILD_PATH}{RESET}")
        return

    telemetry = SpecSyncBridge.Telemetry()
    engine = SpecSyncBridge.OptimizationEngine()
    log_file = f"SpecSync_Session_{datetime.now().strftime('%Y%m%d_%H%M')}.csv"
    
    print(f"\n{BOLD}{'='*75}{RESET}")
    print(f"{BOLD} SPECSYNC MONITOR | Target: {config['FPS']} FPS | Crit: {config['TEMP']}°C {RESET}")
    print(f"{'='*75}")
    print(f"{'Timestamp':<12} | {'FPS':<6} | {'CPU %':<6} | {'Temp':<7} | {'VRAM':<8} | {'Status'}")
    print(f"{'-'*12}-+-{'-'*6}-+-{'-'*6}-+-{'-'*7}-+-{'-'*8}-+-{'-'*15}")

    with open(log_file, mode='w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Time", "FPS", "CPU_Pct", "Temp_C", "VRAM_MB", "Status"])
        
        try:
            while True:
                telemetry.Tick(config['INT'])
                data = telemetry.GetLatestData()
                commands = engine.Process(data, config['INT'])
                
                # Terminal Output with Color Coding
                row_color = RESET
                status_text = "NOMINAL"
                
                if data.GpuTemp >= config['TEMP']:
                    row_color = RED + BOLD
                    status_text = "!! OVERHEAT !!"
                elif data.CurrentFPS < config['FPS']:
                    row_color = YELLOW
                    status_text = "PERF DROP"
                elif commands:
                    row_color = GREEN
                    status_text = "OPTIMIZING"

                timestamp = datetime.now().strftime("%H:%M:%S")
                writer.writerow([timestamp, round(data.CurrentFPS, 2), round(data.CpuUsage, 1), 
                                 round(data.GpuTemp, 1), data.AvailableVRAM, status_text])
                
                print(f"{row_color}{timestamp:<12} | {data.CurrentFPS:<6.1f} | {data.CpuUsage:<6.1f} | "
                      f"{data.GpuTemp:<5.1f}°C | {data.AvailableVRAM:<5}MB | {status_text}{RESET}")
                
                time.sleep(config['INT'])
        except KeyboardInterrupt:
            print(f"\n{BOLD}{'='*75}{RESET}\nSession Saved to {log_file}")

if __name__ == "__main__":
    start_logging()