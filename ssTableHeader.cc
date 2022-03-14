#include "ssTableHeader.h"

void ssTableHeader::readHeader(const std::string& path) {
    std::ifstream in;
    in.open(path, std::ios_base::in | std::ios_base::binary);
    if (!in.is_open()) {
        std::cerr << "Fatal: failed to open file " << path << std::endl; 
        exit(1);
    }
    
    in.seekg(0, std::ios_base::beg);
    in.read((char*)&timestamp, sizeof(uint64_t));
    in.read((char*)&num, sizeof(uint64_t));
    in.read((char*)&min, sizeof(uint64_t));
    in.read((char*)&max, sizeof(uint64_t));

    if (!in.good()) {
        std::cerr << "Fatal: Read header error on " << path << std::endl; 
        exit(1);
    }
    in.close();
}

void ssTableHeader::writeHeader(const std::string& path) {
    std::ofstream out;
    out.open(path, std::ios_base::out | std::ios_base::binary);
    if (!out.is_open()) {
        std::cerr << "Fatal: failed to open file " << path << std::endl; 
        exit(1);
    }

    out.seekp(0, std::ios_base::beg);
    out.write((char*)&timestamp, sizeof(uint64_t));
    out.write((char*)&num, sizeof(uint64_t));
    out.write((char*)&min, sizeof(uint64_t));
    out.write((char*)&max, sizeof(uint64_t));

    if (!out.good()) {
        std::cerr << "Fatal: Write header error on" << path << std::endl;
        exit(1);
    }
    out.close();
}