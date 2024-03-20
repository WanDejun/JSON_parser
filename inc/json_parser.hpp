#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <type_traits>

template <typename T, typename U>
constexpr bool is_decay_same() {
    using std::is_same, std::decay;
    return is_same<typename decay<T>::type, typename decay<U>::type>::value;
}

typedef enum {INT, DOUBLE, STR, BOOL, LIST, DICT} json_type; 

class json_atom;

class json_int{
public:
    int val;
    json_int(std::string& str, int& strIdx);

    template<typename T> 
    inline T& as() {
        if constexpr (is_decay_same<T, int>()) return val;
        throw;
    }
};

class json_double {
public:
    double val;
    json_double(std::string& str, int& strIdx);

    template<typename T>
    inline T& as() {
        if constexpr (is_decay_same<T, double>()) return val;
        throw;
    }
};

class json_string {
public:
    std::string val;
    json_string(std::string& str, int& strIdx);

    template<typename T>
    T& as() {
        if constexpr (is_decay_same<T, std::string>()) return val;
        throw;
    }
};

class json_bool {
public:
    bool val;
    json_bool(std::string& str, int& strIdx);
    template<typename T>
    T& as() {
        if constexpr (is_decay_same<T, bool>()) return val;
        throw;
    }
};

class json_list {
public:
    std::vector<json_atom> val;
    json_list(std::string& str, int& strIdx);
    json_atom& operator[] (int idx);
};

class json_dict {
public:
    std::map<std::string, json_atom> val;

    json_dict(std::string& str, int& strIdx);
    json_atom& operator[] (std::string idx);
};

class json_atom {
public:
    json_type type;
    void* p_union = nullptr;
    int* cnt = nullptr;
    
    json_atom();
    json_atom(json_type type, std::string& str, int& strIdx);
    json_atom(json_atom& A);
    json_atom(json_atom&& A);

    const json_atom& operator=(json_atom& A);
    const json_atom& operator=(json_atom&& A);

    ~json_atom();

    template<typename T>
    inline json_atom& operator[](const T& idx) {
        if (type == LIST) {
            if constexpr (is_decay_same<T, int>()) {
                return (*((json_list*)p_union))[idx];
            }
        }
        if (type == DICT ) {
            if constexpr (is_decay_same<T, char*>() || is_decay_same<T, std::string>()) {
                return (*((json_dict*)p_union))[idx];
            }
        }
        throw;
    }

    template<typename T>
    inline T& as() { // 获取值，类型为T
        switch (type)
        {
        case INT:
            return (*((json_int*)p_union)).as<T>();
            break;
        case DOUBLE:
            return (*((json_double*)p_union)).as<T>();
            break;
        case STR:
            return (*((json_string*)p_union)).as<T>();
            break;
        case BOOL:
            return (*((json_bool*)p_union)).as<T>();
            break;        
        default:
            exit(1);
            break;
        }
    }
};

class DynamicJsonDocument {
private:
    json_atom json_root;
    void removeBlock(std::string& strTar, std::string strSrc);
public:
    DynamicJsonDocument(std::string str);
    template<typename T>
    inline json_atom& operator[](const T& idx) { return json_root[idx]; }

    template<typename T>
    inline T& as() { return json_root.as<T>(); }
};
