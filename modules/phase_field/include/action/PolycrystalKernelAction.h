/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef POLYCRYSTALKERNELACTION_H
#define POLYCRYSTALKERNELACTION_H

#include "Action.h"

/**
 * Action that sets up ACGrGrPoly, ACInterface, TimeDerivative, and ACGBPoly
 * kernels.
 */
class PolycrystalKernelAction: public Action
{
public:
  PolycrystalKernelAction(const InputParameters & params);

  virtual void act();

protected:
  /// number of grains to create
  unsigned int _op_num;

  /// base name for the order parameter variables
  std::string _var_name_base;

  /// kernels are implicit?
  bool _implicit;

  /// number of deformed grains
  unsigned int _ndef;
};

template<>
InputParameters validParams<PolycrystalKernelAction>();

#endif //POLYCRYSTALKERNELACTION_H
