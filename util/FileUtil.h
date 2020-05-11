//
// Created by Philip on 3/2/2020.
//

#ifndef PLACEHOLDER_FILEUTIL_H
#define PLACEHOLDER_FILEUTIL_H

#include <nlohmann/json.hpp>
#include <string>
#include <tinyxml.h>

using json = nlohmann::json;

class FileUtil {
public:
    static const std::string RESOURCE_PREFIX;

    static char *readResourceFile(const std::string &resourcePath);

    static json readJsonFile(const std::string &jsonPath);

    static TiXmlDocument readXmlFile(const std::string &xmlPath);

    static void writeJsonFile(const std::string &jsonPath, const json &data);

private:
    static std::string resource(std::string relativePath);
};


#endif //PLACEHOLDER_FILEUTIL_H
