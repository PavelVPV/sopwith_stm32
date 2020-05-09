# Overview

This is port of Sopwith game on [STM3241G-EVAL evaluation board](https://www.st.com/en/evaluation-tools/stm3241g-eval.html). 
The game was originally created by David L. Clark of BMB Compuscience in 1984.

[![Watch the demo video.](http://img.youtube.com/vi/mSCojWch0iY/0.jpg)](https://youtu.be/mSCojWch0iY)


You can find all the information about the game on the following websites:
<br>http://www.sopwith.org/
<br>http://davidlclark.com/sopwith/sourcecode.html

It does not require any additional hardware except those that is provided in the evaluation board.

## Limitations

For the full game experience it is required to have 11 buttons. Buttons for break, 
missile and startburst are not set (see [swbuttons_hal.c](stm3241g/src/swbuttons_hal.c)). 
But you can uncomment the code in `swbuttons_hal.c` to replace one of the current action 
(e.g. autopilot to home) with missing one.

# How to compile

You can either compile from the source code or use the precompiled binary.

## Compile from source files

To compile from the source code, do:

1. Clone STM32CubeF4 repo from GitHub: https://github.com/STMicroelectronics/STM32CubeF4
<br>I used version `Release v1.24.1`.
2. Make sure that `STM32CUBEF4_ROOT` variable in Makefile points to the cloned repo.
3. GCC Arm Toolchain 
4. Download GCC Arm Toolchain 8-2019-q3-update from http://developer.arm.com
5. Make sure that `GNU_INSTALL_ROOT` variable in Makefile points to the gcc toolchain folder.
6. Download and install `st-flash` utility from https://st.com
<br>For Mac OS do: `brew install stlink`. 
<br>My stlink version is `1.6.0`.
7. Make sure `st-flash` utility is accessible from `PATH` env variable.
8. Run `make sopwith` to compile the game.
9. Run `make flash` to flash the binary.

## Use precompiled binary

Use any tool you have (e.g. `st-flash`) to flash `sopwith.bin` provided in `bin` folder 
at address 0x8000000.

# How to play

Buttons mapping is the following:

| Button 			| Action						    |
|-------------------|-----------------------------------|
| Joystick Up 		| Flap Up / Menu Up 				|
| Joystick Down 	| Flap Down / Menu Down 			|
| Joystick Right 	| Accelerate 						|
| Joystick Left 	| Deaccelerate 						|
| Joystick Select 	| Drop a bomb / Select menu item 	|
| Key 				| Shoot 							|
| Tamper 			| Flip 								|
| Wakeup 			| Autopilot to home 				|

In the menu you can select one of the game types:
- Novice
- Expert
- Game against computer

Additionaly you can:
- Enable missile and startburts
- Switch off sound

*To be able to use missile or starburst, you need to re-map on of the buttons. 
See limitations section for the details.*
