#include <cmath>
#include <cstdio>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

class attribute 
{
public:
    string name;
    string value;
};


class node
{
public:
    vector<node *> children;

    string name;
    vector<attribute> attr;
};


node* get_tag(istream &cin, node **root)
{
    string s;
    node *n = nullptr;
    bool input = false;

    while(getline(cin, s))
    {
        input = true;

        // Remove "<>"
        s.erase(s.begin());
        s.erase(s.end() - 1);

        stringstream ss(s);

        if(s[0] == '/')
        {  
            return nullptr;
        }   

        if(!*root)
        {
            *root = new node();
        }

        ss >> s;
        n = new node();
        n->name = s;

        while(ss >> s && s != "=")
        {
            attribute at;
            at.name = s;
            ss >> s >> s;
            at.value = s;

            // Remove " "
            auto pos = at.value.find('"');
            while(pos != string::npos)
            {
                at.value.erase(pos, 1);
                pos = at.value.find('"');
            }

            n->attr.push_back(at);
        }

        // Get children
        while(get_tag(cin, &n));

        (*root)->children.push_back(n);     
    }   

    if(input)
        return *root;

    return nullptr;
}

void print_tree(node *root)
{
    if(!root)
        return;

    static size_t level = 0;
    string ident(level, ' ');
    string name = (root->name.empty() ? "<no-name>" : root->name);

    cout << ident << name << endl;

    for(auto& it : root->attr)
    {
        cout << ident << "-> " << it.name << " : " << it.value << endl;
    }

    for(auto& it : root->children)
    {
        level++;
        print_tree(it);
        level--;
    }
}


void free_tree(node *root)
{
    if(!root)
        return;

    for(auto& it : root->children)
    {
        it->attr.clear();
        free_tree(it);
    }

    free(root);
}


string get_value(node *root, string &path)
{
    string ret, name;
    vector<string> names;
    node *tag = root, *next;

    auto p = path.find('.');
    if(p != string::npos)
    {
        name = path.substr(0, p);
        next = nullptr;

        for(auto& it : tag->children)
        {
            if(it->name == name)
            {
                next = it;
                break;
            }
        }

        if(!next)
            return "Not Found!";

        tag = next;
        path.erase(0, p + 1);

        return get_value(tag, path);
    }

    auto last = path.find('~');
    if(last != string::npos)
    {
        name = path.substr(0, last);
        next = nullptr;

        for(auto& it : tag->children)
        {
            if(it->name == name)
            {
                next = it;
                break;
            }
        }

        if(!next)
            return "Not Found!";

        tag = next;
        path.erase(0, last + 1);

        return get_value(tag, path);
    }

    for(auto& it : tag->attr)
    {
        if(it.name == path)
            return it.value;
    }
    
    return "Not Found!";
}

int main() {

    size_t N, Q; 
    string s;
    stringstream ss;

    cin >> N >> Q;
    cin.ignore(1, '\n');

    while(N-- && getline(cin, s))
    {
        ss << s << endl;
    }

    node *root = new node();

    get_tag(ss, &root);

    /* print_tree(root); */

    while(Q-- && getline(cin, s))
    {
        if(s.find('~') == string::npos)
        {
            cout << "Not Found!" << endl;
            continue;
        }

        cout << get_value(root, s) << endl;
    }

    free_tree(root);
    return 0;
}
