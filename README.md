# Graupel-1 HAB

Graupel-1 is a high altitude balloon (HAB), launched with the goal of reaching 100,000' altitude and capturing the entire flight on video.  The project draws much of its inspiration from the [Edge Pro Weather Balloon Kit](https://www.highaltitudescience.com/products/eagle-pro-near-space-kit) by High Altitude Science, but with a more DIY approach. We are following the excellent tutorial on HABs by [Overlook Horizon](https://www.overlookhorizon.com/how-to-launch-weather-balloons/), and plan on flying the OLHZN flight computer for GPS, telemetry, and APRS tracking.

This repository contains documentation and software supporting the launch.

## Learning Goals

1. HAB construction and launch techniques.
2. High altitude flight regulations: FAA regulations, filing flight plans, NOTAMs.
3. HAB tracking techniques, including amateur packet radio (APRS).

## Payloads

1. [OLHZN Track](https://github.com/OverlookHorizon/OLHZN_Track): Arduino-based APRS tracker and data logger.
1. [Spot Trace](https://www.findmespot.com/en-us/products-services/spot-trace): Commercial satellite-based tracker.
1. Camera TBD

## Software

There are two principle software systems in this repository:

1. A clone of the OLHZN tracker firmware, customized for this mission.
1. The software for the base station, which is simply a laptop with a software-defined radio (SDR).

Our goal for this mission is to have a pretty high confidence of retrieving the payloads, so not only will we have redundant tracking payloads (one commercial, one DIY), but we will have multiple methods of retrieving the location from the OLHZN tracker.

### Theory of operation

1. The OLHZN tracker reads position data from its GPS module

### Dependencies
...

## FAQ
1. What does the name **Graupel** mean?
	* [Graupel](https://en.wikipedia.org/wiki/Graupel)
