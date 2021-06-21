import argparse
from overlays import Layout, PlotOverlay
from streaming import FileVideoStream
import cv2
import datetime
import numpy as np
import pandas as pd
import progressbar

from timebudget import timebudget
timebudget.set_quiet()

import matplotlib
matplotlib.use('Agg')

from matplotlib import pyplot as plt
plt.style.use("dark_background")
import matplotlib.dates as mdates
from mpl_toolkits.basemap import Basemap

parser = argparse.ArgumentParser(description='Overlays telemetry data over top a video using opencv.')
parser.add_argument('--input', type=str, required=True)
parser.add_argument('--data', type=argparse.FileType('r'), required=True)
parser.add_argument('--output', type=str)
parser.add_argument('--max-seconds', type=int, help='Quits early for testing purposes.')
parser.add_argument('--live-view', action='store_true', help='Displays the video live as it is processed.')

args = parser.parse_args()

input = FileVideoStream(args.input)
input.start()

output = None
if(args.output is not None):
  fourcc = cv2.VideoWriter_fourcc(*"MJPG")
  output = cv2.VideoWriter(args.output, fourcc, 10, input.shape)

data = pd.read_csv(args.data)

# Convert date/time columns to index data
datetime_cols = ["year", "month", "day", "hour", "minute", "second"]
timestamps = data[datetime_cols].apply(lambda x: "{}-{}-{} {}:{}:{}".format(*x), axis=1)
data.index = pd.to_datetime(timestamps, errors="coerce")
data = data.drop(datetime_cols, axis=1)

# 496507,2021,5,30,16,34,49,7.21,11,0.8,82.1,308,0,47.68717575,-122.3899612,20.25,101214.11,9.24,46.61
# TODO: Read these from command line
offset_ms = 427309 # offset from video time to get data time.
launch_time = data.index[130] #130 is the row representing launch time

class AltitudeOverlay(PlotOverlay):
  def __init__(self, layout):
    super().__init__(layout)
    locator = mdates.AutoDateLocator(minticks=3, maxticks=7)
    formatter = mdates.ConciseDateFormatter(locator)
    self.ax.xaxis.set_major_locator(locator)
    self.ax.xaxis.set_major_formatter(formatter)

  def update(self, df):
    self.ax.plot(df.index, df['gps_altitude_m'], color="tab:blue")
    
    altitude = df['gps_altitude_m'].iloc[-1]
    self.ax.set_title('Altitude: {}m'.format(altitude))
    
    super().update()

class LocationOverlay(PlotOverlay):
  def __init__(self, layout):
    super().__init__(layout)

    ratio = float(layout.width) / layout.height
    map_width=3e6
    map_height = (map_width / ratio)


    padding = 0.005
    self.map = Basemap(epsg=4326,
      lat_0 = data['latitude'].mean(),
      llcrnrlat=data['latitude'].min() - padding,
      urcrnrlat=data['latitude'].max() + padding,
      llcrnrlon=data['longitude'].min() - padding,
      urcrnrlon=data['longitude'].max() + padding,
      lon_0 = data['longitude'].mean())

    self.map.arcgisimage(service='ESRI_Imagery_World_2D')

  def update(self, df):
    self.ax.plot(df['longitude'], df['latitude'], color='red')

    #self.ax.plot(x, y, 'ok', markersize=5)
    #self.ax.text(x, y, ' Seattle', fontsize=12)
    
    super().update()

altitude_overlay = AltitudeOverlay(Layout(0, 0, 320, 240))
location_overlay = LocationOverlay(Layout(0, 240, 320, 240))

# plot it
#altitude_fig, altitax = plt.subplots()
#fig = plt.figure(figsize=(8, 8))
#
# m.etopo(scale=0.5, alpha=0.5)

# # Map (long, lat) to (x, y) for plotting
# x, y = m(-122.3, 47.6)
# plt.plot(x, y, 'ok', markersize=5)
# plt.text(x, y, ' Seattle', fontsize=12)
def format_timedelta(td):
  prefix="T+"
  total_seconds = td.total_seconds()
  if(total_seconds < 0):
    total_seconds = -total_seconds
    prefix="T-"

  hours, remainder = divmod(total_seconds, 3600)
  minutes, seconds = divmod(remainder, 60)
  return '{}{:02}:{:02}:{:02}'.format(prefix, int(hours), int(minutes), int(seconds))

@timebudget
def update_overlays(data_position_ms):
  """Update all the overlays with current data."""
  # Get all the data before on on the current time.
  mission_data = data[data["millis"] <= data_position_ms]
  current_row = mission_data.iloc[-1]
  current_time = mission_data.index[-1]
  mission_time = current_time - launch_time

  altitude_overlay.update(mission_data)
  location_overlay.update(mission_data)

@timebudget
def main():
  next_update = 0
  max_value = input.duration_ms if args.max_seconds is None else args.max_seconds * 1000
  progress_bar = progressbar.ProgressBar(max_value=max_value)
  while(input.running):
    with timebudget('reading frame'):
      frame = input.read()

    if args.max_seconds is not None and frame.position_ms > (args.max_seconds * 1000):
      break

    if frame.position_ms > next_update:
      data_position_ms = (frame.position_ms + offset_ms)*20
      update_overlays(data_position_ms)
      next_update = frame.position_ms + 1000

    with timebudget("drawing"):
      altitude_overlay.draw(frame.image)
      location_overlay.draw(frame.image)

    #cv2.putText(frame.image, format_timedelta(mission_time),(50,600),cv2.FONT_HERSHEY_COMPLEX_SMALL,4,(225,255,255))

    if(args.live_view == True):
      if(cv2.waitKey(25) & 0xFF == ord('q')):
        break

      with timebudget("Showing frame"):
        cv2.imshow("Output Video", frame.image)
    
    if output is not None:
      with timebudget("Writing frame"):
        output.write(frame.image)

    progress_bar.update(int(frame.position_ms))

  # When everything done, release the video capture object
  if output is not None:
    output.release()

  # Closes all the frames
  cv2.destroyAllWindows()

if __name__ == "__main__":
  main()
  timebudget.report('main')