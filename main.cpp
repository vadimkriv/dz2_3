#include <iostream>
#include <fstream>
#include <initializer_list>
#include <queue>
#include <sstream>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node* parent;
    Node(int value) : data(value), left(nullptr), right(nullptr), parent(nullptr) {}
};
class BSTree {
private:
    Node* root;
    void destroy_tree(Node* node) {
        if (node) {
            destroy_tree(node->left);
            destroy_tree(node->right);
            delete node;
        }
    }
    Node* add_element(Node* node, int value) {
        if (!node) return new Node(value);
        if (value < node->data) {
            Node* left_child = add_element(node->left, value);
            node->left = left_child;
            left_child->parent = node;
        } else if (value > node->data) {
            Node* right_child = add_element(node->right, value);
            node->right = right_child;
            right_child->parent = node;
        }
        return node;
    }
    Node* find_min(Node* node) {
        while (node->left) node = node->left;
        return node;
    }
    Node* delete_element(Node* node, int value) {
        if (!node) return nullptr;
        if (value < node->data) {
            node->left = delete_element(node->left, value);
        } else if (value > node->data) {
            node->right = delete_element(node->right, value);
        } else {
            if (!node->left) {
                Node* right_child = node->right;
                if (right_child) right_child->parent = node->parent;
                delete node;
                return right_child;
            } else if (!node->right) {
                Node* left_child = node->left;
                if (left_child) left_child->parent = node->parent;
                delete node;
                return left_child;
            }
            Node* min_larger_node = find_min(node->right);
            node->data = min_larger_node->data;
            node->right = delete_element(node->right, min_larger_node->data);
        }
        return node;
    }
    Node* find_element(Node* node, int value) {
        if (!node || node->data == value) return node;
        if (value < node->data) return find_element(node->left, value);
        return find_element(node->right, value);
    }
    void print(Node* node, int space = 0, int height = 10) {
        if (!node) return;
        space += height;
        print(node->right, space);
        std::cout << std::endl;
        for (int i = height; i < space; i++) std::cout << ' ';
        std::cout << node->data << "\n";
        print(node->left, space);
    }
    void serialize(Node* node, std::ostream& out) {
        if (!node) {
            out << "# ";
        } else {
            out << node->data << " ";
            serialize(node->left, out);
            serialize(node->right, out);
        }
    }
    Node* deserialize(std::istream& in) {
        std::string val;
        in >> val;
        if (val == "#") return nullptr;
        Node* node = new Node(std::stoi(val));
        node->left = deserialize(in);
        if (node->left) node->left->parent = node;
        node->right = deserialize(in);
        if (node->right) node->right->parent = node;
        return node;
    }
public:
    BSTree() : root(nullptr) {}
    BSTree(std::initializer_list<int> list) : root(nullptr) {
        for (int value : list) {
            add_element(value);
        }
    }
    bool add_element(int value) {
        if (!root) {
            root = new Node(value);
            return true;
        }
        root = add_element(root, value);
        return true;
    }
    bool delete_element(int value) {
        if (find_element(root, value)) {
            root = delete_element(root, value);
            return true;
        }
        return false;
    }
    bool find_element(int value) {
        return find_element(root, value) != nullptr;
    }
    void print() {
        print(root);
        std::cout << std::endl;
    }
bool save_to_file(const std::string& path) {
        std::ofstream file(path);
        if (!file.is_open()) return false;
        serialize(root, file);
        file.close();
        return true;
    }
    bool load_from_file(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return false;
        destroy_tree(root);
        root = deserialize(file);
        file.close();
        return true;
    }
    ~BSTree() {
        destroy_tree(root);
    }
};
int main() {
    BSTree tree = {10, 5, 20, 3, 7, 15, 30};
    tree.print();
    tree.add_element(25);
    tree.print();
    tree.delete_element(10);
    tree.print();
    std::cout << (tree.find_element(15) ? "Found" : "Not Found") << std::endl;
    tree.save_to_file("tree.txt");
    BSTree new_tree;
    new_tree.load_from_file("tree.txt");
    new_tree.print();
    return 0;
}
