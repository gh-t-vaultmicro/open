# detector

## Available

In this project directory, you can run: open, pcap, interpreter <br/>
open captures the frame from /dev/video* saves into logs, video, pictures <br/>
pcap captures the packet from /dev/usbmon* saves into log showing raw packets <br/>
interpreter changes log to parsed_log showing success rate of usb video packet tranfer at the end <br/>
isochronous webcam shows specific loss packet </br>
only works in linux

## How to build / use

### OPEN
0. Go to open directory
1. Go to build directory
2. cmake ..
3. make
4. sudo ./open <br/>
//to see the device status
5. e.g.) sudo ./open -d 0 -f 24 -w 1920 -h 1080 -v <br/>
//device number, fps, pixel width and height needs to be set <br/>
//streaming screen will be shown
6. q or ctrl+c to stop
7. Go to log directory
8. log file & ffmpeg log file made with name date-time.txt
9. Go to video directory
10. .mp4 file made
11. Go to picture_frame directory to date-time directory
12. Every frame pictured with .jpg
 
### PCAP
0. Go to pcap directory
1. Go to build directory
2. cmake ..
3. make
4. e.g.) sudo ./pcaplpc -d usbmon1 <br/>
//make sure "sudo modprobe usbmon" before running <br/>
//run while cheese or any video player is opened <br/>
//or run while ./open <br/>
//lsusb shows the Bus of device, change any device you would like
5. ctrl+c to stop
6. Go to log directory
7. log file made with name date-time.txt

### INTERPRETER
0. Go to pcap directory
1. Go to src directory
2. python3 interpreterp.py
3. type ../log/log_pcap_date-time.txt
4. Go to log directory
5. log file made with name date-time_parsed_log.txt


#### now working

Version 0.0.4 multi processing, multi threading included <br/>
Working on test code <br/>

