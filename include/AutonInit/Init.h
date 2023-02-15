#include "BrainOS/VariableConfig.h"
#include "Sensors/PotDial.h"
#include "vex.h"
class Auton
{
    static inline size_t idSet = 0;
    static inline std::vector<Auton*> refList;
    static inline std::vector<Button*> buttons;
    static inline Auton* auton;
    static inline bool autonSelected = false;
    static inline bool ready = false;
    static void configCallback(int i)
    {
        auton = refList[i];
        autonSelected = true;
        ready = true;
        cout << "Auton Selected" << endl;
    }
    static inline VariableConfig conf = VariableConfig({}, "Auton", configCallback);
    string name;
    size_t index;
    function<void()> fn;
    SelectorArr potGet;
    typedef Auton& chain_method;
    chain_method setFn(std::function<void()> a)
    {
        this->fn = a;
        CHAIN;
    }
    chain_method setName(string n)
    {
        conf.setOptionName(index, n);
        name = n;
        CHAIN;
    }
    Auton()
    {
        conf.addOption("");
        index = idSet++;
        refList.push_back(this);
    }

public:
    Auton(string name, std::function<void()> fn) : Auton()
    {
        setName(name);
        setFn(fn);
    }
    Auton(string name, vector<int> v) : Auton()
    {
        setName(name);
        potGet = SelectorArr(v);
    }
    Auton(const Auton&) = default;
    Auton(Auton&&) = default;
    chain_method call()
    {
        fn();
        CHAIN;
    }
    Auton& attachArr(vector<int> v)
    {
        potGet = SelectorArr(v, fn);
        return *this;
    }
    string getName()
    {
        return name;
    }
    static inline void callAuton()
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

    Auton& operator+(std::function<void()> fn)
    {
        setFn(fn);
        potGet.attachFn(fn);
        return *this;
    }
    Auton& operator+(vector<int> arr)
    {
        attachArr(arr);
        return *this;
    }
    static bool isSelected()
    {
        return ready;
    }
    static string selectedName()
    {
        if (auton == NULL)
        {
            return "";
        }
        return auton->getName();
    }
};
#ifdef MAKE
Auton operator+(const char* n, std::function<void()> f)
{
    return Auton(n, f);
}
Auton operator+(const char* n, vector<int> v)
{
    return Auton(n, v);
}
#else
Auton operator+(const char* n, std::function<void()> f);
Auton operator+(const char* n, vector<int> v);
#endif
