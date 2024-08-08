#!/bin/bash
# Run both programs simultaneously with the same device ID

DEVICE_ID=0

# Start the camera capture program
./open -d $DEVICE_ID -f 30 -w 640 -h 480 -v &
CAMERA_PID=$!

# Start the packet capture program
./pcaplp -d $DEVICE_ID &
USB_PID=$!

# Wait for both programs to complete
wait $CAMERA_PID
wait $USB_PID
