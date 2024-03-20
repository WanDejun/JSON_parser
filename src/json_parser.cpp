#include "json_parser.hpp"


static bool is_int(std::string& str, int strIdx) {
    while (str[strIdx] <= '9' && str[strIdx] >= '0') strIdx++;
    if (str[strIdx] == '.') return false;
    return true;
}

std::string get_str(std::string& str, int& strIdx) {
    std::string s;
    
    strIdx++; // 首 "

    while (str[strIdx] != '"') {
        s = s + str[strIdx];
        strIdx++;
    }

    strIdx++; // 尾 "

    return s;
}

json_atom::json_atom() { }

json_atom::json_atom(json_atom& A) {
    type = A.type;
    cnt = A.cnt;
    p_union = A.p_union;

    (*cnt)++;
}

json_atom::json_atom(json_atom&& A) {
    type = A.type;
    cnt = A.cnt;
    p_union = A.p_union;

    (*cnt)++;
}

json_atom::json_atom(json_type type, std::string& str, int& strIdx) {
    this->type = type;
    
    cnt = new int(1);

    switch (type) {
    case STR:
        p_union = (void*)(new json_string(str, strIdx));
        break;
    case LIST:
        p_union = (void*)(new json_list(str, strIdx));
        break;
    case BOOL:
        p_union = (void*)(new json_bool(str, strIdx));
        break;
    case DICT:
        p_union = (void*)(new json_dict(str, strIdx));
        break;
    case DOUBLE:
        p_union = (void*)(new json_double(str, strIdx));
        break;
    default:
        p_union = (void*)(new json_int(str, strIdx));
        break;
    }
}

const json_atom& json_atom::operator=(json_atom& A) {
    if (cnt != nullptr) {
        if (*(cnt) == 1) {
            switch (type) {
            case STR:
                delete (json_string*) p_union;
                break;
            case LIST:
                delete (json_list*) p_union;
                break;
            case BOOL:
                delete (json_bool*) p_union;
                break;
            case DICT:
                delete (json_dict*) p_union;
                break;
            case DOUBLE:
                delete (json_double*) p_union;
                break;
            default:
                delete (json_int*) p_union;
                break;
            }
            delete cnt;
        }
        else (*cnt)--;

        p_union = nullptr;
        cnt = nullptr;
    }
    if (A.p_union != nullptr) {
        type = A.type;
        cnt = A.cnt;
        p_union = A.p_union;

        (*cnt)++;
    }
    return *this;
}

const json_atom& json_atom::operator=(json_atom&& A) {
    if (cnt != nullptr) {
        if (*(cnt) == 1) {
            switch (type) {
            case STR:
                delete (json_string*) p_union;
                break;
            case LIST:
                delete (json_list*) p_union;
                break;
            case BOOL:
                delete (json_bool*) p_union;
                break;
            case DICT:
                delete (json_dict*) p_union;
                break;
            case DOUBLE:
                delete (json_double*) p_union;
                break;
            default:
                delete (json_int*) p_union;
                break;
            }
            delete cnt;
        }
        else (*cnt)--;

        p_union = nullptr;
        cnt = nullptr;
    }
    if (A.p_union != nullptr) {
        type = A.type;
        cnt = A.cnt;
        p_union = A.p_union;

        (*cnt)++;
    }
    return *this;
}

json_atom::~json_atom() {
    if (*cnt == 1) {
        switch (type) {
        case STR:
            delete (json_string*)p_union;
            break;
        case LIST:
            delete (json_list*)p_union;
            break;
        case BOOL:
            delete (json_bool*)p_union;
            break;
        case DICT:
            delete (json_dict*)p_union;
            break;
        case DOUBLE:
            delete (json_double*)p_union;
            break; 
        default:
            delete (json_int*)p_union;
            break;
        }
        delete cnt;
    }
    else (*cnt)--;
}


json_int::json_int(std::string& str, int& strIdx) {
    val = 0;
    bool flag = 0;

    if (str[strIdx] == '-') {
        flag = 1;
        strIdx++;
    }

    while (str[strIdx] >= '0' && str[strIdx] <= '9') {
        val = val * 10 + (str[strIdx] - '0');
        strIdx++;
    }

    if (flag) val *= -1;
}


