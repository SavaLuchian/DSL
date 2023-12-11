
#include "lexer.cpp"
using namespace std;

class Node
{
public:
    token tkn;
    Node *root;
    Node *left;
    Node *right;
    Node()
    {
        root = nullptr;
        left = nullptr;
        right = nullptr;
    }
};
bool validate(token list[], int n)
{
    bool check = 0;
    int end_strt = 0;
    for (int i = 0; i < n; i++) // check every token
    {
        if (list[i].type == "start")
        {
            if (list[i - 1].type == "start")
            {
                cout << endl
                     << "ERROR: Invalid start syntax";
                return false;
            }
            end_strt++;
        }
        if (list[i].type == "end")
            end_strt--;
        //  ERROR: var not declared
        if (list[i].type == "var")
        {
            check = 0;
            for (int j = 0; j < i; j++)
                if ((list[j].type == "var_int" || list[j].type == "var_decimal") && list[j].name == list[i].name)
                {
                    check = 1;
                    break;
                }
            if (check == 0)
            {
                cout << endl
                     << "ERROR: No '" << list[i].name << "' was declared";
                return false;
            }
        }
        // ERROR: Operation cant be empty
        if (list[i].type == "operation")
        {
            if (list[i + 1].type == "start" && list[i + 2].type == "end")
            {
                cout << endl
                     << "ERROR: Operation can't be empty";
                return false;
            }
        }
        // ERROR: Logic cant be empty
        if (list[i].type == "logic")
        {
            if (list[i + 1].type == "start" && list[i + 2].type == "end")
            {
                cout << endl
                     << "ERROR: Logic can't be empty";
                return false;
            }
        }
        // ERROR: IF missing logic
        if (list[i].type == "if")
        {
            if (list[i + 1].type != "logic")
            {
                cout << endl
                     << "ERROR: IF statement missing logic expression";
                return false;
            }
        }
        // ERROR: WHILE missing logic
        if (list[i].type == "while")
        {
            if (list[i + 1].type != "logic")
            {
                cout << endl
                     << "ERROR: WHILE statement missing logic expression";
                return false;
            }
        }
        // ERROR: Print syntax
        if (list[i].type == "print")
        {
            if (list[i + 1].type != "logic" && list[i + 1].type != "operation" && list[i + 1].type != "var" && list[i + 1].type != "integer" && list[i + 1].type != "decimal" && list[i + 1].type != "string" && list[i + 1].type != "endline")
            {
                cout << endl
                     << "ERROR: Print doesn't output anything";
                return false;
            }
        }

        //  ERROR: unknown
        if (list[i].type == "unknown")
        {
            cout << endl
                 << "ERROR: Unknown syntax: " << list[i].name;
            return false;
        }
        //  ERROR: equal sign syntax mistake
        if (list[i].type == "equal")
        {
            if (list[i - 1].type != "var" && list[i - 1].type != "var_int" && list[i - 1].type != "var_decimal")
            {
                cout << endl
                     << "ERROR: Equal sign left operand is invalid";
                return false;
            }
            if (list[i + 1].type != "var" && list[i + 1].type != "operation" && list[i + 1].type != "integer" && list[i + 1].type != "decimal")
            {
                cout << endl
                     << "ERROR: Equal sign right operand is invalid";
                return false;
            }
        }
        //  ERROR: string syntax mistake
        if (list[i].type == "string")
        {
            if (list[i + 1].type != "start")
            {
                cout << endl
                     << "ERROR: Invalid string syntax";
                return false;
            }
        }
    }
    //  ERROR: { } missmatch
    if (end_strt != 0)
    {
        cout << endl
             << "ERROR: End/Start missmatch";
        return false;
    }

    // check specific syntax

    // ERROR: no start
    if (list[0].type != "start")
    {
        cout << endl
             << "ERROR: Program doesn't have a start";
        return false;
    }
    // ERROR: no end
    if (list[n - 1].type != "end")
    {
        cout << endl
             << "ERROR: Program doesn't have an end";

        return false;
    }

    return true;
}
void addLeft(Node *root, token value)
{
    Node *newNode = new Node();
    newNode->tkn = value;
    newNode->root = root;
    root->left = newNode;
}

