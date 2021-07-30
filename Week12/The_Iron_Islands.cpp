#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <iomanip>

void fill_maps(int men, int num_islands, int waterway, std::map<int,std::pair<int,int>> &best,
               std::map<int,std::pair<int,int>> &sbest)
{
  std::pair<int,int> none = std::make_pair(0,0);
  std::pair<int,int> new_res = std::make_pair(num_islands,waterway);
  if (best[men] == none) best[men] = new_res; //first time for this number of men
  else if (best[men].first < num_islands){ //better than the best value encounterd yet
    sbest[men] = best[men];
    best[men] = new_res;
  }
  else { 
    if (sbest[men] == none || sbest[men].first < num_islands) sbest[men] = new_res;
  }
}

void testcase(){
  int n, k, w; //#islands, #men available, #waterways
  std::cin >> n >> k >> w;
  std::vector<int> defense(n);
  
  for (int i = 0; i < n; ++i){
    std::cin >> defense[i];
  }
  
  std::vector<int> from_Pyke(w);
  std::vector<int> succ(n,-1); //will stay -1 if the waterway ends there
  
  //waterways
  for (int i = 0; i < w; ++i){
    int l, from, to;
    std::cin >> l >> from >> to;
    from_Pyke[i] = to;
    for (int i = 2; i < l; ++i){
      from = to;
      std::cin >> to;
      succ[from] = to;
    }
  }
  
  int res = 0;
  
  //keep track of best and second-best waterway to take for any number of men needed (starting from Pyke)
  //Sinceit has to be a path we can use at most
  //two waterways. So if we used one waterway we will want to know the best watereway
  //to take with the leftover men in addition (not the same one)
  std::map<int,std::pair<int,int>> best; // men needed -> (num islands,waterway)
  std::map<int,std::pair<int,int>> second_best;
  
  best[0] = std::make_pair(0,-1); 
  second_best[0] = std::make_pair(0,-1); 
  
  std::pair<int,int> none = std::make_pair(0,0);
  
  //Do not take pyke => can only take one waterway
  //Perform sliding window
  for (int i = 0; i < w; ++i){
    int start = from_Pyke[i];
    int curend = from_Pyke[i];
    std::queue<int> q;
    q.push(start);
    int men_needed = defense[start];
    if (men_needed == k) res = std::max(res,1);
    
    //while we can move further along the path or we reached the men treshold (second necessary, as we might still get a
    //good path taking away from the front)
    while(succ[curend] != -1 || men_needed >= k){ 
      if (q.front() == start) fill_maps(men_needed,int(q.size()),i,best,second_best); //if we start next to Pyke it goes in our maps
      if (succ[curend] != -1 && men_needed < k){//if we can move further along the path and we have men left, take another island
        curend = succ[curend];
        q.push(curend);
        men_needed += defense[curend];
      }
      else if (men_needed == k) { //got a potential path
        res = std::max(res,int(q.size()));
        int first = q.front(); q.pop();
        men_needed -= defense[first];
      }
      else { //take away an island from the front
        int first = q.front(); q.pop();
        men_needed -= defense[first];
      }
    }
    if (q.front() == start) fill_maps(men_needed,int(q.size()),i,best,second_best); //might be viable for our maps after exiting while loop
  }
  
  //Do take Pyke
  int men_needed_P = defense[0];
  if (men_needed_P == k) res = std::max(res,1); //possible to only take Pyke
  if (best[k-men_needed_P] != none){ //check best solution for only one waterway with Pyke
    if (1 + best[k-men_needed_P].first > res) res = 1 + best[k-men_needed_P].first;
  }
  
  //check for each waterway from Pyke for each number of islamnds one can take with
  //at most k men with Pyke the best possible second waterway to take in addition
  for (int i = 0; i < w; ++i){
    int start = from_Pyke[i];
    int curend = from_Pyke[i];
    std::queue<int> q;
    q.push(start);
    int men_needed = men_needed_P + defense[start];
    if (best[k-men_needed] != none) {
      if (best[k-men_needed].second != i) res = std::max(res,int(q.size()) + 1 + best[k-men_needed].first);
      else if (second_best[k-men_needed] != none) res = std::max(res,int(q.size()) + 1 + second_best[k-men_needed].first);
    }
    while(succ[curend] != -1 && men_needed <= k){
      curend = succ[curend];
      q.push(curend);
      men_needed += defense[curend];
      if (best[k-men_needed] != none) {
        if (best[k-men_needed].second != i) res = std::max(res,int(q.size()) + 1 + best[k-men_needed].first);
        else if (second_best[k-men_needed] != none) res = std::max(res,int(q.size()) + 1 + second_best[k-men_needed].first);
      }
    }
  }
  std::setprecision(0);
  std::cout << std::fixed << res << "\n";
  
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for(int i = 0; i < t; ++i){
    testcase();
  }
  return 0;
}