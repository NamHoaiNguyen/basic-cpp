#include <iostream>

template<typename T>
class BstNode {
    private:
        std::shared_ptr<T> left;
        std::shared_ptr<T> right;
        T value;
    public:
        BstNode(T value) : value(std::move(value));
}

int main()
{
    return 0;
}