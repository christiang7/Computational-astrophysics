#include <cmath>

// calculates the enery E/m of a planet moving around Sun 

float energy (float pos[], float vel[], float const &GM)
{      
  float r ;
 
  // radial distance
  r = sqrt( pos[0] * pos[0] + pos[1] * pos[1] ) ;

  // return total energy (gravitationl + kinetic)
  return 0.5 * ( vel[0] * vel[0] + vel[1] * vel[1] ) - GM / r ;

}
