#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../../src/OptimizationEngine/OptimizationEngine.cpp"

namespace py = pybind11;

PYBIND11_MODULE(SpecSyncBridge, m) {
    // Bind the Action structure for Python
    py::class_<SpecSync::OptimizationAction>(m, "OptimizationAction")
        .def_readwrite("MessageForChatbot", &SpecSync::OptimizationAction::MessageForChatbot)
        .def_readwrite("TargetSetting", &SpecSync::OptimizationAction::TargetSetting);

    // Bind the Engine class
    py::class_<SpecSync::OptimizationEngine>(m, "OptimizationEngine")
        .def(py::init<>())
        .def("AnalyzePerformance", &SpecSync::OptimizationEngine::AnalyzePerformance);
}