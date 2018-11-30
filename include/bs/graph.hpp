//
// Created by Mehmet Fatih BAKIR on 2018-11-08.
//

#pragma once

#include <boost/graph/adjacency_list.hpp>

struct edge;
namespace bs
{
using namespace boost;

using graph_traits = adjacency_list_traits<vecS, vecS, directedS>;

using my_vertex_name_t = property<vertex_name_t, std::string>;

using name_t = property<edge_name_t, ::edge*>;
using weight_t = property<edge_weight_t, int, name_t>;
using reverse_t = property<edge_reverse_t, bs::graph_traits::edge_descriptor, weight_t>;
using resid_capacity_t = property<edge_residual_capacity_t, int, reverse_t>;
using capacity_t = property<edge_capacity_t, int, resid_capacity_t>;

using graph_t =
        adjacency_list<
            vecS, vecS, directedS,

            my_vertex_name_t,

            capacity_t
        >;
}
