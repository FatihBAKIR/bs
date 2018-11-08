#include <iostream>
#include <bs/graph.hpp>

#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

int main()
{
    using namespace boost;
    bs::graph_t g;

    auto capacity = get(boost::edge_capacity, g);
    auto rev = get(boost::edge_reverse, g);
    auto residual_capacity = get(boost::edge_residual_capacity, g);
    auto weight = get(boost::edge_weight, g);

    auto my_add_edge = [&](int from, int to, int cap, int cost)
    {
        auto [fwd_desc, added1] = boost::add_edge(from, to, g);
        auto [bwd_desc, added2] = boost::add_edge(to, from, g);
        capacity[fwd_desc] = cap;
        weight[fwd_desc] = cost;

        capacity[bwd_desc] = 0;
        weight[bwd_desc] = -cost;

        rev[fwd_desc] = bwd_desc;
        rev[bwd_desc] = fwd_desc;
    };

    my_add_edge(0, 1, 1, 1);
    my_add_edge(0, 2, 5, 1);
    my_add_edge(1, 3, 5, 1);
    my_add_edge(2, 3, 2, 1);

    successive_shortest_path_nonnegative_weights(g, 0, 3);
    auto r = edmonds_karp_max_flow(g, 0, 3);

    std::cout << "max flow: " << r << '\n';
    std::cout << "min cost: " << find_flow_cost(g) << '\n';

    return 0;
}