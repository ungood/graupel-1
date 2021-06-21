This package contains code for post-processing of our recorded video and telemetry files. We have 2 primary sources of data:
1. The GoPro video(s), which contain telemetry data, documented here: https://github.com/JuanIrache/gopro-utils
2. The log data recorded by the tracker - code included in this package.

# Install

1. Cartopy has several requirements, which are detailed [here](https://scitools.org.uk/cartopy/docs/latest/installing.html#installing).
1. `pipenv install`

# Processing

## Extracting the GoPro binary data

1. Find the index of the GoPro MET stream with: `ffmpeg -i $VIDEO_NAME.mp4`
1. Extract it with: `ffmpeg -y -i $VIDEO_NAME.mp4 -codec copy -map 0:3 -f rawvideo $VIDEO_NAME.bin`
1. 

# Existing Software
* http://www.dashware.net/customer-videos/#prettyPhoto
* https://racerender.com/Products/index.html
* https://github.com/juanmcasillas/gopro2gpx

# TODO:

* Views
  * Pre-launch
  * Minimal
  * Verbose

* Graphs, start from T-0
  * Altitude - remove this.
  * Map - start with the bounding box, bearing
  * Altitude bar, with a high water mark

* Map, 
* Altitude on the right
* Compass
* Data:
  * Air temp
  * Pressure
  * Speed
  * Ground distance
