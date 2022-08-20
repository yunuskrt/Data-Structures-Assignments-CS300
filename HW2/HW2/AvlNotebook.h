//  Yunus Kerestecioglu

#ifndef AvlNotebook_h
#define AvlNotebook_h

#include <string>
#include "AVLTree.h"

using namespace std;

class AvlNotebook
{
public:
    AvlNotebook();
    void AddSection( const AvlTree<string> & x );
    void DeleteSection( const string & x );
    bool isEmpty( ) const;
    void DisplaySections( ) const;
    AvlTree<string> * SelectSection( const string & x ) const;
    
private:
    AvlTree<string> *root;
    
    AvlTree<string> * Clone(const AvlTree<string> & avl ) const;
    AvlTree<string> * findMin( AvlTree<string> *t ) const;
    void AddSection( const AvlTree<string> & x, AvlTree<string> * & t ) const;
    void DeleteSection( const string & x, AvlTree<string> * & t ) const;
    void DisplaySections( AvlTree<string> * t) const;
    AvlTree<string> * SelectSection( const string & x, AvlTree<string> * t) const;
    
    // Avl manipulations
    int height( AvlTree<string> *t ) const;
    int max( int lhs, int rhs ) const;
    void rotateWithLeftChild( AvlTree<string> * & k2 ) const;
    void rotateWithRightChild( AvlTree<string> * & k1 ) const;
    void doubleWithLeftChild( AvlTree<string> * & k3 ) const;
    void doubleWithRightChild( AvlTree<string> * & k1 ) const;
};


/*
 Constructor
 */
AvlNotebook::AvlNotebook()
{
    root = NULL;
}


// Avl manipulations

/*
        * Return the height of notebook t, or -1, if NULL.
*/
int AvlNotebook::height( AvlTree<string> *t ) const
{
    if (t == NULL)
        return -1;
    return t->h;
}

/*
         * Return maximum of lhs and rhs.
*/
int AvlNotebook::max( int lhs, int rhs ) const
{
    if (lhs > rhs)
        return lhs;
    return rhs;
}

/*
         * Rotate binary tree node with left child.
         * For AVL trees, this is a single rotation for case 1.
         * Update heights, then set new root.
*/
void AvlNotebook::rotateWithLeftChild(AvlTree<string> * & k2) const
{
    AvlTree<string> *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->h = max( height( k2->left ), height( k2->right ) ) + 1;
    k1->h = max( height( k1->left ), k2->h ) + 1;
    k2 = k1;
}

/*
 * Rotate binary tree node with right child.
 * For AVL trees, this is a single rotation for case 4.
 * Update heights, then set new root.
*/
void AvlNotebook::rotateWithRightChild(AvlTree<string> * & k1) const
{
    AvlTree<string> *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->h = max( height( k1->left ), height( k1->right ) ) + 1;
    k2->h = max( height( k2->right ), k1->h ) + 1;
    k1 = k2;
}


/*
         * Double rotate binary tree node: first left child.
         * with its right child; then node k3 with new left child.
         * For AVL trees, this is a double rotation for case 2.
         * Update heights, then set new root.
*/
void AvlNotebook::doubleWithLeftChild( AvlTree<string> * & k3 ) const
{
    rotateWithRightChild( k3->left );
    rotateWithLeftChild( k3 );
}

/*
 * Double rotate binary tree node: first right child.
 * with its left child; then node k1 with new right child.
 * For AVL trees, this is a double rotation for case 3.
 * Update heights, then set new root.
*/
void AvlNotebook::doubleWithRightChild( AvlTree<string> * & k1 ) const
{
    rotateWithLeftChild( k1->right );
    rotateWithRightChild( k1 );
}

/*
 * Internal method that converts a normal AVLtree to pointer AVLtree
*/
AvlTree<string> * AvlNotebook::Clone(const AvlTree<string> & avl) const
{
    if (avl.root==NULL)
        return NULL;
    
    AvlTree<string> * avlPtr = new AvlTree<string>(avl.ITEM_NOT_FOUND,avl.TreeTitle);
    avlPtr->root = avl.clone(avl.root);
    return avlPtr;
}

/*
         * Internal method to find the smallest item in a notebook.
         * Return node containing the smallest item.
*/
AvlTree<string> * AvlNotebook::findMin( AvlTree<string> *t ) const
{
    if( t == NULL )
        return NULL;
    if( t->left == NULL )
        return t;
    return findMin( t->left );
}

