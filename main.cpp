#include <iostream>
#include <bs/graph.hpp>

#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#include <fstream>

struct node{};
struct edge { 
	int s, t;
	int cap;
	int cost;
};

std::vector<edge> load_edges(std::istream& is);

int main()
{
	std::ifstream ifs("C:/Users/mfati/Documents/bs/input.txt");

	//std::istringstream iss{ "0;1;1;1\n0;2;5;1\n1;3;5;1\n2;3;2;1\n" };
	auto res = load_edges(ifs);

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

	for (auto& e : res)
	{
		my_add_edge(e.s, e.t, e.cap, e.cost);
	}

    successive_shortest_path_nonnegative_weights(g, 1, 2);
    auto r = edmonds_karp_max_flow(g, 1, 2);

    std::cout << "max flow: " << r << '\n';
    std::cout << "min cost: " << find_flow_cost(g) << '\n';

    return 0;
}

std::vector<edge> load_edges(std::istream & is)
{
	std::vector<edge> res;

	while (is)
	{
		edge e;
		char c;
		is >> e.s >> c >> e.t >> c >> e.cap >> c >> e.cost;
		if (e.cap == -1) e.cap = std::numeric_limits<int>::max() / 2;
		res.push_back(e);
	}

	res.pop_back();

	return res;
}
