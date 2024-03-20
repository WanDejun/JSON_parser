#include "json_read.hpp"
#include <fstream>  
#include <sstream>  

std::string json_read(const char* FILE_NAME) {
    std::ifstream t(FILE_NAME); 
    if (!t.is_open()) {
        throw;
    }
    std::stringstream buffer;  
    buffer << t.rdbuf();  
    return std::string(buffer.str());
}
