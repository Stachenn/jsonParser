#pragma once

#include <iostream>
#include <cstring>

#include "parser.hpp"

/// @brief Create jsonParser object
/// @param content json content to parse can be change later
/// @return nothing

//jsonParser::addToTable()
jsonParser::jsonParser(std::string content){
    this->content = content;
}

/// @brief Delete jsonParser object
jsonParser::~jsonParser(){
}

int jsonParser::find(std::string varibleName){
    for (int i = 0; i < this->varNames.size()-1; i++){
        //std::cout << this->varNames[i];
        if (this->varNames[i] == varibleName){
            return i;
        }
    }
    return -1;
}


/*const char** jsonParser::getAll(std::vector <std::string> table){
    const char* tabl[table.size()-1];
    for (int i = 0; i < table.size()-1; i++){
        tabl[i] = table[i].c_str();
    }
    return tabl;
}
*/

int jsonParser::getType(){
    int i = 0;
    int j = std::string(content).length();

    while (true){
        if (std::string(content).length() < i){
            return -1;
            break;
        }
        if (content[i] != ' ' || (int)content[i] == 0){
            break;
        }
        i++;
    }
    j = content.size()-1;
    while (true){
        if (0 > j){
            return -1;
            break;
        }
        if (content[j] != ' ' || content[j] != '\0' || content[j] != 0 || content[j] != 10 || content[j] != 13 || content[j] != '\n'){
            break;
        }
        j--;
    }
    if (content[i] == '[' && content[j] == ']'){
        return 1;
    }
    if (content[i] == '{' && content[j] == '}'){
        return 0;
    }
    else{
        return -1;
    }
}

