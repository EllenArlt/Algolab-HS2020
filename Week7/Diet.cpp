#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
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

/*In this LP we have objective function:
  amount_food_0*price[food_0] + ... + amount_food_(m-1)*price[food_(m-1)] 
  and constraints:
  0 : amount_food_0*nutrtient_0_per_food_0 + ... + amount_food_(m-1)*nutrtient_0_per_food_(m-1) <= max_amount_of_nutrtient_0
  .
  .
  .
  n-1 : amount_food_0*nutrtient_(n-1)_per_food_0 + ... + amount_food_(m-1)*nutrtient_(n-1)_per_food_(m-1) <= max_amount_of_nutrtient_(n-1)
  n : -amount_food_0*nutrtient_0_per_food_0 - ... - amount_food_(m-1)*nutrtient_0_per_food_(m-1) <= -min_amount_of_nutrtient_0
  .
  .
  .
  2*n-1 : -amount_food_0*nutrtient_(n-1)_per_food_0 - ... - amount_food_(m-1)*nutrtient_(n-1)_per_food_(m-1) <= -min_amount_of_nutrtient_(n-1)
*/

void testcase(int n, int m)
{
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  //as you cannot eat negative amounts of food
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  
  //constraints on nutrients
  for (int i = 0; i < n; ++i){
    int minn, maxn; //minimum and maximum amount of nutrient
    std::cin >> minn >> maxn;
    lp.set_b(i, maxn);
    lp.set_b(n+i, -minn);
  }
  
  //foods
  for (int j = 0; j < m; ++j){
    int p; std::cin >> p; //price of food
    lp.set_c(j, p);
    for (int i = 0; i < n; ++i){
      int c; std::cin >> c;//amount of nutrient i per portion of this food
      lp.set_a(j,i, c);
      lp.set_a(j,n+i, -c);
    }
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  if (s.is_optimal()) std::cout << floor_to_double(s.objective_value());
  else std::cout << "No such diet.";
}





int main(){
  std::ios_base::sync_with_stdio(false);
  
  int n; std::cin >> n; // number of nutrients
  int m; std::cin >> m; // number of foods
  while (!(n == 0 && m == 0)){
     testcase(n,m); // Solve a particular test case
     std::cout << std::endl;
     std::cin >> n >> m;
  }
  return 0;

}