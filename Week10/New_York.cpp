  #include <iostream>
  #include <vector>
  #include <stack>
  #include <queue>
  #include <algorithm>
  #include <set>
  

  
  
  //Note every vertex in the described graph has exactly one predecessor
  void testcase()
  { int n; std::cin >> n; //# critical points
    int m; std::cin >> m; //optimal trip length
    int k; std::cin >> k; //maximum risk

    std::vector<int> pred(n);
    
    std::vector<bool> sol(n,false);
    std::vector<int> temperatures(n);
   
    for(int j = 0; j < n; ++j){
      int h; std::cin >> h;
      temperatures[j] = h;
    }
    
    std::vector<bool> end(n,true); //wether a vertex (other than New York) in the route graph is a leaf 
    std::vector<bool> memo(n,false); //memorize if route ending at each vertex was already checked
    
    for(int j = 0; j < n-1; ++j){
      int u; std::cin >> u;
      int v; std::cin >> v;
      pred[v] = u;
      end[u] = false; //there is an edge ending here so it is not a leaf
    }
    pred[0] = -1;
    
    
    //as  the routes form a tree (n-1 edges) we can check all routes from end to start
    // to observe a unique path. Keep track of temperatures in a multiset to easily
    // keep track of min and max temperature.
    for (int i = 0; i < n; ++i){
      if (end[i] && !memo[i]){
        std::queue<int> chain;
        std::stack<int> s;
        std::multiset<int> ts;
        s.push(i);
        memo[i] = true; //checked route ending here
        int mint = temperatures[i];
        int maxt = mint;
        
        while (!s.empty()){ //go on until we reach New York 
        //add predecessor of the first critical point in the considered route
        int v = s.top();
        chain.push(v);
        ts.insert(temperatures[v]);
        maxt = *ts.rbegin();
        mint = *ts.begin();
        
        s.pop();
        int last = chain.front();

        if ( chain.size() >= m || maxt - mint > k){ //if route not too short or too risky
          memo[last] = true; //route checked
          if (maxt - mint <= k) { //==> chain.size() >= m, so there definitely is a route starting of length m here that is not too risky
            sol[v] = true;
          }
          //too risky or too many points, so route definitely bad after adding to the start ==> delete last point 
          auto it = ts.find(temperatures[last]); // First one needs an iterator pointing to the element
          ts.erase(it);
          chain.pop();
          last = chain.front();
          if (memo[last]) break; //break if we try to check a route again
        }
            
            if(pred[v] != -1) {
              s.push(pred[v]);
            }
          }
        }
        
        
      }

    bool solex = false;
    //print 
      for (int it = 0; it < n; ++it){
        if (sol[it]){
          std::cout << it << " ";
          solex = true;
        }
      }
    
    if(!solex) std::cout << "Abort mission" ;
    
  
    
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