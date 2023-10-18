#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    // 创建随机数引擎对象
    default_random_engine e;
    // 创建随机数分布对象
    uniform_int_distribution<int> d(1, 100);
    // 创建容器对象
    vector<int> v(10);
    // 填充容器
    generate(v.begin(), v.end(), [&]()->int{return d(e);});
    // 打印容器
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}
