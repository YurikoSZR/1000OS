#include <iostream>
#include <vector>
#include <stack.h>
#define SIZE 64
#define INFTY 0xfffffff
using namespace std;

// 边结构体（链表节点）
struct Edge {
    int dest;    // 目标顶点编号
    int weight;  // 边权重（可根据需要修改）
    Edge* next;  // 下一条边指针

    Edge(int d, int w = 1) : dest(d), weight(w), next(nullptr) {}
};

// 顶点结构体
struct Vertex {
    int data;       // 顶点数据（可根据需要扩展）
    Edge* edges;    // 邻接表头指针

    Vertex() : data(0), edges(nullptr) {}
    explicit Vertex(int d) : data(d), edges(nullptr) {}
};

// 图结构体（邻接表实现）
struct Graph {
    vector<Vertex> vertices;    // 顶点集合
    bool directed;              // 是否为有向图

    // 构造函数
    Graph(int n = 0, bool dir = false) : directed(dir) {
        initialize(n);
    }

    // 初始化n个顶点
    void initialize(int n) {
        vertices.resize(n);
        for(int i = 0; i < n; ++i) {
            vertices[i].data = i; // 默认数据设为顶点编号
        }
    }

    // 添加边（默认无向图）
    void addEdge(int src, int dest, int weight = 1) {
        // 源到目标的边
        Edge* newEdge = new Edge(dest, weight);
        newEdge->next = vertices[src].edges;
        vertices[src].edges = newEdge;

        // 如果是无向图，添加反向边
        if(!directed) {
            Edge* reverseEdge = new Edge(src, weight);
            reverseEdge->next = vertices[dest].edges;
            vertices[dest].edges = reverseEdge;
        }
    }

    // 打印图结构
    void print() {
        for(size_t i = 0; i < vertices.size(); ++i) {
            cout << "顶点" << i << " [";
            Edge* current = vertices[i].edges;
            while(current) {
                cout << "->" << current->dest;
                if(current->weight != 1) {
                    cout << "(" << current->weight << ")";
                }
                current = current->next;
            }
            cout << "]" << endl;
        }
    }

    // 析构函数（释放内存）
    ~Graph() {
        for(auto& v : vertices) {
            Edge* current = v.edges;
            while(current) {
                Edge* temp = current;
                current = current->next;
                delete temp;
            }
            v.edges = nullptr;
        }
    }

    // 深度优先搜索（DFS）- 递归版
    void DFSRecursive(int start) {
        vector<bool> visited(vertices.size(), false);
        cout << "递归DFS从顶点 " << start << " 开始: ";
        DFSHelper(start, visited);
        cout << endl;
    }

private:
    // 递归辅助函数
    void DFSHelper(int current, vector<bool>& visited) {
        visited[current] = true;
        cout << current << " ";

        Edge* edge = vertices[current].edges;
        while (edge != nullptr) {
            int neighbor = edge->dest;
            if (!visited[neighbor]) {
                DFSHelper(neighbor, visited);
            }
            edge = edge->next;
        }
    }
};

struct graph_{
    //图函数的栈
    unsigned int visit=0;static stack<int> Stack;
    //图的存储结构
    int vertex[SIZE];
    int edge[SIZE*SIZE];

    //索引函数
    void assign(int i,int j,int weight){
        edge[i*SIZE+j]=weight;
    }
    int etr(int i,int j){
        return i*SIZE+j;
    }

    //构造函数
    graph_(){
        for(int i = 0 ;i< SIZE;i++)
            for(int j=0;j<SIZE;j++){
                if(i==j)assign(i,j,0);
                else
                assign(i,j,INFTY);
            }
    }
    void DFS(int start){
        if(vertex[start]==0){
            std::cout<<"Empty Node"<<std::endl;
            return;
        }
        Stack.push(start);
        LP:
        while(Stack.sp>=0){
            if((visit| 1<< Stack.top())){
                Stack.sp--;
                continue;
            }
            else{
                std::cout<<"Visit:"<<Stack.top()<<std::endl;
                visit|=1<< Stack.top();
                for(int i=0;i< SIZE;i++){
                    if((visit| 1<< i))continue;
                    if(edge[etr(Stack.sp,i)]<INFTY)Stack.push(i);
                }
            }
        }
        visit &= 0;
    }
};

    