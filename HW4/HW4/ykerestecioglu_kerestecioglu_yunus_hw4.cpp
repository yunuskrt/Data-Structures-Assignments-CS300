// Yunus Kerestecioglu

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

using namespace std;

// returns a string to Uppercase
string toUpper(string word){
    string newWord;
    for (int i = 0; i < word.length(); i++)
        newWord += toupper(word[i]);
    return newWord;
}

// object that holds the record
struct PhoneRecord {
    string name;
    string surname;
    string phone;
    string city;
    
    PhoneRecord(){} // default constructor
    PhoneRecord(string n, string s, string p, string c) // parametric constructor
        :name(n),surname(s),phone(p),city(c){}
    string toStr() const{ // convert class to string
        return toUpper(name) + " " + toUpper(surname) + " " + phone + " " + city;
    }
};

// object will be used for full name search
struct FullName {
    string name;
    string surname;
    
    FullName(){};
    FullName(string n, string s)
        :name(n),surname(s){}
    
    string toStr() const{
        return name + " " + surname;
    }
    friend ostream & operator << (ostream& os, const FullName& rhs)
    {
        os << rhs.toStr();
        return os;
    }
};
string toUpper(FullName fn){
    return toUpper(fn.toStr());
}

// Operator for comparing phone records while sorting
const bool operator < (const PhoneRecord & lhs, const PhoneRecord & rhs){
    return ( lhs.name < rhs.name ) ||
     ( (lhs.name==rhs.name) && (lhs.surname < rhs.surname) );
}

// Operators for full contact name search
const bool operator == (const PhoneRecord & lhs, const FullName & rhs){
    return lhs.name == rhs.name && lhs.surname == rhs.surname;
}
const bool operator < (const PhoneRecord & lhs, const FullName & rhs){
    return ( lhs.name < rhs.name ) ||
           ( (lhs.name==rhs.name) && (lhs.surname<rhs.surname) );
}
const bool operator > (const PhoneRecord & lhs, const FullName & rhs){
    return !(lhs < rhs) && !(lhs == rhs);
}

// Operators for partial contact name search
const bool operator == (const PhoneRecord & lhs, const string & rhs){
    return lhs.name.find(rhs) == 0;
}
const bool operator < (const PhoneRecord & lhs, const string & rhs){
    return !(lhs == rhs) && lhs.name < rhs;
}
const bool operator > (const PhoneRecord & lhs, const string & rhs){
    return !(lhs == rhs) && !(lhs < rhs);
}

// Constructs the phonebooks from a file
void ConstructArrays(string filename, vector<PhoneRecord> & ins, vector<PhoneRecord> & qck, vector<PhoneRecord> & mrg, vector<PhoneRecord> & hp){
    
    string line,name,surname,phone,city;
    ifstream input;
    
    input.open(filename.c_str());
    while (getline(input,line)) {
        istringstream ss(line);
        ss >> name >> surname >> phone >> city;
        PhoneRecord record(name,surname,phone,city);
        ins.push_back(record);
        qck.push_back(record);
        mrg.push_back(record);
        hp.push_back(record);
    }
    input.close();
}

// Function for calculating speedup between two operations
double SpeedUp(double time1, double time2){
    return time1/time2;
}

// Swap Function for 2 elements
template <class Comparable>
inline void Swap( Comparable & obj1, Comparable & obj2){
    Comparable temp = obj1;
    obj1 = obj2;
    obj2 = temp;
}

// SORT FUNCTIONS

// --- Functions for Insertion Sort ---

// Insertion sort
template <class Comparable>
void InsertionSort (vector <Comparable> & vec)
{
    int j;
    for (int p = 1;  p < vec.size(); p++)
    {
        Comparable tmp = vec[p];
        for (j = p; j > 0 &&  tmp < vec[j - 1]; j--)
            vec[j] = vec[j-1];
        vec[j] = tmp;
    }
}

// --- Functions for Quick Sort ---

/*
    * Internal quicksort method that makes recursive calls.
    * a is an array of Comparable items.
    * left is the left-most index of the subarray.
    * right is the right-most index of the subarray.
*/
template <class Comparable>
void quicksort(vector<Comparable> & a, int left, int right){
    if (left < right){
        // 1) i=left-1, j=right
        int i = left-1, j = right;
        
        // 2) Select Pivot as median
        int median = (left+right)/2;
        Comparable pivot = a[median];
        
        // 3) Swap Pivot and last element
        Swap(a[median], a[right]);
        
        for ( ; ; )
        {
            // 4) Move i right until an element larger than the pivot is found
            while ( a[ ++i ] < pivot ) { }

            // 5) Move j left until an element smaller than the pivot is found
            while ( pivot < a[ --j ] ) { }

            // 6) i is smaller than j, so swap the elements
            if ( i < j )
                Swap( a[ i ], a[ j ] );
            else
                break;
        }
        // 7) i and j have crossed, swap pivot with element at i
        Swap( a[ i ], a[ right ] );
        
        quicksort(a, left, i-1); // 8) sort smaller elements
        quicksort(a, i+1, right); // 8) sort larger elements
    }
}
        