int jsonParser::parse(){
    std::string content = this->content;
    std::string cache = "";

    bool isInObject = false;
    bool isInString = false;
    bool isObject = false;
    bool elseBool = false;
    bool copyElseBool = false;
    bool scientificNotation = false;
    bool isFloat = false;
    bool isNegativeNum = false;
    bool afterParsingObject = false;

    int multiplayerE = 0;
    int errorcode = 0;
    int objectSkips = 0;
    int index = 0;
    int copyIndex = 0;
    int skips = 0;
    int type = getType();
    int currentVarAmount = 0;
    if (type == 1){
        objectSkips++;
    }

    while (true){
        // Start saving var name
        isObject = false;
        elseBool = false;
        afterParsingObject = false;
        if (content[index] == '"' && type != 1){
            elseBool = true;
            while (true){
                index++;

                if (content[index] == '"'){
                    varNames.push_back(cache);
                    cache = "";
                    break;
                }
                cache += content[index];
            }
        }
        if (content[index] == ':' || (type == 1 && (content[index] != ' ') || (type == 1 && content[index] == ','))){
            elseBool = true;
            skips = 0;
            index++;

            if (type == 1){
                varNames.push_back(std::to_string(varAmount));
            }

            while (true){
                if (content[index] != ' ' && content[index] != 0 && content[index] != 10){
                    break;
                }
                index++;
            }

            while (true){
                if (content[index] == '"'){
                    isInString = !isInString;
                }
                if (content[index] == '/' && content[index+1] == '"'){
                  index++;
                }
                if ((content[index] == '{' || content[index] == '[') && !isInString){
                    isInObject = true;
                    isObject = true;
                    skips++;
                }
                if (content[index] == '}' || content[index] == ']' && !isInString){
                    skips--;
                }
                if (isInObject && skips == 0){
                    isInObject = false;
                }
                if ((content[index] == ',' || content[index] == '}' || content[index] == ']') && (!isInObject && !isInString)){
                    copyIndex = cache.length();
                    std::cout << varAmount;

                    while (true){

                        if (cache[copyIndex] != ' ' && cache[copyIndex] != 0 && cache[copyIndex] != '\n'){
                            if (!isInString){
                              break;
                            }
                            if (cache[copyIndex] != '}' && cache[copyIndex] != ']'){
                              break;
                            }
                        }
                        cache.erase(copyIndex, 1);
                        copyIndex--;
                    }
                    if ((content[index] == '}' || content[index] == ']') && isObject){
                        cache += content[index];
                    }
                    varValues.push_back(cache);
                    cache = "";
					          skips = 0;
                    varAmount++;

                    break;
                }
                cache += content[index];
                index++;
            }
        }

        if (varAmount > currentVarAmount){
            //std::cout << '*' << varValues[varAmount-1] << '*';
            copyElseBool = true;
            //while (true){
            if (varValues[varAmount-1] == "false" || varValues[varAmount-1] == "true"){
                copyElseBool = true;
                varTypes.push_back("bool");
            }
            if (varValues[varAmount-1] == "null"){
                copyElseBool = true;
                varTypes.push_back("null");
            }
            if (varValues[varAmount-1][0] == '"' && varValues[varAmount-1][varValues[varAmount-1].length()-1] == '"'){
                std::cout << varValues[varAmount-1];
                copyElseBool = true;
                varTypes.push_back("string");
                std::cout << content[index] << '@';

            }
            if (varValues[varAmount-1][0] > 47 && varValues[varAmount-1][0] < 58){
                copyElseBool = true;
                for (int i = 0; i < varValues[varAmount-1].length()-1; i++){
                    if (varValues[varAmount-1][i] == '-' && !isNegativeNum){
                        isNegativeNum = true;
                    }
                    if ((varValues[varAmount-1][i] == 'e' || varValues[varAmount-1][i] == 'E') && !scientificNotation){
                        scientificNotation = true;
                        isNegativeNum = false;
                    }
                    if (varValues[varAmount-1][i] == '.' && !isFloat){
                        isFloat = true;
                    }
                    else{
                        if (!(varValues[varAmount-1][i] > 47 && varValues[varAmount-1][i] < 58)){
                            return JSON_TYPE_ERROR;
                        }
                    }
                }
                varTypes.push_back("int");
                scientificNotation = false;
                isNegativeNum = false;
                isFloat = false;
            }
            if (varValues[varAmount-1][0] == '{' && varValues[varAmount-1][varValues[varAmount-1].length()-1] == '}' ||
                varValues[varAmount-1][0] == '[' && varValues[varAmount-1][varValues[varAmount-1].length()-1] == ']'   ){
                copyElseBool = true;

                if (varValues[varAmount-1][0] == '[' && varValues[varAmount-1][varValues[varAmount-1].length()-1] == ']'){
                    varTypes.push_back("table");
                }
                else{
                    varTypes.push_back("object");
                }

                jsonParser object(varValues[varAmount-1].c_str());

                int returnCode = object.parse();

                if (returnCode != JSON_OK){
                    std::cout << "aha";
                    return returnCode;
                }

                std::vector<std::string> objNames = object.getNames();
                std::vector<std::string> objTypes = object.getTypes();
                std::vector<std::string> objValues = object.getValues();
                std::cout << content[index] << '@';
                for (int i = 0; i < object.varAmount; i++){

                    varTypes.push_back(objTypes[i]);
                    varNames.push_back(objNames[i]);
                    varValues.push_back(objValues[i]);
                }
                varAmount += object.varAmount;
                currentVarAmount += object.varAmount;
                index++;

            }

            if (copyElseBool == false){
                return JSON_TYPE_ERROR;
            }

            copyElseBool = false;

            currentVarAmount++;
        }

        if (content[index] == '{' || content[index] == '[' && !afterParsingObject){
            elseBool = true;
            objectSkips++;
        }
        if (content[index] == '}' || content[index] == ']' && !afterParsingObject){
            elseBool = true;
            objectSkips--;
        }
        if (objectSkips == 0 && !afterParsingObject){
            elseBool = true;
            std::cout << '-' << content[index] << '-';
            return JSON_OK;
        }

        if (!elseBool && !afterParsingObject){
            if (content[index] != ' ' && content[index] != 10){
                std::cout << ":|" << index << "|:";
                std::cout << ':' << content[index] << ':';
                return JSON_UNEXPECTED_CHAR;
            }
            //std::cout << content[index];
        }
        index++;
    }
}
