#include <graph.h>

// 使用示例
int main() {
    // 创建包含5个顶点的无向图
    Graph g(5);
    
    // 添加边
    g.addEdge(0, 1);
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    // 打印邻接表
    g.DFSRecursive(0);
    return 0;
}