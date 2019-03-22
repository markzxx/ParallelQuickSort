#include <iostream>
#include <algorithm>
#include <ctime>
#include <random>
#include <thread>
#include <memory>
#include <sys/time.h>
using namespace std;

long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

struct Pivot{
    int idx;
    int value;
    Pivot(int i, int v):idx(i), value(v){}
    Pivot() = default;
    bool operator< (Pivot other) const {
        return value<other.value;
    }
};

int partition(vector<int> &arr, int beg, int end){
    size_t candidate_size = 11; // this size can influence the performance
    vector<Pivot>candidate(candidate_size);
    for(int i=0;i<candidate_size;i++)
    {
        int ramdom_index = rand() % (end - beg) + beg;
        candidate[i] = Pivot(ramdom_index, arr[ramdom_index]);
    }
    sort(candidate.begin(), candidate.end());
    int pivot = candidate[candidate_size/2].idx;// generate several random index, select the middle number of them to be pivot, to make left and right more balance

    swap(arr[beg], arr[pivot]);
    int x = arr[beg];
    int i = beg;
    for (int j = i+1; j<end; j++){
        if (arr[j] < x)
            swap(arr[++i], arr[j]);
    }
    swap(arr[beg], arr[i]);
    return i;
}

void thread_sort(vector<int> &arr, int beg, int end, int level){
    if(level<4){   // <4 means 2^3 thread will be create, thus 8 threads in this sample
        int mid = partition(arr, beg, end);
        thread thr1(thread_sort, ref(arr), beg, mid, level+1);
        thread thr2(thread_sort, ref(arr), mid, end, level+1);
        thr1.join();
        thr2.join();
    } else{
        sort(arr.begin()+beg, arr.begin()+end);
    }
}


void test1(vector<int> &arr2){
    auto timeStart = getCurrentTime();
    thread_sort(arr2, 0, arr2.size(), 0);
    auto timeEnd = getCurrentTime();
    printf("multiple threads sorting time is %d ms\n\n", timeEnd - timeStart);
}

void test2(vector<int> &arr1){
    auto timeStart = getCurrentTime();
    sort(arr1.begin(), arr1.end());
    auto timeEnd = getCurrentTime();
    printf("single thread sorting time is %d ms\n\n", timeEnd - timeStart);
}

void init(vector<int> &arr1, vector<int> &arr2, size_t size){
    srand(time(nullptr));
    for(size_t i = 0; i < size; ++i){
        int tmp = rand();
        arr1[i] = tmp;
        arr2[i] = tmp;
    }
}

int main()
{
    size_t size = 100000000;
    vector<int>arr1(size), arr2(size);
    init(arr1, arr2, size);
    test1(arr1);
    test2(arr2);
    printf("%d == %d\n", arr1[465616], arr2[465616]);   // test if the sorting works
    return 0;
}