#include "iterpreter.cpp"

int main()
{
    bool developer = false;
    string expression = readFileToString("input.txt");
    tokenize(expression);
    if (developer)
    {
        for (int i = 0; i < k; i++)
        {
            cout << "<" << t_list[i].type;
            if (t_list[i].type == "decimal")
                cout << " " << t_list[i].decimal << "> ";
            else if (t_list[i].type == "integer")
                cout << " " << t_list[i].number << "> ";
            else if (t_list[i].type == "text")
                cout << " " << t_list[i].name << "> ";
            else if (t_list[i].type == "var_int")
                cout << " " << t_list[i].name << "> ";
            else if (t_list[i].type == "var_decimal")
                cout << " " << t_list[i].name << "> ";
            else if (t_list[i].type == "var")
                cout << " " << t_list[i].name << "> ";
            else
                cout << "> ";
        }
        cout << endl;
    }
    if (validate(t_list, k))
    {
        Node *Root = new Node;
        Root->tkn.type = "start";
        gen_tree(Root);
        if (developer)
            printBT(Root);
        traverse_tree(Root);
    }
}