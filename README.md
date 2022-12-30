
# PsxJamBootstrap

Bootstrap with all the tools and code to start working on PSX for GameJam: GameDev Challenge 2023

This repo is mostly based on [VSCodePSX repo](https://github.com/NDR008/VSCodePSX) with some modifications to suit our needs.

## Setup

1. Before running the install scripts you will need install python3 and git for windows if you don't have them already installed.
2. Start by running setup.py which will install all the required dependencies and tools.
3. Add tools/g++-mipsel/bin to your PATH environment variable
4. Install visual studio code with C/C++ and GDB extensions
5. Download the original PsyQ sdk from [PSXDEV](http://www.psxdev.net/downloads.html) for docs and samples.

## Tools

- **FBX2PSX**: Conversor from FBX format to .h header file. tools\FBX2PSX\bin\FBX2PSX.exe (see draw_teapot sample)
- **BMP2TIM**: Conversor from BMP format to TIM (psx texture format). tools\BMP2TIM\BMP2TIM.exe (see draw_mesh_textured sample)
  Example: BMP2TIM.exe smile.bmp smile.tim 4 -org=640,0 -clut=0,480

## Samples
* [Samples page](https://github.com/rlopezll/PsxJamBootstrap/tree/main/samples)

## Debugging

To be able to debug using the PCSX-Redux Emulator, you will need to go to Configuration->Emulation and:

* Disable "Dynarec CPU"
* Enable "Enable Debugger" and "Enable GDB Server"



## Links and Docs

* [PSX Game Dev on Windows 10/11 in Visual Studio Code!](https://www.youtube.com/watch?v=Axyq_H_o8gI)
* [Fixed Point Math](http://rsync.irixnet.org/tutorials/pstutorials/chapter1/5-fixedpoint.html)
* [TIM Texture Format](https://github.com/ABelliqueux/nolibgs_hello_worlds/wiki/TIM)
* [Samples: NoLibgs_HelloWorld](https://github.com/ABelliqueux/nolibgs_hello_worlds)
* [Samples: PS1FixedPoint library C++](https://github.com/alextrevisan/PS1FixedPoint)
* [Samples: Repositories PSX1 very useful](https://github.com/alextrevisan?tab=repositories)


