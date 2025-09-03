#include <iostream>
#ifndef EXPRESSIONTREE_H 
#define EXPRESSIONTREE_H

template <class Type>
struct Node{
    Type item;
    Node<Type> *left;
    Node<Type> *right;
};

template <class Type>
class BTree {

    public:

        BTree(); //Constructor: Creates an empty tree

        BTree(const BTree<Type>&); //Constructor: Creates a copy of a tree

        BTree<Type> operator=(const BTree<Type>&); //Overaloaded assignment operator. Copies the BTree

        ~BTree(); //Destructor: Deallocates memory

        void insert(Type item); //Inserts a node into the tree

        void preOrder(); //Displays a pre-order traversal of the tree

        void inOrder(); //Displays an in-order traversal of the tree

        void postOrder(); //Displays a post-order traversal of the tree

        int nodeCount(); //Calculates the number of items in the tree
    
    protected:
        Node<Type> *root;

    private:

        void copyTree(Node<Type>*curr);
        void destroy(Node<Type> *curr);
        void insert(Type item, Node<Type>* curr);
        void preOrder(Node<Type>* curr);
        void inOrder(Node<Type>* curr);
        void postOrder(Node<Type>* curr);
        int nodeCount(Node<Type>* curr);

};

template <class Type>
BTree<Type>::BTree(const BTree<Type>&b){
    copyTree(b.root);

}

template <class Type>
BTree<Type> BTree<Type>::operator=(const BTree<Type>&b){
    copyTree(b.root);
    return *this;
} 

template <class Type>
void BTree<Type>::copyTree(Node<Type>*curr){
    if(curr) {
        insert(curr->item);
        copyTree(curr->left);
        copyTree(curr->right);
    }
}

template <class Type>
BTree<Type>::BTree(){
    root = nullptr;
}

template <class Type>
BTree<Type>::~BTree(){
    destroy(root);
}

template <class Type>
void BTree<Type>::destroy(Node<Type> *curr){
    if(curr != nullptr){
        destroy(curr->left);
        destroy(curr->right);
        delete curr;
    }
}

template <class Type>
void BTree<Type>::insert(Type item){
    if(root == nullptr){
        root = new Node<Type>;
        root->item = item;
        root->left = nullptr;
        root->right = nullptr;
    }
    else{
        insert(item, root);
    }
}

template <class Type>
void BTree<Type>::insert(Type item, Node<Type> *curr){
    if(item < curr->item) {
        if (curr->left == nullptr){
            auto tmp = new Node<Type>;
            tmp->item = item;
            tmp->right = nullptr;
            tmp->left = nullptr;
            curr->left = tmp;
        }
        else{
            insert(item, curr->left);
        }//this ends the inner if
    }// this ends the outter if
else{
    if (curr->right == nullptr){
            auto tmp = new Node<Type>;
            tmp->item = item;
            tmp->right = nullptr;
            tmp->left = nullptr;
            curr->right = tmp;
        }
        else{
            insert(item, curr->right);
        }//this ends the inner if
}
}//this ends the insert

template <class Type>
void BTree<Type>:: preOrder(){
    std::cout << "Pre Order: ";
        preOrder(root);
} 

template <class Type>
void BTree<Type>:: inOrder(){
    std::cout << "In Order: ";
        inOrder(root);
} 
    
template <class Type>
void BTree<Type>:: postOrder(){
    std::cout << "Post Order: ";
        postOrder(root);
} 

template <class Type>
void BTree<Type>::preOrder(Node<Type>* curr){
    if(curr){
       std:: cout << curr->item << " ";
        preOrder(curr->left);
        preOrder(curr->right);
    }
}
    
template <class Type>
void BTree<Type>::inOrder(Node<Type>* curr){
   if(curr){
        inOrder(curr->left);
        std::cout << curr->item << " ";
        inOrder(curr->right);
    }
}
   
template <class Type>
void BTree<Type>::postOrder(Node<Type>* curr){
      if(curr){
        postOrder(curr->left);
        postOrder(curr->right);
        std::cout << curr->item << " ";
    }
}

