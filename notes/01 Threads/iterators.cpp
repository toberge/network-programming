#include <iostream>
#include <list>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    vector<string> things = {"hi", "there"};
    //vector<string> things;

    auto it = things.begin();
    cout << *it << endl;
    it++;

    things.emplace_back("spook");

    cout << *it << endl;
    it++;
    cout << *it << endl;
    
    return 0;
}
