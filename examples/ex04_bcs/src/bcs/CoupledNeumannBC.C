/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "CoupledNeumannBC.h"

template<>
InputParameters validParams<CoupledNeumannBC>()
{
  InputParameters params = validParams<IntegratedBC>();

  // Here we are adding a parameter that will be extracted from the input file by the Parser
  params.addParam<Real>("alpha", 1.0, "Value multiplied by the coupled value on the boundary");
  params.addRequiredCoupledVar("some_var", "Flux Value at the Boundary");
  return params;
}

CoupledNeumannBC::CoupledNeumannBC(const InputParameters & parameters) :
    IntegratedBC(parameters),
    _alpha(getParam<Real>("alpha")),
    _some_var_val(coupledValue("some_var"))
{}

Real
CoupledNeumannBC::computeQpResidual()
{
  return -_test[_i][_qp]*_alpha*_some_var_val[_qp];
}
