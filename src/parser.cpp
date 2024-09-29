#pragma once

#include <iostream>
#include <cstring>

#include "parser.hpp"

/// @brief Create jsonParser object
/// @param content json content to parse can be change later
/// @return nothing

jsonParser::jsonParser(std::string content){
    this->content = content;
}

/// @brief Delete jsonParser object
jsonParser::~jsonParser(){
}

int jsonParser::find(std::string varibleName){
    for (int i = 0; i < this->varNames.size(); i++){
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
        if (content[i] != ' '){
            break;
        }
        i++;
    }
    while (true){
        if (0 > j){
            return -1;
            break;
        }
        if (content[j] != ' '){
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
        elseBool = false;
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
        if (content[index] == ':' || (type == 1 && content[index] != ' ')){
            elseBool = true;
            skips = 0;
            index++;

            if (type == 1){
                varNames.push_back(std::to_string(varAmount));
            }

            while (true){
                if (content[index] != ' '){
                    break;
                }
                index++;
            }

            while (true){
                if (content[index] == '"'){
                    isInString = !isInString;
                }
                if (content[index] == '{' || content[index] == '[' && !isInString){
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
                    while (true){
                        if (cache[copyIndex] != ' ' && cache[copyIndex] != '\n' && (int)cache[copyIndex] != 0){
                            break;
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
            copyElseBool = true;
            if (varValues[varAmount-1] == "false" || varValues[varAmount-1] == "true"){
                copyElseBool = true;
                varTypes.push_back("bool");
            }
            if (varValues[varAmount-1] == "null"){
                copyElseBool = true;
                varTypes.push_back("null");
            }
            if (varValues[varAmount-1][0] == '"' && varValues[varAmount-1][varValues[varAmount-1].length()-1] == '"'){
                copyElseBool = true;
                varTypes.push_back("string");
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
                    return returnCode;
                }

                std::vector<std::string> objNames = object.getNames();
                std::vector<std::string> objTypes = object.getTypes();
                std::vector<std::string> objValues = object.getValues();

                for (int i = 0; i < object.varAmount; i++){
                    varTypes.push_back(objTypes[i]);
                    varNames.push_back(objNames[i]);
                    varValues.push_back(objValues[i]);
                }

            }
            if (copyElseBool == false){
                return JSON_TYPE_ERROR;
            }
            copyElseBool = false;

            currentVarAmount++;
        }

        if (content[index] == '{' || content[index] == '['){
            elseBool = true;
            objectSkips++;
        }
        if (content[index] == '}' || content[index] == ']'){
            elseBool = true;
            objectSkips--;
        }
        if (objectSkips == 0){
            elseBool = true;

            return JSON_OK;
        }

        if (!elseBool){
            if ((int)content[index] != 32 && content[index] != '\n' && (int)content[index] != 10 && (int)content[index] != 15 && (int)cache[copyIndex] != 0){
                return JSON_UNEXPECTED_CHAR;
            }    
        }
        index++;
    }
}
