#include <vector>
#include <set>
#include <iostream>
#include <algorithm>

void testcase(){
  int n, m; //number of friends, boxes
  std::cin >> n >> m;
  std::vector<int> strengths(n);
  std::multiset<int> boxes;
  
  //read friends
  for (int i = 0; i < n; ++i){
    std::cin >> strengths[i];
  }
  
  //read boxes
  for (int i = 0; i < m; ++i){
    int b; std::cin >> b;
    boxes.insert(b);
  }
  
  //sort friends by strength
  std::sort(strengths.begin(), strengths.end(),std::greater<int>());
  
  if (*boxes.rbegin() > strengths[0]){ //if heaviest box cannot be carried by the strongest friend
    std::cout << "impossible";
    return;
  }
  
  //Greedy approach: Each run each friend carries the heaviest box they can carry
  int runs = 0;
  while (boxes.begin() != boxes.end()){ //while there are boxes
    ++runs;
    for (auto s : strengths){
      //find heaviest box that friend can carry
      auto box_index = boxes.upper_bound(s);
      if (box_index == boxes.begin()) break;
      --box_index;
      boxes.erase(box_index); //box is being carried
    }
  }
  
  std::cout << 3*runs - 1; //3 seconds per run up and down, need not come up again after last run


}


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i){
    testcase();
    std::cout << "\n";
  }
  return 0;
}