// Quick sort
template <class T>
void QuickSort(vector<T> & vec){
    quicksort(vec, 0, vec.size()-1);
}

// --- Functions for merge sort ---

template <class Comparable>
void merge(vector<Comparable> & vec,int left, int mid, int right){
    
    // divide the place into 2
    int pos1 = left, pos2 = mid+1;

    while (pos1 <= mid && pos2 <= right) {
        
        Comparable val1 = vec[pos1], val2 = vec[pos2];
        // increment pos1 if value1 is smaller
        
        if (val2 < val1){
            for (int i = pos2; pos1 < i; i--)
                // shift elements right
                vec[i] = vec[i-1];
                
            // store element in right place
            vec[pos1] = val2;
            
            // increment all values since they are shifted right
            pos1++;
            pos2++;
            mid++;
            
        }
        else
            pos1++;
    }
}

template <class Comparable>
void mergesort(vector<Comparable> & vec, int left, int right ){
    if (left < right){
        int center = ( left + right )/ 2;
        mergesort(vec, left, center);
        mergesort(vec, center+1, right);
        merge(vec, left, center, right);
    }
}

// Merge sort
template <class Comparable>
void MergeSort(vector<Comparable> & vec)
{
    mergesort(vec, 0, vec.size()-1);
}

// --- Functions for heap sort ---

inline int leftChild( int i )
{
    return 2*i+1;
}
template <class Comparable>
// for deleteMax
// a is the array, i is the position to percolate down from
// n is the logical size of the array
void percDown( vector<Comparable> & a, int i, int n ){
    int child;
    Comparable tmp;

    for (tmp=a[i] ; leftChild(i) < n; i = child ){
        child = leftChild(i);
        if ( child != n-1 && a[ child  ] < a[ child+1 ] )
            child++;
        if ( tmp < a[child] )
            a[i] = a[ child ];
        else
            break;
    }
    a[ i ] = tmp;
}

// Heap sort
template <class Comparable>
void HeapSort(vector<Comparable> & a)
{
    // buildHeap
    for (int i = a.size()/2; i >=0; i--)
        percDown(a, i, a.size());

    // sort
    for (int j = a.size()-1; j >0; j--)
    {
        swap(a[0], a[j]);    // swap max to the last pos.
        percDown(a, 0, j); // re-form the heap
    }
}


// SEARCH FUNCTIONS

// Sequential search
template <class T>
void SequentialSearch (const vector<PhoneRecord> & phonebook, const T & element, bool printCheck = true)
{
    bool itemFound = false;
    for (int i = 0; i < phonebook.size(); i++){
        PhoneRecord record = phonebook[i];
        if (record == element){
            itemFound = true;
            if (printCheck)
                cout << record.toStr() << endl;
        }
    }
    if (!itemFound && printCheck)
        cout << toUpper(element) << " does NOT exist in the dataset" << endl;
}

// Binary search
template <class T>
void BinarySearch (const vector<PhoneRecord> & phonebook, const T & element, bool printCheck = true)
{
    int left=0, right=phonebook.size()-1, mid;
    mid = (left+right)/2;
    bool itemFound = false;
    
    while (left <=right && !itemFound) {
        mid = (left+right)/2;
        PhoneRecord record = phonebook[mid];
        if (record < element) // item is larger, get the right half
            left = mid + 1;
        else if (record > element) // item is smaller, get the left half
            right = mid-1;
        else
            itemFound = true; // item is found
    }
    
    if (!itemFound && printCheck)
        cout << toUpper(element) << " does NOT exist in the dataset" << endl;
    else{
        // search for multiple values
        while (phonebook[mid-1] == element && mid > 0)
            mid--; // go down till no more same item
        while (phonebook[mid+1] == element) {
            // go beyond till no more same item
            if (printCheck)
                cout << phonebook[mid].toStr() << endl;
            mid++;
        }
        if (printCheck)
            cout << phonebook[mid].toStr() << endl; // print the current item in any case
    }
}

