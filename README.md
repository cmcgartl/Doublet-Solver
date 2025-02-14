Doublet Game Solver (Word Ladder)

##Overview
This is a c++ implementation of the doublet game using A* search via a priority queue implemented by a Min Heap. 
Given a start word and end word as input, the program will print the shortest sequence of words that solves the game

##Doublet
Doublet is a game in which the player is provided a dictionary of words (word bank), from which a start word and goal word are selected.
The point of the game is to transform the start word into the goal word by only changing one letter at a time, and to do this with the least number of transformations possible.
Each letter change must produce a word that exists in the dictionary.

Example: Cold -> Warm (Assume all words used exist in the dictionary)
Cold -> Cord -> Card -> Ward -> Warm 
Note at each step, only one letter is changed and produces a new valid word
For example, the move Cold -> Card is not valid, as two letters are changed in one step.

##Features: 
1. Uses A* search algorithm to find the optimal path
2. Implements a Min Heap to serve as a priority queue
3. Creates a graph with a node for each word, edges between words that form a valid move
4. Prints the path of the optimal solution

##How it works:
1. The program reads in a text file containing the list of words. The file contains the number of words in the file on its own line at the top of the file. Each word in the dictionary is then listed on its own line

2. The program creates a graph with a node for each word, and an edge between words that form a valid transformation

3. The program creates a map<string, Node>, with the string representing the word and the Node storing data for that word including neighbors graph, if it has been visited/expanded, and previous word in path

4. The Program then uses A* search using the min heap as a priority queue, sorting the words by lowest heuristic value: the number of letters in the current word that are different than the goal word.

    For example, if the goal word is Warm, and we want to determine the heuristic for the word Card, the heuristic function determines that C != W, A = A, R = R, D != M, therefore Card is at least 2 transformations from Warm: heuristic = 2

5. Once a solution is found, the program traces back through each words' recorded optimal predecessor to generate the solution path

##How to Run
1. Copy the repository URL and run git clone <url>, then cd to the cloned repository folder
2. use make to compile the program with the provided make file
3. Use the provided text file or create your own test file that follows the required format as follows:
    5
    AAAA
    BAAA
    BBAA
    BBBA
    BBBB

    **Note: all words must be the same length. If start and goal word are not of equal length, no solution is possible and the program will terminate accordingly. If there are intermediate words of different length, the program will not terminate prematurely, but these words cannot be considered in solution path.

4. run ./doublet <start word> <goal word> <dictionary text file>
5. If the words chosen have a solution path, the output will be as follows:
    Number of steps: <# steps>
    Number of expansions: <# expansions>
    Solution path: <Solution path>

** The Number of steps is the number of moves required to transform the start word to the goal word
** The number of expansions is the number of words that were expanded in looking for a solution where an expansion is adding all of the words neighbors to the queue and checking them for the solution/next step.