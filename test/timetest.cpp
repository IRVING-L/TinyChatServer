#include <iostream>
#include <chrono>
#include <ctime>
using myClock = std::chrono::system_clock;

std::string getCurrentTime()
{
    myClock::time_point rightNow = myClock::now();
    std::time_t tt = myClock::to_time_t(rightNow);
    char *current = ctime(&tt);
    std::string ret(current, sizeof(current));
    return ret;

}

int main()
{
    myClock::time_point rightNow = myClock::now();
    std::time_t tt = myClock::to_time_t(rightNow);
    char *current = ctime(&tt);
    std::string s(current);
    s.erase(s.end()-1);
    std::cout << s << std::endl;

    return 0;
}