# MySnake Module Instructions

## Running Options
You can choose to:
- **Run Directly**: Use `mysnake.ko` directly.
- **Compile First**: Compile `mysnake.c` and then run the resulting `.ko` file.

## Compile Instructions

1. **Change Directory**:
   - Navigate to the folder that contains `mysnake.c` and `Makefile`.
2. **Source Environment**:
   - Execute `source /ad/eng/courses/ec/ec535/bashrc_ec535`.
3. **Compile**:
   - Make the source file and move the `.ko` file to the root directory:
     ```bash
     make
     ```

## BeagleBone Setup

1. **Device Setup**:
   - Set up the device and the serial console.
   - **Press-and-Hold the S2 Button**: Do this before, during, and for the first 3 seconds after connecting the power.

2. **Pin Connections**:
   - **Up-Command**: GPIO_26
   - **Right-Command**: GPIO_46
   - **Down-Command**: GPIO_47
   - **Left-Command**: GPIO_27

## Running the Program

1. **Create Device Node**:
   ```bash
   mknod /dev/myscreensaver c 61 0
   insmod /root/myscreensaver.ko
2. **Directly use .ko**:
   ```bash
   insmod /root/mysnake.ko 
## Youtube Link

https://youtu.be/Wy9uWW-lp54
