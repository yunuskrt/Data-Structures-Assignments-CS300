//  Yunus Kerestecioglu

#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>

#include "BinaryNotebook.h"
#include "AvlNotebook.h"

using namespace std;

void MENU(){
    cout << "MENU" << endl << "Please enter an input between[1-6]:" << endl
         << "1- Display the sections [AVL]" << endl << "2- Display the sections [BST]"
         << endl <<"3- Select a section" << endl <<"4- Add new section" << endl
         << "5- Delete a section" << endl << "6- Exit" << endl;
}

void SectionMENU(){
    cout << "Please enter an input between [1-7]:" << endl
         << "1- Display the items [AVL]" << endl << "2- Display the items [BST]"
         << endl <<"3- Display the information of a item" << endl << "4- Add new item"
         << endl << "5- Update the information of a item" << endl
    << "6- Delete an item" << endl << "7- Return to main menu" << endl;

}
/*
 * This function will be used when the line starts with '-'.
 * To parse the line into itemTitle and itemInfo
*/
void splitLine(const string & line, string & title, string & info){

    int first = line.find("-");
    int second = line.rfind("-");
    title = line.substr(first+1,second-1);
    info = line.substr(second+1);
}
/*
 * This function constructs the BST and AVL notebook by returning them by reference.
*/
void constructNotebooks(BinaryNotebook & binNote, AvlNotebook & avlNote){
    ifstream input;
    string itemTitle, itemInfo;
    string filename = "data.txt", line;
    input.open(filename.c_str());
    
    BinaryNotebook binNotebook;
    AvlNotebook avlNotebook;
    
    int i = 0;
    BinarySearchTree<string> bst("notFound","title");
    AvlTree<string> avl("notFound","title");
    while(getline(input,line)){
        if (line.at(0)!='-'){ // construct trees (section line)
            if(i > 0){
                auto start = chrono::high_resolution_clock::now(); // start time
                avlNotebook.AddSection(avl);
                auto end = chrono::high_resolution_clock::now(); // end time
                long long timer = (end-start).count() / 1000.0;
                cout << "Section " << "\"" << avl.getTitle() << "\"" << " has been inserted into the AVL notebook." << endl;
                cout << "[AVL] Elapsed time: " << timer <<" microseconds" << endl;
                
                auto start2 = chrono::high_resolution_clock::now(); // start time
                binNotebook.AddSection(bst);
                auto end2 = chrono::high_resolution_clock::now(); // end time
                long long timer2 = (end2-start2).count() / 1000.0;
                cout << "Section " << "\"" << bst.getTitle() << "\"" << " has been inserted into the BST notebook." << endl;
                cout << "[BST] Elapsed time: " << timer2 <<" microseconds" << endl << endl;
            }
            BinarySearchTree<string> newBstTree("notFound",line);
            AvlTree<string> newAvlTree("notFound",line);
            
            bst = newBstTree;
            avl = newAvlTree;
        
            i++;
        }
        else{ // modify trees (item line)
            splitLine(line, itemTitle, itemInfo);
            bst.insert(itemTitle, itemInfo);
            avl.insert(itemTitle, itemInfo);
        }
    }
    auto start = chrono::high_resolution_clock::now(); // start time
    avlNotebook.AddSection(avl);
    auto end = chrono::high_resolution_clock::now(); // end time
    long long timer = (end-start).count() / 1000.0;
    cout << "Section " << "\"" << avl.getTitle() << "\"" << " has been inserted into the AVL notebook." << endl;
    cout << "[AVL] Elapsed time: " << timer <<" microseconds" << endl;
    
    auto start2 = chrono::high_resolution_clock::now(); // start time
    binNotebook.AddSection(bst);
    auto end2 = chrono::high_resolution_clock::now(); // end time
    long long timer2 = (end2-start2).count() / 1000.0;
    cout << "Section " << "\"" << bst.getTitle() << "\"" << " has been inserted into the BST notebook." << endl;
    cout << "[BST] Elapsed time: " << timer2 <<" microseconds" << endl << endl;
    input.close();
    
    // returning notebooks by reference
    binNote = binNotebook;
    avlNote = avlNotebook;
}


