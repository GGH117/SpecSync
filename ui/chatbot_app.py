import tkinter as tk
from tkinter import ttk
import customtkinter as ctk

# Set the theme to dark
ctk.set_appearance_mode("dark")
ctk.set_default_color_theme("blue")

class SpecSyncUI(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.title("SpecSync Agent")
        self.geometry("400x600")

        # Header
        self.header = ctk.CTkLabel(self, text="SpecSync Chatbot", font=("Roboto", 20, "bold"))
        self.header.pack(pady=20)

        # Chat Display
        self.chat_display = ctk.CTkTextbox(self, width=350, height=400)
        self.chat_display.pack(padx=20, pady=10)
        self.chat_display.insert("0.0", "Agent: Hardware Scan Complete. I'm monitoring your FPS. How can I help?\n\n")
        self.chat_display.configure(state="disabled")

        # Input Area
        self.input_frame = ctk.CTkFrame(self)
        self.input_frame.pack(pady=10, fill="x", padx=20)

        self.user_input = ctk.CTkEntry(self.input_frame, placeholder_text="Ask about performance...")
        self.user_input.pack(side="left", fill="x", expand=True, padx=(0, 10))

        self.send_button = ctk.CTkButton(self.input_frame, text="Send", width=60, command=self.handle_message)
        self.send_button.pack(side="right")

    def handle_message(self):
        msg = self.user_input.get()
        if msg:
            self.update_chat("You", msg)
            self.user_input.delete(0, tk.END)
            self.generate_response(msg)

    def update_chat(self, sender, text):
        self.chat_display.configure(state="normal")
        self.chat_display.insert(tk.END, f"{sender}: {text}\n\n")
        self.chat_display.configure(state="disabled")
        self.chat_display.see(tk.END)

    def generate_response(self, user_msg):
        # Placeholder for the AI Logic. 
        # In the next step, we would connect this to the C++ Telemetry data.
        response = "Agent: I've noticed your Frame Time is spiking. Suggesting a 10% reduction in Volumetric Clouds for stability."
        self.update_chat("Agent", response)

if __name__ == "__main__":
    app = SpecSyncUI()
    app.mainloop()