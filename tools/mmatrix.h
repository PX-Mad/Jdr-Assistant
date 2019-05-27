#ifndef MMATRIX_H
#define MMATRIX_H

#include <map>

template<typename T>
class MMatrix
{
public:

    MMatrix() {}
    void set(T value, int x, int y, int z){
        _data[std::to_string(x)+":"+std::to_string(y)+":"+std::to_string(z)] = value;
    };
    T get(int x, int y, int z)
    {
        return _data[std::to_string(x)+":"+std::to_string(y)+":"+std::to_string(z)];
    };
protected:

    std::map<std::string, T> _data;
};

#endif // MMATRIX_H
