// $Id$
//==============================================================================
//!
//! \file MpCCIJob.h
//!
//! \date Feb 23 2023
//!
//! \author Arne Morten Kvarving / SINTEF
//!
//! \brief The IFEM MpCCI adapter job class.
//!
//==============================================================================

#ifndef MPCCIJOB_H_
#define MPCCIJOB_H_

#include <mpcci.h>

#include <string_view>
#include <vector>

class SIMinput;

namespace MpCCI {

//! \brief Class handling a MpCCI job.
//! \details Only a single instance is allowed.
class Job
{
public:
  static Job* globalInstance; //!< Singleton static pointer
  static bool dryRun; //!< To perform a dry run - used in tests

  //! \brief The constructor initializes the MpCCI job.
  Job(SIMinput& simulator);

  //! \brief The destructor deinitializes the MpCCI job.
  ~Job();

  //! \brief Called during MpCCI_Init.
  //! \details Used to defines nodes and elements
  static int definePart(MPCCI_SERVER* server, MPCCI_PART* part);

  //! \brief Struct for holding a linear FEM mesh definition.
  struct MeshInfo {
      std::vector<int> nodes; //!< Global nodes on interface
      std::vector<double> coords; //!< Coordinates for nodes on interface
      std::vector<int> elms; //!< Element node indices on interface
      std::vector<unsigned> types; //!< Type of each element
  };

  //! \brief Returns the linear FEM mesh for a given topology set.
  MeshInfo meshData(std::string_view name) const;

private:
  MPCCI_JOB* mpcciJob{nullptr}; //!< MpCCI job info structure
  MPCCI_TINFO mpcciTinfo{0}; //!< MpCCI time step information
  SIMinput& sim; //!< Reference to IFEM simulator
};

}

#endif
