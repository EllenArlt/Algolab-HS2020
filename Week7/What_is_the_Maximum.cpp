#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>



// choose input type (all inputs comfortably fit into an integer)
typedef int IT;
// choose exact type for solver 
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

double ceil_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 > x) a -= 1;
  return a;
}

void testcase(int p)
{
  
  int a, b; //parameters
  std::cin >> a >> b;
  
  if (p == 1){
    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    //as x,y are bounded below by 0
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    
    // set the coefficients of A and b
    const int X = 0; 
    const int Y = 1;
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);  //  x + y  <= 4
    lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);  // 4x + 2y <= ab
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);  // -x + y <= 1

    // objective function
    lp.set_c(X, a); lp.set_c(Y, -b);      // ax - by  (minimize to maximize by - ax)
  
  
    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    if (s.is_infeasible()){
      std::cout << "no";
      return;
    }
    if (s.is_unbounded()){
      std::cout << "unbounded";
      return;
    }
    
    // output solution
    std::cout << floor_to_double(-s.objective_value()); 
  }
  else {
    // create an LP with Ax >= b, no lower bounds and upper bound 0
    //as x,y,z are bounded above by 0
    Program lp (CGAL::LARGER, false, 0, true, 0); 
    
    // set the coefficients of A and b
    const int X = 0; 
    const int Y = 1;
    const int Z = 2;
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, -4);  //  x + y  >= -4
    lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a*b);  // 4x + 2y + z >= -ab
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1);  // -x + y >= -1

    // objective function
    lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);     // ax + by + z
  
  
    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    if (s.is_infeasible()){
      std::cout << "no";
      return;
    }
    if (s.is_unbounded()){
      std::cout << "unbounded";
      return;
    }
    
    // output solution
    std::cout << ceil_to_double(s.objective_value()); 
  }
}






int main(){
  std::ios_base::sync_with_stdio(false);
  
  int p; std::cin >> p; // Read the number of test cases
  while (p != 0){
     testcase(p); // Solve a particular test case
     std::cout << std::endl;
     std::cin >> p;
  }
  return 0;

}