template <class Type>
int BTree<Type>::nodeCount(){
    return nodeCount(root);
}; 

template <class Type>
int BTree<Type>::nodeCount(Node<Type>* curr){
    if(curr){
        return 1 + nodeCount(curr->left) + nodeCount(curr->right);
    }
    return 0;
}; 

class ExpressionTree {
public:
    
    ExpressionTree() : root(nullptr), pos(0) {}
    explicit ExpressionTree(const std::string& expr) : root(nullptr), pos(0) { setExpression(expr); }
    ExpressionTree(const ExpressionTree& other) : root(clone(other.root)), src(), pos(0) {}
    ExpressionTree& operator=(const ExpressionTree& other) { if (this != &other) { destroy(root); root = clone(other.root); src.clear(); pos = 0; } return *this; }
    ~ExpressionTree() { destroy(root); }
    void setExpression(const std::string& expr) { destroy(root); root = nullptr; src = expr; pos = 0; root = parse(); }
    double getResult() const { return evaluate(root); }
    void printParseTreeInOrder(std::ostream& os) const { printInOrder(root, os); }
    void printParseTreePostOrder(std::ostream& os) const { bool first = true; printPostOrder(root, os, first); }

private:
    struct Node {
        std::string data;   
        Node* left;
        Node* right;
        explicit Node(const std::string& d) : data(d), left(nullptr), right(nullptr) {}
    };

    Node* root;
    std::string src;  
    int pos;       

    
    void skipSpaces() {
        while (pos < src.size() && std::isspace(static_cast<unsigned char>(src[pos]))) 
        ++pos;
     }


    static bool isOp(char c){ 
    return c=='+' || c=='-' || c=='*' || c=='/' || c=='^'; 
        }


    
    Node* parse() {
        skipSpaces();
        if (pos >= src.size()) return nullptr;

        if (src[pos] == '(') {
            ++pos;                          
            Node* L = parse();               
            skipSpaces();
            std::string op;
            if (pos < src.size() && isOp(src[pos])) op.push_back(src[pos++]);
            Node* R = parse();              
            skipSpaces();
            if (pos < src.size() && src[pos] == ')') ++pos;
            Node* n = new Node(op);
            n->left = L; n->right = R;
            return n;
        }

        if (std::isdigit(static_cast<unsigned char>(src[pos]))) { 
            std::string num;
            while (pos < src.size() && std::isdigit(static_cast<unsigned char>(src[pos]))) num.push_back(src[pos++]);
            return new Node(num);
        }

        return nullptr; 
    }

    
    static double evaluate(const Node* n) {
        if (!n) return 0.0;
        if (!n->left && !n->right) {
             if (n->data.empty()) {
             return 0.0;
              } else {
                return std::stod(n->data);
             }
         }
        double a = evaluate(n->left), b = evaluate(n->right);
        if (n->data == "+") return a + b;
         if (n->data == "-") return a - b;
        if (n->data == "*") return a * b;
        if (n->data == "/") return a / b;
         if (n->data == "^") return std::pow(a, b);
        return 0.0;
    }

    
    static void printInOrder(const Node* n, std::ostream& os) {
        if (!n) return;
        printInOrder(n->left, os);
        os << n->data;
        printInOrder(n->right, os);
    }

    static void printPostOrder(const Node* n, std::ostream& os, bool& first) {
        if (!n) return;
        printPostOrder(n->left, os, first);
        printPostOrder(n->right, os, first);
        if (!n->data.empty()) { if (!first) os << ' ';
         os << n->data; first = false; }
    }

    
    static void destroy(Node* n) {
         if (!n) return; 
         destroy(n->left);
          destroy(n->right); 
          delete n;
         }

    static Node* clone(const Node* n) { 
      if (!n) return nullptr; 
        Node* c = new Node(n->data);
         c->left = clone(n->left); 
         c->right = clone(n->right); 
         return c; 
        }
}; 


#endif //EXPRESSIONTREE_H
