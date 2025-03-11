
# Simply (and buggy) c++ parser

# Available support :
|           []           | Yes | No | Experimental (implemented) | Probably (tested) | Not tested(I dont know)(not implemented) |
|:----------------------:|-----|----|----------------------------|-------------------|------------------------------------------|
| Int                    | ✅   |    |                            |                   |                                          |
| Float/double           |     |    | ✅                          |                   |                                          |
| String                 | ✅   |    |                            |                   |                                          |
| Objects                | ✅   |    |                            |                   |                                          |
| Arrays                 | ✅    |    |                            |                  |                                          |
| Arrays (as main type)  |   ✅  |    |                            |                  |                                          |
| Objects (as main type) | ✅   | |                            |                   |                                          |
| Unicode                |     | |                            |                   | ✅                                        |
| Comments               |     | ✅  |                            |                   |                                          |
| null                   |  ✅   |     |                            |                   |                                          |

# How to use its *easy **trust me** (mhm)*

### Sample usage:


```cpp
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
```
or you can download example file at [example](example/example.cpp)
## Others
If you have any suggestion's / bug's don't be afraid and write your suggestion in "Issues - > Idea -> New Issue -> Labels -> Idea" "Issues - > New Issue - > Lables -> [select type]"
## License

[MIT](https://github.com/Stachenn/Blender-Sddm-Theme/blob/main/LICENSE)


## Functions and varibles

## ==========
## Functions
## ==========

`+` = or other compatible type with std::string

* ## .jsonParser(std::string);
```cpp
jsonParser parser(""); 
jsonParser parser1();
```
| Parameter | Return Type     | Description                |
| :-------- | :------- | :------------------------- |
| `std::string`+ - content(optional) | void(nothing) | Init parser |


* ### .parse()

```cpp
parser.parse();
```

| Arguments | Return Type     | Description                |
| :-------- | :------- | :------------------------- |
| none | `int` (error code) | Parsing **.content** varible |

## =============

* ### .getNames();

```cpp
parser.getNames();
```

| Arguments | Return Type     | Description                |
| :-------- | :------- | :------------------------- |
| none | `std::vector<string>` | **Retruns names** of var's from parsed **.content**|

## =============

* ### .getValues();

```cpp
parser.getValues();
```

| Arguments | Return Type     | Description                |
| :-------- | :------- | :------------------------- |
| none | `std::vector<string>` | **Retruns values** of var's from parsed **.content**|

## =============

* ### .getTypes();

```cpp
parser.getTypes();
```

| Arguments | Return Type     | Description                |
| :-------- | :------- | :------------------------- |
| none | `std::vector<string>` | **Retruns types** of var's from parsed **.content**|

## =============

* ### .find(std::string varibleName);

```cpp
parser.find("xyz");
```

| Arguments | Return Type     | Description                |
| :-------- | :------- | :------------------------- |
| `std::string`+ - varibleName name to search | `std::vector<string>` | **Retruns index** of first varible that name matches **varibleName** or -1 if not founded|

## =============
## Varibles
## =============
* ### .content;
```cpp
parser.content = "xyz";
```
| Usage | Type     
| :-------- | :------- |
| When .parse() will be called this will be used as content to parse  | **std::string** or other **comatible type with std::string** |

## Authors

### Stachenn 2023-2025

