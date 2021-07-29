#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h> //inexact constructions give wrong results
#include <iomanip>


typedef CGAL::Exact_predicates_exact_constructions_kernel KE;
typedef KE::Point_2 PE;
typedef KE::Segment_2 SE;
typedef KE::Ray_2 RE;

/*The trick is to shorten the ray to end at the first hit encountered so far
  every time. While this means we need to construct a segment after each hit,
  we can save the operations necessary to find out if the new intersection
  comes later or earlier (also needs constructions to calculate distance), since
  the predicate saying whether the segment and leftover ray intersect will 
  evaluate to false much more often. 
  So for randomized input we will on average save a lot of constructions*/



double floor_to_double(const KE::FT& x)
{
 double a = std::floor(CGAL::to_double(x));
 while (a > x) a -= 1;
 while (a+1 <= x) a += 1;
 return a;
}


void testcase(const int &n) {
  
  long x; std::cin >> x; 
  long y; std::cin >> y; 
  long a; std::cin >> a; 
  long b; std::cin >> b; 
  
  PE p1(x,y), p2(a,b);
  RE ray(p1,p2); //construct ray
  
  std::vector<SE> Segments; //vector of segments

  
  for (int i = 0; i < n; ++i){
    long r; std::cin >> r; 
    long s; std::cin >> s; 
    long t; std::cin >> t; 
    long u; std::cin >> u; 
    
    //construct segments and store in vector
    PE ps1(r,s), ps2(t,u);
    SE segment(ps1,ps2);
    Segments.push_back(segment);

  } 
  
  //make sure we do not use the worst case in which we go last to first hit
  std::random_shuffle(Segments.begin(), Segments.end()); 
  
  PE pcurrent; //current first hit
  //keep track of whether we found a hit yet. Once we found an intersection, we 
  // only consider subsegments of the ray instead of the ray itself
  //the difference is important since we cannot store a ray and segment in the same 
  //variable
  bool pcurrent_set = false; 
  //in case the ray(/its' current subsegment) and segment intersect in a segment,
  //I need the first intersection point from the starting position
  PE first_inter_on_segment; 
  SE seg_of_ray; //sub-segment of ray from starting position to current first hit pcurrent
  
  
  //go through all segments (need to go through all since any could be the first hit)
  for (std::vector<SE>::const_iterator it = Segments.begin(); it != Segments.end(); ++it){
    //if pcurrent_set is true, we already found a hit and are now considering
    // the subsegmrent seg_of_ray of the ray
    if (pcurrent_set){
      //check if the segments intersect
      if (CGAL::do_intersect(seg_of_ray,*it)) {
          auto intersection = CGAL::intersection(seg_of_ray,*it);
          //if they intersect, then either in a point or in a segment
          //any intersection gives rise to a at leats as good first hit, since
          //we consider the subsegment with as the endpoint the previous first hit
          if (const PE* p = boost::get<PE>(&*intersection)) { //if they intersect in a point, the hit is obvious
              if (*p == p1){ //if we intersect at the start of the ray, we can stop (there are definitely no earlier hits)
                pcurrent = p1;
                seg_of_ray = SE(p1,pcurrent);
                break;
              } 
              else { //otherwise just save the new first hit and shorten the subsegment of the ray accordingly 
                pcurrent = *p;
                seg_of_ray = SE(p1,pcurrent);
              }
        
          }
          //if they intersect in a segment S, the first hit is the endpoint of S closer to the start of the ray
          else if (const SE* se = boost::get<SE>(&*intersection)){
              
              //find out which endpoint of S is closer to the start of the ray. Make that first_inter_on_segment
              if (CGAL::has_smaller_distance_to_point(p1,(*se).source(),(*se).target())){
                first_inter_on_segment = (*se).source();
              }
              else {first_inter_on_segment = (*se).target();}
              
              if (first_inter_on_segment == p1){ //break if our first intersection is already the start of the ray
                  pcurrent = p1;
                  seg_of_ray = SE(p1,pcurrent);
                  break;
                } 
        
    
              else { //otherwise just save the new first hit and shorten the subsegment of the ray accordingly 
              pcurrent = first_inter_on_segment;
              seg_of_ray = SE(p1,pcurrent);
            
              }
          }
  
      }
    }
    //Here we deal with the case where we have not found any hits yet, so we need to consider the full ray
    else{
      //Check if the ray intersects the current segment
      if (CGAL::do_intersect(ray,*it)) {
          //if they intersect, then either in a point or in a segment
          //any intersection gives rise to a at leats as good first hit, since
          //we consider the subsegment with as the endpoint the previous first hit
          auto intersection = CGAL::intersection(ray,*it);
          if (const PE* p = boost::get<PE>(&*intersection)) {
              if (*p == p1){//break if our first intersection is already the start of the ray
                pcurrent = p1;
                pcurrent_set = true;
                seg_of_ray = SE(p1,pcurrent);
                break;
              } 
               //otherwise just save the new first hit and shorten the ray to a subsegment of the ray accordingly 
               //and set pcurrent_set to true (from now on we ned not consider the full ray anymore)
              else { 
                pcurrent = *p;
                seg_of_ray = SE(p1,pcurrent);
                pcurrent_set = true;
        
              }
        
          }
          //if they intersect in a segment S, the earlier hit is the endpoint of S closer to the start of the ray
          else if (const SE* se = boost::get<SE>(&*intersection)){
              
              //find out which endpoint of S is closer to the start of the ray. Make that first_inter_on_segment
              if (CGAL::has_smaller_distance_to_point(p1,(*se).source(),(*se).target())){
                first_inter_on_segment = (*se).source();
              }
              else {first_inter_on_segment = (*se).target();}
              
              if (first_inter_on_segment == p1){//break if our first intersection is already the start of the ray
                  pcurrent = p1;
                  seg_of_ray = SE(p1,pcurrent);
                  pcurrent_set = true;
                  break;
                } 
        
              //otherwise just save the new first hit and shorten the ray to a subsegment of the ray accordingly 
              //and set pcurrent_set to true (from now on we ned not consider the full ray anymore)  
              else { 
                pcurrent = first_inter_on_segment;
                seg_of_ray = SE(p1,pcurrent);
                pcurrent_set = true;
        
              }
    
          }
  
      }
    }
  }//end for-loop
  std::cout << std::setprecision(0);
  if (!pcurrent_set) std::cout << "no";
  else std::cout << std::fixed << floor_to_double(pcurrent.x()) << " " << floor_to_double(pcurrent.y());
  
  
  

     
}
  


int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n; std::cin >> n;
  while (n != 0){
     testcase(n); // Solve a particular test case
     std::cout << std::endl;
     std::cin >> n;
  }
  return 0;

}