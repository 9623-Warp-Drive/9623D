#include <cmath>
#include <algorithm>

enum EASE_FUNCTION {
  EASE_IN_OUT_CUBIC = 0
};

double getEasingFactor(int easeFunction, double completionFactor){
  // Returns the factor (from 0 to 1) by which a velocity should be multiplied to achieve a given ease at a specific percentage of task completion.
  // The velocity is the derivative of the position.

  // This is the lowest factor that can be returned by this function.
  double floor_factor = 0.25;
  // Higher = runs at max speed for more time. Minimum of 1 (don't run at, but peak at max speed). This is NOT LINEAR!
  double run_at_max_speed_factor = 20;
  // The scale by which the function must be multiplied to fit within the floor (computed from floor_factor and run_at_max_speed_factor)
  double scale_factor = ((-4*floor_factor) + 4) * run_at_max_speed_factor;

  switch(easeFunction){
    case EASE_IN_OUT_CUBIC:
      	return std::min(((completionFactor < 0.5) 
        ? floor_factor + (scale_factor * pow(completionFactor, 2)) 
        : floor_factor + (scale_factor * pow((1 - completionFactor), 2))),double(1));
        // We take the minimum of the ease factor and 1 so that we never return a factor greater than 1. When run_at_max_speed_factor is greater than 1, this is effective.
  }

  


  // should never reach
  return 0;
}