/*
         * Internal method to insert a tree.
         * x is item to compare.
         * t is the tree that roots the tree.
*/
void AvlNotebook::AddSection(const AvlTree<string> &x, AvlTree<string> *&t) const
{
    if ( t == NULL ){
        if (x.isEmpty())
            t = new AvlTree<string>(x.ITEM_NOT_FOUND,x.TreeTitle);
        else
            t = Clone(x);
    }

    else if ( x.TreeTitle < t->TreeTitle ) { // X should be inserted to the left tree!
        AddSection( x, t->left );
           
    // Check if the left tree is out of balance (left subtree grew in height!)
        if ( height( t->left ) - height( t->right ) == 2 ){
            if ( x.TreeTitle < t->left->TreeTitle )  // X was inserted to the left-left subtree!
                rotateWithLeftChild( t );
            else                 // X was inserted to the left-right subtree!
                doubleWithLeftChild( t );
        }
    }
    else if( t->TreeTitle < x.TreeTitle ) {    // Otherwise X is inserted to the right subtree
        AddSection( x, t->right );
        
        if ( height( t->right ) - height( t->left ) == 2 ){ // height of the right subtree increased
            if ( t->right->TreeTitle < x.TreeTitle ) // X was inserted to right-right subtree
                rotateWithRightChild( t );
            else // X was inserted to right-left subtree
                doubleWithRightChild( t );
        }
    }
    else
        ;  // Duplicate; do nothing
                
    // update the height the node
    
    t->h = max( height( t->left ), height( t->right ) ) + 1;
}

/*
 Member method to add section 'x'
 */
void AvlNotebook::AddSection( const AvlTree<string> & x)
{
    AddSection( x, root );
}

/*
         * Internal method to delete a tree.
         * x is the item to delete.
         * t is the node that roots the notebook.
         * Set the new root.
*/
void AvlNotebook::DeleteSection( const string & x, AvlTree<string> *&t ) const
{
    if( t == NULL )
        return;   // Item not found; do nothing
    
    // Perform Standart Binary Search Tree Deletion
    if( x < t->TreeTitle )
        DeleteSection( x, t->left );
    else if( t->TreeTitle < x )
        DeleteSection( x, t->right );
    else if( t->left != NULL && t->right != NULL ) {// Two children
        t->TreeTitle = findMin( t->right )->TreeTitle;
        // get the nodes
        t-> root = t->clone(findMin(t->right)->root);
        DeleteSection( t->TreeTitle, t->right );
    }
    else{ // one or no children
        AvlTree<string> * oldNode = t;
        t = ( t->left != NULL ) ? t->left : t->right;
        oldNode->makeEmpty();
    }
    
    if (t == NULL)
        return;
    
    // Update the height of the node
    t->h = max( height( t->left ), height( t->right ) ) + 1;
    
    
    // Make Rotations if Necessary
    if (height( t->left ) - height( t->right ) > 1) {
        if (height( t->left->left ) - height( t->left->right ) >= 0) {
            rotateWithLeftChild(t);
        }
        else {
            doubleWithLeftChild(t);
        }
    }
    if (height( t->right ) - height( t->left ) > 1) {
        if (height(t->right->right) - height(t->right->left) >= 0) {
            rotateWithRightChild(t);
        }
        else {
            doubleWithRightChild(t);
        }
    }
}

/*
         * Remove x from the notebook;
*/
void AvlNotebook::DeleteSection( const string & x)
{
    DeleteSection( x, root );
}

/*
      * Internal method to print a notebook rooted at t in sorted order.
*/
void AvlNotebook::DisplaySections( AvlTree<string> * t ) const
{
    if ( t != NULL ){
        DisplaySections( t->left );
        cout << t->TreeTitle << endl;
        DisplaySections( t->right );
    }
}

/*
         * Print the tree contents (sections) in sorted order.
*/
void AvlNotebook::DisplaySections( ) const
{
    cout << endl << "*****" << endl;
    DisplaySections( root );
    cout << "*****" << endl << endl;
}

/*
         * Internal method to find a subtree.
         * x is item to search for.
         * t is the node that roots the tree.
*/
AvlTree<string> * AvlNotebook::SelectSection( const string & x, AvlTree<string> * t ) const
{
    if ( t == NULL )
        return NULL;
    else if( x < t->TreeTitle )
        return SelectSection( x, t->left );
    else if( t->TreeTitle < x )
        return SelectSection( x, t->right );
    else
        return t;    // Match
}
/*
 Member method to find the given section with 'x'
 */
AvlTree<string> * AvlNotebook::SelectSection( const string & x ) const
{
    return SelectSection( x, root );
}

/*
 Member method checks if notebook is empty
 */
bool AvlNotebook::isEmpty() const
{
    return root == NULL;
}


#endif /* AvlNotebook_h */
