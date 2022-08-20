// Yunus Kerestecioglu


#ifndef BinaryNode_h
#define BinaryNode_h

using namespace std;

template <class Comparable>
class BinaryNode
{
    template <class Comparab> friend class BinarySearchTree;
    friend class BinaryNotebook;
    
    public:
        BinaryNode( const Comparable & theElement, const Comparable & elementInfo,BinaryNode *lt, BinaryNode *rt )
            : element( theElement ), info(elementInfo), left( lt ), right( rt ) { }
    
        Comparable getInfo(){
            return info;
        }
        void setInfo(const Comparable & newInfo){
            info = newInfo;
        }
    
    private:
        Comparable element;
        Comparable info;
        BinaryNode  *left;
        BinaryNode  *right;
};


#endif /* BinaryNode_h */




