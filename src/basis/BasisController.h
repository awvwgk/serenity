/**
 * @file BasisController.h
 *
 * @date Jul 30, 2015
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
#ifndef BASISCONTROLLER_H
#define	BASISCONTROLLER_H
/* Include Serenity Internal Headers */
#include "basis/Basis.h"
/* Include Std and External Headers */
#include <memory>
#include <string>
#include <vector>


namespace Serenity {
/**
 * @class ShellPairData
 * @brief Prescreening data for a shell pair
 */
class ShellPairData {
public:
  /**
   * @brief Default Constructor.
   */
  ShellPairData() : bf1(0), bf2(0), factor(0.0){};
  /**
   * @brief Constructor.
   *
   * @param shellI The first index of the shell pair.
   * @param shellJ The second index of the shell pair.
   * @param prescreeningfactor The prescreening factor.
   */
  ShellPairData(unsigned int indexI,
    unsigned int indexJ,
    double prescreeningfactor,
    bool sameBasis = true):
      bf1(indexI),
      bf2(indexJ),
      factor(prescreeningfactor) {
    if (sameBasis) assert(bf2<=bf1);
  }
  /**
   * @brief Default destructor.
   */
  virtual ~ShellPairData() =default;
  /**
   * @brief Less than operator for sorting purposes.
   * @param rhs Right hand side of the comparison.
   * @return Returns true if the factor in lhs is smaller than the one in the rhs.
   */
  bool operator < (const ShellPairData& rhs) const
      {
          return (factor < rhs.factor);
      }
  /**
   * @brief The index of the first shell of the shell pair.
   */
  unsigned int bf1;
  /**
   * @brief The index of the second shell of the shell pair.
   */
  unsigned int bf2;
  /**
   * @brief The prescreening factor.
   *
   * Calculated as \f$ \sqrt{\langle ij|ij \rangle} \f$
   */
  double factor;
};

/**
 * @class BasisController BasisController.h
 * @brief Manages a basis set, e.g. for a whole molecule.
 * 
 * Usually one would work with atom-centered basis functions (see AtomCenteredBasisController).
 * However, most functionalities work without that assumption, thus this abstract class is created
 * to stay general.
 */
class BasisController : public NotifyingClass<Basis>,
                        public ObjectSensitiveClass<Shell>{
  friend class BasisController__TEST_SUPPLY;

public:
  /**
   * @brief Constructor.
   * @param basisString  The name of the controlled basis.
   */
  BasisController(const std::string& basisString) :  _basis(nullptr), _shellPairList(nullptr) ,
                                                     _RIPrescreeningFactors(nullptr), _basisString(basisString) {}

  virtual ~BasisController() = default;
  /**
   * Caution when accessing elements of this vector;
   * A set of functions of the same shell are only one
   * entry.
   *
   * @returns the underlying data object
   */
  inline const Basis& getBasis() {
    if (!_basis) produceBasis();
    return *_basis;
  }
  /**
   * @returns the name of the controlled basis
   */
  inline const std::string& getBasisString() const {
    return _basisString;
  }
  /**
   * @returns the actual number of basis functions. Not to confuse with getReducedNBasisFunctions().
   */
  inline unsigned int getNBasisFunctions() {
    if (!_basis){
    	produceBasis();
    }
    return _nBasisFunctions;
  }
  /**
   * @returns the actual number of basis functions. Not to confuse with getReducedNBasisFunctions().
   *          Cartesian counting is enforced.
   */
  inline unsigned int getNBasisFunctionsCartesian() {
    if (!_basis) produceBasis();
    return _nBasisFunctionsCart;
  }
  /**
   * @returns the actual number of basis functions as counted with pure spherical harmonics.
   *          Not to confuse with getReducedNBasisFunctions() and getNBasisFunctions().
   */
  inline unsigned int getNBasisFunctionsSpherical() {
    if (!_basis) produceBasis();
    return _nBasisFunctionsSpherical;
  }
  /**
   * @returns a reduced number of basis functions counting whole shells of functions as one
   *          (e.g. one set of p-Functions, so actually three functions only increases this
   *          number by one).
   */
  inline size_t getReducedNBasisFunctions() {
    if (!_basis) produceBasis();
    return _basis->size();
  }
  /**
   * @returns the index number of a shell of basis functions to a specific basis function
   *          (e.g. when putting in the index of a p_y function the index of its p-shell is
   *          returned).
   */
  inline unsigned int reducedIndex(unsigned int index) {
    if (!_basis) produceBasis();
    return _reducedIndex[index];
  }
  /**
   * @returns the index of the first specific basis function of the shell with the index given as
   *          input.
   */
  inline unsigned int extendedIndex(unsigned int index) {
    if (!_basis) produceBasis();
    return _extendedIndex[index];
  }
  /**
   * @returns the index of the first specific basis function of the shell with the index given as
   *          input. Counting in terms of Cartesian functions is enforced.
   */
  inline unsigned int extendedIndexCartesian(unsigned int index) {
    if (!_basis) produceBasis();
    return _extendedIndexCart[index];
  }
  /**
   * @returns the index of the first specific basis function of the shell with the index given as
   *          input (pure spherical harmonics counting is enforced).
   */
  inline unsigned int extendedIndexSpherical(unsigned int index) {
    if (!_basis) produceBasis();
    return _extendedIndexSpherical[index];
  }
  /**
   * @returns the highest angular momentum present in the basis functions of this basis
   */
  unsigned int getMaxAngularMomentum() {
    if (!_basis) produceBasis();
    return _maxAngularMomentum;
  }
  void notify(){
    this->notifyObjects();
    this->_shellPairList = nullptr;
    this->_RIPrescreeningFactors = nullptr;
  }
  /**
   * @returns true iff all basis function shells are cartesian
   */
  bool isPureCartesian() const {
    return _pureCartesian;
  }
  /**
   * @returns true iff all basis function shells are spherical
   */
  bool isPureSpherical() const {
    return _pureSpherical;
  }

  std::shared_ptr<std::vector<ShellPairData> > getShellPairData(){
    if (!_shellPairList) createShellPairData();
    return _shellPairList;
  }

  std::shared_ptr<std::vector<ShellPairData> > getRIPrescreeningFactors(){
    if (!_RIPrescreeningFactors) calculateRIPrescreeningFactors();
    return _RIPrescreeningFactors;
  }

protected:
  void produceBasis();

  std::unique_ptr<Basis> _basis;
  std::shared_ptr<std::vector<ShellPairData> > _shellPairList;
  std::shared_ptr<std::vector<ShellPairData> > _RIPrescreeningFactors;
  void calculateRIPrescreeningFactors();
  void createShellPairData();
  virtual std::unique_ptr<Basis> produceBasisFunctionVector() = 0;
  virtual void postConstruction() = 0;

  std::string _basisString;
  
  unsigned int _nBasisFunctions;
  unsigned int _nBasisFunctionsCart;
  unsigned int _nBasisFunctionsSpherical;
  /*
   * Holds precalculated values for the corresponding function
   */
  std::vector<unsigned int> _reducedIndex;
  /*
   * Holds precalculated values for the corresponding function
   */
  std::vector<unsigned int> _extendedIndex;
  std::vector<unsigned int> _extendedIndexCart;
  std::vector<unsigned int> _extendedIndexSpherical;
  
  unsigned int _maxAngularMomentum;
  
  bool _pureCartesian;
  bool _pureSpherical;
};

} /* namespace Serenity */
#endif	/* BASISCONTROLLER_H */