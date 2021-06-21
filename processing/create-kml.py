import argparse
import pandas as pd
import simplekml

parser = argparse.ArgumentParser(description='Creates a KML from a log data CSV.')
parser.add_argument('--input', type=argparse.FileType('r'), required=True)
parser.add_argument('--output', type=str, required=True)
args = parser.parse_args()

data = pd.read_csv(args.input, na_values="*", index_col=0)

kml = simplekml.Kml()

simplekml.LineString()
path = kml.newlinestring(name='Actual path')
path.coords = data[['latitude','longitude','pressure_altitude_m']].to_numpy()
path.extrude = 1
path.altitudemode = simplekml.AltitudeMode.absolute

kml.save(args.output)