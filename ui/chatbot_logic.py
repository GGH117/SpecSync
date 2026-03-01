import SpecSyncBridge

# Initialize the C++ engine
engine = SpecSyncBridge.OptimizationEngine()

def process_game_frame(fps, vram):
    # Create a mock data object for the C++ engine
    data = SpecSyncBridge.FrameData()
    data.CurrentFPS = fps
    data.AvailableVRAM = vram
    
    # Get optimization advice from C++
    action = engine.AnalyzePerformance(data)
    
    if action.TargetSetting != "NONE":
        # Pass the 'MessageForChatbot' to your UI's text box
        return action.MessageForChatbot
    return None