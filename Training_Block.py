import os
import logging
from pwnagotchi import plugins

class TrainingBlock(plugins.Plugin):
    __author__ = 'https://github.com/ATOMNFT'
    __version__ = '1.0.0'
    __license__ = 'MIT'
    __description__ = 'A plugin to move PCAP files with SSIDs containing "PT_" to a specific directory and skip their upload to wpa-sec.'

    def __init__(self):
        super().__init__()
        self.plugin_name = "TrainingBlock"
        self.handshake_dir = '/root/handshakes'
        self.pt_handshake_dir = '/root/PT_handshakes'
        logging.info(f"{self.plugin_name} plugin loaded successfully!")
        
        # Ensure the target directory exists
        if not os.path.exists(self.pt_handshake_dir):
            os.makedirs(self.pt_handshake_dir)
            logging.info(f"Created directory: {self.pt_handshake_dir}")

    # Hook into on_handshake event to check handshake files before uploading
    def on_handshake(self, agent, filename, bssid, station):
        # Check if the filename contains 'PT_'
        if self._contains_blocked_ssid_in_filename(filename):
            logging.info(f"PCAP file '{filename}' contains 'PT_'. Moving file to {self.pt_handshake_dir} and skipping upload.")
            
            # Move the file to the PT_handshakes directory
            self._move_file(filename)
            
            return None  # Skip upload to wpa-sec

        # Otherwise, allow the file to be uploaded
        return filename

    # Helper function to check if the filename contains 'PT_'
    def _contains_blocked_ssid_in_filename(self, filename):
        # Check if 'PT_' is part of the filename
        if 'PT_' in filename:
            return True
        return False

    # Helper function to move the file to the PT_handshakes directory
    def _move_file(self, filename):
        # Construct the full path for the original and target file
        original_path = os.path.join(self.handshake_dir, filename)
        new_path = os.path.join(self.pt_handshake_dir, filename)
        
        # Move the file
        if os.path.exists(original_path):
            os.rename(original_path, new_path)
            logging.info(f"Moved file '{filename}' to '{self.pt_handshake_dir}'.")
        else:
            logging.warning(f"File '{filename}' not found in '{self.handshake_dir}'.")
