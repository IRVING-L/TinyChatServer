#include <iostream>

class Single1
{
public:
    static Single1 *instance();
private:
    Single1() = default;
};
Single1 *Single1::instance()
{
    std::cout << "instance" << std::endl;
    static Single1 _s;
    return &_s;
}

int main(int argc, char const *argv[])
{
    std::cout << "饿汉模式测试" << std::endl;
    //Single1::instance();
    return 0;
}