int main() {
    //  1) Get inputs from user, (file to process and query to search)
    string filename, query;
    cout << "Please enter the contact file name:" << endl;
    cin >> filename;
    cout << "Please enter the word to be queried :" << endl;
    cin.ignore();
    getline(cin,query);

    //  2) Read the input file and Load the PhoneBook into different vectors
    vector<PhoneRecord> insertion,quick,merge,heap;
    ConstructArrays(filename, insertion, quick, merge, heap);
    
    //  3) Perform sorting operations
    cout << endl << "Sorting the vector copies" << endl << "======================================" << endl;
    
    auto start = std::chrono::high_resolution_clock::now(); // time measure for quick sort
    QuickSort(quick);
    auto QuickSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>
    (std::chrono::high_resolution_clock::now() - start);
    double qckTime = QuickSortTime.count();
    cout << "Quick Sort Time: " << qckTime<< " Nanoseconds\n";
    
    start = std::chrono::high_resolution_clock::now(); // time measure for insertion sort
    InsertionSort(insertion);
    auto InsertionSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>
    (std::chrono::high_resolution_clock::now() - start);
    double insTime = InsertionSortTime.count();
    cout << "Insertion Sort Time: " << insTime<< " Nanoseconds\n";
    
    start = std::chrono::high_resolution_clock::now(); // time measure for merge sort
    MergeSort(merge);
    auto MergeSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>
    (std::chrono::high_resolution_clock::now() - start);
    double mrgTime = MergeSortTime.count();
    cout << "Merge Sort Time: " << mrgTime<< " Nanoseconds\n";
    
    start = std::chrono::high_resolution_clock::now(); // time measure for heap sort
    HeapSort(heap);
    auto HeapSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>
    (std::chrono::high_resolution_clock::now() - start);
    double heapTime = HeapSortTime.count();
    cout << "Heap Sort Time: " << heapTime<< " Nanoseconds\n";
    
    //  4) Process the Query
    bool fullNameSearch = false; // boolean value to check if full contact name search or partial contact name search
    string name,surname;
    FullName full_name;
    istringstream ss(query);
    ss >> name;
    if(ss >> surname){ // go with full name search
        fullNameSearch = true;
        full_name = FullName(name, surname);
    }

    //  5) Perform searching operations
    cout << endl << "Searching for " << query << endl << "======================================" << endl;
    
    double binarySearchTime,sequentialSearchTime;
    int K = 100;
    if (fullNameSearch){ // Full Contact Name Search
        start = std::chrono::high_resolution_clock::now(); // time measure for binary search
        for (int i = 0; i < K; i++)
            BinarySearch(insertion, full_name, false);
        auto BSTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
        binarySearchTime = BSTime.count();
        BinarySearch(insertion, full_name);
        cout << "Binary Search Time: " << binarySearchTime << " Nanoseconds" << endl << endl;

        start = std::chrono::high_resolution_clock::now(); // time measure for sequential search
        for (int i = 0; i < K; i++)
            SequentialSearch(insertion, full_name, false);
        auto SSTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
        sequentialSearchTime = SSTime.count();
        cout << "Search results for Sequential Search:" << endl;
        SequentialSearch(insertion, full_name);
        cout << "Sequential Search Time: " << sequentialSearchTime << " Nanoseconds" << endl;
    }
    else{ // Partial Contact Name Search
        start = std::chrono::high_resolution_clock::now(); // time measure for binary search
        for (int i = 0; i < K; i++)
            BinarySearch(insertion, name, false);
        auto BSTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
        binarySearchTime = BSTime.count();
        BinarySearch(insertion, name);
        cout << "Binary Search Time: " << binarySearchTime << " Nanoseconds" << endl << endl;

        start = std::chrono::high_resolution_clock::now(); // time measure for sequential search
        for (int i = 0; i < K; i++)
            SequentialSearch(insertion, name, false);
        auto SSTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
        sequentialSearchTime = SSTime.count();
        cout << "Search results for Sequential Search:" << endl;
        SequentialSearch(insertion, name);
        cout << "Sequential Search Time: " << sequentialSearchTime << " Nanoseconds" << endl;
    }
    cout << endl;
    
    // 6) Calculate SpeedUps
    cout << "SpeedUp between Search Algorithms" << endl << "======================================" << endl;
    cout << "(Sequential Search/ Binary Search) SpeedUp = " << SpeedUp(sequentialSearchTime, binarySearchTime) << endl << endl;
    
    cout << "SpeedUps between Sorting Algorithms" << endl << "======================================" << endl;
    cout << "(Insertion Sort/ Quick Sort) SpeedUp = " << SpeedUp(insTime, qckTime) << endl;
    cout << "(Merge Sort / Quick Sort) SpeedUp = " << SpeedUp(mrgTime, qckTime) << endl;
    cout << "(Heap Sort / Quick Sort) SpeedUp = " << SpeedUp(heapTime, qckTime) << endl;
   
    return 0;
}
