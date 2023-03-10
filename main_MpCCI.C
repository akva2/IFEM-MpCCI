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

#include "IFEM.h"
#include "Profiler.h"
#include "SIMMpCCIStructure.h"
#include "SIM3D.h"

#include "MpCCIJob.h"

#include <iostream>
#include <stdexcept>


/*!
  \brief Main program for the IFEM MpCCI adapter.

  The input to the program is specified through the following
  command-line arguments. The arguments may be given in arbitrary order.
*/

int main (int argc, char** argv)
{
  if (argc < 1) {
      std::cout << "Need one parameter, the input file to use." << std::endl;
      return 1;
  }
  Profiler prof(argv[0]);
  utl::profiler->start("Initialization");
  IFEM::Init(argc,argv,"MpCCI adapter");
  utl::profiler->stop("Initialization");

  SIMMpCCIStructure<SIM3D> sim;

  if (!sim.read(argv[1]))
    return 2;

  if (!sim.preprocess())
    return 3;

  if (!sim.initSystem(sim.opt.solver,1))
    return 4;

  try {
    MpCCI::Job mpjob(sim, sim);
  } catch(const std::runtime_error& err) {
     std::cerr << err.what() << std::endl;
     return 1;
  }

  return 0;
}

