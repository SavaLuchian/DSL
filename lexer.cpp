#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <algorithm>
using namespace std;

struct token
{
    long int number;
    double decimal;
    string name;
    string type;
} t_list[9999];
int k = 0;
std::string readFileToString(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return ""; // Return an empty string to indicate failure
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}
void tokenize(string input)
{
    int i = 0;
    while (i < input.length())
    {
        if (t_list[k - 2].type == "string") //------------------------------------
        {
            string numStr = "";
            while (input[i] != '}')
            {
                numStr += input[i];
                i++;
            }
            t_list[k].name = numStr;
            t_list[k].type = "text";
            k++;
        }
        else if (isalpha(input[i])) //------------------------------------
        {
            // Read variable/functions
            string numStr = "";
            while ((isalpha(input[i]) || ispunct(input[i]) || isdigit(input[i])) && input[i] != '{' && input[i] != '}' && input[i] != '=' && input[i] != '*' && input[i] != '+' && input[i] != '%' && input[i] != '#' && input[i] != '-' && input[i] != '/' && input[i] != ')' && input[i] != '(' && input[i] != '>' && input[i] != '<')
            {
                numStr += input[i];
                i++;
            }
            // Read var
            if (numStr == "integer" && input[i] == ' ')
            {
                numStr = "";
                while (input[i] == ' ')
                    i++;
                while ((isalpha(input[i]) || ispunct(input[i]) || isdigit(input[i])) && input[i] != '=')
                {
                    numStr += input[i];
                    i++;
                }
                if (count_if(numStr.begin(), numStr.end(), ::ispunct) - count(numStr.begin(), numStr.end(), '_') == 0 && !isdigit(numStr[0]))
                {
                    t_list[k].name = numStr;
                    t_list[k].type = "var_int";
                }
                else
                {
                    t_list[k].type = "unknown";
                    t_list[k].name = numStr;
                }
            }
            else if (numStr == "decimal" && input[i] == ' ')
            {
                numStr = "";
                while (input[i] == ' ')
                    i++;
                while ((isalpha(input[i]) || ispunct(input[i]) || isdigit(input[i])) && input[i] != '=')
                {
                    numStr += input[i];
                    i++;
                }
                if (count_if(numStr.begin(), numStr.end(), ::ispunct) - count(numStr.begin(), numStr.end(), '_') == 0 && !isdigit(numStr[0]))
                {
                    t_list[k].name = numStr;
                    t_list[k].type = "var_decimal";
                }
                else
                {
                    t_list[k].name = numStr;
                    t_list[k].type = "unknown";
                }
            }
            else if (numStr == "if")
            {
                t_list[k].type = "if";
            }
            else if (numStr == "and")
            {
                t_list[k].type = "and";
            }
            else if (numStr == "or")
            {
                t_list[k].type = "or";
            }
            else if (numStr == "while")
            {
                t_list[k].type = "while";
            }
            else if (numStr == "else")
            {
                t_list[k].type = "else";
            }
            else if (numStr == "print")
            {
                t_list[k].type = "print";
            }
            else if (numStr == "operation")
            {
                t_list[k].type = "operation";
            }
            else if (numStr == "logic")
            {
                t_list[k].type = "logic";
            }
            else if (numStr == "string")
            {
                t_list[k].type = "string";
            }
            else if (count_if(numStr.begin(), numStr.end(), ::ispunct) - count(numStr.begin(), numStr.end(), '_') == 0)
            {
                t_list[k].name = numStr;
                t_list[k].type = "var";
            }
            else
            {
                t_list[k].name = numStr;
                t_list[k].type = "unknown";
            }
            k++;
        }
        else if (isdigit(input[i]) || (input[i] == '-' && isdigit(input[i + 1])))
        {
            // Read numbers, including negative numbers
            string numStr = "";
            if (input[i] == '-')
            {
                numStr += input[i];
                i++;
            }

            while (isdigit(input[i]) || input[i] == '.' || isalpha(input[i]))
            {
                numStr += input[i];
                i++;
            }

            if (count_if(numStr.begin(), numStr.end(), ::isalpha) > 0)
            {
                t_list[k].name = numStr;
                t_list[k].type = "unknown";
            }
            else if (count(numStr.begin(), numStr.end(), '.') == 1)
            {
                t_list[k].decimal = stof(numStr);
                t_list[k].type = "decimal";
            }
            else if (count(numStr.begin(), numStr.end(), '.') > 1)
            {
                t_list[k].name = numStr;
                t_list[k].type = "unknown";
            }
            else
            {
                t_list[k].number = stoi(numStr);
                t_list[k].type = "integer";
            }
            k++;
        }

        else if (ispunct(input[i])) //------------------------------------
        {
            // Read symbols
            string numStr = "";
            while (ispunct(input[i]) && input[i] != '{' && input[i] != '}' && input[i] != '(' && input[i] != ')')
            {
                if (input[i] == '-' && input[i - 1] == '=')
                    break;
                numStr += input[i];
                i++;
            }
            if (numStr == "" && (input[i] == '{' || input[i] == '}' || input[i] == '(' || input[i] == ')'))
            {
                numStr += input[i];
                i++;
            }
            if (numStr == "+" || numStr == "-" || numStr == "*" || numStr == "/" || numStr == "%" || numStr == "#")
            {
                // math operators
                t_list[k].type = numStr;
            }
            else if (numStr == "(")
            {
                // Open parentheses
                t_list[k].type = "o_prnth";
            }
            else if (numStr == ")")
            {
                // Closed parantheses
                t_list[k].type = "c_prnth";
            }
            else if (numStr == "{")
            {
                // Open brackets
                t_list[k].type = "start";
            }
            else if (numStr == "}")
            {
                // Close brackets
                t_list[k].type = "end";
            }
            else if (numStr == "==")
            {
                // Equal to sign
                t_list[k].type = "equal_to";
            }
            else if (numStr == ">=")
            {
                // Greater or equal sign
                t_list[k].type = "greater_or_equal_to";
            }
            else if (numStr == "<=")
            {
                // Less or equal sign
                t_list[k].type = "less_or_equal_to";
            }
            else if (numStr == ">")
            {
                // Greater sign
                t_list[k].type = "greater";
            }
            else if (numStr == "<")
            {
                // Less sign
                t_list[k].type = "less";
            }
            else if (numStr == "=")
            {
                // Equal sign
                t_list[k].type = "equal";
            }
            else
            {
                // Unknown
                t_list[k].type = "unknown";
                t_list[k].name = numStr;
            }

            k++;
        }
        else if (isspace(input[i]))
        {
            // Skip whitespaces
            i++;
        }
        else
        {
            // Unknown character
            t_list[k].type = "unknown";
            k++;
        }
    }
}
