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
        # Get the handshake directory from the Pwnagotchi configuration, fallback to /root/handshakes if not set
        self.handshake_dir = self.config.get("main.handshakes", "/root/handshakes")
        self.pt_handshake_dir = self.config.get("main.pt_handshakes", "/root/PT_handshakes")
        logging.info(f"{self.plugin_name} plugin loaded successfully!")

        # Ensure the target directory exists
        if not os.path.exists(self.pt_handshake_dir):
            try:
                os.makedirs(self.pt_handshake_dir)
                logging.info(f"Created directory: {self.pt_handshake_dir}")
            except OSError as e:
                logging.error(f"Failed to create directory '{self.pt_handshake_dir}': {str(e)}")

    # Hook into on_handshake event to check handshake files before uploading
    def on_handshake(self, agent, filename, bssid, station):
        try:
            # Check if the filename contains 'PT_'
            if 'PT_' in filename:
                logging.info(f"PCAP file '{filename}' (BSSID: {bssid}, Station: {station}) contains 'PT_'. Moving file to {self.pt_handshake_dir} and skipping upload.")

                # Move the file to the PT_handshakes directory
                self._move_file(filename)
                
                return None  # Skip upload to wpa-sec

            # Otherwise, allow the file to be uploaded
            return filename
        except FileNotFoundError as e:
            logging.error(f"File '{filename}' not found: {str(e)}")
            return filename
        except OSError as e:
            logging.error(f"OS error while processing '{filename}': {str(e)}")
            return filename

    # Helper function to move the file to the PT_handshakes directory
    def _move_file(self, filename):
        original_path = os.path.join(self.handshake_dir, filename)
        new_path = os.path.join(self.pt_handshake_dir, filename)
        
        try:
            # Move the file
            if os.path.exists(original_path):
                os.rename(original_path, new_path)
                logging.info(f"Moved file '{filename}' to '{self.pt_handshake_dir}'.")
            else:
                logging.warning(f"File '{filename}' not found in '{self.handshake_dir}'.")
        except OSError as e:
            logging.error(f"Failed to move file '{filename}': {str(e)}")
