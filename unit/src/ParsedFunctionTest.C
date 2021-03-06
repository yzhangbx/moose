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

#include "ParsedFunctionTest.h"

//Moose includes
#include "InputParameters.h"
#include "MooseParsedFunction.h"
#include "FEProblem.h"
#include "MooseUnitApp.h"
#include "AppFactory.h"
#include "GeneratedMesh.h"
#include "MooseParsedFunctionWrapper.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ParsedFunctionTest );

void
ParsedFunctionTest::init()
{
  const char *argv[2] = { "foo", "\0" };

  _app = AppFactory::createApp("MooseUnitApp", 1, (char**)argv);
  _factory = &_app->getFactory();

  InputParameters mesh_params = _factory->getValidParams("GeneratedMesh");
  mesh_params.set<MooseEnum>("dim") = "3";
  mesh_params.set<std::string>("_object_name") = "mesh";
  _mesh = new GeneratedMesh(mesh_params);

  InputParameters problem_params = _factory->getValidParams("FEProblem");
  problem_params.set<MooseMesh *>("mesh") = _mesh;
  problem_params.set<std::string>("_object_name") = "FEProblem";
  _fe_problem = new FEProblem(problem_params);
}

void
ParsedFunctionTest::finalize()
{
  delete _fe_problem;
  delete _mesh;
  delete _app;
}


void
ParsedFunctionTest::basicConstructor()
{
  init();

  InputParameters params = _factory->getValidParams("ParsedFunction");
  //test constructor with no additional variables
  params.set<FEProblem *>("_fe_problem") = _fe_problem;
  params.set<SubProblem *>("_subproblem") = _fe_problem;
  params.set<std::string>("value") = std::string("x + 1.5*y + 2 * z + t/4");
  params.set<std::string>("_object_name") = "test";
  MooseParsedFunction f(params);
  f.initialSetup();
  CPPUNIT_ASSERT(f.value(4, Point(1,2,3)) == 11);

  finalize();
}

void
ParsedFunctionTest::advancedConstructor()
{
  init();

  //test the constructor with one variable
  std::vector<std::string> one_var(1);
  one_var[0] = "q";

  InputParameters params = _factory->getValidParams("ParsedFunction");

  params.set<FEProblem *>("_fe_problem") = _fe_problem;
  params.set<SubProblem *>("_subproblem") = _fe_problem;
  params.set<std::string>("value") = "x + y + q";
  params.set<std::vector<std::string> >("vars") = one_var;
  params.set<std::vector<std::string> >("vals") = std::vector<std::string>(1, "-1"); // Dummy value, will be overwritten in test below
  params.set<std::string>("_object_name") = "test1";


  MooseParsedFunction f(params);
  f.initialSetup();
  // Access address via pointer to MooseParsedFunctionWrapper that contains pointer to libMesh::ParsedFunction
  f._function_ptr->_function_ptr->getVarAddress("q") = 4;
  CPPUNIT_ASSERT( f.value(0, Point(1,2)) == 7 );

  //test the constructor with three variables
  std::vector<std::string> three_vars(3);
  three_vars[0] = "q";
  three_vars[1] = "w";
  three_vars[2] = "r";

  InputParameters params2 = _factory->getValidParams("ParsedFunction");
  params2.set<FEProblem *>("_fe_problem") = _fe_problem;
  params2.set<SubProblem *>("_subproblem") = _fe_problem;
  params2.set<std::string>("value") = "r*x + y/w + q";
  params2.set<std::vector<std::string> >("vars") = three_vars;
  params2.set<std::vector<std::string> >("vals") = std::vector<std::string>(3, "-1"); // Dummy values, will be overwritten in test below
  params2.set<std::string>("_object_name") = "test2";

  MooseParsedFunction f2(params2);
  f2.initialSetup();
  f2._function_ptr->_function_ptr->getVarAddress("q") = 4;
  f2._function_ptr->_function_ptr->getVarAddress("w") = 2;
  f2._function_ptr->_function_ptr->getVarAddress("r") = 1.5;
  CPPUNIT_ASSERT( f2.value(0, Point(2,4)) == 9 );

  //test the constructor with one variable that's set
  std::vector<std::string> one_val(1);
  one_val[0] = "2.5";

  InputParameters params3 = _factory->getValidParams("ParsedFunction");
  params3.set<FEProblem *>("_fe_problem") = _fe_problem;
  params3.set<SubProblem *>("_subproblem") = _fe_problem;
  params3.set<std::string>("value") = "q*x";
  params3.set<std::vector<std::string> >("vars") = one_var;
  params3.set<std::vector<std::string> >("vals") = one_val;
  params3.set<std::string>("_object_name") = "test3";

  MooseParsedFunction f3(params3);
  f3.initialSetup();
  CPPUNIT_ASSERT( f3.value(0,2) == 5 );

  //test the constructor with three variables
  std::vector<std::string> three_vals(3);
  three_vals[0] = "1.5";
  three_vals[1] = "1";
  three_vals[2] = "0";

  InputParameters params4 = _factory->getValidParams("ParsedFunction");
  params4.set<FEProblem *>("_fe_problem") = _fe_problem;
  params4.set<SubProblem *>("_subproblem") = _fe_problem;
  params4.set<std::string>("value") = "q*x + y/r + w";
  params4.set<std::vector<std::string> >("vars") = three_vars;
  params4.set<std::vector<std::string> >("vals") = three_vals;
  params4.set<std::string>("_object_name") = "test4";

  MooseParsedFunction f4(params4);
  f4.initialSetup();
  f4._function_ptr->_function_ptr->getVarAddress("r") = 2;
  CPPUNIT_ASSERT( f4.value(0, Point(2, 4)) == 6 );
  f4._function_ptr->_function_ptr->getVarAddress("r") = 4;
  CPPUNIT_ASSERT( f4.value(0, Point(2, 4)) == 5 );

  finalize();
}

