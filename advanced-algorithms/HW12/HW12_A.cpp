#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

class SuffixTree;

class Suffix;

class Node{


    Node(Node*, int, int*, int);

    int edge_length() { return *end_index - begin_index + 1; }

    void add_child(SuffixTree &, Node*);

    void remove_child(SuffixTree &, Node*);

    bool is_leaf() { return children.empty(); }

    void split_edge(const SuffixTree &, int, int);

    Node* get_child(const SuffixTree &, int char_index);

    int get_key(SuffixTraWAvsqgfY$rhb5jmfnv csaq    1dVee &, Node*, int) const;
private:
    Node* parent;
    std::map<int, Node*> children;
    std::vector<int> labels;
    Node* suffix_link;
    int begin_index;
    int* end_index;
    int ID;
};


Node::Node(Node* parent, int begin_index, int* end_index, int ID) {
    this->parent = parent;
    this->begin_index = begin_index;
    this->end_index = end_index;
    this->ID = ID;
    suffix_link = NULL;
}

void Node::add_child(SuffixTree &tree, Node* child_to_add) {
    int key = get_key(tree, child_to_add, child_to_add->begin_index);
    children[key] = child_to_add;
}

void Node::remove_child(SuffixTree &tree, Node* child_to_remove) {
    int key = get_key(tree, child_to_remove, child_to_remove->begin_index);
    children.erase(key);
}

int Node::get_key(SuffixTree &tree, Node* node, int index) const {
    char ch = tree.get_char_at_index(index);
    return (ch != '$' ? ch * (-1) : index);
}

void Node::split_edge(const SuffixTree &tree, int char_index, int new_node_ID) {
    Node* new_node = new Node(
            parent, begin_index, new int(char_index), new_node_ID);

    parent->remove_child(tree, this);
    parent->add_child(tree, new_node);

    this->parent = new_node;
    this->begin_index = char_index + 1;
    new_node->add_child(tree, this);
}

Node* Node::get_child(const SuffixTree &tree, int char_index) {
    int key = get_key(tree, this, char_index);
    std::map<int, Node*>::iterator it = children.find(key);
    if (it != children.end())
        return it->second;
    else
        return NULL;
}

class Suffix{
    friend class SuffixTree;

private:
    Suffix(Node*, int);

    Node* node;
    bool new_internal_node;
    int char_index;

    bool ends_at_node() const;

    bool ends_at_leaf() const;

    bool continues_with_char(const SuffixTree &, int) const;

    bool RULE2_conditions(const SuffixTree &, int) const;

    Node* walk_up(int &, int &) const;

};

Suffix::Suffix(Node* n, int c) : node(n), char_index(c) {
    new_internal_node = false;
}

bool Suffix::ends_at_node() const {
    return char_index == *node->end_index;
}

bool Suffix::ends_at_leaf() const {
    return node->is_leaf() && ends_at_node();
}


bool Suffix::continues_with_char(const SuffixTree &tree, int tree_index) const {
    char ch = tree.get_char_at_index(tree_index);
    bool terminal(ch == '$');
    return (ends_at_node() && node->get_child(tree, tree_index) != NULL)
           || (!ends_at_node() && tree.get_char_at_index(char_index + 1) == ch
               && (!terminal || char_index + 1 == tree_index));
}

Node* Suffix::walk_up(int &begin_index, int &end_index) const {
    if (ends_at_node() && node->suffix_link != NULL){
        begin_index = *node->end_index;
        end_index = *node->end_index - 1;
        return node;
    }
    else{
        begin_index = node->begin_index;
        end_index = char_index;
        return node->parent;
    }
}

bool Suffix::RULE2_conditions(const SuffixTree &tree, int tree_index) const {
    return !ends_at_leaf() && !continues_with_char(tree, tree_index);
}

class SuffixTree{
public:
    SuffixTree();

    void construct(std::string);

    std::string log_tree();

    char get_char_at_index(int);

private:
    std::string tree_string;

    std::string log_node(Node* parent);

    std::string get_substr(int, int);

    // Suffix Extension rules (Gusfield, 1997)
    enum Rule{
        RULE_2, RULE_3
    };

    // SPA: Single Phase Algorithm (Gusfield, 1997)
    void SPA(int);

    // SEA: Single Extension Algorithm (Gusfield, 1997)
    Rule SEA(Suffix &, int, int);

    // The 'skip/count' trick for traversal (Gusfield, 1997)
    Suffix get_suffix(Node*, int, int);

    // Apply Suffix Extension Rule 2 (Gusfield, 1997)
    void RULE2(Suffix &, int, int);

    Node* root;
    int internal_node_ID;
    int length;
    int* current_end;
    Node* last_leaf_extension;
};


SuffixTree::SuffixTree() {
    // Internal node IDs start at zero and decrement. For example, the root node,
    // which can be considered the first internal node has an ID of 0. The next
    // internal node has an ID of -1, followed by -2 and so forth.

    // While not neccessary for the algorithm to function, each node having a
    // unique ID is important when using Graphiz to visualize the structure.
    internal_node_ID = 0;

    current_end = new int(0);
    root = new Node(NULL, 1, current_end, internal_node_ID);
}