int main(){
    
    // Welcome Message
    cout << "Welcome to the Notebook!" << endl << endl;
    
    // Forming empty notebooks
    BinaryNotebook binNotebook;
    AvlNotebook avlNotebook;
    
    // assigning corresponding values to empty notebooks
    constructNotebooks(binNotebook, avlNotebook);

    int menuChoice, sectionChoice; // options for menus (controlling while loops)
    
    MENU();
    
    do{
        cout << "Input: ";
        cin >> menuChoice;
        if (menuChoice == 1){ // Display the sections [AVL]
            avlNotebook.DisplaySections();
        }
        else if (menuChoice == 2){ // Display the sections [BST]
            binNotebook.DisplaySections();
        }
        else if (menuChoice == 3){ // Select a section
            string sectionName;
            cout << "Enter the title of the section: ";
            cin >> sectionName;
            
            AvlTree<string> * selectedAvlTree = avlNotebook.SelectSection(sectionName);
            BinarySearchTree<string> * selectedBinTree = binNotebook.SelectSection(sectionName);
            
            if (selectedAvlTree && selectedBinTree){ // given tree is found
                cout << endl <<"Selected section -> " << selectedBinTree->getTitle() << endl;
                SectionMENU();
                do{
                    
                    cout << "Input: ";
                    cin >> sectionChoice;
                    if (sectionChoice == 1){ // Display the items [AVL]
                        selectedAvlTree->printTree();
                    }
                    else if (sectionChoice == 2){ // Display the items [BST]
                        selectedBinTree->printTree();
                    }
                    else if (sectionChoice == 3){ // Display the information of a item
                        string itemTitle;
                        cout << "Enter the title of item: ";
                        cin >> itemTitle;
                        
                        auto start = chrono::high_resolution_clock::now(); // start time
                        AvlNode<string> * selectedAvlItem = selectedAvlTree->find(itemTitle);
                        auto end = chrono::high_resolution_clock::now(); // end time
                        long long timer = (end-start).count() / 1000.0;
                        cout << "[AVL] Elapsed time: " << timer <<" microseconds" << endl;
                        
                        auto start2 = chrono::high_resolution_clock::now(); // start time
                        BinaryNode<string> * selectedBinItem = selectedBinTree->find(itemTitle);
                        auto end2 = chrono::high_resolution_clock::now(); // end time
                        long long timer2 = (end2-start2).count() / 1000.0;
                        cout << "[BST] Elapsed time: " << timer2 <<" microseconds" << endl;
                        
                        if (selectedAvlItem && selectedBinItem) // given item is found
                            cout << selectedAvlItem->getInfo() << endl << endl;
                        else
                            cout << "Invalid title." << endl << endl;
                    }
                    else if (sectionChoice == 4){ // Add new item
                        string itemTitle, itemDesc;
                        cout << "Enter a title for the item: ";
                        cin >> itemTitle;
                        
                        if(selectedBinTree->find(itemTitle)) // item already exist in the tree
                            cout << "Item " "\"" << itemTitle << "\" already exists in the \"" << selectedBinTree->getTitle() << "\"." << endl << endl;
                        
                        else{ // insert item into tree
                            cout << "Enter a description for the item: ";
                            cin >> itemDesc;
                            
                            auto start = chrono::high_resolution_clock::now(); // start time
                            selectedAvlTree->insert(itemTitle, itemDesc);
                            auto end = chrono::high_resolution_clock::now(); // end time
                            long long timer = (end-start).count() / 1000.0;
                            cout << "[AVL] Elapsed time: " << timer <<" microseconds" << endl;
                            
                            auto start2 = chrono::high_resolution_clock::now(); // start time
                            selectedBinTree->insert(itemTitle, itemDesc);
                            auto end2 = chrono::high_resolution_clock::now(); // end time
                            long long timer2 = (end2-start2).count() / 1000.0;
                            cout << "[BST] Elapsed time: " << timer2 <<" microseconds" << endl;
                            
                            cout << "The new item \"" << itemTitle << "\" has been inserted." << endl << endl;
                            
                        }
                    }
                    else if (sectionChoice == 5){ // Update the information of a item
                        string itemTitle,itemNewInfo;
                        
                        cout << "Enter the title of the item: ";
                        cin >> itemTitle; // item will be searched by its title
                        
                        auto start = chrono::high_resolution_clock::now(); // start time
                        AvlNode<string> * selectedAvlItem = selectedAvlTree->find(itemTitle);
                        auto end = chrono::high_resolution_clock::now(); // end time
                        long long timer = (end-start).count() / 1000.0;
                        cout << "[AVL] Elapsed time: " << timer <<" microseconds" << endl;
                        
                        auto start2 = chrono::high_resolution_clock::now(); // start time
                        BinaryNode<string> * selectedBinItem = selectedBinTree->find(itemTitle);
                        auto end2 = chrono::high_resolution_clock::now(); // end time
                        long long timer2 = (end2-start2).count() / 1000.0;
                        cout << "[BST] Elapsed time: " << timer2 <<" microseconds" << endl;
                        
                        if (selectedAvlItem && selectedBinItem){ // item is found
                            
                            // getting the new item info.
                            cout << "Enter the new information: ";
                            cin >> itemNewInfo;
                            
                            // setting the new info.
                            selectedBinItem->setInfo(itemNewInfo);
                            selectedAvlItem->setInfo(itemNewInfo);
                            cout << "The content " << itemTitle << " has been updated." << endl << endl;
                            
                        }
                        else // item does not exist in the tree
                            cout << "Item \"" << itemTitle << "\" does not exist in the \"" << selectedBinTree->getTitle() << "\"." << endl << endl;
                    }
                    
                    else if (sectionChoice == 6){ // Delete an item
                        string itemTitle;
                        cout << "Enter the title of the item: ";
                        cin >> itemTitle;
                        
                        AvlNode<string> * selectedAvlItem = selectedAvlTree->find(itemTitle);
                        BinaryNode<string> * selectedBinItem = selectedBinTree->find(itemTitle);
                        
                        if (selectedBinItem && selectedAvlItem){ // item found
                            auto start = chrono::high_resolution_clock::now(); // start time
                            selectedAvlTree->remove(itemTitle);
                            auto end = chrono::high_resolution_clock::now(); // end time
                            long long timer = (end-start).count() / 1000.0;
                            cout << "[AVL] Elapsed time: " << timer <<" microseconds" << endl;
                            
                            auto start2 = chrono::high_resolution_clock::now(); // start time
                            selectedBinTree->remove(itemTitle);
                            auto end2 = chrono::high_resolution_clock::now(); // end time
                            long long timer2 = (end2-start2).count() / 1000.0;
                            cout << "[BST] Elapsed time: " << timer2 <<" microseconds" << endl;
                            
                            cout << "The item \"" << itemTitle << "\" has been deleted." << endl << endl;
                        }
                        else // item not found
                            cout << "Item \"" << itemTitle << "\" does not exist in the \"" << selectedBinTree->getTitle() << "\"." << endl << endl;
                            
                    }
                    else{ // Display Menu
                        MENU();
                    }
                }while(sectionChoice != 7); //
            }
            else
                cout << "Invalid title!" << endl << endl;
            
        }
        
        else if (menuChoice == 4){ // Add new section
            string sectionTitle;
            cout << "Enter a title for the section: ";
            cin >> sectionTitle;
            
            if ( binNotebook.SelectSection(sectionTitle) ) // given tree is in noteboook
                cout << "Section " << "\"" << sectionTitle << "\" already exists." << endl << endl;
            else{ // add new tree into notebook
                binNotebook.AddSection(BinarySearchTree<string>("notFound",sectionTitle));
                avlNotebook.AddSection(AvlTree<string>("notFound",sectionTitle));
                cout << "The new section " << "\"" << sectionTitle << "\" has been inserted." << endl << endl;
            }
        }
        
        else if (menuChoice == 5){ // Delete a section
            // get the title of section
            string sectionTitle;
            cout << "Enter the title of the section: ";
            cin >> sectionTitle;
     
            if(avlNotebook.SelectSection(sectionTitle)){
                binNotebook.DeleteSection(sectionTitle);
                avlNotebook.DeleteSection(sectionTitle);
                cout << "The section has been deleted." << endl << endl;
            }
            else // section not in notebook
                cout << "Section \"" << sectionTitle << "\" does not exist." << endl << endl;
        }
    }while(menuChoice!=6);
    
    return 0;
}
