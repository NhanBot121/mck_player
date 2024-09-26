#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>

class Metadata {
public:
    static void viewMetadata(std::string fname);
    static void changeMetadata(std::string fname);
    static void addMetadata(std::string fname);
};

#endif