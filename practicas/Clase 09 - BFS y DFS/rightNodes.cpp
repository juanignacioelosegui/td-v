#include <iostream>
#include <list>
using namespace std;

//  Data structure to store a binary tree node
struct Node
{
	int key;
	Node *left, *right;

	Node(int key)
	{
		this->key = key;
		this->left = this->right = nullptr;
	}
};

//  Aca pueden decidir si devolver la lista o directamente ir haciendo prints de los nodos

//  NO se debería hacer con recursión
void printRightView ( Node* root )
{
    //  Casos base: raíz nula u hoja
    if ( root == NULL ) return;
    if ( root->right == NULL || root->left == NULL ) return;

    //  Si el nodo de la derecha tiene un elemento, imprimirlo
    if ( root->right->key )
    {
        //  Llamado recursivo para los nodos de izquierda y derecha
        std::cout << root->right->key << std::endl;
        printRightView ( root->left );
        printRightView ( root->right );
    }
}

void printRightView2 ( Node* root )
{
    if ( root == nullptr ) return;
    std::list<Node *> queue;
    queue.push_back( root );
    Node * curr = nullptr;
    while ( !queue.empty() )
    {
        int size = queue.size();
        int i = 0;
        while ( i++ < size )
        {
            curr = queue.front();
            queue.pop_front();
            if ( i == size ) std::cout << curr->key << " ";
            if ( curr-> left ) queue.push_back(curr->left);
            if ( curr->right ) queue.push_back(curr->right);
        }
    }
}
//  TODO: comentar después el código.

//  Vamos a tener una cola llamada L, una variable para iterar i
//  Queremos guardar el size también
//
//  No me voy a preocupar por el vector de estados, porque se van a
//      procesar las cosas una vez.
//
//  En el root, los guardamos en la cola y guardamos el tamaño de la
//      cola en size.
//
//  Queremos chequear i con size. Si lo es, lo voy a printear al nodo.

int main()
{
	Node* root = new Node(1);
	root->left = new Node(2);
	root->right = new Node(3);
	root->left->right = new Node(4);
	root->right->left = new Node(5);
	root->right->right = new Node(6);
	root->right->left->left = new Node(7);
	root->right->left->right = new Node(8);

	printRightView(root);

	return 0;
}