void addRight(Node *root, token value)
{
    Node *newNode = new Node();
    newNode->tkn = value;
    newNode->root = root;
    root->right = newNode;
}
void addNode(Node *root)
{
    Node *newNode = new Node();
    newNode->tkn.type = "node";
    newNode->root = root;
    root->right = newNode;
}
void root_val(Node *root, token value)
{
    root->tkn = value;
}
//-------OPERATION METHOD-------
Node *operation_method(Node *point, int &list_indx)
{
    root_val(point, t_list[list_indx]);
    list_indx++;
    addRight(point, t_list[list_indx]);
    point = point->right;
    while (t_list[list_indx].type != "end")
    {
        if (t_list[list_indx].type == "o_prnth")
        {
            if (point->left == nullptr)
            {
                addLeft(point, t_list[list_indx]);
                point = point->left;
            }
            else
            {
                addRight(point, t_list[list_indx]);
                point = point->right;
            }
        }
        else if (t_list[list_indx].type == "c_prnth")
        {
            point = point->root;
        }
        else if (t_list[list_indx].type == "integer" || t_list[list_indx].type == "decimal" || t_list[list_indx].type == "var")
        {
            if (point->left == nullptr)
                addLeft(point, t_list[list_indx]);
            else
                addRight(point, t_list[list_indx]);
        }
        else if (t_list[list_indx].type == "+" || t_list[list_indx].type == "-" || t_list[list_indx].type == "*" || t_list[list_indx].type == "/" || t_list[list_indx].type == "%" || t_list[list_indx].type == "#")
        {
            root_val(point, t_list[list_indx]);
        }
        list_indx++;
    }
    return point; // return point final location
}
//-------LOGIC METHOD-------
Node *logic_method(Node *point, int &list_indx)
{
    root_val(point, t_list[list_indx]);
    list_indx++;
    addRight(point, t_list[list_indx]);
    point = point->right;
    while (t_list[list_indx].type != "end")
    {
        if (t_list[list_indx].type == "o_prnth")
        {
            if (point->left == nullptr)
            {
                addLeft(point, t_list[list_indx]);
                point = point->left;
            }
            else
            {
                addRight(point, t_list[list_indx]);
                point = point->right;
            }
        }
        else if (t_list[list_indx].type == "c_prnth")
        {
            point = point->root;
        }
        else if (t_list[list_indx].type == "integer" || t_list[list_indx].type == "decimal" || t_list[list_indx].type == "var")
        {
            if (point->left == nullptr)
                addLeft(point, t_list[list_indx]);
            else
                addRight(point, t_list[list_indx]);
        }
        else if (t_list[list_indx].type == "operation")
        {
            if (point->left == nullptr)
            {
                addLeft(point, t_list[list_indx]);
                point = point->left;
                point = operation_method(point, list_indx);
                while (point->tkn.type != "operation")
                    point = point->root;
                point = point->root;
            }
            else
            {
                addRight(point, t_list[list_indx]);
                point = point->right;
                point = operation_method(point, list_indx);
                while (point->tkn.type != "operation")
                    point = point->root;
                point = point->root;
            }
        }
        else if (t_list[list_indx].type == "greater" || t_list[list_indx].type == "less" || t_list[list_indx].type == "equal_to" || t_list[list_indx].type == "greater_or_equal_to" || t_list[list_indx].type == "less_or_equal_to" || t_list[list_indx].type == "and" || t_list[list_indx].type == "or")
        {
            root_val(point, t_list[list_indx]);
        }
        list_indx++;
    }
    return point; // return point final location
}
//-------IF METHOD---------
Node *if_method(Node *point, int &list_indx)
{
    addNode(point);
    point = point->right; // point is on node
    addLeft(point, t_list[list_indx]);
    list_indx++;
    point = point->left; // point is on if
    addLeft(point, t_list[list_indx]);
    point = point->left;
    point = logic_method(point, list_indx);
    while (point->tkn.type != "if")
        point = point->root; // point back on if
    return point;            // return point final location
}
//-------WHILE METHOD---------
Node *while_method(Node *point, int &list_indx)
{
    addNode(point);
    point = point->right; // point is on node
    addLeft(point, t_list[list_indx]);
    list_indx++;
    point = point->left; // point is on while
    addLeft(point, t_list[list_indx]);
    point = point->left;
    point = logic_method(point, list_indx);
    while (point->tkn.type != "while")
        point = point->root; // point back on if
    return point;            // return point final location
}
//--------END METHOD---------
Node *end_method(Node *point, int &list_indx)
{
    addRight(point, t_list[list_indx]);
    while (point->tkn.type != "start") // go to the next start root
    {
        point = point->root;
    }
    if (point->root != nullptr)
        while (point->tkn.type != "node") // go to the next node root if its not the start of the tree
        {
            point = point->root;
        }
    return point; // return point final location
}

