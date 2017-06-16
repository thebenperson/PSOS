# PSOS (Pretty Simple/Stupid Operating System)
## Version 1.0

### Description:

PSOS is a (very) basic x86 16-bit operating system written in assembly.
Currently PSOS is just a simple command line with 4 commands: CIRCLE, CLEAR, HELP, and VGATEST.

- CIRCLE demonstrates VGA mode 13h by drawing a circle.
- CLEAR clears the screen.
- HELP displays a help message.
- VGATEST displays the possible colors in VGA mode 13h.

Note that these commands are in all-caps.
This is because PSOS's keyboard driver does not currently support lowercase.
The only other feature is text scrolling.
If text reaches the bottom of the screen, it is scrolled up.
This is still very buggy.

Here is what I hope to accomplish in the next version:

- ~~Port PSOS to C~~ **(done)**
- ~~Implement exception handling~~ **(done)**
- ~~Add more commands~~ **(done)**
- ~~Implement filesystem~~ **(done)**
- Implement VESA driver

**Check out the [*dev branch*](https://github.com/TheBenPerson/PSOS/tree/dev) for the latest build**

### Compilation Requirements:

- Basic *NIX setup, or sad Windows imitation ([MinGW](http://mingw.org/), [Cygwin](https://cygwin.com/), [Bash for Windows](https://msdn.microsoft.com/en-us/commandline/wsl/about), etc.)
- [NASM (Netwide Assembler)](http://www.nasm.us/)

### Compiling:

To compile PSOS, run `make` in the top level directory.
This should generate a file called PSOS.bin in the bin/ folder.
PSOS.bin is a floppy image.

### Installing:

To install PSOS on a storage device, use the command: `dd if=bin/PSOS.bin of=/dev/device_name && sync`.

***Please do not accidentally install PSOS on your hard drive.***
If you do, make sure to send me a video of it so I can laugh at you.

### Running:

PSOS.bin is a floppy image, so some emulators might have trouble with the format.
If you have trouble, try emulating it as a floppy drive

PSOS uses BIOS functions to operate. For systems using UEFI, you may need to enable legacy support in your firmware settings.
