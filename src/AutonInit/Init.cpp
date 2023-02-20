#include "AutonInit/Init.h"

void Auton::configCallback(int i)
{
    auton = refList[i];
    autonSelected = true;
    ready = true;
    cout << "Auton Selected" << endl;
}

Auton::chain_method Auton::setFn(std::function<void()> a)
{
    this->fn = a;
    CHAIN;
}
Auton::chain_method Auton::setName(string n)
{
    conf.setOptionName(index, n);
    name = n;
    CHAIN;
}
Auton::Auton()
{
    conf.addOption("");
    index = idSet++;
    refList.push_back(this);
}
Auton::Auton(string name, std::function<void()> fn) : Auton()
{
    setName(name);
    setFn(fn);
}
Auton::Auton(string name, vector<int> v) : Auton()
{
    setName(name);
    potGet = SelectorArr(v);
}
Auton::chain_method Auton::call()
{
    fn();
    CHAIN;
}
Auton& Auton::attachArr(vector<int> v)
{
    potGet = SelectorArr(v, fn);
    return *this;
}
string Auton::getName()
{
    return name;
}
void Auton::callAuton()
{
    if (refList.size() == 0)
    {
        cout << "No Auton" << endl;
        return;
    }
    if (ready)
    {
        auton->call();
    }
    else
    {
        SelectorArr::getVal()();
    }
}

Auton& Auton::operator+(std::function<void()> fn)
{
    setFn(fn);
    potGet.attachFn(fn);
    return *this;
}
Auton& Auton::operator+(vector<int> arr)
{
    attachArr(arr);
    return *this;
}
bool Auton::isSelected()
{
    return ready;
}
string Auton::selectedName()
{
    if (auton == NULL)
    {
        return "";
    }
    return auton->getName();
}
Auton operator+(const char* n, std::function<void()> f)
{
    return Auton(n, f);
}
Auton operator+(const char* n, vector<int> v)
{
    return Auton(n, v);
}
