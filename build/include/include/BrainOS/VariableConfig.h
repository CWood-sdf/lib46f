#ifndef VARIABLE_CONFIG_H
#define VARIABLE_CONFIG_H
#include "vex.h"
#include <functional>
class VariableConfig
{
    // The confirm and deny buttons
    static Button confirm;
    static Button deny;
    // The list of all the instances
    static inline LinkedList<VariableConfig*> variables = LinkedList<VariableConfig*>();
    int index = -1;
    int defaultIndex = -1;
    vector<string> optionNames;
    bool selected = false;
    bool ready = false;
    vector<Button*> buttons = vector<Button*>();
    function<bool()> bypass = []()
    { return false; };
    // A callback function for when the variable is finally selected
    function<void(int)> callback = [](int) {};
    string title = "";

    /**
     * @brief draws the individual config screen
     *
     * @return true is done
     * @return false is not done
     */
    bool draw();

public:
    VariableConfig(VariableConfig& other) = default;
    /**
     * @brief Construct a new Variable Config object
     *
     * @param options The string options to choose from
     * @param title The title of the variable
     */
    VariableConfig(vector<string> options, string title);
    /**
     * @brief Construct a new Variable Config object
     *
     * @param options The string options to choose from
     * @param title The title of the variable
     * @param defaultOption The default option index
     */
    VariableConfig(vector<string> options, string title, int defaultOption);
    /**
     * @brief Construct a new Variable Config object
     *
     * @param options The string options to choose from
     * @param title The title of the variable
     * @param callback A callback function for when the variable is finally selected
     */
    VariableConfig(vector<string> options, string title, function<void(int)> callback);
    /**
     * @brief Construct a new Variable Config object
     *
     * @param options The string options to choose from
     * @param title The title of the variable
     * @param defaultOption The default option index
     * @param callback A callback function for when the variable is finally selected
     */
    VariableConfig(vector<string> options, string title, int defaultOption, function<void(int)> callback);
    /**
     * @brief set the bypass function for the variable
     *
     * @param bypass a function that causes the configuration to skip when it returns true
     * @return VariableConfig& *this
     */
    VariableConfig& setBypass(function<bool()> bypass);
    /**
     * @brief Changes an option's name
     *
     * @param index the index of the option
     * @param name the new name of the option
     */
    void setOptionName(int index, string name);
    /**
     * @brief Sets the callback function for when the variable is finally selected
     *
     * @param callback the callback function
     */
    void setCallback(function<void(int)> callback);
    /**
     * @brief Adds an option to the list of options
     *
     * @param option the option to add
     */
    void addOption(string option);
    /**
     * @brief Adds multiple options to the list of options
     *
     * @param options the options to add
     */
    void addOptions(vector<string> options);
    /**
     * @brief Returns the selected option
     *
     * @return string the selected option
     */
    string getSelectedOption();
    /**
     * @brief Returns if the variable is selected
     *
     * @return true the variable is selected
     * @return false the variable is not selected
     */
    bool isSelected();
    /**
     * @brief Returns if the variable can be defaulted
     *
     * @return true the variable can be defaulted
     * @return false the variable is not defaulted
     */
    bool hasDefault();
    /**
     * @brief Draws all the config screens
     *
     * @return true is done
     * @return false is not done
     */
    static bool drawAll(bool);
};
#endif
