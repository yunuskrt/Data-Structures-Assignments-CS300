//  Yunus Kerestecioglu

#ifndef AVLNode_h
#define AVLNode_h

using namespace std;

template <class Comparable>
class AvlNode
{
    template <class Comparab> friend class AvlTree;
    public:
        AvlNode( const Comparable & theElement,const Comparable & elementInfo,AvlNode *lt, AvlNode *rt, int h = 0)
         : element( theElement ), info(elementInfo), left( lt ), right( rt ), height( h ) { }
    
        Comparable getInfo(){ 
            return info;
        }
        
        void setInfo(const Comparable & newInfo){
            info = newInfo;
        }
    
    private:
        Comparable element;
        Comparable info;
        AvlNode *left;
        AvlNode *right;
        int height;
};


#endif /* AVLNode_h */
