#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>

class Metadata {
public:
    void viewMetadata(std::string fname);
    void changeMetadata(std::string fname);
    void addMetadata(std::string fname);
};

#endif