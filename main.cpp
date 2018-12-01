#include <iostream>
#include <bs/graph.hpp>

#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/cycle_canceling.hpp>

#include <fstream>
#include <variant>
#include <map>

struct leftover
{
    int node_num;
};

struct transport
{
    int from;
    int to;
    int time;
};

struct edge { 
	int s, t;
	int cap;
	int cost;
	std::variant<std::monostate, leftover, transport> v;
};

std::vector<edge> load_edges(std::istream& is);

int main()
{
	std::ifstream ifs("C:/Users/mfati/Documents/bs/input.txt");

	auto res = load_edges(ifs);

    using namespace boost;
    bs::graph_t g;

    auto capacity = get(boost::edge_capacity, g);
    auto rev = get(boost::edge_reverse, g);
    auto residual_capacity = get(boost::edge_residual_capacity, g);
    auto weight = get(boost::edge_weight, g);
    auto names = get(boost::edge_name, g);

    auto my_add_edge = [&](::edge* e, int from, int to, int cap, int cost)
    {
        auto [fwd_desc, added1] = boost::add_edge(from, to, g);
        auto [bwd_desc, added2] = boost::add_edge(to, from, g);
        capacity[fwd_desc] = cap;
        weight[fwd_desc] = cost;

        capacity[bwd_desc] = 0;
        weight[bwd_desc] = -cost;

        rev[fwd_desc] = bwd_desc;
        rev[bwd_desc] = fwd_desc;

        names[fwd_desc] = e;
        names[bwd_desc] = nullptr;
    };

	for (auto& e : res)
	{
		my_add_edge(&e, e.s, e.t, e.cap, e.cost);
	}

    //successive_shortest_path_nonnegative_weights(g, 1, 2);
    auto r = edmonds_karp_max_flow(g, 1, 2);
	cycle_canceling(g);

    std::cout << "max flow: " << r << '\n';
    std::cout << "min cost: " << find_flow_cost(g) << '\n';

	std::map<std::tuple<int, int, int>, int> transports;
	std::map<int, int> leftovers;

    auto [it, end] = vertices(g);
    for (; it != end; ++it) {
        auto[edge_it, edge_end] = out_edges(*it, g);
        for (; edge_it != edge_end; ++edge_it) {
            if (names[*edge_it] == nullptr) continue;
            auto &e = *names[*edge_it];
            auto flow = capacity[*edge_it] - residual_capacity[*edge_it];
            if (std::get_if<std::monostate>(&e.v) == nullptr) {
                struct {
                    void operator()(const leftover &l) {
						leftovers.emplace(l.node_num, cur_flow);
                        //std::cout << l.node_num;
                    }

                    void operator()(const transport &t) {
						m_transports.emplace(std::make_tuple(t.from, t.to, t.time), cur_flow);
                        //std::cout << t.from << " " << t.to << " " << t.time;
                    }

                    void operator()(const std::monostate &) {
                    }
					int cur_flow;
					std::map<int, int>& leftovers;
					decltype(transports)& m_transports;
				} visitor{flow, leftovers, transports};

                std::visit(visitor, names[*edge_it]->v);
            }
        }
    }

	std::ofstream lof{ "leftover.txt" };
	for (auto& l : leftovers)
	{
		lof << l.second << ';';
	}

	std::ofstream tof{ "transport.txt" };
	for (auto& t : transports)
	{
		tof << t.second << ';';
	}

    return 0;
}

std::vector<::edge> load_edges(std::istream & is)
{
	std::vector<::edge> res;

	while (is)
	{
		::edge e;
		char c;
		char type;
		is >> e.s >> c >> e.t >> c >> e.cap >> c >> e.cost;
		is >> c >> type >> c;
		if (type == 'l')
        {
		    leftover le;
		    char ex;
		    is >> le.node_num >> c >> ex >> c >> ex;
		    e.v = le;
        }
		else if (type == 't')
        {
		    transport t;
		    is >> t.from >> c >> t.to >> c >> t.time;
		    e.v = t;
        }
		else if (type == 'e')
        {
		    char ex;
		    is >> ex >> c >> ex >> c >> ex;
        }
		else
        {
		    throw std::runtime_error("unrecognized edge type");
        }
		if (e.cap == -1) e.cap = std::numeric_limits<int>::max() / 2;
		res.push_back(e);
	}

	res.pop_back();

	return res;
}
