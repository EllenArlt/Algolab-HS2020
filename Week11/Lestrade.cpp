#include <iostream>
#include <vector>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef double ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;



typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase()
{ 
  int z, u, v, w;//Holmes fee, goal amount of where, who, how
  std::cin >> z >> u >> v >> w; 
  int a; std::cin >> a; //# agents
  // read number of points
  std::size_t g; //# gangsters
  std::cin >> g;
  // read points
  std::vector<K::Point_2> pts;
  pts.reserve(g);
  
  std::vector<int> cheapest_close_agent(g,-1);
  //info per hour for each gang member
  std::vector<int> where(g);
  std::vector<int> who(g);
  std::vector<int> how(g);
  std::map<K::Point_2,int> pos_to_criminal; //map coordinate to gang member id
  
  //read input
  for (std::size_t i = 0; i < g; ++i) {
    int x, y, ui, vi, wi;
    std::cin >> x >> y >> ui >> vi >> wi;
    where[i] = ui;
    who[i] = vi;
    how[i] = wi;
    K::Point_2 pos(x, y);
    pts.push_back(pos);
    pos_to_criminal[pos] = i;
  }
  // construct triangulation of gangsters
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  std::vector<int> pay(a);
  
  //read agents
  for (int i = 0; i < a; ++i) {
    int x, y, zi;
    std::cin >> x >> y >> zi;
    pay[i] = zi;
    K::Point_2 pos(x, y);
    //for each agent determine the closest gang member and if this is the chepest agent for that gang member so far
    //obviously we want to pay the agent as ittle as possible
    int g_ID = pos_to_criminal[t.nearest_vertex(pos)->point()];
    if (cheapest_close_agent[g_ID] == -1) cheapest_close_agent[g_ID] = i;
    else if (pay[cheapest_close_agent[g_ID]] > pay[i]) cheapest_close_agent[g_ID] = i;
  }
  
  
  //Now it is an LP problem
  //The objective function is the sum of the products of each used agent's wage and the # hours they are needed
  //where the #hours are the variables
  //The constraints are that the maximum # hours for each agent must be 24 and that the info amounts add up to at least
  //our goal amounts
  
  Program lp (CGAL::LARGER, true, 0, true, 24); 
  
  int working_agent_ID = 0;
  for (std::size_t i = 0; i < g; ++i) {
    if (cheapest_close_agent[i] != -1){ //there will be gang members with no viable agent
      int agent = cheapest_close_agent[i]; //only consider cheapest agent per gang member
      lp.set_c(working_agent_ID, pay[agent]); 
      lp.set_a(working_agent_ID, 0,  where[i]);
      lp.set_a(working_agent_ID, 1,  who[i]);
      lp.set_a(working_agent_ID, 2,  how[i]);
      ++working_agent_ID;
    }
  }
  lp.set_b(0, u);
  lp.set_b(1, v);
  lp.set_b(2, w);


  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  if (s.is_optimal() && s.objective_value() <= z) std::cout << "L" << "\n";
  else std::cout << "H" << "\n";

}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t;++i){
     testcase(); // Solve a particular test case
  }
  return 0;

}


