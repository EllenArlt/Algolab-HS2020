#include <iostream>
#include <vector>
#include <stack>
#include <map>

void testcase(){
  int n, q; //#species, #queries
  std::cin >> n >> q;
  std::map<std::string,int> to_int; //maps species strings to integer ids
  std::vector<std::string> to_string(n); //maps integers to species strings
  std::vector<int> age(n); //ages of species
  
  //read species and ages
  for (int i = 0; i < n; ++i){
    std::string s;
    int a;
    std::cin >> s >> a;
    to_int[s] = i;
    to_string[i] = s;
    age[i] = a;
  }
  
  std::vector<bool> is_luca(n,true); //is_luca_i is true iff species i is the luca (= species without parents)
  std::vector<std::vector<int>> children(n); //child relations stored as adjacency list
  for (int i = 1; i < n; ++i){
    std::string s, p; //s is an offspring of p
    std::cin >> s >> p;
    children[to_int[p]].push_back(to_int[s]);
    is_luca[to_int[s]] = false; //s has a parent so cannot be luca
  }
  
  //find luca
  int luca = 0;
  for (int i = 1; i < n; ++i){
    if (is_luca[i]) luca = i;
  }
  
  //store queries as age and query id by queried species
  std::vector<std::vector<std::pair<int,int>>> queries(n);
  for (int i = 0; i < q; ++i){
    std::string s;
    int b;
    std::cin >> s >> b; //species, age
    queries[to_int[s]].push_back(std::make_pair(b,i));
  }
  
  
  std::stack<std::pair<int,int>> s;
  s.push(std::make_pair(0,luca)); //(index in branch, current species id)
  std::vector<int> branch; //will inspect branches of the rooted tree
  std::vector<std::string> query_answers(q); //store answers to queries by id
  
  /*Explore one branch at a time keeping track of a path followed. When we hit a new species
    we answer all related queries by a binary search along the followed path. If we hit a leaf, 
    we roll back to the last branching point and explore another subbranch*/ 
  while (!s.empty()){
    int cur = s.top().second;
    int pos_in_branch = s.top().first; //pos_in_branch is the distance from luca, ie the fixed index in branch
    s.pop();
    //if we switched branches we pop the unnecessary vertices from the previous branch the back
    if (pos_in_branch < int(branch.size())){
      while (pos_in_branch < int(branch.size())){
        branch.pop_back();
      }
    }
    
    branch.push_back(cur);
    //push children and their distance from luca on stack
    for (auto it = children[cur].begin(); it != children[cur].end(); ++it){
      s.push(std::make_pair(int(branch.size()),*it));
    }
    //answer all related queries
    for (auto it = queries[cur].begin(); it != queries[cur].end(); ++it){
      int high = int(branch.size());
      int low = 0;
      while (high > low){
        int mid = (high + low)/2;
        if (age[branch[mid]] <= (*it).first) high = mid;
        else low = mid + 1;
      }
      query_answers[(*it).second] = to_string[branch[high]];
    }
    
    
    
  }

  for (int i = 0; i < q; ++i){
    std::cout << query_answers[i] << " ";
  }
  
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