json_double::json_double(std::string& str, int& strIdx) {
    val = 0;
    bool flag = 0;

    if (str[strIdx] == '-') {
        flag = 1;
        strIdx++;
    }

    while (str[strIdx] >= '0' && str[strIdx] <= '9') {
        val = val * 10 + (str[strIdx] - '0');
        strIdx++;
    }

    if (str[strIdx] == '.') { strIdx++; } // 读小数点

    long long t = 10;
    while (str[strIdx] >= '0' && str[strIdx] <= '9') {
        val += double(str[strIdx] - '0') / t;
        t *= 10;
        strIdx++;
    }

    if (str[strIdx] == '.') throw;

    if (flag) val *= -1;
}


json_string::json_string(std::string& str, int& strIdx) : val() {
    val = get_str(str, strIdx);
}


json_bool::json_bool(std::string& str, int& strIdx) : val() {
    if (str[strIdx] == 'f') {
        val = false;
        strIdx += 5;
    }
    else {
        val = true;
        strIdx += 4;
    }
}


json_list::json_list(std::string& str, int& strIdx) {
    int idx = 0;

    strIdx++; //首字母为‘['

    while (str[strIdx] != ']') {
        switch (str[strIdx]) {
        case 'f': // bool
        case 't':
            val.push_back(json_atom(BOOL, str, strIdx));
            break;

        case '{': // dict
            val.push_back(json_atom(DICT, str, strIdx));
            break;

        case '\"': //string
            val.push_back(json_atom(STR, str, strIdx));
            break;

        case '[': // list
            val.push_back(json_atom(LIST, str, strIdx));
            break;  
         
        default:
            if (is_int(str, strIdx)) //int
                val.push_back(json_atom(INT, str, strIdx));
            else // double
                val.push_back(json_atom(DOUBLE, str, strIdx));
            break;
        }

        while (str[strIdx] != ',' && str[strIdx] != ']') {
            strIdx++;
        }

        if (str[strIdx] == ',') {
            strIdx++;
        }
        idx++;
    }

    strIdx++; // 尾 ] 
}

json_atom& json_list::operator[] (int idx) {
    return val[idx];
}


json_dict::json_dict(std::string& str, int& strIdx) : val() {
    strIdx++; //首字母为‘{'

    while (str[strIdx] != '}') {
        std::string key = get_str(str, strIdx);

        strIdx++; // : 号

        switch (str[strIdx]) {
        case 'f':
        case 't': // bool
            val.insert({key, json_atom(BOOL, str, strIdx)});
            break;

        case '{': // dict
            val.insert({key, json_atom(DICT, str, strIdx)});
            break;

        case '\"': //string
            val.insert({key, json_atom(STR, str, strIdx)});
            break;

        case '[': // list
            val.insert({key, json_atom(LIST, str, strIdx)});
            break;   

        default:
            if (is_int(str, strIdx)) // int
                val.insert({key, json_atom(INT, str, strIdx)});
            else // double
                val.insert({key, json_atom(DOUBLE, str, strIdx)});
            break;

        }

        /* 处理尾部 */
        while (str[strIdx] != ',' && str[strIdx] != '}') { 
            strIdx++;
        }

        if (str[strIdx] == ',') {
            strIdx++;
        }
    }
    
    strIdx++; // 尾大括号 }
}

json_atom& json_dict::operator[] (std::string idx) { // 访问DICT
    if (val.find(idx) != val.end()) {
        return val[idx];
    }
    throw;
}

void DynamicJsonDocument::removeBlock(std::string& strTar, std::string strSrc) { // 移出空白符号
    strTar = std::string();
    for (int i = 0; i < strSrc.length(); i++) {
        if (strSrc[i] != ' ' && strSrc[i] != '\t' && strSrc[i] != '\n' && strSrc[i] != '\r') {
            strTar += strSrc[i];
        }
    }
}


DynamicJsonDocument::DynamicJsonDocument(std::string str) {
    int strIdx = 0;
    removeBlock(str, str);
    switch (str[strIdx]) {
    case 'f': // bool
    case 't':
        json_root = json_atom(BOOL, str, strIdx);
        break;

    case '{': // dict
        json_root = json_atom(DICT, str, strIdx);
        break;

    case '\"': //string
        json_root = json_atom(STR, str, strIdx);
        break;

    case '[': // list
        json_root = json_atom(LIST, str, strIdx);
        break;  
        
    default:
        if (is_int(str, strIdx)) //int
            json_root = json_atom(INT, str, strIdx);
        else // double
            json_root = json_atom(DOUBLE, str, strIdx);
        break;
    }
}