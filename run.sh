#!/bin/bash

PORT=8080
TARGET_DIR="test"

echo "=========================================="
echo "    Code Mesh — Full Pipeline Runner     "
echo "=========================================="

echo "--> 1. Building C Systems Engine..."
make clean && make
if [ $? -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi

echo ""
echo "--> 2. Running C AST Engine on '$TARGET_DIR' directory..."
./codemesh_engine "$TARGET_DIR"

echo ""
echo "--> 3. Launching Web Visualizer at http://localhost:$PORT ..."
open "http://localhost:$PORT"

echo "Serving web interface. Press Ctrl+C to stop."
echo "=========================================="
python3 -m http.server $PORT --directory web
