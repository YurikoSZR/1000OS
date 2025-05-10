#include <vector>
#include <iostream>
#include <algorithm>
class heap{
#define LEFT(i) (2*i+1)
#define RIGHT(i) (2*i+2)
std::vector<int> heap_array;
int heap_size = -1;
public:
    void SiftUp(int i){
        while(i > 0 && heap_array[i] > heap_array[(i-1)/2]){
            std::swap(heap_array[i], heap_array[(i-1)/2]);
            i = (i-1)/2;
        }
    }
    void SiftDown(int i){
        while(LEFT(i) <= heap_size){
            int j = LEFT(i);
            int k = RIGHT(i);
            int index;
            if(j>heap_size)return;
            else if( j==heap_size)
            index = j;
            else
                index = (heap_array[j] > heap_array[k]) ? k : j;
            if(heap_array[i]>heap_array[index])
            std::swap(heap_array[i], heap_array[index]);
        }
    }
    void insert(int x){
        heap_array.push_back(x);
        heap_size++;
        SiftUp(heap_size);
    }

    int pop(){
        int x = heap_array[0];
        heap_array[0] = heap_array[heap_size];
        heap_size--;
        SiftDown(0);
        return x;
    }

};