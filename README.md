# tuioRain
Standalone app to send a storm of TUIO events for stability testing.

![](misc/screenshot.gif)

Inspired by [Gal Sasson](https://github.com/galsasson)'s "Touch Rain" built into [ofxInterface](https://github.com/galsasson/ofxInterface). Check it out!

## About

##### Setup
- Set the IP and port of your TUIO listener in the `data/settings.xml` file.

##### Touches
- Emits `X` number of touch events every `Y` frames - set with GUI.
- Touches have a random time between the minimum and maximum touch duration.
- Touches have a 50% chance of being a 'swipe' which moves a random distance according to the max swipe distance slider.

##### Motion
- The overall motion can be purely random or any combination of:
	- Swept horizontally or vertically across the screen and then starting back at the other side
	- Oscillated horizontally or vertically creating wave patterns across the screen
- Random motion or modifying it in just one direction allows you to create multiple random touches across the un-modified axis.
- With both axis being modified only one touch is created per frame.

##### Dependencies
- openFrameworks 0.9~ and ofxTuioWrapper. Just `git submodule init` and `git submodule update`.
