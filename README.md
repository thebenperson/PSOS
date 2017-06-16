# PSOS (Pretty Simple/Stupid Operating System)
## Development Branch

### Description:

As I am working on PSOS, I will upload the latest build here.

### Features:

- A FAT16 filesystem (read only for now)
- A (barely functional) VESA driver w/ bank switching
- A basic RTC driver (for reading the date)
- A very basic audio driver
- A sort of user land w/ 16 system calls
- 8 userland programs including a basic shell


###Compilation Requirements:

- Basic *NIX setup, or sad Windows imitation ([MinGW](http://mingw.org/), [Cygwin](https://cygwin.com/), [Bash for Windows](https://msdn.microsoft.com/en-us/commandline/wsl/about), etc.)
- [NASM (The Netwide Assembler)](http://www.nasm.us/)

### Compiling:

To compile PSOS, run `make` in the top level directory.
This should generate two files in the bin/ folder: PSOS.img and PSOS.iso.
PSOS.img is a floppy image and PSOS.iso is an ISO image (duh).

Ignore the other directories and their contents. (Or don't. I don't care.)

### Installing:

If you want to access the FAT16 filesystem in the image on an OS other than PSOS, use the PSOS.img file. The ISO image will boot fine, but you can't access the filesystem on it.
To install PSOS on a storage device, run `dd if=bin/PSOS.img of=/dev/device_name && sync`.

***Please do not accidentally install PSOS on your hard drive.*** If you do, make sure to send me a video of it so I can laugh at you.

### Running:

Some emulators might have trouble with the floppy image.
If you have trouble, try emulating it as a floppy drive or just use the ISO image.

PSOS uses BIOS functions to operate. For systems using UEFI, you may need to enable legacy support in your firmware settings.

***This branch is a work in progress. Don't expect it to work.***