void SuffixTree::construct(std::string s) {
    length = s.length();
    tree_string = s;

    // Construct Implicit Tree I(1).
    (*current_end)++;
    last_leaf_extension = new Node(root, 1, current_end, 1);
    root->add_child(*this, last_leaf_extension);

    for (int i = 1; i < length; i++)
        SPA(i);
}

// SPA: Single Phase Algorithm (Gusfield, 1997)
void SuffixTree::SPA(int i) {
    // Do phase i + 1.

    Suffix previous_suffix(last_leaf_extension, *current_end);

    // Increment the current_end pointer: this implicitly applies Rule 1 to all
    // leaf edges in the tree.
    (*current_end)++;

    // Explicitly compute successive extensions starting at j(i) + 1 where (i)
    // is the ID of the last leaf extension from the previous phase.
    for (int j = (last_leaf_extension->ID + 1); j <= i + 1; j++){
        Rule rule_applied = SEA(previous_suffix, j, i);
        if (rule_applied == RULE_3)
            break;
    }
}

// SEA: Single Extension Algorithm (Gusfield, 1997)
SuffixTree::Rule SuffixTree::SEA(Suffix &previous_suffix, int j, int i) {
    int begin_index, end_index;
    Node* origin = previous_suffix.walk_up(begin_index, end_index);
    Suffix suffix = (origin == root ? get_suffix(root, j, i)
                                    : get_suffix(origin->suffix_link, begin_index, end_index));

    Rule rule_applied;
    if (suffix.RULE2_conditions(*this, i + 1)){
        RULE2(suffix, i + 1, j);
        rule_applied = RULE_2;
    }
    else{
        rule_applied = RULE_3;
    }

    if (previous_suffix.new_internal_node)
        previous_suffix.node->suffix_link = suffix.node;

    previous_suffix = suffix;
    return rule_applied;
}

// The 'skip/count' trick for suffix tree traversal (Gusfield, 1997)
Suffix SuffixTree::get_suffix(Node* origin, int begin_index, int end_index) {
    int char_index = *origin->end_index;

    while (begin_index <= end_index){
        origin = origin->get_child(*this, begin_index);
        if (origin->edge_length() < end_index - begin_index + 1)
            char_index = *origin->end_index;
        else
            char_index = origin->begin_index + (end_index - begin_index);
        begin_index += origin->edge_length();
    }
    return Suffix(origin, char_index);
}

std::string SuffixTree::get_substr(int start_pos, int end_pos) {
    if (start_pos > end_pos) return std::string();
    // This is 1-indexed to match the algorithm's original description in the
    // paper. For example, "foobar".get_substr(2, 4) == "oob".
    return tree_string.substr(start_pos - 1, end_pos - start_pos + 1);
}


void SuffixTree::RULE2(Suffix &suffix, int char_index, int new_leaf_ID) {
    if (!suffix.ends_at_node()){  // eg. in case 2 (path ends inside an edge)
        suffix.node->split_edge(*this, suffix.char_index, --internal_node_ID);
        suffix.node = suffix.node->parent;
        suffix.new_internal_node = true;
    }
    Node* new_leaf = new Node(suffix.node, char_index, current_end, new_leaf_ID);
    suffix.node->add_child(*this, new_leaf);
    last_leaf_extension = new_leaf;
}

std::string SuffixTree::log_tree() {
    return "digraph g{\n" + log_node(root) + "}";
}

char SuffixTree::get_char_at_index(int index) {
    // Also 1-indexed. For example, "foobar".get_char_at_index(4) == 'b'
    return tree_string[index - 1];
}

std::string SuffixTree::log_node(Node* parent) {
    auto it = parent->children.begin();

    std::stringstream buffer;

    // Internal nodes (nodes with ID <= 0) are unlabelled points, leaves
    // (nodes with ID > 0) show the ID as plaintext.
    buffer << parent->ID << "[shape="
           << ((parent->ID <= 0) ? "point" : "plaintext") << "];\n";

    for (; it != parent->children.end(); it++){
        // Child nodes are stored on the parent node in a map of integers
        // (it->first) to Node pointers (it->second).
        Node* child_node = it->second;
        buffer << parent->ID << "->" << child_node->ID << " [label = \""
               << get_substr(child_node->begin_index, *(child_node->end_index))
               << "\"];\n" << log_node(child_node);
    }

    // Print the suffx link, if there is one.
    Node* suffix_link = parent->suffix_link;
    if (suffix_link)
        buffer << "\"" << parent->ID << "\" -> " << "\""
               << suffix_link->ID << "\" [style=dashed arrowhead=otriangle];\n";

    return buffer.str();
}

int main(int argc, char* argv[]) {
    std::string text;
    std::cin >> text;
    SuffixTree tree;

    tree.construct(text);
    std::cout << tree.log_tree() << std::endl;
    return 0;
}