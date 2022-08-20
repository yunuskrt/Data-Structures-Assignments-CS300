//  Yunus Kerestecioglu


#ifndef Header_h
#define Header_h

#include "BinaryNode.h"

using namespace std;

template <class Comparable>
class BinarySearchTree
{
    friend class BinaryNotebook;
    public:
        explicit BinarySearchTree( const Comparable & notFound,const Comparable & title );
        
        BinarySearchTree( const BinarySearchTree & rhs );
        ~BinarySearchTree( );
    
        const Comparable & findMin( ) const;
        const Comparable & findMax( ) const;
        BinaryNode<Comparable> * find( const Comparable & x ) const;
        bool isEmpty( ) const;
        void printTree( ) const;
        string getTitle() const;
        void makeEmpty( );
        void insert( const Comparable & x, const Comparable & inf );
        void remove( const Comparable & x );

        void operator=( const BinarySearchTree & rhs );
    
    private:
        BinaryNode<Comparable> *root;
        BinarySearchTree  *left;
        BinarySearchTree  *right;
    
        const Comparable ITEM_NOT_FOUND;
        Comparable TreeTitle;
    
        const Comparable & elementAt( BinaryNode<Comparable> *t ) const;

        void insert( const Comparable & x, const Comparable & inf, BinaryNode<Comparable> * & t );
        void remove( const Comparable & x, BinaryNode<Comparable> * & t ) const;
        BinaryNode<Comparable> * findMin( BinaryNode<Comparable> *t ) const;
        BinaryNode<Comparable> * findMax( BinaryNode<Comparable> *t ) const;
        BinaryNode<Comparable> * find( const Comparable & x,BinaryNode<Comparable> *t ) const;
        void makeEmpty( BinaryNode<Comparable> * & t ) const;
        void printTree( BinaryNode<Comparable> *t ) const;
        BinaryNode<Comparable> * clone( BinaryNode<Comparable> *t ) const;

};


/*
  * Construct the tree.
*/
template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable & notFound,const Comparable & title )
    :ITEM_NOT_FOUND( notFound ), root( NULL ), left(NULL), right(NULL), TreeTitle(title){}

/*
  * Internal method to get element field in node t.
  * Return the element field or ITEM_NOT_FOUND if t is NULL.
*/
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::elementAt( BinaryNode<Comparable> *t ) const
{
    return t == NULL ? ITEM_NOT_FOUND : t->element;
}

/*
  * Find item x in the tree.
  * Return the matching item or ITEM_NOT_FOUND if not found.
*/
template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::find( const Comparable & x ) const
{
    return find( x, root );
}

/*
         * Internal method to find an item in a subtree.
         * x is item to search for.
         * t is the node that roots the tree.
         * Return node containing the matched item.
*/
template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::find( const Comparable & x, BinaryNode<Comparable> * t ) const
{
    if ( t == NULL )
        return NULL;
    else if( x < t->element )
        return find( x, t->left );
    else if( t->element < x )
        return find( x, t->right );
    else
        return t;    // Match
}

/*
         * Find the smallest item in the tree.
         * Return smallest item or ITEM_NOT_FOUND if empty.
*/
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMin( ) const
{
    return elementAt( findMin( root ) );
}

/*
         * Internal method to find the smallest item in a subtree t.
         * Return node containing the smallest item.
*/
template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::findMin( BinaryNode<Comparable> *t ) const
{
    if( t == NULL )
        return NULL;
    if( t->left == NULL )
        return t;
    return findMin( t->left );
}

/*
         * Find the smallest item in the tree.
         * Return smallest item or ITEM_NOT_FOUND if empty.
*/
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMax( ) const
{
    return elementAt( findMax( root ) );
}

/*
         * Internal method to find the largest item in a subtree t.
         * Return node containing the largest item.
*/
template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::findMax( BinaryNode<Comparable> *t ) const
{
    if( t != NULL )
        while( t->right != NULL )
            t = t->right;
    return t;
}

/*
         * Insert x into the tree; duplicates are ignored.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::insert( const Comparable & x,const Comparable & inf )
{
    insert( x,inf, root );
}

/*
         * Internal method to insert into a subtree.
         * x is the item to insert.
         * t is the node that roots the tree.
         * Set the new root.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::insert( const Comparable & x,const Comparable & inf, BinaryNode<Comparable> * & t )
{
    if( t == NULL ) //  create a new node at the right place
        t = new BinaryNode<Comparable>( x, inf, NULL, NULL );
    else if( x < t->element )
        insert( x, inf, t->left );  // insert at the left or
    else if( t->element < x )
        insert( x, inf, t->right );  // right subtree
    else
        ;  // Duplicate; do nothing
}

/*
         * Remove x from the tree. Nothing is done if x is not found.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::remove( const Comparable & x )
{
    remove( x, root );
}

/*
         * Internal method to remove from a subtree.
         * x is the item to remove.
         * t is the node that roots the tree.
         * Set the new root.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::remove( const Comparable & x, BinaryNode<Comparable> * & t ) const
{
    if( t == NULL )
        return;   // Item not found; do nothing
    if( x < t->element )
        remove( x, t->left );
    else if( t->element < x )
        remove( x, t->right );
    else if( t->left != NULL && t->right != NULL ) {// Two children
        t->element = findMin( t->right )->element;
        remove( t->element, t->right );
    }
    else{ // one or no children
        BinaryNode<Comparable> *oldNode = t;
        t = ( t->left != NULL ) ? t->left : t->right;
        delete oldNode;
    }
}

/*
  * Make the tree logically empty.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( )
{
    makeEmpty( root );
}

/*
         * Internal method to make subtree empty.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( BinaryNode<Comparable> * & t ) const
{
    if( t != NULL ){
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = NULL;
}

/*
         * Check there is node in the tree.
*/
template <class Comparable>
bool BinarySearchTree<Comparable>::isEmpty() const
{
    return root == NULL;
}

/*
         * Print the tree contents in sorted order.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::printTree( ) const
{
    cout << endl << "*****" << endl;
    printTree( root );
    cout << "*****" << endl << endl;
}

/*
      * Internal method to print a subtree rooted at t in sorted order.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::printTree( BinaryNode<Comparable> * t ) const
{
    if ( t != NULL ){
        printTree( t->left );
        cout << t->element << endl;
        printTree( t->right );
    }
}

/*
         * Destructor for the tree.
*/
template <class Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree( )
{
    makeEmpty( );
}

/*
         * Copy constructor.
*/
template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree( const BinarySearchTree<Comparable> & rhs )
    :root( NULL ), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND )
        {
            *this = rhs;
        }

/*
 * Internal method to clone subtree.
 */
template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::clone( BinaryNode<Comparable> * t ) const
{
    if ( t == NULL )
        return NULL;
    else
        return new BinaryNode<Comparable>( t->element,t->info,clone( t->left ),clone( t->right ) );
}

/*
 * Operator overload =
*/
template <class Comparable>
void BinarySearchTree<Comparable>::operator=( const BinarySearchTree<Comparable> & rhs )
{
    //:ITEM_NOT_FOUND( notFound ), root( NULL ), left(NULL), right(NULL), TreeTitle(title){}
    
    root = clone(rhs.root);
    left = NULL;
    right = NULL;
    TreeTitle = rhs.TreeTitle;
}

/*
 * Getting title of the tree
*/
template <class Comparable>
string BinarySearchTree<Comparable>::getTitle() const
{
    return TreeTitle;
}


#endif /* Header_h */
