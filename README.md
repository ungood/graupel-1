# Graupel-1 HAB

Graupel-1 is a high altitude balloon (HAB), launched with the goal of reaching 100,000' altitude and capturing the entire flight on video.  The project draws much of its inspiration from the [Edge Pro Weather Balloon Kit](https://www.highaltitudescience.com/products/eagle-pro-near-space-kit) by High Altitude Science, but with a more DIY approach. We are following the excellent tutorial on HABs by [Overlook Horizon](https://www.overlookhorizon.com/how-to-launch-weather-balloons/), and plan on flying the OLHZN flight computer for GPS, telemetry, and APRS tracking.

This repository contains documentation and software supporting the launch.

## Learning Goals

1. HAB construction and launch techniques.
2. High altitude flight regulations: FAA regulations, filing flight plans, NOTAMs.
3. HAB tracking techniques, including amateur packet radio (APRS).

## Construction

### Balloon and Lift Gas

### Payloads

1. [OLHZN Track](https://github.com/OverlookHorizon/OLHZN_Track): Arduino-based APRS tracker and data logger, detailed below.
1. [Spot Trace](https://www.findmespot.com/en-us/products-services/spot-trace): Commercial satellite-based tracker.
1. Camera TBD

### Recovery

## Launch

## Tracking

As we plan on flying a rather high end camera on-board this flight, a primary goal is to be able to recover it.  In order to facilitate this, several redundant tracking systems will be employed:

1. A commercial satellite-based tracker: This tracker is designed to work at normal altitudes, and likely will not work at all about 50,000'. It also may fail to report an accurate position if LOS to the GPS and comm satellites is lost (perhaps due to the payload tipping over on landing).
1. An APRS tracker: This tracker will be designed to operate at high altitudes and communicate it's location to ground stations (called digipeaters) which will forward telemetry data to the internet (called i-gates). It will likely not work very well once the balloon has landed due to losing LOS with the digipeaters.
1. An audible beacon to aid in recovery once approximate location is determined.
1. And if all else fails: a message asking someone to call us.

Because communication with the digipeaters cannot be assured, we will run a mobile ground station with the ability to receive signals from the APRS tracker.  Because an internet connection cannot be assured, we will have software available on the ground station for being able to map, find directions to, and display a bearing to the landing site.

### Hardware

### Software

There are two principle software systems in this repository:

1. A clone of the OLHZN tracker firmware, customized for this mission.
1. The software for the base station, which is simply a laptop with a software-defined radio (SDR).


### Theory of operation

1. The OLHZN tracker reads position data from its GPS module

### Dependencies

1. librtlsdr: For the `sdr_fm` command-line tool to pipe the SDR output to direwolf
1. [direwolf](https://github.com/wb2osz/direwolf): For APRS decoding.

## Todo

1. Use cell phone for APRS tracking?
1. Use cell phone as GPS for laptop? [Share GPS](https://play.google.com/store/apps/details?id=com.jillybunch.shareGPS&hl=en_US&gl=US)

## FAQ
1. What does the name **Graupel** mean?
	* [Graupel](https://en.wikipedia.org/wiki/Graupel)
