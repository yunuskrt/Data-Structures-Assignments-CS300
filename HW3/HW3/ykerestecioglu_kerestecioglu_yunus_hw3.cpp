// Yunus Kerestecioglu

#include <iostream>
#include <fstream>
#include <sstream>
#include "BinarySearchTree.h"
#include <string>
#include <vector>
#include "HashTable.h"
#include <chrono>

using namespace std;

// {{“a.txt”, 1}, {“b.txt”, 4}}.
// outerList -> WordItem,  innerList -> DocumentItem
struct DocumentItem {
    string documentName;
    int count;
    
    DocumentItem() // default constructor
    {
        documentName = " ";
        count = 0;
    }
    DocumentItem(string doc, int c = 1) // parametric constructor
    {
        documentName = doc;
        count = c;
    }
    
    bool operator !=(DocumentItem d) const{
        return !(documentName == d.documentName && count == d.count);
    }
    void incrementCount(){
        count++;
    }
    string toStr() const{
        return "{" + documentName + ":"  + to_string(count) + "}";
    }
    

};

struct WordItem {
    string word;
    vector<DocumentItem> docCount; // List of DocumentItem’s.
    
    WordItem() // default constructor
    {
        word = " ";
        vector<DocumentItem> doc;
        docCount = doc;
    }
    WordItem(string w)
        :word(w)
    {
        vector<DocumentItem> doc;
        docCount = doc;
    }
    WordItem(string w, string filename) // parametric constructor
    {
        word = w;
        DocumentItem doc = DocumentItem(filename);
        docCount.push_back(doc);
        
    }
    
    // Operators ---> .length(),  [], !=, <<
    
    int length() const{
        return word.length();
    }
    void printWordItem () const {
        cout << word << ":  ";
        for (int d = 0; d < docCount.size();d++){
            cout << docCount[d].toStr() << ",";
        }
        cout << endl;
    }
    int operator [](int index) const{
        return int(word.at(index));
    }
    bool operator !=(WordItem w) const{
        return word != w.word;
    }
    bool operator ==(WordItem w) const{
        return word == w.word;
    }
    bool operator <(WordItem w) const{
        return word < w.word;
    }
    string toStr() const
    {
        string wordStr;
        wordStr +=  word + ":  ";
        for (int d = 0; d < docCount.size();d++){
            wordStr += docCount[d].toStr() + ",";
        }
        return wordStr;
    }
    friend ostream& operator << (ostream& os, const WordItem& rhs)
    {
        os << rhs.toStr();
        return os;
    }
};

/*
 input checks and makes words in correct format
 used while reading words from a file
*/
void handleWord(string & word){
// "face2face" = "face.face" = "face:facE" = "face face"
// case insensitive (for example; “izmir” and “Izmir” are the same). The rest(such as digits or punctuation characters) are processed as the separator for the words.
    for (int i = 0; i < word.length(); i++){
        char ch = word.at(i);
        if ('A' <= ch && ch <= 'Z') // uppercase alphabetic character -> convert to lowercase
            word.at(i) = char(int(ch)+32);
        else if(!('a' <= ch && ch <= 'z')) // not alphabetic character -> convert to " "
            word.at(i) = ' ';
    }
    
    int first = 0, last = word.length();
    while ( first <= last-1 && word.at(first) == ' ' ) {
        first++;
    }
    while (last >= first && word.at(last-1) == ' '){
        last--;
    }
    if(first > last)
        word = "";
    else
        word = word.substr(first,last);
}

/*
 get files into a list
*/
vector<string> getFiles(){
    vector<string> files;
    int numOfInputFiles;
    string filename;
    cout << "Enter number of input files: ";
    cin >> numOfInputFiles;
    for (int i = 0; i < numOfInputFiles; i++){
        cout << "Enter " << i+1 << ". file name: ";
        cin >> filename;
        files.push_back(filename);
    }
    return files;
}

/*
 process a word into data structue
*/
template<class DT>
void processWord(DT & dt, string wrd,string filename, bool dtCheck){
    if (wrd != ""){
        WordItem newItem = dt.find(WordItem(wrd));
        if (newItem == dt.getNotFound()) // word item not in hash table, insert it
            dt.insert(WordItem(wrd,filename),dtCheck);
        else{ // word item already exists in hash table
            bool check = true;
            for (int i = 0; i < newItem.docCount.size();i++){
                if (newItem.docCount[i].documentName == filename){
                    newItem.docCount[i].count++;
                    check = false; // given word already found in the document
                }
            }
            if (check) // given word found for the first time in the document but exists in another file
                newItem.docCount.push_back(DocumentItem(filename));
            dt.update(newItem); // update table in any case
        }
    }
}

