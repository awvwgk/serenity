/**
 * @file ABHFPotential.cpp
 *
 * @date Jun 23, 2018
 * @author Moritz Bensberg
 * @copyright \n
 *  This file is part of the program Serenity.\n\n
 *  Serenity is free software: you can redistribute it and/or modify
 *  it under the terms of the LGNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.\n\n
 *  Serenity is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.\n\n
 *  You should have received a copy of the LGNU Lesser General
 *  Public License along with Serenity.
 *  If not, see <http://www.gnu.org/licenses/>.\n
 */

#include "potentials/ABFockMatrixConstruction/ABHFPotential.h"
#include "data/ElectronicStructure.h"
#include "basis/ABShellPairCalculator.h"
#include "system/SystemController.h"
#include "potentials/ABFockMatrixConstruction/ABExchangePotential.h"
#include "potentials/ABFockMatrixConstruction/ABCoulombInteractionPotential.h"

namespace Serenity {

template <Options::SCF_MODES SCFMode>
ABHFPotential<SCFMode>::ABHFPotential(
    std::shared_ptr<SystemController> system,
    std::shared_ptr<BasisController> basisA,
    std::shared_ptr<BasisController> basisB,
    std::vector<std::shared_ptr<DensityMatrixController<SCFMode> > > dMats,
    double exchangeRatio,
    bool topDown,
    Options::DENS_FITS densityFitting,
    std::shared_ptr<BasisController> auxBasisAB,
    std::vector<std::shared_ptr<BasisController> > envAuxBasisController) :
    ABPotential<SCFMode>(basisA,basisB),
    _system(system),
    _exchangeRatio(exchangeRatio){
  //Basis
  this->_basisA->addSensitiveObject(ObjectSensitiveClass<Basis>::_self);
  this->_basisB->addSensitiveObject(ObjectSensitiveClass<Basis>::_self);
  //Densities
  for(const auto& densityController : dMats) {
    densityController->addSensitiveObject(ObjectSensitiveClass<DensityMatrix<SCFMode> >::_self);
  }
  //Build Coulomb and exchange parts.
  if(_exchangeRatio != 0.0) {
    _abExchange = std::make_shared<ABExchangePotential<SCFMode> > (
        _system,
        this->_basisA,
        this->_basisB,
        dMats,
        _exchangeRatio);
  }
  _abCoulomb = std::make_shared<ABCoulombInteractionPotential<SCFMode> > (
      _system,
      this->_basisA,
      this->_basisB,
      dMats,
      topDown,
      densityFitting,
      auxBasisAB,
      envAuxBasisController);
}

template <Options::SCF_MODES SCFMode>
SPMatrix<SCFMode>& ABHFPotential<SCFMode>::getMatrix() {
  if (!_abPotential) {
    const unsigned int nBasisA = this->_basisA->getNBasisFunctions();
    const unsigned int nBasisB = this->_basisB->getNBasisFunctions();
    _abPotential.reset(new SPMatrix<SCFMode>(nBasisA,nBasisB));
    SPMatrix<SCFMode>& f_AB = *_abPotential;
    if (_exchangeRatio != 0.0) f_AB += _abExchange->getMatrix();
    f_AB += _abCoulomb->getMatrix();
  } /* if !_abPotential */
  return *_abPotential;
}

template class ABHFPotential<Options::SCF_MODES::RESTRICTED>;
template class ABHFPotential<Options::SCF_MODES::UNRESTRICTED>;

} /* namespace Serenity */