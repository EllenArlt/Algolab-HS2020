#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

int testcase(){
  int n, m;
  long D, T;
  std::cin >> n >> m >> D >> T;
  int half = n/2;
  std::vector<std::pair<long,long>> mov_l(half);
  std::vector<std::pair<long,long>> mov_r(n-half);
  std::vector<long> gulps(m+1,0);
  
  for (int i = 0; i < n/2; ++i){
    std::cin >> mov_l[i].first >> mov_l[i].second;
  }
  
  for (int i = 0; i < n-half; ++i){
    std::cin >> mov_r[i].first >> mov_r[i].second;
  }
  
  for (int i = 0; i < m; ++i){
    std::cin >> gulps[i+1];
  }
  
  std::vector<std::vector<long>> L1;
  std::vector<std::vector<long>> L2;
  
  std::vector<long> cur(3);
  
  for (int s = 0; s < 1<<(n/2); ++s) { // Iterate through all subsets
    long dsum = 0;
    long tsum = 0;
    long nnum = 0;
    for (int i = 0; i < (n/2); ++i) {
      if (s & 1<<i) {
        dsum += mov_l[i].first; // If i-th element in subset
        tsum += mov_l[i].second; // If i-th element in subset
        ++nnum;
      }
        
    }
      cur[0] = dsum;
      cur[1] = tsum;
      cur[2] = nnum;
      L1.push_back(cur);
  }
  
  for (int s = 0; s < 1<<(n-n/2); ++s) { // Iterate through all subsets
    long dsum = 0;
    long tsum = 0;
    long nnum = 0;
    for (int i = 0; i < (n-n/2); ++i) {
      if (s & 1<<i) {
        dsum += mov_r[i].first; // If i-th element in subset
        tsum += mov_r[i].second; // If i-th element in subset
        ++nnum;
      }
        
    }
      cur[0] = dsum;
      cur[1] = tsum;
      cur[2] = nnum;
      L2.push_back(cur);
  }
  
  std::sort(L2.begin(), L2.end(),
  [](const std::vector<long>& p1, const std::vector<long>& p2) -> bool {
  return p1[1] < p2[1];
  });
  
  std::map<long, std::vector<long>> best_dist_in_n_for_t;
  std::vector<long> best_dist_for_n(n+1,0);
  
  
  for (auto x : L2){
    if (x[1] >= T) break;
    best_dist_for_n[x[2]] = std::max(x[0], best_dist_for_n[x[2]]);
    best_dist_in_n_for_t[x[1]] = best_dist_for_n;
  }
  
  for (auto y : L1){
    long Tn = T - y[1];
    
    auto a = best_dist_in_n_for_t.lower_bound(Tn);
    if( a == best_dist_in_n_for_t.begin()) continue;
    --a;
    for (int i = 0; i <= (n - y[2]);++i){
      int mov = y[2] + i; 
      long new_dist = y[0] + a->second[i];
      if(a->second[i] == 0) continue;
      best_dist_for_n[mov] = std::max(best_dist_for_n[mov],new_dist);
    }
    
    
  }

  
  
  int low = 0;
  int high = m+1;
  while (high > low){
    int mid = (high+low)/2;
    long maxd = -1;
    for (int i = 1; i <= n; ++i){
      if(best_dist_for_n[i] == 0) continue;
      maxd = std::max(maxd,best_dist_for_n[i] + i*gulps[mid]);
    }
    if (maxd >= D) high = mid;
    else low = mid + 1;
  }
  
  if (low == m+1) return -1;
  else return low;


  
}


int main(){
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; ++i){
    int res = testcase();
    if (res == -1) std::cout << "Panoramix captured";
    else std::cout << res;
    std::cout << "\n";
  }
}