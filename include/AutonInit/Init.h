#include "BrainOS/VariableConfig.h"
#include "Sensors/PotDial.h"
#include "stuff.h"
class Auton
{
    static inline size_t idSet = 0;
    static inline std::vector<Auton*> refList;
    static inline std::vector<Button*> buttons;
    static inline Auton* auton;
    static inline bool autonSelected = false;
    static inline bool ready = false;
    static void configCallback(int i);
    static inline VariableConfig conf = VariableConfig({}, "Auton", configCallback);
    string name;
    size_t index;
    function<void()> fn;
    SelectorArr potGet;
    typedef Auton& chain_method;
    chain_method setFn(std::function<void()> a);
    chain_method setName(string n);
    Auton();

public:
    Auton(string name, std::function<void()> fn);
    Auton(string name, vector<int> v);
    Auton(const Auton&) = default;
    Auton(Auton&&) = default;
    chain_method call();
    Auton& attachArr(vector<int> v);
    string getName();
    static void callAuton();

    Auton& operator+(std::function<void()> fn);
    Auton& operator+(vector<int> arr);
    static bool isSelected();
    static string selectedName();
};
Auton operator+(const char* n, std::function<void()> f);
Auton operator+(const char* n, vector<int> v);