void
ParsedFunctionTest::testVariables()
{
  init();

  //a lot of this functionality is tested in advancedConstructor as well
  //test one variable, make sure we can change it by the reference any time
  std::vector<std::string> one_var(1);
  one_var[0] = "q";

  InputParameters params = _factory->getValidParams("ParsedFunction");
  params.set<FEProblem *>("_fe_problem") = _fe_problem;
  params.set<SubProblem *>("_subproblem") = _fe_problem;
  params.set<std::string>("value") = "x + y + q";
  params.set<std::vector<std::string> >("vars") = one_var;
  params.set<std::vector<std::string> >("vals") = std::vector<std::string>(1, "-1"); // Dummy value, will be overwritten in test below
  params.set<std::string>("_object_name") = "test1";

  MooseParsedFunction f(params);
  f.initialSetup();
  Real & q = f._function_ptr->_function_ptr->getVarAddress("q");
  q = 4;
  CPPUNIT_ASSERT( f.value(0, Point(1, 2)) == 7 );
  q = 2;
  CPPUNIT_ASSERT( f.value(0, Point(1, 2)) == 5 );
  q = -4;
  CPPUNIT_ASSERT( f.value(0, Point(1, 2)) == -1 );

  //test three variables, test updating them randomly
  std::vector<std::string> three_vars(3);
  three_vars[0] = "q";
  three_vars[1] = "w";
  three_vars[2] = "r";

  InputParameters params2 = _factory->getValidParams("ParsedFunction");
  params2.set<FEProblem *>("_fe_problem") = _fe_problem;
  params2.set<SubProblem *>("_subproblem") = _fe_problem;
  params2.set<std::string>("value") = "r*x + y/w + q";
  params2.set<std::vector<std::string> >("vars") = three_vars;
  params2.set<std::vector<std::string> >("vals") = std::vector<std::string>(3, "-1"); // Dummy values, will be overwritten in test below
  params2.set<std::string>("_object_name") = "test2";

  MooseParsedFunction f2(params2);
  f2.initialSetup();
  Real & q2 = f2._function_ptr->_function_ptr->getVarAddress("q");
  Real & w2 = f2._function_ptr->_function_ptr->getVarAddress("w");
  Real & r2 = f2._function_ptr->_function_ptr->getVarAddress("r");
  q2 = 4; w2 = 2; r2 = 1.5;
  CPPUNIT_ASSERT( f2.value(0, Point(2, 4)) == 9 );
  q2 = 1; w2 = 4; r2 = 2.5;
  CPPUNIT_ASSERT( f2.value(0, Point(2, 4)) == 7 );
  q2 = 2;
  CPPUNIT_ASSERT( f2.value(0, Point(2, 4)) == 8 );
  w2 = 3;
  CPPUNIT_ASSERT( f2.value(0, Point(2, 6)) == 9 );

  finalize();
}

void
ParsedFunctionTest::testConstants()
{
  init();

  //this functions tests that pi and e get correctly substituted
  //it also tests built in functions of the function parser
  InputParameters params = _factory->getValidParams("ParsedFunction");
  params.set<std::string>("_object_name") = "test1";
  params.set<FEProblem *>("_fe_problem") = _fe_problem;
  params.set<SubProblem *>("_subproblem") = _fe_problem;
  params.set<std::string>("value") = "log(e) + x";

  MooseParsedFunction f(params);
  f.initialSetup();
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, f.value(0,1), 0.0000001 );

  InputParameters params2 = _factory->getValidParams("ParsedFunction");
  params2.set<std::string>("_object_name") = "test2";
  params2.set<FEProblem *>("_fe_problem") = _fe_problem;
  params2.set<SubProblem *>("_subproblem") = _fe_problem;
  params2.set<std::string>("value") = "sin(pi*x)";

  MooseParsedFunction f2(params2);
  f2.initialSetup();
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, f2.value(0,1), 0.0000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, f2.value(0,0.5), 0.0000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, f2.value(0,1.5), 0.0000001 );

  finalize();
}
