# JezNES
A new puzzle game for the NES written in C and ASM.

![jeznes_007](https://github.com/boingoing/jeznes/assets/1569843/da100c31-6ede-403b-a4ce-0841fb190268)

# How to play
![image](https://github.com/boingoing/jeznes/assets/1569843/3d1db512-5a49-4b8f-b2d0-0b78f46f6018)

JezNES is a simple puzzle game similar to QIX. The goal is to clear over a certain percentage of tiles in the playfield while avoiding the balls bouncing around in the playfield area. Each playfield begins as a completely uncleared set of playfield tiles.

![image](https://github.com/boingoing/jeznes/assets/1569843/bbaecc72-7f48-4ded-84af-1a8fa40a46c3)

The player controls a ship within the playfield. The player ship may collide with balls, walls, or any tiles within the playfield without worry. The ship may be rotated with the B-button and the ship may begin drawing a line with the A-button.

A line is started at the playfield location directly under the player ship. The line will begin extending in both directions (either up and down or left and right depending on the player ship orientation). If a ball in the playfield collides with an in-progress line segment, that line segment will be destroyed and the player will lose a life. When a line segment reaches a wall tile, that line segment will be completed and all playfield tiles making up the line will become cleared. When a line segment is completed, all playfield regions which do not capture a ball will become cleared playfield tiles.

![image](https://github.com/boingoing/jeznes/assets/1569843/a939a6e5-60a0-45f3-93d6-d293df51a8fe)

When the playfield is 75% cleared, the player will advance to the next level. Each successive level contains one additional ball. The game begins with 10 lives and an extra life is granted at each level up. When the player runs out of lives, it is game over.

# History
JezzBall shipped for Windows 3.1 in the Microsoft Entertainment Pack. Seems it might have been intended to help teach office workers about the right-mouse-button. [Read more](https://en.wikipedia.org/wiki/JezzBall).

![image](https://github.com/boingoing/jeznes/assets/1569843/49875bd1-0d41-4e8f-b9e9-4075850b0b78)

JezNES began as an implementation of JezzBall for the NES.

# Building
JezNES may be built via CMAKE and requires cc65. Tested on Ubuntu 22.04 and MacOS.

# Thanks
- Much thanks to [nesdoug](https://github.com/nesdoug) for providing many useful [tutorials](https://nesdoug.com/), [code samples](https://github.com/nesdoug), and libraries.
- [FrankenGraphics](https://twitter.com/FrankenGraphics) for extending NES Screen Tool into [NEXXT](https://frankengraphics.itch.io/nexxt) and providing a lot of help learning NES graphics.
- Many thanks to [Shiru](https://shiru.untergrund.net/code.shtml) for producing libraries like FamiTone2, neslib, and providing sample NES sound effects.
- NES controller graphic provided by [dimensions.com](https://www.dimensions.com/element/nes-controller).
- Kevin Zurawel for the [Famicom Party](https://famicom.party/book/) tutorial series.
