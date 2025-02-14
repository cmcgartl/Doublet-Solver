#include <iostream>
#include <vector>
#include <string>

template <class T>
  class MinHeap {
     public:
       MinHeap (int d);
       /* Constructor that builds a d-ary Min Heap
          This should work for any d >= 2,
          but doesn't have to do anything for smaller d.*/

       ~MinHeap ();

       void add (T item, int priority);
         /* adds the item to the heap, with the given priority. */

       const T & peek () const;
         /* returns the element with smallest priority.  
			Break ties however you wish.  
			Throws an exception if the heap is empty. */

       void remove ();
         /* removes the element with smallest priority.
			Break ties however you wish.
            Throws an exception if the heap is empty. */

       bool isEmpty ();
         /* returns true iff there are no elements on the heap. */

   private:
    std::vector<std::pair<T, int>> items;
    int dimension;
    void trickleDown(int pos);
    void bubbleUp(int pos);
      // whatever you need to naturally store things.
      // You may also add helper functions here.
  };

template<typename T>
MinHeap<T>::MinHeap(int d)
{
  dimension = d;
}

template<typename T>
MinHeap<T>::~MinHeap()
{
}
//adds item to end of heap, sorts through bubble up
template<typename T>
void MinHeap<T>::add(T item, int priority)
{
  items.push_back(std::make_pair(item,priority));
  bubbleUp(items.size() - 1);
}
//if heap is not empty, returns the root key
template<typename T>
const T& MinHeap<T>::peek() const
{
  if(items.size() == 0){
    throw std::runtime_error("Heap is empty");
  }
  return items[0].first;
}
//if heap is not empty, removes the root item, resorts through trickledown
template<typename T>
void MinHeap<T>::remove()
{
  if(items.size() == 0){
    throw std::runtime_error("Heap is empty");
  }
  std::swap(items[0], items[items.size() - 1]);
  items.pop_back();
  trickleDown(0);
}

template<typename T>
bool MinHeap<T>::isEmpty()
{
  if(items.size() == 0){
    return true;
  }
  return false;
}

template<typename T>
void MinHeap<T>::bubbleUp(int pos)
{
  //item value is less than its parent, swap spots and recall bubbleUp
  if(pos > 0 && items[pos].second < items[(pos-1)/dimension].second){
    std::swap(items[pos], items[(pos -1)/dimension]);
    bubbleUp((pos-1)/dimension);
  }
  //if item value is equal to parent value, break tie by operator < on keys
  if(pos > 0 && items[pos].second == items[(pos-1)/dimension].second){
    if(items[pos].first < items[(pos-1)/dimension].first){
      std::swap(items[pos], items[(pos -1)/dimension]);
      bubbleUp((pos-1)/dimension);
    }
  }
}

template<typename T> 
void MinHeap<T>::trickleDown(int pos)
{
  int child = (dimension*pos) + 1;
  int smallestChild = -1;
  int end = std::min(child + dimension, static_cast<int>(items.size()));
  for(int i = child; i < end; i++){ //loop through children to find smallest
      if(smallestChild == -1){
        smallestChild = i;
      }
      else if(items[i].second < items[smallestChild].second){
        smallestChild = i;
      }
      else if(items[i].second == items[smallestChild].second){ //break tie with < op on key
        if(items[i].first < items[smallestChild].first){
         smallestChild = i;
        }
      }
  }
  //if smallestChild has been set and is less than its paarent, swap, recall trickledown
  if(smallestChild != -1){
    if (items[smallestChild].second < items[pos].second){
      std::swap(items[pos], items[smallestChild]);
      trickleDown(smallestChild);
    }
    //break tie
    else if(items[smallestChild].second == items[pos].second){
      if(items[smallestChild].first < items[pos].first){
        std::swap(items[pos], items[smallestChild]);
        trickleDown(smallestChild);
      }
    }
  }
}