/**
 * @file PipekMezeyLocalization_test.cpp
 * 
 * @date Dec 14, 2015
 * @author Thomas Dresselhaus
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
/* Include Serenity Internal Headers */
#include "data/matrices/CoefficientMatrix.h"
#include "math/Matrix.h"
#include "data/matrices/MatrixInBasis.h"
#include "integrals/OneElectronIntegralController.h"
#include "data/OrbitalController.h"
#include "analysis/orbitalLocalization/PipekMezeyLocalization.h"
#include "system/SystemController.h"
#include "testsupply/SystemController__TEST_SUPPLY.h"
/* Include Std and External Headers */
#include <gtest/gtest.h>


namespace Serenity {
using namespace std;

class PipekMezeyLocalizationTest : public ::testing::Test{
protected:
  static void TearDownTestCase() {
    SystemController__TEST_SUPPLY::cleanUp();
  }
};

/**
 * @test PM Localization Restricted CO minimal basis
 */
TEST_F(PipekMezeyLocalizationTest, testLocalizationRestricted){
  // Create the test system
  auto system =
      SystemController__TEST_SUPPLY::getSystemController(TEST_SYSTEM_CONTROLLERS::CO_MINBAS);
  // Perform SCF
  system->getElectronicStructure<Options::SCF_MODES::RESTRICTED>();
  // Create a copy of the SCF orbitals
  auto orbitals = make_shared<OrbitalController<Options::SCF_MODES::RESTRICTED> > (
    *system->getActiveOrbitalController<Options::SCF_MODES::RESTRICTED>());
  CoefficientMatrix<Options::SCF_MODES::RESTRICTED> coefficients = orbitals->getCoefficients();
  
  const auto& oneIntController = system->getOneElectronIntegralController();
  const auto& overlaps = oneIntController->getOverlapIntegrals();
  /*
   * Calculate a simple Pipek-Mezey-like localization measure to check whether the algorithm does
   * localize something.
   */
  double convMeasure = 0.0;
  for (unsigned int i=0; i<7; ++i) {
    double sumCoeffsThisOrbFirstAtom = 0.0;
    for (unsigned int mu=0; mu<5; ++mu) {
      for (unsigned int nu=0; nu<10; ++nu) {
        sumCoeffsThisOrbFirstAtom += coefficients(mu,i) * coefficients(nu,i) * overlaps(mu,nu);
      }
    }
    convMeasure += sumCoeffsThisOrbFirstAtom*sumCoeffsThisOrbFirstAtom;
    double sumCoeffsThisOrbSecondAtom = 0.0;
    for (unsigned int mu=5; mu<10; ++mu) {
      for (unsigned int nu=0; nu<10; ++nu) {
        sumCoeffsThisOrbSecondAtom += coefficients(mu,i) * coefficients(nu,i) * overlaps(mu,nu);
      }
    }
    convMeasure += sumCoeffsThisOrbSecondAtom*sumCoeffsThisOrbSecondAtom;
  }  
  PipekMezeyLocalization<Options::SCF_MODES::RESTRICTED> localizer(system);
  /*
   * Perform localization
   */
  localizer.localizeOrbitals(*orbitals,500);
  coefficients = orbitals->getCoefficients();
  /*
   * check if MOs still orthogonal
   */
  for_spin(coefficients){
    Matrix<double> overlapMatrix = coefficients_spin.transpose() * overlaps
                                   * coefficients_spin;
    Matrix<double> identity(orbitals->getNOrbitals(),orbitals->getNOrbitals());
      identity.setIdentity();
      Matrix<double> diffMatrix = overlapMatrix - identity;
      double diff=fabs(diffMatrix.maxCoeff());
      diff = fabs(diffMatrix.minCoeff()) > diff ? fabs(diffMatrix.minCoeff()) : diff;
      EXPECT_NEAR(0.0000000000, diff, 2.0e-9);
  };
  /*
   * Calculate the convergence measure again for the now localized, occupied orbitals
   * and compare with the value before.
   */
  double convMeasureLoc = 0.0;
  for (unsigned int i=0; i<7; ++i) {
    double sumCoeffsThisOrbFirstAtom = 0.0;
    for (unsigned int mu=0; mu<5; ++mu) {
      for (unsigned int nu=0; nu<10; ++nu) {
        sumCoeffsThisOrbFirstAtom += coefficients(mu,i) * coefficients(nu,i) * overlaps(mu,nu);
      }
    }
    convMeasureLoc += sumCoeffsThisOrbFirstAtom*sumCoeffsThisOrbFirstAtom;
    double sumCoeffsThisOrbSecondAtom = 0.0;
    for (unsigned int mu=5; mu<10; ++mu) {
      for (unsigned int nu=0; nu<10; ++nu) {
        sumCoeffsThisOrbSecondAtom += coefficients(mu,i) * coefficients(nu,i) * overlaps(mu,nu);
      }
    }
    convMeasureLoc += sumCoeffsThisOrbSecondAtom*sumCoeffsThisOrbSecondAtom;
  }
  EXPECT_GT(convMeasureLoc, convMeasure+0.6);
}

//Test PM Localization Unrestricted
TEST_F(PipekMezeyLocalizationTest, testLocalizationUnrestricted){
  // Create the test system
  auto system = SystemController__TEST_SUPPLY::getSystemController(
      TEST_SYSTEM_CONTROLLERS::CO_MINBAS);
  // Perform SCF
  system->getElectronicStructure<Options::SCF_MODES::UNRESTRICTED>();
  // Create a copy of the SCF orbitals
  auto orbitals = make_shared<OrbitalController<Options::SCF_MODES::UNRESTRICTED> > (
      *system->getActiveOrbitalController<Options::SCF_MODES::UNRESTRICTED>());
  CoefficientMatrix<Options::SCF_MODES::UNRESTRICTED> coefficients = orbitals->getCoefficients();

  const auto& oneIntController = system->getOneElectronIntegralController();
  const auto& overlaps = oneIntController->getOverlapIntegrals();

  double convMeasure = 0.0;
  for_spin(coefficients){
  for (unsigned int i=0; i<7; ++i) {
    double sumCoeffsThisOrbFirstAtom = 0.0;
    for (unsigned int mu=0; mu<5; ++mu) {
      for (unsigned int nu=0; nu<10; ++nu) {
        sumCoeffsThisOrbFirstAtom += coefficients_spin(mu,i) * coefficients_spin(nu,i) * overlaps(mu,nu);
      }
    }
    convMeasure += sumCoeffsThisOrbFirstAtom*sumCoeffsThisOrbFirstAtom;
    double sumCoeffsThisOrbSecondAtom = 0.0;
    for (unsigned int mu=5; mu<10; ++mu) {
      for (unsigned int nu=0; nu<10; ++nu) {
        sumCoeffsThisOrbSecondAtom += coefficients_spin(mu,i) * coefficients_spin(nu,i) * overlaps(mu,nu);
      }
    }
    convMeasure += sumCoeffsThisOrbSecondAtom*sumCoeffsThisOrbSecondAtom;
  }
  };

  PipekMezeyLocalization<Options::SCF_MODES::UNRESTRICTED> localizer(system);
  /*
   * Perform localization
   */
  localizer.localizeOrbitals(*orbitals,500);
  coefficients = orbitals->getCoefficients();
  /*
   * check if MOs still orthogonal
   */
  for_spin(coefficients){
    Matrix<double> overlapMatrix = coefficients_spin.transpose() * overlaps
                                   * coefficients_spin;
    Matrix<double> identity(orbitals->getNOrbitals(),orbitals->getNOrbitals());
      identity.setIdentity();
      Matrix<double> diffMatrix = overlapMatrix - identity;
      double diff=fabs(diffMatrix.maxCoeff());
      diff = fabs(diffMatrix.minCoeff()) > diff ? fabs(diffMatrix.minCoeff()) : diff;
      EXPECT_NEAR(0.0000000000, diff, 2.0e-9);
  };
  /*
    * Calculate the convergence measure again for the now localized, occupied orbitals
    * and compare with the value before.
    */
   double convMeasureLoc = 0.0;
   for_spin(coefficients){
   for (unsigned int i=0; i<7; ++i) {
     double sumCoeffsThisOrbFirstAtom = 0.0;
     for (unsigned int mu=0; mu<5; ++mu) {
       for (unsigned int nu=0; nu<10; ++nu) {
         sumCoeffsThisOrbFirstAtom += coefficients_spin(mu,i) * coefficients_spin(nu,i) * overlaps(mu,nu);
       }
     }
     convMeasureLoc += sumCoeffsThisOrbFirstAtom*sumCoeffsThisOrbFirstAtom;
     double sumCoeffsThisOrbSecondAtom = 0.0;
     for (unsigned int mu=5; mu<10; ++mu) {
       for (unsigned int nu=0; nu<10; ++nu) {
         sumCoeffsThisOrbSecondAtom += coefficients_spin(mu,i) * coefficients_spin(nu,i) * overlaps(mu,nu);
       }
     }
     convMeasureLoc += sumCoeffsThisOrbSecondAtom*sumCoeffsThisOrbSecondAtom;
   }
   };
   EXPECT_GT(convMeasureLoc, convMeasure+0.6);
}

} /* namespace Serenity */