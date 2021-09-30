#include <cassert>
#include <iostream>
#include <limits>
#include <list>
#include <numeric>
#include <stack>
#include <type_traits>
#include <vector>

template<typename T, typename = void>
class Graph {

};

template<typename T>
class Graph<T, typename std::enable_if_t<std::numeric_limits<T>::is_integer>> {
private:
    T vertex_;
    std::vector<std::list<T>> adj;

public:
    Graph() = delete;

    explicit Graph (T vertex) : vertex_(vertex) {
        std::cout << "Primary template" << std::endl;
        adj.resize(vertex_);
        for (int i = 0; i < vertex_; i++) {
            adj.push_back(std::list<T>());
        }
    }

    void addEdge(const T& start, const T& des) {
        assert(start < vertex_ && des < vertex_);
        adj[start].push_back(des);
    }

    void test() {
        for (const auto &v : adj) {
            for (const auto &adj_v: v) {
                std::cout << adj_v << " ";
            }
        }
    }

    void topoSort() {
        std::vector<bool> visited(vertex_, false);
        std::stack<T> stack;

        for (int v = 0; v < vertex_; v++) {
            if ( !visited[v] ) {
                topoSortUtil(v, visited, stack);
            }
        }
        while (!stack.empty()) {
            std::cout << stack.top() << " ";
            stack.pop();
        }
        std::cout << std::endl;
    }

    void topoSortUtil(T v, std::vector<bool> &visited, std::stack<T>& stack) {
        visited[v] = true;

        for (const auto& v_list : adj[v]) {
            if ( !visited[v_list] ) {
                topoSortUtil(v_list, visited, stack);
            }
        }
        stack.push(v);
    }
};

namespace test_struct{
    struct test {
        int start, dest;
    };
}

template<>
class Graph<test_struct::test, 
            typename std::enable_if_t<std::is_class_v<test_struct::test>>> {
private:
    int vertex_;
    std::vector<std::list<int>> adj;

public:
    Graph() = delete;
    
    explicit Graph(int vertex) : vertex_(vertex) {
        std::cout << "Constructor specialization" << std::endl;
        adj.resize(vertex_);
        for (int v = 0; v < vertex_; v++) {
            adj.push_back(std::list<int>());
        }
    }

    void addEdge(test_struct::test v) {
        adj[v.start].push_back(v.dest);
    }

    void topoSort() {
        std::vector<bool> visited(vertex_, false);
        std::stack<int> stack;
        
        for (int v = 0; v < vertex_; v++) {
            if ( !visited[v] ) {
                topoSortUtil(v, visited, stack);
            }
        }

        while (!stack.empty()) {
            std::cout << stack.top() << " ";
            stack.pop();
        }

        std::cout << std::endl;
    }

    void topoSortUtil(int v, std::vector<bool>& visited, std::stack<int>& stack) {
        visited[v] = true;
        for (const auto& v_list : adj[v]) {
            if ( !visited[v_list] ) {
                topoSortUtil(v_list, visited, stack);
            }
        }
        stack.push(v);
    }
};

int main()
{
    Graph<int> g(6);
    g.addEdge(5, 2);
    g.addEdge(5, 0);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);
    g.topoSort();

    Graph<test_struct::test> g_s(6);
    test_struct::test s1{5, 2};
    test_struct::test s2{5, 0};
    test_struct::test s3{4, 0};
    test_struct::test s4{4, 1};
    test_struct::test s5{2, 3};
    test_struct::test s6{3, 1};
    g_s.addEdge(s1);
    g_s.addEdge(s2);
    g_s.addEdge(s3);
    g_s.addEdge(s4);
    g_s.addEdge(s5);
    g_s.addEdge(s6);
    g_s.topoSort();

    return 0;
}