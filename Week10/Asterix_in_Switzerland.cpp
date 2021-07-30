#include <iostream>

// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

/*
If there is such a free-standing union then, when connecting all initial debts to the target
and all initial incomes from the source, there must be more income than the maximum flow. 
That is because exactly then there is at least one province for which not all income reaches
the sink, implying it can pay off all its debt with its income and money it is owed and can
actually be paid by other provinces (Paths to the sink are preferred, so the provinces that owe
and give their money were able to pay their debt somehow). Thus taking provinces with leftover
money and those that pay them (transitatively) gives a group of provinces where
at least one has a positive amount of money and the others are not in the negatives.

If there is as much flow as income there is no free-standing union, as creating a 
union does not decrease the amount of money sent out of the union or the debt paid
so at least the income will need to be paid and so no one could in the end have 
a positive amount of money.
*/

void testcase() {
  int n; //number of provinces
  int m; //number of debt relations between provinces
  std::cin >> n >> m;
  
  graph G(n);
  edge_adder adder(G);


  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  long total_income = 0;
  
  for (int i = 0; i < n; ++i){
    int b; std::cin >> b;
    if (b > 0) {
      adder.add_edge(v_source, i, b);
      total_income += b;
    }
    if (b < 0) adder.add_edge(i, v_sink, -b);
  }
  
  for (int k = 0; k < m; ++k){
    int i, j;
    int d;
    std::cin >> i >> j >> d;
    adder.add_edge(i, j, d);
  }

  // Calculate flow from source to sink
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  if (flow < total_income) std::cout << "yes" << "\n";
  else std::cout << "no" << "\n";

}



int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     testcase(); // Solve a particular test case
  }
  return 0;

}