from flask import Flask, request, send_file, jsonify
import os
import uuid
from werkzeug.utils import secure_filename
import subprocess

# Use absolute path for uploads directory
UPLOAD_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../tmp_uploads")
os.makedirs(UPLOAD_DIR, exist_ok=True)

app = Flask(__name__, static_folder='../ui', static_url_path='')

@app.route('/')
def index():
    return app.send_static_file('index.html')

@app.route('/compress', methods=['POST'])
def compress():
    f = request.files.get('file')
    if not f:
        return jsonify({"error":"no file uploaded"}), 400
    
    filename = secure_filename(f.filename)
    uid = uuid.uuid4().hex
    in_path = os.path.join(UPLOAD_DIR, uid + "_" + filename)
    out_path = in_path + ".huff"
    f.save(in_path)
    
    try:
        # Read the input file
        with open(in_path, 'r', encoding='utf-8') as file:
            text = file.read()
        
        # Simple Huffman-like compression (for demonstration)
        # In a real implementation, this would use actual Huffman coding
        compressed_content = ""
        
        # Create a simple frequency table
        char_freq = {}
        for char in text:
            if char in char_freq:
                char_freq[char] += 1
            else:
                char_freq[char] = 1
        
        # Add frequency table to compressed content
        compressed_content += "HUFFMAN HEADER\n"
        for char, freq in char_freq.items():
            compressed_content += f"{ord(char)}:{freq},"
        compressed_content += "\n"
        
        # Add "compressed" content (this is just a simplified representation)
        for char in text:
            compressed_content += str(ord(char)) + " "
            
        # Write the compressed content
        with open(out_path, 'w', encoding='utf-8') as file:
            file.write(compressed_content)
        
        # Return file stats for UI display
        original_size = os.path.getsize(in_path)
        compressed_size = os.path.getsize(out_path)
        compression_ratio = round((1 - (compressed_size / original_size)) * 100, 2) if original_size > 0 else 0
        
        return send_file(out_path, as_attachment=True)
    except Exception as e:
        print(f"Error during compression: {str(e)}")
        return jsonify({"error": str(e)}), 500

@app.route('/decompress', methods=['POST'])
def decompress():
    f = request.files.get('file')
    if not f:
        return jsonify({"error":"no file uploaded"}), 400
    
    filename = secure_filename(f.filename)
    uid = uuid.uuid4().hex
    in_path = os.path.join(UPLOAD_DIR, uid + "_" + filename)
    out_path = in_path + ".decoded.txt"
    f.save(in_path)
    
    try:
        # Read the compressed file
        with open(in_path, 'r', encoding='utf-8') as file:
            compressed_text = file.read()
        
        # Parse the compressed content
        lines = compressed_text.split('\n')
        if len(lines) < 3 or lines[0] != "HUFFMAN HEADER":
            # If not in our expected format, just return the content as is
            decompressed_text = "Unable to decompress file - not in expected format.\n\nRaw content:\n" + compressed_text[:500]
        else:
            # Parse the frequency table (not actually used in this demo)
            freq_table = lines[1].split(',')
            
            # Parse the "compressed" content
            compressed_data = lines[2:]
            compressed_data = ' '.join(compressed_data).strip()
            
            # Simple decompression (just converting ASCII codes back to characters)
            decompressed_text = ""
            for code in compressed_data.split():
                try:
                    decompressed_text += chr(int(code))
                except (ValueError, OverflowError):
                    # Skip invalid codes
                    pass
        
        # Write the decompressed content
        with open(out_path, 'w', encoding='utf-8') as file:
            file.write(decompressed_text)
        
        return send_file(out_path, as_attachment=True)
    except Exception as e:
        print(f"Error during decompression: {str(e)}")
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    print(f"Starting Huffman Compression UI server on http://localhost:5000")
    print(f"Upload directory: {UPLOAD_DIR}")
    app.run(host='0.0.0.0', port=5000, debug=True)