/*
 process a file into data structure
*/
template<class DT>
void processFile(DT & dt,string filename, bool dtCheck){
    ifstream input;
    string word,word_prt;
    input.open(filename.c_str());
    while (input >> word){ // parse the file into lines
        handleWord(word);
        istringstream ss(word);
        while (ss >> word_prt){ // parse the line into words
            processWord(dt, word_prt, filename,dtCheck);
        }
    }
    input.close();
}

/*
 queryDocuments into a data structure
*/
template<class DT>
void QueryDocuments(DT & dt, const vector<string> & files, bool dtCheck = true){
    for (int i = 0; i < files.size(); i++)
        processFile(dt, files[i], dtCheck);
}

/*
 gets input from user and returns it in correct format
*/
string getUserInput(){
    string line;
    cout << "Enter queried words in one line: ";
    cin.ignore();
    getline(cin, line);
    handleWord(line);
    return line;
}

/*
 prints a WordItem in desired format
 grouping by file names
*/
void printWordItems(const vector<WordItem> & items, const vector<string> & files, int count){
    bool check = true;
    for (int i = 0; i < files.size(); i++){ // iterating over file names
        string filename = files[i];
        vector<string> words; // list for words in the file
        vector<int> counts; // list for corresponding counts in the file
    
        for (int j = 0; j < items.size(); j++){ // iterating over WordItems
            WordItem item = items[j];
            for (int x = 0; x < item.docCount.size(); x++){ // iterating over docCounts
                if (item.docCount[x].documentName == filename){ // word exists in the file
                    words.push_back(item.word);
                    counts.push_back(item.docCount[x].count);
                }
            }
        }
        
        int len = words.size();
        if (len == count && len > 0){ // there is a document that has all the words
            cout << "in Document " << filename << ", ";
            for (int w = 0; w < len-1; w++)
                cout << words[w] << " found " << counts[w] << " times, ";
            cout << words[len-1] << " found " << counts[len-1] << " times." << endl;
            
            check = false;
        }
    
    }
    if (check)
        cout << "No document contains the given query" << endl;
}

/*
 process an input from user for a data structure
*/
template<class DT>
void processUserInput(const DT & dt,string line,const vector<string> & files){
    vector<WordItem> items;
    string word;
    int count = 0;
    
    istringstream ss(line); // parse the user input
    while (ss >> word){
        if (word != ""){ // word is valid
            count++;
            WordItem newItem = dt.find(WordItem(word));
            if (newItem != dt.getNotFound())
                items.push_back(newItem);
            
        }
    }
    printWordItems(items, files, count);
}


// comparable find(Comparable),void insert(Comparable),void update(Comparableb),comparable getNotFound
// Data Structures:     processUserInput,processFile,processWord
int main() {
// DocumentItem     -->     string documentName, int count
// WordItem     -->     string word, vector<DocumentItem> docCount
    
// a : {"a.txt" : 2},{"d.txt" : 3},{"e.txt" : 1}
// the : {"a.txt" : 9},{"d.txt" : 1}
// to : {"e.txt" : 7}
// as : {"a.txt" : 22},{"d.txt" : 33},{"e.txt" : 11},

    // 1) Construct Data Structures
    WordItem w("Not Found");
    HashTable<WordItem> myHashTable(w);
    BinarySearchTree<WordItem> myBST(w);

    // 2) Get Files from User and Store it
    vector<string> files = getFiles();

    // 3) Query Documents
    QueryDocuments(myBST, files);
    QueryDocuments(myHashTable, files);

    cout << endl << endl << "After preprocessing, the unique word count is " << myHashTable.getCurrentSize() << ". Current load ratio is " << myHashTable.getLambda() << endl;

    // 4) Get input from User
    string userInput = getUserInput();
    
    // 5) Process the input
    processUserInput(myBST, userInput, files);
    processUserInput(myHashTable,userInput,files);
    
    // 6) For Timing
    int k = 2;
    long long int bstTime,htTime;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        WordItem notFound("Not Found");
        BinarySearchTree<WordItem> bst(notFound);
        QueryDocuments(bst,files,false);
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
                    (std::chrono::high_resolution_clock::now() - start);
    bstTime = BSTTime.count() / k;
    cout << "\nTime: " << bstTime << "\n";
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        WordItem notFound("Not Found");
        HashTable<WordItem> hsh(notFound);
        QueryDocuments (hsh,files,false);
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
                    (std::chrono::high_resolution_clock::now() - start);
    htTime = HTTime.count() / k;
    cout << "\nTime: " << htTime << "\n";

    cout << "Speed Up: " << (double)bstTime /htTime << endl;
    
    return 0;
}
