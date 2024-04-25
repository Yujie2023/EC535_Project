you can choos to run mysnake.ko directly, or you can firstly compile mysnake.c and then run it.

Compile:/n
change directory to the folder contains mysnake.c and Makefile
source /ad/eng/courses/ec/ec535/bashrc_ec535 make the scouce file and put the .ko file to root diretory
enter: make

Beagleboone setup:
set up the device and the serial console
press-and-hold the S2 button9 before, during, and for the first 3 seconds after connecting the power

Run program:
mknod /dev/myscreensaver c 61 0
insmod /root/myscreensaver.ko (compile from mysanke.c)
 or insmod /root/mysnake.ko (run directly mysanke.ko)
