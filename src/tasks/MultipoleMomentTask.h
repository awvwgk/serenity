/**
 * @file MultipoleMomentTask.h
 *
 * @date Nov 23, 2015
 * @author Jan Unsleber
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

#ifndef TASKS_MULTIPOLEMOMENTTASK_H_
#define TASKS_MULTIPOLEMOMENTTASK_H_
/* Include Serenity Internal Headers */
#include "settings/Options.h"
#include "geometry/Point.h"
#include "settings/Reflection.h"
#include "tasks/Task.h"
/* Include Std and External Headers */
#include <memory>


namespace Serenity {
class SystemController;


using namespace Serenity::Reflection;
struct MultipoleMomentTaskSettings {
  MultipoleMomentTaskSettings():
    highestOrder(2),
    numerical(false),
    origin(Options::REFERENCE_POINT::ORIGIN) { }
  REFLECTABLE(
    (unsigned int) highestOrder,
    (bool) numerical,
    (Options::REFERENCE_POINT) origin
  )
};

/**
 * @class  MultipoleMomentTask MultipoleMomentTask.h
 * @brief  A task to calculate the multipole moments of a system, numerically or analytically
 */
template<Options::SCF_MODES SCFMode>
class MultipoleMomentTask: public Task {
public:
  /**
   * @brief Constructor
   * @param systemController The system of which the multipole moments should be calculated
   */
  MultipoleMomentTask(std::shared_ptr<SystemController> systemController);
  /**
   * @brief Default destructor.
   */
  virtual ~MultipoleMomentTask() = default;
  /**
   * @see Task
   */
  void run();
  /**
   * @brief The settings/keywords for MultipoleMomentTask:
   *        -highestOrder : Available options are:
   *          -1 : dipole moments only
   *          -2 : dipole and quadrupole moments (default)
   *          -3 : dipole, quadrupole and octupole moments
   *        -numerical : If true, calculate dipole moments numerically (default: false)
   *        -origin : The origin used to calculate the moments. Available options are:
   *          -ORIGIN : Use (0,0,0)^T as origin (default)
   *          -COM : Use center of mass as origin
   */
  MultipoleMomentTaskSettings settings;
private:
  ///@brief The underlying systemController
  std::shared_ptr<SystemController> _systemController;
  ///@brief The origin used to calculate origin-corrected multipole moments
  Point _origin = {0,0,0};
};




} /* namespace Serenity */

#endif /* TASKS_MULTIPOLEMOMENTSTASK_H_ */