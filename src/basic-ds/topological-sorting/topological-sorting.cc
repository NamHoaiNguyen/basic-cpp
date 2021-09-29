#include <cassert>
#include <iostream>
#include <limits>
#include <list>
#include <numeric>
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
    g.test();
}