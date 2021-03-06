/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "PorousFlowEffectiveSaturationVG.h"

namespace PorousFlowEffectiveSaturationVG
{
Real
seff(Real p, Real al, Real m)
{
  Real n, seff;

  if (p >= 0.0)
    return 1.0;
  else
  {
    n = 1.0 / (1.0 - m);
    seff = 1.0 + std::pow(- al * p, n);
    return std::pow(seff, - m);
  }
}

Real
dseff(Real p, Real al, Real m)
{
  if (p >= 0.0)
    return 0.0;
  else
  {
    Real n = 1.0 / (1.0 - m);
    Real inner = 1.0 + std::pow(- al * p, n);
    Real dinner_dp = - n * al * std::pow( - al * p, n - 1.0);
    Real dseff_dp = - m * std::pow(inner, - m - 1) * dinner_dp;
    return dseff_dp;
  }
}

Real
d2seff(Real p, Real al, Real m)
{
  if (p >= 0.0)
    return 0.0;
  else
  {
    Real n = 1.0 / (1.0 - m);
    Real inner = 1.0 + std::pow( - al * p, n);
    Real dinner_dp = - n * al * std::pow(- al * p, n - 1.0);
    Real d2inner_dp2 = n * (n - 1.0) * al * al * std::pow(- al * p, n - 2.0);
    Real d2seff_dp2 = m * (m + 1.0) * std::pow(inner, - m - 2.0) * std::pow(dinner_dp, 2.0) - m * std::pow(inner, - m - 1.0) * d2inner_dp2;
    return d2seff_dp2;
  }
}
}
