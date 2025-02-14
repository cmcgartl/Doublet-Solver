#include <iostream>
#include <fstream>
#include "MinHeap.h"
#include <string>
#include <map>
#include <cctype>
using namespace std;

struct Node //Node contains needed information for each word in Asearch
{ 
    vector<string> edges; //edges contains all of the neighboring words in doublet
    bool hasBeenExpanded = false;
    int distance = 0;
    bool unVisited = true;
    bool queued = false;
    int queuedPriority = -1;
    Node *Optimal_previous = nullptr;
    string associated_word;
};
//forms the graph needed for A search
void makeGraph(map<string, Node>& newmap)
{
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    map<string, Node>::iterator it = newmap.begin();
    string tempstring;
    while(it !=newmap.end()){ 
        for(int i = 0; i < it->first.length(); i++){ //for every possible 1 letter difference in a given word
            for(int j = 0; j < alphabet.length(); j++){
                tempstring = it->first;
                if((tempstring[i]) != alphabet[j]){
                    tempstring[i] = alphabet[j];
                    map<string, Node>::iterator temp = newmap.find(tempstring); 
                    if(temp != newmap.end()){ //if the created word is in the map, add it to that word's edge list
                        it->second.edges.push_back(tempstring);
                    }
                }
            }

        }
    it++;
    }
}
//converts string to all uppercase
string makeUpper(string word)
{
    for(int i = 0; i < word.length(); i++){
        word[i] = toupper(word[i]);
    }
    return word;
}
//returns the h value for heuristic calculation
int getHValue(map<string,Node>& mymap, string word, string target)
{
    int count = 0;

    //make sure word and target are equal length
    if(word.length() != target.length()){
        cout << "No solution Possible: input word is not the same length as the target word" << endl;
        exit(0);
    }

    //count the number of letters that are different
    for(int i = 0; i < word.length(); i++){
        if(word[i] != target[i]){
            count++;
        }
    }
    return count;
}
//calculates the heuristic value that is used to set the priority for 
//a word when entered in in the map
int getPriority(map<string,Node>& mymap, string word, string target)
{
    int h = getHValue(mymap, word, target);
    int g = mymap.at(word).distance;
    int n = word.length();
    return ((h + g) * (n + 1) + h);
}

//function will only be called upon successfully finding a solution
//Given the goal and start node, recursively walk balkwards along the path found and print it
void print_solution_path(Node &start, Node &goal, map<string, Node> &graph, vector<string> &path){
    //base case: if the goal word is equal to the start word, then print the path
    if(start.associated_word == goal.associated_word){
        for(int i = path.size() - 1; i >= 0; i--){
            cout << path[i];
            if(i > 0){
                cout << " -> ";
            }
        }
        cout << endl;
        return;
    }
    //ensures no infinite recursion, if we find no previous step (error occurred), return
     if(goal.Optimal_previous == nullptr){
        return;
    }
    //the next word in the path is the word associated with the previously accessed node
    string next_step = goal.Optimal_previous->associated_word;
    path.push_back(next_step);

    //recursive call with start and the previous node to goal
    print_solution_path(start, *goal.Optimal_previous, graph, path);
}

//A* search to find the shortest path between start and goal word
void Asearch(string start, string goal, map<string,Node>& mymap)
{   
    //ensure a solution is theoretically possible given the start and end words
    if(mymap.find(start) == mymap.end()){
        cout << "No solution possible: start word is not in the word bank" << endl;
        return;
    }
    if(mymap.find(goal) == mymap.end()){
        cout << "No solution possible: goal word is not in the word bank" << endl;
        return;
    }
    start = makeUpper(start);
    goal = makeUpper(goal);
    int steps = 0;
    int expansions = 0;
    string temp;
    MinHeap<string> Heap(2);
    Heap.add(start, getPriority(mymap, start, goal));
    mymap.at(start).unVisited = false;

    //while items are in the heap, execute search
    while(!Heap.isEmpty()){ 
        temp = Heap.peek(); //pull root item to be expanded
        while(!Heap.isEmpty() && mymap.at(temp).hasBeenExpanded == true){ //if item has already been expanded
            Heap.remove(); //remove it without searching it's neighbors
            if(Heap.isEmpty() == false){
                temp = Heap.peek();
            }
        }
        mymap.at(temp).hasBeenExpanded = true;
        if(mymap.at(goal).hasBeenExpanded == true){ //if goal word has been expanded, then done
            steps = mymap.at(goal).distance;
            cout << "Number of steps: " << steps << endl;
            cout << "Number of expansions: " << expansions << endl;
            vector<string> sol_path;
            sol_path.push_back(mymap.at(goal).associated_word); //add goal word to solution path
            cout << "Solution path: ";

            //recursively build solutino path starting from goal word and print
            print_solution_path(mymap.at(start), mymap.at(goal), mymap, sol_path);
            cout << endl;
            return;
        }
        if(!Heap.isEmpty()){ //expand root item if heap not empty,
            Heap.remove();
            expansions++;
        }
        for(int i = 0; i < mymap.at(temp).edges.size(); i++){ //for every edge from a given word
            string neighbor = mymap.at(temp).edges[i];
            //if neighbor hasn't been visited or if reached neighbor with new shorter path, update neighbor's distance
            //also update neighbor's current optimal previous step to temp
            if(mymap.at(neighbor).unVisited == true || mymap.at(temp).distance + 1 < mymap.at(neighbor).distance){
                mymap.at(neighbor).distance = mymap.at(temp).distance + 1; 
                mymap.at(neighbor).Optimal_previous = &mymap.at(temp);
            }
            //if the neighbor has not been expanded and isn't in queue, add to queue 
            if(mymap.at(neighbor).hasBeenExpanded == false){
                if(mymap.at(neighbor).queued == false){
                    mymap.at(neighbor).queuedPriority = getPriority(mymap, neighbor, goal);
                    Heap.add(neighbor, getPriority(mymap, neighbor, goal));
                }
                //if neighbor has not been expanded and is in queue, if current priority is 
                //lower than queued priority, add it to the queue
                else if (mymap.at(neighbor).queuedPriority < getPriority(mymap, neighbor, goal)){
                    Heap.add(neighbor, getPriority(mymap, neighbor, goal));
                    }
                }
            mymap.at(neighbor).unVisited = false;
        }
     }
    if(mymap.at(goal).hasBeenExpanded == false){ //if goal not found, no solution
        cout << "No transformation possible" << endl;
        cout << "Number of expansions: " << expansions << endl;
        return;
    }
}

int main(int argc, char* argv[])
{
    if(argc < 4){
        cout << "not enough arguments" << endl;
        return 0;
    }
    int size = 0;
    string word;
    map<string, Node> newmap;
    ifstream ifile(argv[3]);
    if(ifile.fail()){
        cout << "couldn't open file" << endl;
        return 0;
    }
    ifile >> size;
    while(!ifile.fail()){
        ifile >> word;
        Node newNode;
        newNode.associated_word = word;
        newmap.insert(std::make_pair(makeUpper(word),newNode));
    }
    ifile.close();
    makeGraph(newmap);
    Asearch(argv[1], argv[2], newmap);
    return 0;
}

