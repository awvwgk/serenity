/**
 *  @file TDEmbeddingTask_python.cpp
 *
 *  @date   Jul 19, 2016
 *  @author Jan Unsleber
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
#include "tasks/TDEmbeddingTask.h"
#include "system/SystemController.h"
/* Include Std and External Headers */
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Serenity;


void export_ProjectionBasedEmbTask(py::module &spy){

  py::class_<TDEmbeddingTaskSettings>(spy, "TDEmbeddingTaskSettings",
      "@brief Default constructor for Settings all set to their default values.")
    .def_readwrite("levelShiftParameter",&TDEmbeddingTaskSettings::levelShiftParameter)
    .def_readwrite("locType",&TDEmbeddingTaskSettings::locType)
    .def_readwrite("naddXCFunc",&TDEmbeddingTaskSettings::naddXCFunc)
    .def_readwrite("naddKinFunc",&TDEmbeddingTaskSettings::naddKinFunc)
    .def_readwrite("longRangeNaddKinFunc",&TDEmbeddingTaskSettings::longRangeNaddKinFunc)
    .def_readwrite("embeddingMode",&TDEmbeddingTaskSettings::embeddingMode)
    .def_readwrite("useEnvSys",&TDEmbeddingTaskSettings::useEnvSys)
    .def_readwrite("orbitalThreshold",&TDEmbeddingTaskSettings::orbitalThreshold)
    .def_readwrite("enforceCharges",&TDEmbeddingTaskSettings::enforceCharges)
    .def_readwrite("smoothFactor",&TDEmbeddingTaskSettings::smoothFactor)
    .def_readwrite("singValThreshold",&TDEmbeddingTaskSettings::singValThreshold)
    .def_readwrite("lbDamping",&TDEmbeddingTaskSettings::lbDamping)
    .def_readwrite("lbCycles",&TDEmbeddingTaskSettings::lbCycles)
    .def_readwrite("carterCycles",&TDEmbeddingTaskSettings::carterCycles)
    .def_readwrite("noSupRec",&TDEmbeddingTaskSettings::noSupRec)
    .def_readwrite("truncationFactor",&TDEmbeddingTaskSettings::truncationFactor)
    .def_readwrite("truncAlgorithm",&TDEmbeddingTaskSettings::truncAlgorithm)
    .def_readwrite("netThreshold",&TDEmbeddingTaskSettings::netThreshold)
    .def_readwrite("borderAtomThreshold",&TDEmbeddingTaskSettings::borderAtomThreshold)
    .def_readwrite("basisFunctionRatio",&TDEmbeddingTaskSettings::basisFunctionRatio)
    .def_readwrite("nonOrthogonalCrit",&TDEmbeddingTaskSettings::nonOrthogonalCrit)
    .def_readwrite("load",&TDEmbeddingTaskSettings::load)
    .def_readwrite("name",&TDEmbeddingTaskSettings::name);

  py::class_<TDEmbeddingTask<RESTRICTED> >(spy,"TDEmbeddingTask_R")
        .def(py::init<std::shared_ptr<SystemController>,std::shared_ptr<SystemController>>())
        .def("run", &TDEmbeddingTask<RESTRICTED>::run )
        .def_readwrite("settings",&TDEmbeddingTask<RESTRICTED>::settings);

  py::class_<TDEmbeddingTask<UNRESTRICTED> >(spy,"TDEmbeddingTask_U")
        .def(py::init<std::shared_ptr<SystemController>,std::shared_ptr<SystemController>>())
        .def("run", &TDEmbeddingTask<UNRESTRICTED>::run )
        .def_readwrite("settings",&TDEmbeddingTask<UNRESTRICTED>::settings);


}


