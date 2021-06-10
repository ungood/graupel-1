# Graupel-1 HAB

Graupel-1 is a high altitude balloon (HAB) mission, launched with the goal of reaching 100,000' altitude and capturing the entire flight on video.  The project draws much of its inspiration from the [Edge Pro Weather Balloon Kit](https://www.highaltitudescience.com/products/eagle-pro-near-space-kit) by High Altitude Science, but with a more DIY approach. We are following the excellent tutorial on HABs by [Overlook Horizon](https://www.overlookhorizon.com/how-to-launch-weather-balloons/), and plan on flying the OLHZN flight computer for GPS, telemetry, and APRS tracking.

This repository contains documentation and software supporting the launch.

## Learning Goals

1. HAB construction and launch techniques.
2. High altitude flight regulations: FAA regulations, filing flight plans, NOTAMs.
3. HAB tracking techniques, including amateur packet radio (APRS).

## Construction

### Balloon and Lifting Gas

We are planning on using a 1000g weather balloon, with 112 cu ft of helium, giving us a predicted ascent rate of 4.34 m/s and a burst altitude of 31726 m.

### Payloads

1. [OLHZN Track](https://github.com/OverlookHorizon/OLHZN_Track): Arduino-based APRS tracker and data logger, detailed below.
1. [Spot Trace](https://www.findmespot.com/en-us/products-services/spot-trace): Commercial satellite-based tracker.
1. Camera: GoPro Hero 9 Black

Total payload weight is 1350 grams.

### Recovery

We're using a 3' [Rocketman High Altitude Balloon parachute](https://the-rocketman.com/recovery-html/) which is designed with a loop at the top to attach the balloon too.  Recovery is simple: while the balloon ascends, the parachute is pulled upwards and shut, adding very little to the drag. When the balloon bursts, the payload will begin to fall and naturally open the chute.

We tested the parachute by dropping it off the Fremont Bridge and measuring the time it took to fall with a simulated payload weighing 2.69 pounds.  After several timings, we measured a descent rate of between 18.1 and 20.9 feet per second which matches the expected descent rates on the Rocketman website.

## Launch

Using the information, we plugged everything into a [balloon flight predictor](https://predict.habhub.org/) starting 10 days prior to launch to find our predicted flight path. We repeated this daily to determine if the predictions started stabalizing as we got closer to our launch day.

## Tracking

As we plan on flying a rather high end camera on-board this flight, a primary goal is to be able to recover it.  In order to facilitate this, several redundant tracking systems will be employed:

1. A commercial satellite-based tracker: This tracker is designed to work at normal altitudes, and likely will not work at all about 50,000'. It also may fail to report an accurate position if LOS to the GPS and comm satellites is lost (perhaps due to the payload tipping over on landing).
1. An APRS tracker: This tracker will be designed to operate at high altitudes and communicate it's location to ground stations (called digipeaters) which will forward telemetry data to the internet (called i-gates). It will likely not work very well once the balloon has landed due to losing LOS with the digipeaters. A backup plan was developed by creating a directional Yagi antenna from a measuring tape which can be used to search for the APRS tracker and triangulate its position if need be.
1. An audible beacon to aid in recovery once approximate location is determined.
1. And if all else fails: a message asking someone to call us.

Because communication with the digipeaters cannot be assured, we will run a mobile ground station with the ability to receive signals from the APRS tracker.  Because an internet connection cannot be assured, we will have software available on the ground station for being able to map, find directions to, and display a bearing to the landing site.

### Software

There are several software systems in this repository:

1. `./tracker`: The APRS tracker firmware, based loosely on the OLHZN_Track firmware.
1. `./station`: The software for the base station, which is simply a laptop with a software-defined radio (SDR).  It uses direwolf as a APRS modem to decode the signal from the tracker.
1. `./processing`: Post processing of the tracker log files for various analysis.

### Dependencies

1. librtlsdr: For the `sdr_fm` command-line tool to pipe the SDR output to direwolf
1. [direwolf](https://github.com/wb2osz/direwolf): For APRS decoding.

## FAQ
1. What does the name **Graupel** mean?
	* [Graupel](https://en.wikipedia.org/wiki/Graupel) also called soft hail, hominy snow, or snow pellets, is precipitation that forms when supercooled water droplets are collected and freeze on falling snowflakes, forming 2–5 mm (0.08–0.20 in) balls of rime.
