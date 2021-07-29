#include <iostream>
#include <vector>
#include <cmath>
#include <CGAL/Gmpq.h>
#include <stack>

/*My solution relies on the fact that all we need to find out wether two rays
  starting from the y-axis intersect is their starting points and their slopes.
  That is why there is no need to construct points or lines. As all starting points 
  are distinct, if both rays go upward, they intersect iff the lower ray is strictly
  steeper. If so, the lower ray has in any case a longer distance from starting point to
  intersection point. Similarly, if both rays go downward, they intersect iff the upper ray is strictly
  steeper. If so, the upper ray has in any case a longer distance from starting point to
  intersection point. If the upper ray goes upward and the lower downward, they will never intersect.
  If the upper ray goes downward and the lower upward, they will always intersect. Only in this case
  we need more "elaborate" and most importantly exact calculations to find out who is elimiated.
  Note that they intersect in only exactly that point even if extended to lines. Therefore, 
  interpreting both as lines y = c1 + m1*x and y = c2 + m2*x, where c is the starting y-value and
  m is the slope, we can equate the RHSs to get the x-coordinate and hence by substituting
  into either equation, the y-coordinate. The upper(/left) cyclist wins iff his starting 
  y-coordinate is strictly closer to the intersection y-coordinate.*/


void testcase(){
  int n; std::cin >> n;//number of cyclists
  
  std::vector<CGAL::Gmpq> steigung(n);//slopes
  std::vector<bool> down(n,false);//down[i] : is driver i going strictly downward?
  int numdown = 0;//number of drivers going downward
  std::vector<long> starts(n);//starting y-coordinates
  std::vector<int> indeces(n);//driver indeces
  std::vector<std::tuple<int,long,CGAL::Gmpq>> info(n);//info [i] will contain indeces[i],starts[i],steigung[i] in that order
  
  for (int i = 0; i < n; ++i){
    long y0; std::cin >> y0;
    long x1; std::cin >> x1;
    long y1; std::cin >> y1;
    indeces[i] = i;
    
    starts[i] = y0;
    if (y1 < y0) {
      down[i] = true;
      numdown += 1;
    }
    steigung[i] = CGAL::Gmpq((y1-y0),x1);
    info[i] = {indeces[i],starts[i],steigung[i]};
  }
  
  //sort info by starting y-coordinates, up to down
  std::sort(info.begin(), info.end(),
  [](const std::tuple<int,long,CGAL::Gmpq> &p1, const std::tuple<int,long,CGAL::Gmpq> &p2) -> bool {
  return std::get<1>(p1) > std::get<1>(p2);
  });
    
  std::vector<bool> still_going(n,true);//true if a driver is still driving into the sunset
  
  if (numdown == 0){//first three testcases
  /*This case is simple (see also explanation at line 7). A driver is eliminated iff
    some driver starting above him has a smaller slope. Hence it is sufficient to
    compare only to the driver with the smallest slope from above that is still 
    in the running. As this also implies still going drivers ordered from higher starting 
    points to lower ones are also oredered from higher to lower slopes. We also noted in line 7 onwards
    that an upper driver cannot be eliminated in this case.
    Thus I take the lowest driver that is yet confirmed to make it as indicater for lower drivers
    to compare to*/
    
    int ind0;
    long pos0;
    CGAL::Gmpq st0;
    std::tie(ind0,pos0,st0) = info[0];
    
    
    int indicator = ind0;
    for (int i = 1; i < n; ++i){
      int ind;
      long pos;
      CGAL::Gmpq st;
      std::tie(ind,pos,st) = info[i];
      if (steigung[indicator] >= st){
        indicator = ind;
      }
      else{
        still_going[ind] = false;
      }
    }
  }
  else {
    /*Now as in the explanation in line 7, there are a lot of cases to consider.
      The most important idea here is that, if a driver drives upward, she either
      keeps going until she is eliminated by some driver going downward or eliminates
      all of them. So still-going downwards-going drivers can only be found after
      the last still-going upwards-going driver. Also, as otherwise the downwards drivers eliminate each 
      other first, the upwards driver from below will not hinder any downwards drivers starting above
      the downwards driver he is eliminated by. Hence it is sufficient to keep a stack of
      downwards drivers (the ones that could potentially make it/no conflicts with winning drivers above)
      below the last confirmed upwards driver and compare lower upwards drivers to them in order. Also
      note the similar relationship between two downwards drivers as in the other case
    */
    int lastup = -1;//index of the last still-going driver going upward
    std::stack<int> s;//not eliminated going down, put in order of starting y-coordinates, up to down
    for (int i = 0; i < n; ++i){//check all drivers in order of starting coordinate
      
      int ind;
      long pos;
      CGAL::Gmpq st;
      std::tie(ind,pos,st) = info[i];
      //drives downwards
      if (down[ind]){
        if(s.empty()){//first downward going driver below last upward driver => no conflicts yet
          s.push(ind);
        }
        else {//need to eliminate all downgoing drivers above this one with a more negative slope(use that downwars drivers on s must be ordered by increasing slope)
          while (!s.empty() && steigung[s.top()] < st){
            still_going[s.top()] = false;
            s.pop(); 
          }
          s.push(ind);//that driver starts below, so cannot be eliminated by above downwards(and definitely not upwards) drivers, so consider it
        }
      }
      
      else {//now the current driver goes upward
        if(lastup == -1 && s.empty()){//implies this is the very first driver
          lastup = ind;
        }
        else if (s.empty()){//only upward going drivers still going
          if (steigung[lastup] >= st){
            lastup = ind;
          }
          else{
            still_going[ind] = false;
          }
        }
        else {//there are some downward drivers above still in the running 
          while (still_going[ind] && !s.empty()){//keep eliminating until ether the current driver or all downwards drivers are eliminted
            int curdown = s.top();
            if (-steigung[curdown] < steigung[ind]){ //upper driver wins 
              still_going[ind] = false;
            }
            else {//lower driver wins
              still_going[curdown] = false;
              s.pop(); 
            }
          }
          if (s.empty()){ // if all going down were eliminated first
          //in this case check if some above upwards drivers eliminate him 
            if (lastup == -1 || steigung[lastup] >= st){
              lastup = ind;
            }
            else {
              still_going[ind] = false;
            }
          }
        }
      }

    }
  }
  //output all drivers in order of indeces
  for (int it = 0; it < n; ++it){
    if (still_going[it] == 1){
      std::cout << it << " ";
    }
  }
}



int main(){
  std::ios_base::sync_with_stdio(false); 
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i){
    testcase();
    std::cout << std::endl;
  }
  return 0;
} 

