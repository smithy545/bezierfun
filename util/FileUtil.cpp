//
// Created by Philip on 3/2/2020.
//

#include "FileUtil.h"

#include <iostream>
#include <fmt/format.h>
#include <fstream>
#include <sstream>


const std::string FileUtil::RESOURCE_PREFIX = "../res/{0}";

std::string FileUtil::resource(std::string relativePath) {
    return fmt::format(RESOURCE_PREFIX, relativePath);
}

char *FileUtil::readResourceFile(const std::string &resourcePath) {
    std::ifstream resourceStream(resource(resourcePath), std::ifstream::ate);
    if (resourceStream) {
        // get file length (ifstream::ate flag puts cursor at end of file)
        int length = resourceStream.tellg();
        resourceStream.seekg(0, std::ios_base::beg);

        // store in buffer
        char *buf = new char[length];
        int count = 0;
        while (resourceStream.read(buf + count, length - count) || resourceStream.gcount() != 0) {
            count += resourceStream.gcount();
        }
        resourceStream.close();

        // null terminate at actual read file count
        buf[count] = '\0';

        // MUST BE DELETED WHEN FINISHED WITH USE. TODO: Find safer (no memleaks) way to read file
        return buf;
    }

    // open failed for some reason
    std::string message = fmt::format("Can't read file at {0}", resource(resourcePath));
    throw std::exception(message.c_str());
}


json FileUtil::readJsonFile(const std::string &jsonPath) {
    std::ifstream file(resource(jsonPath));
    if (file.good())
        return json::parse(file);
    std::string message = fmt::format("Can't read json file at {0}", resource(jsonPath));
    throw std::exception(message.c_str());
}

TiXmlDocument FileUtil::readXmlFile(const std::string &xmlPath) {
    TiXmlDocument file(resource(xmlPath).c_str());
    if (file.LoadFile())
        return file;

    std::string message = fmt::format("Can't read xml file at {0}", resource(xmlPath));
    throw std::exception(message.c_str());
}

void FileUtil::writeJsonFile(const std::string &jsonPath, const json &data) {
    std::ofstream file;
    file.open(resource(jsonPath), std::ofstream::out);
    file << data;
    file.close();
}
