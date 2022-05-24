#include "sum.h"

int sum(int n)
{
    std::cout << "n: " << n << std::endl;
    int ans = 0;
    for(int i = 1; i <= n; ++i) { ans += i; }
    std::cout << "sum is: " << ans << std::endl;
    return ans;
}