#include <iostream> 
#include <vector>
#include <algorithm>
#include <climits>
int dual(int i,int M){
    return M-i;
}
int max(int i,int j){
    return i>j?i:j;
}
int min(int i,int j){
    return i>j?j:i;
}

using namespace std;
int media(const vector<int>& list1, const vector<int>& list2) {
    // 确保list1是较短的数组
    if (list1.size() > list2.size()) {
        return media(list2, list1);
    }
    
    int n1 = list1.size();int n2 = list2.size();
    int total = n1 + n2;
    int half = (total + 1) / 2; // 左半部分需要包含的元素个数
    // 二分查找正确的分割点
    int low = 0, high = n1-1,list1Left,list1Right,list2Left,list2Right;
    while (low <=high) {
        int i = (low + high) / 2; // list1的分割点（左半部分有i个元素）
        int j = dual(i,half);         // list2的分割点（左半部分有j个元素）
        // 处理边界情况
        list1Left = (i == 0) ? INT_MIN : list1[i - 1];
        list1Right = (i == n1) ? INT_MAX : list1[i];
        list2Left = (j == 0) ? INT_MIN : list2[j - 1];
        list2Right = (j == n2) ? INT_MAX : list2[j];
        //寻找到分割点
        if (list1Left <= list2Right && list2Left <= list1Right) {
            if (total % 2 == 1) {
                return max(list1Left, list2Left);
            } else {
                int leftMax = max(list1Left, list2Left);
                int rightMin = min(list1Right, list2Right);
                return (leftMax + rightMin) / 2;
            }
        } 
        else if (list1Left > list2Right) {
            //对分割点上下界作摄动
            high = i - 1;
        } else {
            low = i + 1;
        }
    }
    return 0;
}






int main(){
    std::vector<int> A={-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13};
    std::vector<int> B={2,4,6,8,10,12,14,16,18,20};
    std::cout<<media(A,B)<<std::endl;
}