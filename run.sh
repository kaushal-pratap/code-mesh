#!/bin/bash

PORT=8080

echo "=========================================="
echo "    Code Mesh — Local Web Visualizer     "
echo "=========================================="
echo "Serving web visualizer at http://localhost:$PORT ..."
echo "Press Ctrl+C to stop the server."
echo ""

# Launch web browser automatically (macOS)
open "http://localhost:$PORT"

# Serve the web directory on port 8080
python3 -m http.server $PORT --directory web
