#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
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

void testcase()
{ 
  long xs, ys; //starting coordinates
  int n; //number of roman legions
  std::cin >> xs >> ys >> n;
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  
  // set the coefficients of A and b
  const int X = 0; 
  const int Y = 1;
  const int PT = 2;
  
  for (int i = 0; i < n; ++i){
    long a, b, c; //line coefficients
    long v; //speed
    std::cin >> a >> b >> c >> v;
    
    long norm = sqrt(a*a+b*b); //compute denominator of distance to line
    
    /*Depending on which side of the legion we started, we want the preparation
      point to be the same side of the line (Do not cross legions on the way!)
      and we want the distance they traverse while the potion is prepared to 
      be at most the distance from the preparation point, ie
      v*PT <= |d((xs,ys),ax+bx+c)|
      The sign of d((xs,ys),ax+bx+c) depends only on which side of the line 
      the potion is prepared, hence the side of the starting point*/
    
    if (a*xs+b*ys+c <= 0){ 
      lp.set_a(X, i,  a); lp.set_a(Y, i, b); lp.set_b(i, -c);
      lp.set_a(X, n+i,  a); lp.set_a(Y, n+i, b); lp.set_a(PT, n+i, v*norm); lp.set_b(n+i, -c);
    }
    else {
      lp.set_a(X, i, -a); lp.set_a(Y, i, -b); lp.set_b(i, c);
      lp.set_a(X, n+i, -a); lp.set_a(Y, n+i, -b); lp.set_a(PT, n+i, v*norm); lp.set_b(n+i, c);
    }
  }
  // set lower bound (only positive preparation time is valid)
  lp.set_l(PT, true, 0);                                   
  // objective function
  lp.set_c(PT, -1);                                       // -PT (want to maximize preparation time)

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  std::cout << floor_to_double(-s.objective_value()); 
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