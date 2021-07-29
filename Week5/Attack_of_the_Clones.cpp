#include <iostream>
#include <vector>
#include <algorithm>
#include <map>


int testcase(){
  int n, m;
  std::cin >> n >> m;
  std::map<int,int> segs; //map mapping segments to number of jedi added compared to previous segment
  std::vector<std::pair<int,int>> jedi(n); //jedi[0] == {i,j} ==> jedi 0 protects segments i to j
  for (int i = 0; i < n; ++i){
    int a, b;
    std::cin >> a >> b;
    --a; --b; //need to be zero-based
    jedi[i] = {a,b};
    if (b < a) ++segs[0];
    ++segs[a];
    if (b < m-1) --segs[b+1];
  }
  
  int minseg = 0;
  int min_k = 11;
  int cur_jedi = 0;
  
//find segment with least number of jedi by sliding thogh the entries in segs
  for (auto it = segs.begin(); it != segs.end(); ++it){
    cur_jedi += it->second;
    if (cur_jedi < min_k){
      minseg = it->first;
      min_k = cur_jedi;
    }
  }
  
  std::vector<std::pair<int,int>> s_jedi;
  //recenter jedi such that the segment with min. # of jedi becomes new segment 0
  for (int i = 0; i < n; ++i){
    if (jedi[i].first >= minseg) jedi[i].first -= minseg;
    else jedi[i].first = m - minseg + jedi[i].first;
    if (jedi[i].second >= minseg) jedi[i].second -= minseg;
    else jedi[i].second= m - minseg + jedi[i].second;
    
    //treat jedi that protect minseg specially
    if (jedi[i].first > jedi[i].second || jedi[i].first == 0 || jedi[i].second == 0){
      s_jedi.push_back({jedi[i].first,jedi[i].second});
      jedi[i].first = -1;
      jedi[i].second = -1;
    }
       
  }
  
  //sort jedi by end segment
  std::sort(jedi.begin(), jedi.end(),
      [](const std::pair<int,int>& e1, const std::pair<int,int>& e2) -> bool {
        return e1.second < e2.second;
            });

  //now use a greedy approach according to earliest admissible end segment for each jedi
  //(to fit as many jedi as possible on the circle)

  int res = 0;
  

  //no jedi protecting minseg

  int end = -1;
  
  for (auto j : jedi){
    if (j.first != -1){
      if (j.first > end){
        ++res;
        end = j.second;
      }
    }
  }

  //once for each jedi protecting minseg
  
  for (auto s : s_jedi){
    int cur_res = 1;
    int final_free = s.first - 1;
    if (final_free == -1) final_free = m-1;
    end = s.second;
    for (auto j : jedi){
      if (j.first != -1){
        if (j.first > end && j.second <= final_free){
          ++cur_res;
          end = j.second;
        }
      }
    }
    res = std::max(res, cur_res);
  }
  
  
  
  return res;
  
}



int main(){
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; ++i){
    int maxres = testcase();
    std::cout << maxres;
    std::cout << "\n";
  }
}