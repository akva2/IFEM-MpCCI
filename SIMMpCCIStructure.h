// $Id$
//==============================================================================
//!
//! \file SIMMpCCIStructure.h
//!
//! \date Mar 13 2023
//!
//! \author Arne Morten Kvarving / SINTEF
//!
//! \brief Elastic structure solver for IFEM MpCCI.
//!
//==============================================================================

#ifndef _SIM_MPCCI_STRUCTURE_H_
#define _SIM_MPCCI_STRUCTURE_H_

#include "MpCCIDataHandler.h"
#include "SIMElasticityWrap.h"

class IntegrandBase;


/*!
  \brief Driver wrapping the elasticity solver with a solveStep.
*/

template<class Dim> class SIMMpCCIStructure : public SIMElasticityWrap<Dim>,
                                              public MpCCI::DataHandler
{
public:
  //! \brief Default constructor.
  SIMMpCCIStructure();

  //! \brief The destructor clears the VTF-file pointer.
  virtual ~SIMMpCCIStructure() = default;

  //! \brief Computes the solution for the current time step.
  bool solveStep(TimeStep& tp) override;

  //! \brief Returns the actual integrand.
  Elasticity* getIntegrand() override;

  //! \brief Get data from MpCCI server.
  void getData(int quant_id, const std::vector<int>& nodes, const double* data) override;

  //! \brief Write data to MpCCI server.
  void writeData(int quant_id, const std::vector<int>& nodes, double* data) const override;

protected:
  //! \brief Assemble the nodal interface forces from the fluid solver.
  bool assembleDiscreteTerms(const IntegrandBase*, const TimeDomain&) override;

  std::map<int, std::array<double,3>> loadMap;
};

#endif
