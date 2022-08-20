//  Yunus Kerestecioglu
#pragma once
#include <vector>
#include <cmath>
using namespace std;

#ifndef HashTable_h
#define HashTable_h

template <class HashedObj>
class HashTable
{
    public:
        enum EntryType { ACTIVE, EMPTY, DELETED }; // ---
        struct HashEntry
        {
            HashedObj element;
            EntryType info;

            HashEntry( const HashedObj & e = HashedObj( ),EntryType i = EMPTY )
                : element( e ), info( i ) { }
        };
    
        explicit HashTable( const HashedObj & notFound, int size = 53 ); // ***
        HashTable( const HashTable & rhs ) // ***
            : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
                     array( rhs.array ), currentSize( rhs.currentSize ) { }

        const HashedObj & find( const HashedObj & x ) const; // ***

        void makeEmpty( ); // ***
        void insert( const HashedObj & x,bool printCheck = true ); // ***
        void remove( const HashedObj & x ); // ***
            
        double getLambda(){ // get lambda of hash table
            return (double)currentSize/array.size();
        }
        int getArraySize(){ // get capacity of hash table
            return array.size();
        }
        int getCurrentSize(){ // get number of elements in hash table
            return currentSize;
        }
    
        void printActiveElements(){ // prints inserted elements in hash table
            for (int i = 0; i < array.size();i++){
                HashEntry entry = array[i];
                if (entry.info == ACTIVE)
                    cout << "Element at index " << i << ": " << entry.element << endl;
            }
        }
        void printAll(){ // prints all elements in hash table
            for (int i = 0; i < array.size(); i++)
                cout << "Element at index " << i << ": " << array[i].element << " status: " << array[i].info << endl;
        }
    
        const HashTable & operator=( const HashTable & rhs );
    
        void update(const HashedObj & x){
            int position = findPos(x);
            if (isActive(position))
                array[position].element = x;
        }
        const HashedObj getNotFound() const{
            return ITEM_NOT_FOUND;
        }
            
        HashedObj getItem(int index){
            return array[index].element;
        }
            
    private:
        
        vector<HashEntry> array; // ---
        int currentSize; // ---
        const HashedObj ITEM_NOT_FOUND; // ---

        int hash(const HashedObj & key, int tableSize) const; // ***
        bool isActive( int currentPos ) const; // ***
        int findPos( const HashedObj & x ) const; // ***
        void rehash( ); // ***
        bool isPrime(int n) const; // ***
        int nextPrime(int n) const; // ***
};
 
/*
 * Construct the hash table.
*/
template <class HashedObj>
HashTable<HashedObj>::HashTable( const HashedObj & notFound, int size )
: ITEM_NOT_FOUND( notFound ), array( nextPrime( size ) )
{
    makeEmpty( );
    currentSize = 0;
}

/*
 * hash function
*/
template <class HashedObj>
int HashTable<HashedObj>::hash(const HashedObj & key, int tableSize) const
{
    int hashVal = 0;
  
    for (int i = 0; i < key.length();i++)
      hashVal = 37 * hashVal + key[ i ];

    hashVal = hashVal % tableSize;

    if (hashVal < 0)
      hashVal = hashVal + tableSize;

    return(hashVal);
}

/*
 * Method that performs quadratic probing resolution.
 * Return the position where the search for x terminates.
*/
template <class HashedObj>
int HashTable<HashedObj>::findPos( const HashedObj & x ) const // (private)
{
    int collisionNum = 0;
    int currentPos = hash( x, array.size( ) );

    while ( array[ currentPos ].info != EMPTY && array[ currentPos ].element != x )
    {
        currentPos += pow(++collisionNum, 2) ;  //add the difference
        if ( currentPos >= array.size( ) )              // perform the mod
            currentPos %= array.size( );                // if necessary
    }
    return currentPos;
}

/*
 * Return true if currentPos exists and is active.
*/
template <class HashedObj>
bool HashTable<HashedObj>::isActive( int currentPos ) const // (private)
{
    return array[ currentPos ].info == ACTIVE;
}

/*
 * Remove item x from the hash table.
 *  x has to be in the table
*/
template <class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x )
{
      int currentPos = findPos( x );
      if ( isActive( currentPos ) )
          array[ currentPos ].info = DELETED;
}

/*
 * Find item x in the hash table.
 * Return the matching item, or ITEM_NOT_FOUND, if not found.
*/
template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find( const HashedObj & x ) const
{
    int currentPos = findPos( x );
    if (isActive( currentPos ))
        return array[ currentPos ].element;
    
    return   ITEM_NOT_FOUND;
}

/*
 * Insert item x into the hash table. If the item is
 * already present, then do nothing.
*/
template <class HashedObj>
void HashTable<HashedObj>::insert( const HashedObj & x, bool printCheck)
{
    // Insert x as active
    int currentPos = findPos( x );
    if ( isActive( currentPos ) )
        return;
    array[ currentPos ] = HashEntry( x, ACTIVE );
    
    // enlarge the hash table if necessary
    if ( ++currentSize >= array.size( ) *(0.9) ){
        int oldSize = getArraySize();
        rehash( );
        if (printCheck)
            cout << "rehashed..." << endl << "previous table size:" << oldSize << ", new table size: " << getArraySize() << ", current unique word count " << currentSize << ", current load factor: " << getLambda() << endl;
    }
 }

/*
 * Expand the hash table.
*/
template <class HashedObj>
void HashTable<HashedObj>::rehash( )
{
    vector<HashEntry> oldArray = array;
    
    // Create new double-sized, empty table
    array = vector<HashEntry>( nextPrime( 2 * oldArray.size( ) ));
    makeEmpty();
    
    // Copy table over
    currentSize = 0;
    for ( int i = 0; i < oldArray.size( ); i++ )
        if ( oldArray[ i ].info == ACTIVE )
            insert( oldArray[ i ].element );
}

/*
 * Internal method to test if a positive number is prime.
 * Not an efficient algorithm.
*/
template <class HashedObj>
bool HashTable<HashedObj>::isPrime( int n ) const // (private)
{
    if ( n == 2 || n == 3 )
        return true;

    if ( n == 1 || n % 2 == 0 )
        return false;
    
    for ( int i = 3; i * i <= n; i += 2 )
        if ( n % i == 0 )
            return false;
    
    return true;
}

/*
 * Internal method to return a prime number
 * at least as large as n.  Assumes n > 0.
*/
template <class HashedObj>
int HashTable<HashedObj>::nextPrime(int n) const // (private)
{
    if (n % 2 == 0)
        n++;
    for ( ; ! isPrime(n); n+= 2 )
        ;
    return n;
}

/*
 * Make the hash table logically empty.
*/
template <class HashedObj>
void HashTable<HashedObj>::makeEmpty( )
{
    for( int i = 0; i < array.size( ); i++ ){
        if (array[i].info != EMPTY)
            array[i].info = EMPTY;
    }
    // make all the elements in the vector empty
}


#endif /* HashTable_h */
