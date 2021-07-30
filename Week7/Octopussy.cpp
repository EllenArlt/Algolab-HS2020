#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>


void testcase(){
  int n; std::cin >> n; //number of balls
  std::vector<std::pair<int,int>> exp_t(n); //exp_t[i] = (i,time at whoch bomb i explodes)
  
  for (int i = 0; i < n; ++i){
    int t; std::cin >> t; //explosion time
    exp_t[i] = std::make_pair(i,t);
  }
  
  //greedy choice: sort balls by explosion times
  std::sort(exp_t.begin(), exp_t.end(),
  [](const std::pair<int,int>& p1, const std::pair<int,int>& p2) -> bool {
  return p1.second < p2.second;
  });
  
  //always check if you can deactivate the next exploding bomb in time
  //and if so, deactivate that bombs and all x bombs that need to be deactivated
  //to reach that bomb in x+1 minutes
  
  std::vector<std::vector<int>> below(n); //below[i] is a vector containing all balls this ball touches from above
  
  for (int j = 0; j <= (n-3)/2; ++j){
    below[j].push_back(2*j+1);
    below[j].push_back(2*j+2);
  }
  
  std::vector<bool> activated(n,true); //true if bomb i is still active
  
  int time_t = 0;//keep track of time passed
  
  for (int i = 0; i < n; ++i){
    //if the next to explode bomb is still activated
    if (activated[exp_t[i].first]){
      //deactivate that bomb and all bombs below it (as otherwise this bomb cannot be deactivated)
      //by sorting we know all bombs below will not explode earlier, so we can definetely deactivate
      //them before this one
      std::stack<int> s;
      s.push(exp_t[i].first);
      while (!s.empty()){
        int cur = s.top(); s.pop();
        if (activated[cur]){
          activated[cur] = false;
          ++time_t;
          for (auto it = below[cur].begin(); it != below[cur].end(); ++it){
            s.push(*it);
          }
        }  
      }
      //check if we deactivated all these bombs in time. By the above it is sufficient to
      //only compare to te next exploding bomb
      if (time_t > exp_t[i].second){
        std::cout << "no" << "\n";
        return;
      }
    }
  }
  
  std::cout << "yes" << "\n";
  


}

int main(){
  std::ios_base::sync_with_stdio(false); // Always!
  
  int tc; std::cin >> tc; //number of testcases
  for (int i = 0; i < tc; ++i){
    testcase();
  }
}