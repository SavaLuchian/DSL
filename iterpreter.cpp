#include "parser.cpp"
#include <iomanip>
using namespace std;

int precision = 8;
// END TRAVERSE
Node *end_trav(Node *point)
{
    while (point->tkn.type != "start")
        point = point->root;
    if (point->root != nullptr)
    {
        if (point->root->tkn.type == "while")
            point = point->root;
        else
        {
            while (point->tkn.type != "node")
                point = point->root;
            point = point->right;
        }
    }
    return point;
}
// FIND VARIABLE WITH NAME
int find_var(string x)
{
    for (int i = 0; i < k; i++)
    {
        if (t_list[i].type == "var_int" || t_list[i].type == "var_decimal")
            if (t_list[i].name == x)
                return i;
    }
    return 0;
}

// OPERATION RESULT
float operation_result(Node *point)
{
    if (point->tkn.type == "integer" || point->tkn.type == "decimal" || point->tkn.type == "var")
    {
        if (point->tkn.type == "var")
            if (t_list[find_var(point->tkn.name)].type == "var_int")
                return t_list[find_var(point->tkn.name)].number;
            else
                return t_list[find_var(point->tkn.name)].decimal;
        else if (point->tkn.type == "integer")
            return point->tkn.number;
        else if (point->tkn.type == "decimal")
            return point->tkn.decimal;
    }

    else if (point->tkn.type == "+")
        return operation_result(point->left) + operation_result(point->right);
    else if (point->tkn.type == "-")
        return operation_result(point->left) - operation_result(point->right);
    else if (point->tkn.type == "*")
        return operation_result(point->left) * operation_result(point->right);
    else if (point->tkn.type == "/")
        return operation_result(point->left) / operation_result(point->right);
    else if (point->tkn.type == "%")
        return int(operation_result(point->left)) % int(operation_result(point->right));
    else if (point->tkn.type == "#")
        return int(operation_result(point->left) / operation_result(point->right));
    return 0;
}
// RETURN VARIABLE VALUE
float return_value(Node *point)
{
    if (point->tkn.type == "var")
        if (t_list[find_var(point->tkn.name)].type == "var_int")
            return t_list[find_var(point->tkn.name)].number;
        else
            return t_list[find_var(point->tkn.name)].decimal;
    else if (point->tkn.type == "integer")
        return point->tkn.number;
    else if (point->tkn.type == "decimal")
        return point->tkn.decimal;
    else if (point->tkn.type == "operation")
    {
        point = point->right;
        return operation_result(point);
    }
    else
        return point->tkn.decimal;
}
// LOGIC RESULT
bool logic_result(Node *point)
{
    if (point->right->tkn.type == "integer" || point->right->tkn.type == "decimal" || point->right->tkn.type == "var" || point->right->tkn.type == "operation")
    {
        if (point->left->tkn.type == "integer" || point->left->tkn.type == "decimal" || point->left->tkn.type == "var" || point->right->tkn.type == "operation")
        {
            // operation type
            if (point->tkn.type == "less")
                return return_value(point->left) < return_value(point->right);
            else if (point->tkn.type == "greater")
                return return_value(point->left) > return_value(point->right);
            else if (point->tkn.type == "equal_to")
                return return_value(point->left) == return_value(point->right);
            else if (point->tkn.type == "greater_or_equal_to")
                return return_value(point->left) >= return_value(point->right);
            else if (point->tkn.type == "less_or_equal_to")
                return return_value(point->left) <= return_value(point->right);
        }
    }
    else if (point->tkn.type == "or")
        return logic_result(point->left) || logic_result(point->right);
    else if (point->tkn.type == "and")
        return logic_result(point->left) && logic_result(point->right);
    return false;
}

// IF/WHILE TRAVERSE
Node *if_while_trav(Node *point)
{
    if (logic_result(point->left->right))
        point = point->right;
    else
    {
        while (point->tkn.type != "node")
            point = point->root;
        point = point->right;
        if (point->left->tkn.type == "else")
            point->left->tkn.type = "else_act";
    }
    return point;
}
// PRINT TRAVERSE
Node *print_trav(Node *point)
{
    if (point->right->tkn.type == "operation")
        cout << setprecision(precision) << operation_result(point->right->right) << endl;
    else if (point->right->tkn.type == "logic")
    {
        if (logic_result(point->right->right))
            cout << "True" << endl;
        else
            cout << "False" << endl;
    }
    else if (point->right->tkn.type == "var")
        cout << setprecision(precision) << return_value(point->right) << endl;
    else if (point->right->tkn.type == "decimal")
        cout << setprecision(precision) << point->right->tkn.decimal << endl;
    else if (point->right->tkn.type == "integer")
        cout << point->right->tkn.number << endl;
    else
        cout << point->right->right->tkn.name << endl;
    while (point->tkn.type != "node")
        point = point->root;
    point = point->right;
    return point;
}
// EQUAL TRAVERSE
Node *equal_trav(Node *point)
{
    if (t_list[find_var(point->left->tkn.name)].type == "var_int")
        t_list[find_var(point->left->tkn.name)].number = return_value(point->right);
    else
        t_list[find_var(point->left->tkn.name)].decimal = return_value(point->right);
    while (point->tkn.type != "node")
        point = point->root;
    point = point->right;
    return point;
}

void traverse_tree(Node *root)
{
    Node *point = root;
    while (true)
    { // equal print
        if (point->tkn.type == "node")
            point = point->left;
        else if (point->tkn.type == "start")
            point = point->right;
        else if (point->tkn.type == "else")
        {
            point = point->root;
            point = point->right;
        }
        else if (point->tkn.type == "var_int"||point->tkn.type == "var_decimal")
        {
            point = point->root;
            point = point->right;
        }
        else if (point->tkn.type == "else_act")
        {
            point->tkn.type = "else";
            point = point->right;
        }
        else if (point->tkn.type == "print")
            point = print_trav(point);
        else if (point->tkn.type == "equal")
            point = equal_trav(point);
        else if (point->tkn.type == "if" || point->tkn.type == "while")
            point = if_while_trav(point);
        else if (point->tkn.type == "end")
        {
            point = end_trav(point);
            if (point->root == nullptr)
                break;
        }

        else
        {
            if (point->right == nullptr)
            {
                cout << "ERROR: Out of bounds at " << endl
                     << point->tkn.type << endl
                     << point->tkn.name;

                break;
            }
            point = point->right;
        }
    }
}


