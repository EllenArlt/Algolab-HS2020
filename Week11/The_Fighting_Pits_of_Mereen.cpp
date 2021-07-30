#include <iostream>
#include <vector>
#include <cmath>

//encode p-q as diff = 13 + p - q (this way we will never start froma state with 26 < diff or diff < 0, because otherwise,
//then |p-q| >= 13 and so as we can earn excitement at most 3000 each round, the round score will be negative, hence is invalid)

//encode whole state as (k+1)^(2*(m-1))*diff + (((k+1)^(2*(m-1))-1))*x_1 + ... + (((k+1)^(2*(m-1))-2*(m-1)))*x_(2*(m-1))
//where x_1, x_2 (or just x_1) are the last two or last fighters sent south and
//x_3, x_4 (or just x_2) are the last two or last fighters sent north in order

int DP(std::vector<int> &queue, int offset, int state_code, std::vector<std::vector<int>> &memo,
       int k, int n, int m){
  
  //if we are out of fighters
  if (offset == n) return 0;
  if (memo[offset][state_code] != -2) return memo[offset][state_code]; //recall from memory
  
  int new_fighter = queue[offset] + 1;
  
  int dis_fighters_north = 1;
  int dis_fighters_south = 1;
  int diff = 13;
  int ub = k+1;
  int new_north_code = 0;
  int new_south_code = 0;
  
  if (m == 2){
    //decode state_code
    int old_fighter_north = state_code % ub;
    int old_fighter_south = ((state_code-old_fighter_north)/ub) % ub;
    diff = (state_code - old_fighter_north - ub*old_fighter_south)/(ub*ub);
    
    if (old_fighter_north != 0 && old_fighter_north != new_fighter) ++dis_fighters_north;
    if (old_fighter_south != 0 && old_fighter_south != new_fighter) ++dis_fighters_south;
    
    //set next state codes
    new_north_code = (diff+1)*(ub*ub)+ub*old_fighter_south+new_fighter;
    new_south_code = (diff-1)*(ub*ub)+ub*new_fighter+old_fighter_north;
  }
  else {
    //decode state_code
    int old_fighter_north1 = state_code % ub;
    int old_fighter_north2 = ((state_code-old_fighter_north1)/ub) % ub;
    int old_fighter_south1 = ((state_code - old_fighter_north1 - ub*old_fighter_north2)/(ub*ub))%ub;
    int old_fighter_south2 = ((state_code - old_fighter_north1 - ub*old_fighter_north2 - ub*ub*old_fighter_south1)/(ub*ub*ub))%ub;
    diff = ((state_code - old_fighter_north1 - ub*old_fighter_north2 - ub*ub*old_fighter_south1 - ub*ub*ub*old_fighter_south2)/(ub*ub*ub*ub));
    
    if (old_fighter_north1 != 0 && old_fighter_north1 != new_fighter) ++dis_fighters_north;
    if (old_fighter_north2 != 0 && old_fighter_north2 != new_fighter && old_fighter_north2 != old_fighter_north1) ++dis_fighters_north;
    if (old_fighter_south1 != 0 && old_fighter_south1 != new_fighter) ++dis_fighters_south;
    if (old_fighter_south2 != 0 && old_fighter_south2 != new_fighter && old_fighter_south2 != old_fighter_south1) ++dis_fighters_south;
    
    //set next state codes
    new_north_code = (diff+1)*(ub*ub*ub*ub)+(ub*ub*ub)*old_fighter_south2+(ub*ub)*old_fighter_south1+ub*old_fighter_north1 + new_fighter;
    new_south_code = (diff-1)*(ub*ub*ub*ub)+(ub*ub*ub)*old_fighter_south1+(ub*ub)*new_fighter+ub*old_fighter_north2 + old_fighter_north1;
  }
  
  //decode diff for both gates
  int south_diff;
  int north_diff;
  if (diff+1 >= 13) north_diff = diff-13+1;
  else north_diff = 13 - (diff+1);
  if (diff-1 >= 13) south_diff = diff-13-1;
  else south_diff = 13 - (diff-1);
  
  //compute scores
  int north_score = dis_fighters_north*1000 - pow(2,north_diff);
  int south_score = dis_fighters_south*1000 - pow(2,south_diff);
  
  int res;
  //need to check which actions are valid
  if (north_score >= 0 && south_score >= 0) {
    res = std::max(north_score + DP(queue,offset+1,new_north_code,memo,k,n,m),
                     south_score + DP(queue,offset+1,new_south_code,memo,k,n,m));
  }
  else if (north_score >= 0) res = north_score + DP(queue,offset+1,new_north_code,memo,k,n,m);
  else if (south_score >= 0) res = south_score + DP(queue,offset+1,new_south_code,memo,k,n,m);
  //if none are valid, return a result negative enough to give an overall negative excitement
  else res = -n*3000;
  
  memo[offset][state_code] = res;
  return res;
  
}

void testcase(){
  int n, k, m;
  std::cin >> n >> k >> m;
  std::vector<int> queue(n);
  
  for (int i = 0; i < n; ++i){
    std::cin >> queue[i];
  }
  int max_states = (27)*pow(k+1,2*(m-1));
  std::vector<std::vector<int>> memo(n,std::vector<int>(max_states,-2));
  
  //diff = 13 corresponds to p-q = 0
  int start_state = 13*pow(k+1,2*(m-1));
  
  int max_score = DP(queue,0,start_state,memo,k,n,m);
  
  std::cout << max_score;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     testcase(); // Solve a particular test case
     std::cout << "\n";
  }
  return 0;

}