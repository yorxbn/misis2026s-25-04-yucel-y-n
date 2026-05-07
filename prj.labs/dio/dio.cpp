#include <iostream>
#include <string>
#include <sstream>

class DioStrB{
private:
    std::string potok;
    size_t pos;
public:
    DioStrB() : pos(0) {}
    DioStrB(const std::string& str) : potok(str), pos(0) {}
    DioStrB& operator<<(const std::string& str) {
        potok +=str;
        return *this;
    }
    DioStrB& operator<<(int num) {
        std::stringstream ss;
        ss << num;
        potok += ss.str();
        return *this;
        
    }
    DioStrB& operator<<(double num) {
        std::stringstream ss;
        ss << num;
        potok += ss.str();
        return *this;
    }
    DioStrB& operator<<(char ch) {
        potok += ch;
        return *this;
    }
    DioStrB& operator<<(char* ch) {
        potok += ch; 
        return *this;
    }
    DioStrB& operator>>(std::string& out) {
        if (pos >= potok.length()) {
            out="";
            return *this;
        }
        size_t end = potok.find(' ',pos);
        if (end == std::string::npos) {
            out = potok.substr(pos) ;
            pos = potok.length();
        } else {
            out = potok.substr(pos, end - pos);
            pos = end+1;
        }
        return *this;
    }
    std::string val() {
        return potok;
    }
    std::string& str() {
        return potok;
    }
    const std::string&  str() const {
        return potok;
    }
};

int main() {

}
