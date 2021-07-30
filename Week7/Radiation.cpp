#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpz IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


//compute powers of base up to power n
void precompute_pows(CGAL::Gmpz base, std::vector<CGAL::Gmpz> &res){
  int n = res.size();
  res[0] = CGAL::Gmpz(1);
  for (int i = 1; i < n; ++i){
    res[i] = res[i-1]*base;
  }
}


bool try_degree(int deg, std::vector<std::vector<std::vector<CGAL::Gmpz>>> &healthy, int h,
                         std::vector<std::vector<std::vector<CGAL::Gmpz>>> &tumor, int t){ 
                           
  
  // create an LP with Ax <= b and no lower/upper bounds
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  

  
  // read the healthy cells
  for (int i = 0; i < h; ++i) {
    int varnum = 0;
    for (int xi = 0; xi <= deg; ++xi){
      for (int yi = 0; yi <= deg-xi; ++yi){
        for (int zi = 0; zi <= deg-xi-yi; ++zi){
          CGAL::Gmpz coeff = healthy[i][0][xi]*healthy[i][1][yi]*healthy[i][2][zi];
          lp.set_a(varnum, i, coeff);
          ++varnum;
        }
      }
    }
    lp.set_b(i, -1);
  }
  // read the tumor cells
  for (int i = 0; i < t; ++i) {
    int varnum = 0;
    for (int xi = 0; xi <= deg; ++xi){
      for (int yi = 0; yi <= deg-xi; ++yi){
        for (int zi = 0; zi <= deg-xi-yi; ++zi){
          CGAL::Gmpz coeff = -tumor[i][0][xi]*tumor[i][1][yi]*tumor[i][2][zi];
          lp.set_a(varnum, h+i, coeff);
          ++varnum;
        }
      }
    }
    lp.set_b(h+i, -1);
  }

  // solve the program, using ET as the exact type and Bland's rule
  CGAL::Quadratic_program_options options; 
  options.set_pricing_strategy(CGAL::QP_BLAND);     // Bland's rule
  Solution s = CGAL::solve_linear_program(lp, ET(), options);

  // output solution
  if (s.is_optimal() ||s.is_unbounded()) return true;
  return false;
  
 
  
  
}

int testcase(){
  
  int h, t; //number of healthy and tumor cells
  std::cin >> h >> t;
  
  std::vector<std::vector<std::vector<CGAL::Gmpz>>> healthy(h,std::vector<std::vector<CGAL::Gmpz>>(3,std::vector<CGAL::Gmpz>(31)));
  std::vector<std::vector<std::vector<CGAL::Gmpz>>> tumor(t,std::vector<std::vector<CGAL::Gmpz>>(3,std::vector<CGAL::Gmpz>(31)));
  
  // read the healthy cells
  for (int i = 0; i < h; ++i){
    long x, y, z; //cell coordinates
    std::cin >> x >> y >> z;
    precompute_pows(x,healthy[i][0]);
    precompute_pows(y,healthy[i][1]);
    precompute_pows(z,healthy[i][2]);
  }
  
  // read the tumor cells
  for (int i = 0; i < t; ++i){
    long x, y, z; //cell coordinates
    std::cin >> x >> y >> z;
    precompute_pows(x,tumor[i][0]);
    precompute_pows(y,tumor[i][1]);
    precompute_pows(z,tumor[i][2]);
  }
  
  for (int i = 0; i <= 30; ++i){
    if (try_degree(i,healthy,h,tumor,t)) return i;
  }
  
  return -1;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i){
     int d = testcase(); // Solve a particular test case
     if (d == -1) std::cout << "Impossible!";
     else std::cout << d;
     std::cout << std::endl;
  }
  return 0;

}