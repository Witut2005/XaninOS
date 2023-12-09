import { MODIFIER_PREFIX } from "./xanc_structures";

export function getAllIndexes(mainString: string, subString: string) {
  let indexes = [];
  let index: number = mainString.indexOf(subString);

  while (index !== -1) {
    indexes.push(index);
    index = mainString.indexOf(subString, index + 1);
  }

  return indexes;
}

export function extractSubstring(inputString: string): string {
  return inputString.slice(inputString.indexOf(MODIFIER_PREFIX));
}
