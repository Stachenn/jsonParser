// including json lib
#include "../src/parser.cpp"
// only for output
#include <iostream>
// for storing names, values and types
#include <vector>
// for reading from files
#include <fstream>

int main(int argc, char** argv){

  std::vector<std::string> values = {};
  std::vector<std::string> names = {};
  std::vector<std::string> types = {};
  // passing nothing as content because we need
  // to read file
  jsonParser parser("");
  // opening file
  // varibles for reading from file
  int dataFileSize = 0;
  int errorCode = -1;
  std::string dataFileContent = "";

  std::ifstream dataFile("data.json", std::ios::in | std::ios::ate);
  //reading from file
  dataFileSize = dataFile.tellg();

  dataFile.seekg(0, std::ios::beg);

  for (int i = 0; i < dataFileSize-1; i++){
      dataFileContent += dataFile.get();
  }
  dataFile.close();

  // setting content to parse to data.json data
  parser.content = dataFileContent;

  // checking if parser don't return with error if yes
  // then throw error
  errorCode = parser.parse();

  if (errorCode != 1){
    std::cout << "Something went wrong, error code :" << errorCode;
    return 1;
  }

  //getting values, types, names from parser
  values = parser.getValues();
  names = parser.getNames();
  types = parser.getTypes();

  //printing parsed names, values, types
  for (int i = 0; i < values.size(); i++){
    std::cout << i << ". Varible - " << names[i] << ' ' << values[i] << ' '<< types[i] << '\n';
  }
  return 0;
}