//-------PRINT METHOD-------
Node *print_method(Node *point, int &list_indx)
{
    addNode(point);
    point = point->right; // point is on node
    addLeft(point, t_list[list_indx]);
    list_indx++;
    point = point->left; // point is on print
    addRight(point, t_list[list_indx]);
    point = point->right;
    if (point->tkn.type == "operation")
        operation_method(point, list_indx);
    else if (point->tkn.type == "logic")
        point = logic_method(point, list_indx);
    else if (point->tkn.type == "string")
    {
        list_indx += 2;
        addRight(point, t_list[list_indx]);
        list_indx++;
    }
    while (point->tkn.type != "node")
        point = point->root; // point on node
    return point;            // return point final location
}
//-------ELSE METHOD-------
Node *else_method(Node *point, int &list_indx)
{
    addNode(point);
    point = point->right; // point is on node
    addLeft(point, t_list[list_indx]);
    point = point->left; // point is on else
    return point;        // return point final location
}
//-------EQUAL METHOD-------
Node *equal_method(Node *point, int &list_indx)
{
    addNode(point);
    point = point->right; // point is on node
    list_indx++;
    addLeft(point, t_list[list_indx]);
    list_indx--;
    point = point->left; // point is on equal
    addLeft(point, t_list[list_indx]);
    list_indx += 2;
    addRight(point, t_list[list_indx]);
    point = point->right;
    if (point->tkn.type == "operation")
        operation_method(point, list_indx);
    while (point->tkn.type != "node")
        point = point->root; // point on node
    return point;            // return point final location
}
//-------ANY METHOD-------
Node *any_method(Node *point, int &list_indx)
{
    addNode(point);
    point = point->right; // point is on node
    addLeft(point, t_list[list_indx]);
    return point; // return point final location
}
//-------START METHOD-------
Node *start_method(Node *point, int &list_indx)
{
    addRight(point, t_list[list_indx]);
    point = point->right; // point is on start
    return point;         // return point final location
}
//------PRINT TREE------
void printBT(const std::string &prefix, const Node *node, bool isLeft)
{
    if (node != nullptr)
    {
        cout << prefix;

        cout << (isLeft ? "├──" : "└──");

        // print the value of the node
        cout << node->tkn.type << endl;

        // enter the next tree level - left and right branch
        printBT(prefix + (isLeft ? "│   " : "    "), node->left, true);
        printBT(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void printBT(const Node *node)
{
    printBT("", node, false);
}
void gen_tree(Node *root)
{
    int list_indx = 1;
    Node *point = root;
    for (; list_indx < k; list_indx++)
    {
        if (t_list[list_indx].type == "if")
            point = if_method(point, list_indx); // IF subtree
        else if (t_list[list_indx].type == "end")
            point = end_method(point, list_indx); // END subtree
        else if (t_list[list_indx].type == "else")
            point = else_method(point, list_indx); // ELSE subtree
        else if (t_list[list_indx].type == "start")
            point = start_method(point, list_indx); // START subtree
        else if (t_list[list_indx].type == "while")
            point = while_method(point, list_indx); // WHILE subtree
        else if (t_list[list_indx].type == "string")
            ;
        else if (t_list[list_indx].type == "print")
            point = print_method(point, list_indx); // PRINT subtree
        else if (t_list[list_indx + 1].type == "equal")
            point = equal_method(point, list_indx); // EQUAL subtree
        else
            point = any_method(point, list_indx); // ANY subtree
    }
}
