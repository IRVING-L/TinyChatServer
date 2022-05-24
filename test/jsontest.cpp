#include "json.hpp"

using json = nlohmann::json;

#include <iostream>
#include <string>
#include <map>

int main(){

    json js;
    js["name"] = "irving";
    js["nums"] = {1,2,3,4,5};
    js["label1"]["label2"] = "text";
    std::map<int, std::string> mymap;
    mymap.insert({1, "a"});
    js["mapValue"] = mymap;
    std::cout << js << std::endl;
    std::string str = js.dump();
    std::cout << str << std::endl;
    return 0;
}