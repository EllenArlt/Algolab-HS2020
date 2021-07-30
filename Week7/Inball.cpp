#include <iostream>
#include <cmath>


#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit) 
//as I am using CGAL::Gmpz for the norm (int can contain the norm as well, but seems to be slower)
typedef CGAL::Gmpz IT;
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

double ceil_to_double(const CGAL::Quotient<ET>& x)
{
 double a = std::ceil(CGAL::to_double(x));
 while (a < x) a += 1;
 while (a-1 > x) a -= 1;
 return a;
}

/*
This is an LP with objective function -r (minus the radius, to maximize the radius)
and n inequalities:

0; x_0*(a_0)_0 + ... + x_d-1*(a_0)_d-1 + r*norm((a_0)_0, ..., (a_0)_d-1) <= b_0
.
.
.
n-1; x_0*(a_n-1)_0 + ... + x_d*(a_n-1)_d-1 + r*norm((a_n-1)_0, ..., (a_n-1)_d-1) <= b_n-1

Where the result for x_0,...,x_d-1 corresponds to the center of the largest possible inball.
Ie. the inequalities mean there is at least r times the norm distance from the center point
to the outline of the cave.
*/




void testcase(int n)
{ 
  int d; std::cin >> d; //dimension of cave
  
  // create an LP with Ax <= b, no lower bounds and no upper bounds
  // since the coordinates of the ball center point cannot be bounded
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  
  const int r = d+1; //variable corresponding to radius of the inball
  
  //read inequalities
  for (int i = 0; i < n; ++i){
    CGAL::Gmpz norm = 0; //will contain norm of the (ai)j's
    for (int j = 0; j < d; ++j){
      int a; std::cin >> a;
      lp.set_a(j, i,  a);
      norm += CGAL::square(a);
    }
    norm = CGAL::sqrt(norm);
    lp.set_a(r, i, norm);
    int b; std::cin >> b;
    lp.set_b(i, b);
  }
  
  lp.set_c(r, -1);  //want to maximize r, so minimize -r
  lp.set_l(r, true, 0); //no negative radius allowed

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  if (s.is_optimal()) std::cout << floor_to_double(-s.objective_value()) << "\n"; //do not forget to invert the result before flooring (as objective function is -r)
  else if (s.is_infeasible()) std::cout << "none" << "\n";
  else std::cout << "inf" << "\n";
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n; std::cin >> n; // Read the number of test cases
  while (n != 0){
     testcase(n); // Solve a particular test case
     std::cin >> n;
  }
  return 0;

}