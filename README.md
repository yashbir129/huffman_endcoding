# Huffman Compression Tool

## Overview
This project implements Huffman coding (lossless text compression) in C. It includes:
- Core C modules for frequency counting, Huffman tree construction, encoding and decoding.
- A simple Flask server to integrate a browser UI with the compiled C binaries.
- A basic web UI (HTML/CSS/JS) for uploading text files and downloading compressed `.huff` files (and vice versa).

## Repo layout
- `src/core/` : C source for Huffman functionality.
- `src/ui/` : Static UI files (index.html, style.css, app.js).
- `src/integration/server.py` : Flask server to handle file uploads and call compiled binaries.
- `bin/` : Compiled `encoder` and `decoder` binaries (created by `make`).
- `tests/test_runner.c` : Simple test runner (compile and link into a binary if desired).

## Build
1. Ensure `gcc` is installed.
2. From project root run:

