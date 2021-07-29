#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS> graph;
                              
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;


/*Whether such a matching exists is the same as whether we can pairwise match all students
  such that each pair has f+1 or more common characteristics. This corresponds to the
  existence of a perfect matching in a graph G, where two vertices u and v are 
  connected by an edge iff students u and v have at least f+1 common characteristcs
  (We use edmonds maximum cardinality matching algorithm for that)
  Note to meet the time requirements we need to sufficiently efficiently find out
  whether a pair of sudents have at least f+1 common charcteristcs ( we definitely
  need to check all pairs). For that, sort all vectors of characterisics and use that
  if one characterisic in C1 is smaller than the other charcteristic ci in the other vector C2, then you will not
  find the smaller characteristic in C to the right of ci. Thus we can find all common characteristics of
  two studends in O(c)*/






//get size of maximum cardinality matching
int max_matching_size(const graph &G, int n){
  
  std::vector<vertex_desc> mate_map(n); // exterior property map
  
  boost::edmonds_maximum_cardinality_matching(G,
        boost::make_iterator_property_map(mate_map.begin(),
        boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
        boost::make_iterator_property_map(mate_map.begin(),
        boost::get(boost::vertex_index, G)));
  
  
  return matching_size;
}



bool testcase(){
  
  int n; std::cin >> n;//number of students
  int c; std::cin >> c;//number of characteristics
  int f; std::cin >> f;//min number of common characterisics in teacher's matching
  
  //characteristics[i] will be the vector of characteristics of student i
  std::vector<std::vector<std::string>> characteristics(n,std::vector<std::string>(c));
  
  for (int i = 0; i < n; ++i){
    for (int j = 0; j < c; ++j){
      std::string s; std::cin >> s;
      characteristics[i][j] = s;
    }
    std::sort(characteristics[i].begin(),characteristics[i].end());//sort for faster search later
  }
  
  graph G(n);//want G to be the graph where two vertices are connected by an edge iff they have at least f+1 common interests
  
  //for each pair of students (unorered) find out if they have at least f+1
  //common characteristics. If so add an edge in G
  for (int i = 0; i < n; ++i){
    for (int j = i; j < n; ++j){
      int foundCommon = 0;//keep track of number of common interests between i and j found jet
      //currently considered indeces of characteristics[i] and characteristics[j] respectively
      int iti = 0;
      int itj = 0;
      //while we are still in bounds of both characteristics vectors and we have not found enough common characteristics yet
      while (iti < c && itj < c && foundCommon <= f){ 
        //are both characteristics the same?
        if (characteristics[i][iti] == characteristics[j][itj]){
          //if so, move on in both vectors (each characteristic comes up only once and increment foundCommon)
          ++foundCommon;
          ++itj;
          ++iti;
        }
        
        
        //if characteristics[i][iti] < characteristics[j][itj], as both vectors are sorted,
        //then characteristics[i][iti] is definetly not in characteristics[j] (as it did not come up before as well)
        //so we can move on in characteristics[i]
        else if (characteristics[i][iti] < characteristics[j][itj]){
          ++iti;
        }
        //otherwise characteristics[i][iti] > characteristics[j][itj] so the reverse argument applies
        else ++itj;
        
        //if we have found enough common characteristics add an edge in G
        //loop will break before the next iteration as foundCommon > f
        if (foundCommon > f){
          boost::add_edge(i,j,G);
        }
      }

    }
  }
  //find the size of a maximum cardinality matching in this graph
  int size = max_matching_size(G,n);
  
  //if it is perfect, the teacher's matching was not optimal
  if (size == n/2){
    return 1;
  }
  
  
  else {//std::cout << "x";
    return 0;}
  
}

