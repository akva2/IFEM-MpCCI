// $Id$
//==============================================================================
//!
//! \file main_MpCCI.C
//!
//! \date Feb 23 2023
//!
//! \author Arne Morten Kvarving / SINTEF
//!
//! \brief Main program for the IFEM MpCCI adapter.
//!
//==============================================================================

#include "MpCCIArgs.h"
#include "SIMMpCCIStructure.h"
#include "SIMSolverMpCCI.h"

#include "IFEM.h"
#include "HHTSIM.h"
#include "NewmarkNLSIM.h"
#include "Profiler.h"
#include "SIM3D.h"

#include <iostream>
#include <stdexcept>


/*!
  \brief Main program for the IFEM MpCCI adapter.

  The input to the program is specified through the following
  command-line arguments. The arguments may be given in arbitrary order.
*/

int main (int argc, char** argv)
{
  Profiler prof(argv[0]);
  utl::profiler->start("Initialization");
  char* infile = nullptr;
  IFEM::Init(argc,argv,"MpCCI adapter");
  MpCCIArgs args;
  for (int i = 1; i < argc; ++i) {
    if (argv[i] == infile || args.parseArg(argv[i]))
      ;
    else if (SIMoptions::ignoreOldOptions(argc,argv,i))
      ;
    else if (!infile) {
      infile = argv[i];
      if (!args.readXML(infile,false))
        return 1;
      i = 0;
    }
  }

  if (!infile) {
    std::cout << "usage: " << argv[0] << " <inputfile>\n";
    return 1;
  }

  utl::profiler->stop("Initialization");

  MpCCI::SIMStructure<SIM3D> sim(args.form);

  try {
    if (args.dynamic) {
      if (args.form == MpCCIArgs::Formulation::Linear) {
        MpCCI::SIMSolver solver(sim);
        return solver.solveProblem(infile, "Solving structure problem");
      } else {
        MpCCI::SIMSolver<MpCCI::SIMStructure<SIM3D>,NewmarkNLSIM> solver(sim);
        return solver.solveProblem(infile, "Solving structure problem");
      }
    } else {
      MpCCI::SIMSolverStat solver(sim);
      if (sim.opt.dumpHDF5(infile))
        solver.handleDataOutput(sim.opt.hdf5,sim.getProcessAdm());
      return solver.solveProblem(infile, "Solving structure problem");
    }
  } catch(const std::runtime_error& err) {
     std::cerr << err.what() << std::endl;
     return 5;
  }

  return 0;
}

