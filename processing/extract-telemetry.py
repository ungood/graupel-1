import argparse

parser = argparse.ArgumentParser(description='Extracts telemetry from a GoPro binary data format.')
parser.add_argument('--input', type=argparse.FileType('rb'), required=True)
parser.add_argument('--output', type=str, required=True)
args = parser.parse_args()
