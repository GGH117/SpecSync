#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../../include/SpecSync.h"

namespace py = pybind11;

PYBIND11_MODULE(SpecSyncBridge, m) {
    m.doc() = "SpecSync Python Bridge";

    py::enum_<SpecSync::Setting>(m, "Setting")
        .value("ResolutionScale", SpecSync::Setting::ResolutionScale)
        .value("TextureStreamingBudget", SpecSync::Setting::TextureStreamingBudget)
        .export_values();

    py::class_<SpecSync::OptimizationCommand>(m, "OptimizationCommand")
        .def(py::init<>())
        .def_readwrite("TargetSetting", &SpecSync::OptimizationCommand::TargetSetting)
        .def_readwrite("TargetValue", &SpecSync::OptimizationCommand::TargetValue);

    py::class_<SpecSync::FrameData>(m, "FrameData")
        .def(py::init<>())
        .def_readwrite("CurrentFPS", &SpecSync::FrameData::CurrentFPS)
        .def_readwrite("CpuUsage", &SpecSync::FrameData::CpuUsage)
        .def_readwrite("GpuUsage", &SpecSync::FrameData::GpuUsage)
        .def_readwrite("GpuTemp", &SpecSync::FrameData::GpuTemp)
        .def_readwrite("AvailableVRAM", &SpecSync::FrameData::AvailableVRAM);

    py::class_<SpecSync::Telemetry>(m, "Telemetry")
        .def(py::init<>())
        .def("Tick", &SpecSync::Telemetry::Tick)
        .def("GetLatestData", &SpecSync::Telemetry::GetLatestData);

    py::class_<SpecSync::OptimizationEngine>(m, "OptimizationEngine")
        .def(py::init<>())
        .def("Process", &SpecSync::OptimizationEngine::Process);
}