//  Yunus Kerestecioglu

#ifndef BinaryNotebook_h
#define BinaryNotebook_h

#include <string>
#include "BinarySearchTree.h"

using namespace std;

class BinaryNotebook
{
public:
    BinaryNotebook();
    
    void AddSection( const BinarySearchTree<string> & x );
    void DeleteSection( const string & x);
    bool isEmpty( ) const;
    void DisplaySections( ) const;
    BinarySearchTree<string> * SelectSection( const string & x ) const;
    
private:
    BinarySearchTree<string> *root;
    
    BinarySearchTree<string> * Clone(const BinarySearchTree<string> & bst ) const;
    BinarySearchTree<string> * findMin( BinarySearchTree<string> *t ) const;
    
    void AddSection( const BinarySearchTree<string> & x, BinarySearchTree<string> * & t ) const;
    void DeleteSection( const string & x, BinarySearchTree<string> * & t ) const;
    void DisplaySections( BinarySearchTree<string> * t) const;
    BinarySearchTree<string> * SelectSection( const string & x, BinarySearchTree<string> * t) const;
};

/*
 * Constructor
*/
BinaryNotebook::BinaryNotebook()
{
    root = NULL;
}


/*
         * Internal method to insert a subtree.
         * x is item to compare.
         * t is the tree that roots the notebook.
*/
void BinaryNotebook::AddSection( const BinarySearchTree<string> & x, BinarySearchTree<string> * & t ) const
{
    if( t == NULL ){ //  create a new node at the right place
        //t = Clone(x);
        if (x.isEmpty())
            t = new BinarySearchTree<string>(x.ITEM_NOT_FOUND,x.TreeTitle);
        else
            t = Clone(x);
    }
    else if( x.TreeTitle < t->TreeTitle)
        AddSection( x, t->left );  // insert at the left or
    else if( t->TreeTitle < x.TreeTitle )
        AddSection( x, t->right );  // right subtree
    else
        ;  // Duplicate; do nothing
}

/*
 Member method to add section 'x'
 */
void BinaryNotebook::AddSection(const BinarySearchTree<string> & x)
{
    AddSection(x,root);
}


/*
         * Print the tree contents (sections) in sorted order.
*/
void BinaryNotebook::DisplaySections( ) const
{
    cout << endl << "*****" << endl;
    DisplaySections( root );
    cout << "*****" << endl << endl;
}

/*
      * Internal method to print a notebook rooted at t in sorted order.
*/

void BinaryNotebook::DisplaySections( BinarySearchTree<string> * t ) const
{
    if ( t != NULL ){
        DisplaySections( t->left );
        cout << t->TreeTitle << endl;
        DisplaySections( t->right );
    }
}

/*
         * Internal method to find a subnote.
         * x is item to search for.
         * t is the node that roots the notebook.
*/
BinarySearchTree<string> * BinaryNotebook::SelectSection( const string & x, BinarySearchTree<string> * t ) const
{
    if ( t == NULL ){
        return NULL;
    }
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
BinarySearchTree<string> * BinaryNotebook::SelectSection( const string & x ) const
{
    return SelectSection( x, root );
}

/*
 * Internal method that converts a normal BST to pointer BST
 */
BinarySearchTree<string> * BinaryNotebook::Clone(const BinarySearchTree<string> & bst) const
{
    if (bst.root==NULL)
        return NULL;
    
    BinarySearchTree<string> * bstPtr = new BinarySearchTree<string>(bst.ITEM_NOT_FOUND,bst.TreeTitle);
    bstPtr->root = bst.clone(bst.root);
    return bstPtr;
}

/*
 * Internal method that finds the minimum of BST's regarding thir titles
 */
BinarySearchTree<string> * BinaryNotebook::findMin(BinarySearchTree<string> *t) const
{
    if( t == NULL )
        return NULL;
    if( t->left == NULL )
        return t;
    return findMin( t->left );
}

/*
         * Internal method to delete a subtree.
         * x is item to compare.
         * t is the tree that roots the tree.
         * Return tree containing the matched item.
*/
void BinaryNotebook::DeleteSection( const string & x, BinarySearchTree<string> * & t ) const
{
    if( t == NULL )
        return;   // Item not found; do nothing
    if (x < t->TreeTitle)
        DeleteSection(x, t->left);
    else if (t->TreeTitle < x)
        DeleteSection(x, t->right);
    else if( t->left != NULL && t->right != NULL ) {// Two children
        t->TreeTitle = findMin(t->right)->TreeTitle; // changing title of tree
        // get the nodes
        t-> root = t->clone(findMin(t->right)->root);
        DeleteSection(t->TreeTitle, t->right);
    }
    else{  // one or no children
        BinarySearchTree<string> * oldTree = t;
        t = (t->left != NULL) ? t->left : t->right;
        oldTree->makeEmpty();
        
    }
}

/*
 Member method to delete section 'x'
 */
void BinaryNotebook::DeleteSection(const string &x)
{
    DeleteSection(x, root);
}

/*
 Member method checking if notebook is empty
 */
bool BinaryNotebook::isEmpty() const
{
    return root == NULL;
}


    
#endif /* BinaryNotebook_h */
