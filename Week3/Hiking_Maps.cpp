#include <vector>
#include <tuple>
#include <iostream> 
#include <set>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef std::pair<P,P> ppair;
typedef std::tuple<ppair,ppair,ppair> ppair_tuple;

//check if leg is inside of the triangular map (orientation-wise)
bool inside(ppair_tuple &map, P &leg1, P &leg2){
  ppair side1 = std::get<0>(map);
  ppair side2 = std::get<1>(map);
  ppair side3 = std::get<2>(map);
  return (!CGAL::right_turn(side1.first,side1.second,leg1) &&
          !CGAL::right_turn(side2.first,side2.second,leg1) &&
          !CGAL::right_turn(side3.first,side3.second,leg1)) &&
          (!CGAL::right_turn(side1.first,side1.second,leg2) &&
          !CGAL::right_turn(side2.first,side2.second,leg2) &&
          !CGAL::right_turn(side3.first,side3.second,leg2));
}



void testcase(){
  
  int m, n; std::cin >> m >> n;
  
  std::vector<K::Point_2> legs;
  std::vector<ppair_tuple> maps;
  
  for (int i = 0; i < m; ++i){
    int x, y; std::cin >> x >> y;
    P l(x,y);
    legs.push_back(l);
  }
  
  for (int i = 0; i < n; ++i){
    int q1x, q1y, q2x, q2y, q3x, q3y, q4x, q4y, q5x, q5y, q6x, q6y; 
    std::cin >> q1x >> q1y >> q2x >> q2y >> q3x >> q3y >> q4x >> q4y >> q5x >> q5y >> q6x >> q6y;
    P q1(q1x,q1y), q2(q2x,q2y), q3(q3x,q3y), q4(q4x,q4y), q5(q5x,q5y), q6(q6x,q6y);
    ppair side1 = std::make_pair(q1,q2);
    ppair side2 = std::make_pair(q3,q4);
    ppair side3 = std::make_pair(q5,q6);
    if (CGAL::right_turn(q1,q2,q3)) side1 = std::make_pair(q2,q1); 
    if (CGAL::right_turn(q3,q4,q5)) side2 = std::make_pair(q4,q3); 
    if (CGAL::right_turn(q5,q6,q1)) side3 = std::make_pair(q6,q5); 
    maps.push_back({side1,side2,side3});
  }
  
  
  //turn map sides so inside is a left turn from all of them
  
  std::set<std::pair<int,int>> set;
  
  for (int i = 0; i < m-1; ++i){
    int j = 0;
    while (!inside(maps[j],legs[i],legs[i+1])) {
      ++j;
    }
    if (j==n) std::cout << "error";
    set.insert({j,i});
  }

  int res = (*set.rbegin()).first - (*set.begin()).first + 1;

  //do sliding window
  while (true){
   std::pair<int,int> cur = *set.begin();
    set.erase(set.begin());
    int i = cur.first + 1;
    while (i < n && !inside(maps[i],legs[cur.second],legs[cur.second+1])) {
      ++i;
    }
    if (i < n) set.insert({i,cur.second});
    else {
      break;
    }
    res = std::min(res,(*set.rbegin()).first - (*set.begin()).first + 1);
  }
  

  std::cout << res;
  
  
}


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     testcase(); // Solve a particular test case
     std::cout << std::endl;
  }
  return 0;

}