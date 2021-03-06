# Testing permeability from porosity
# Trivial test, checking calculated permeability is correct
# k = k_anisotropic * f * d^2 * phi^n / (1-phi)^m

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 3
  xmin = 0
  xmax = 3
[]

[GlobalParams]
  block = 0
  PorousFlowDictator = dictator
[]

[Variables]
  [./pp]
    [./InitialCondition]
      type = ConstantIC
      value = 0
    [../]
  [../]
[]

[Kernels]
  [./flux]
    type = PorousFlowAdvectiveFlux
    gravity = '0 0 0'
    variable = pp
  [../]
[]

[BCs]
  [./ptop]
    type = PresetBC
    variable = pp
    boundary = right
    value = 0
  [../]
  [./pbase]
    type = PresetBC
    variable = pp
    boundary = left
    value = 1
  [../]
[]

[AuxVariables]
  [./poro]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./perm_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./perm_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./perm_z]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./poro]
    type = MaterialRealAux
    property = PorousFlow_porosity_qp
    variable = poro
  [../]
  [./perm_x]
    type = MaterialRealTensorValueAux
    property = PorousFlow_permeability_qp
    variable = perm_x
    row = 0
    column = 0
  [../]
  [./perm_y]
    type = MaterialRealTensorValueAux
    property = PorousFlow_permeability_qp
    variable = perm_y
    row = 1
    column = 1
  [../]
  [./perm_z]
    type = MaterialRealTensorValueAux
    property = PorousFlow_permeability_qp
    variable = perm_z
    row = 2
    column = 2
  [../]
[]

[Postprocessors]
  [./perm_x_bottom]
    type = PointValue
    variable = perm_x
    point = '0 0 0'
  [../]
  [./perm_y_bottom]
    type = PointValue
    variable = perm_y
    point = '0 0 0'
  [../]
  [./perm_z_bottom]
    type = PointValue
    variable = perm_z
    point = '0 0 0'
  [../]
  [./perm_x_top]
    type = PointValue
    variable = perm_x
    point = '3 0 0'
  [../]
  [./perm_y_top]
    type = PointValue
    variable = perm_y
    point = '3 0 0'
  [../]
  [./perm_z_top]
    type = PointValue
    variable = perm_z
    point = '3 0 0'
  [../]
[]

[UserObjects]
  [./dictator]
    type = PorousFlowDictator
    porous_flow_vars = 'pp'
    number_fluid_phases = 1
    number_fluid_components = 1
  [../]
[]

[Materials]
  [./temperature]
    type = PorousFlowTemperature
  [../]
  [./nnn]
    type = PorousFlowNodeNumber
    on_initial_only = true
  [../]

  [./massfrac]
    type = PorousFlowMassFraction
  [../]

  # Fluid pressure
  [./eff_fluid_pressure]
    type = PorousFlowEffectiveFluidPressure # Calculate effective fluid pressure from fluid phase pressures and saturations
  [../]
  [./ppss]
    type = PorousFlow1PhaseP_VG # Calculate fluid pressure and saturation for 1-phase case
    porepressure = pp
    al = 1E-8 # unimportant in this fully-saturated test
    m = 0.8   # unimportant in this fully-saturated test
  [../]

  # Fluid density
  [./dens0]
    type = PorousFlowDensityConstBulk
    density_P0 = 1000
    bulk_modulus = 2.2E9
    phase = 0
  [../]
  [./dens_all]
    type = PorousFlowJoiner
    material_property = PorousFlow_fluid_phase_density
  [../]
  [./dens_all_at_quadpoints]
    type = PorousFlowJoiner
    material_property = PorousFlow_fluid_phase_density_qp
  [../]

  # Porosity
  [./porosity]
    type = PorousFlowPorosityConst
    porosity = 0.1
  [../]

  # Permeability
  [./permeability]
    type = PorousFlowPermeabilityKozenyCarman
    k_anisotropy = '1 0 0  0 2 0  0 0 0.1'
    poroperm_function = kozeny_carman_fd2
    f = 0.1
    d = 5
    m = 2
    n = 7
  [../]
  [./relperm]
    type = PorousFlowRelativePermeabilityCorey
    n_j = 0 # unimportant in this fully-saturated situation
    phase = 0
  [../]
  [./relperm_all]
    type = PorousFlowJoiner
    material_property = PorousFlow_relative_permeability
  [../]

  # Fluid viscosity (assumed constant)
  [./visc0]
    type = PorousFlowViscosityConst
    viscosity = 1E-3
    phase = 0
  [../]
  [./visc_all]
    type = PorousFlowJoiner
    material_property = PorousFlow_viscosity
  [../]
[]

[Preconditioning]
  [./andy]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  solve_type = Newton
  type = Steady
  l_tol = 1E-5
  nl_abs_tol = 1E-3
  nl_rel_tol = 1E-8
  l_max_its = 200
  nl_max_its = 400

  petsc_options_iname = '-pc_type -pc_asm_overlap -sub_pc_type -ksp_type -ksp_gmres_restart'
  petsc_options_value = ' asm      2              lu            gmres     200'
[]


[Outputs]
  file_base = PermFromPoro01
  csv = true
  execute_on = 'initial timestep_end'
[]
