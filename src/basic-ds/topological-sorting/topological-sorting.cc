#include <cassert>
#include <iostream>
#include <limits>
#include <list>
#include <numeric>
#include <stack>
#include <type_traits>
#include <vector>

template<typename T, typename = std::enable_if_t<std::numeric_limits<T>::is_integer, nullptr_t>>
class Graph {
private:
    T vertex_;
    std::vector<std::list<T>> adj;

public:
    Graph() = delete;

    explicit Graph (T vertex) : vertex_(vertex) {
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
            if (visited[v] == false) {
                topoSortUtil(v, visited, stack);
            }
        }
        while (!stack.empty()) {
            std::cout << stack.top() << " ";
            stack.pop();
        }
    }

    void topoSortUtil(T v, std::vector<bool> &visited, std::stack<T>& stack) {
        visited[v] = true;

        for (const auto& v_list : adj[v]) {
            if (!visited[v_list]) {
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
    // g.test();
    g.topoSort();

    return 0;
}