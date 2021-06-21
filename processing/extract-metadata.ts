import { ArgumentParser } from 'argparse';
import * as fs from 'fs';

// @ts-ignore
const gpmfExtract = require("gpmf-extract");
const goproTelemetry = require("gopro-telemetry");

const parser = new ArgumentParser({
  description: "Extracts metadata from a GoPro MP4 file."
});

parser.add_argument('--input', {required: true});
parser.add_argument('--streams', )
parser.add_argument('--output', {required: true});

const inputPath = './data/go-pro-test.mp4'
const outputPath = './data/output_path.json'

async function main() {
  const inputFile = fs.readFileSync(inputPath);
  
  const extractedData = await gpmfExtract(inputFile);
  const rawData = extractedData.rawData;
  const timing = extractedData.timing;
  console.log(`Read ${extractedData.rawData.length} bytes from ${inputPath}`);

  const telemetry = await goproTelemetry(extractedData, {
    stream: []
  });
  fs.writeFileSync(outputPath, JSON.stringify(telemetry, null, 2));
  console.log(`Telemetry saved as JSON to ${outputPath}`);
}

main()
  .then(() => console.log("done"))
  .catch(err => console.error(err));