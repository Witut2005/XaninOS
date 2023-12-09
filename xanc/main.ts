import fs from "fs";
import { MODIFIER_PREFIX } from "./xanc_structures";
const yargs = require("yargs");
const { hideBin } = require("yargs/helpers");
import { extractSubstring, getAllIndexes } from "./helpers";

function parser(err: any, data: string) {
  let fileData: string = "";
  if (err) {
    console.error("Error reading file:", err);
    return;
  }

  fileData = data;
  //   console.log("File content:", fileData);

  //   console.log("indexes: ", getAllIndexes(fileData, MODIFIER_PREFIX));

  //   console.log("File content:", fileData.replaceAll(MODIFIER_PREFIX, ""));
  for (const i in fileData.split("\n")) {
    console.log(extractSubstring(fileData.split("\n")[i]));
  }
}

function main(argv: any) {
  console.log(`file to read: ${argv["file"]}`);
  fs.readFile(argv["file"] as string, "utf8", (err: any, data: string) => {
    parser(err, data);
  });
}

main(yargs(hideBin(process.argv)).argv);
