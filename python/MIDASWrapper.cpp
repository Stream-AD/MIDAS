//
// Created by Mun Hou on 2021/2/19.
//
// A simple wrapper for MIDAS

#include <pybind11/pybind11.h>
#include <RelationalCore.hpp>
#include <FilteringCore.hpp>
#include <NormalCore.hpp>

namespace py = pybind11;

PYBIND11_MODULE(MIDAS, m) {
    m.doc() = "MIDAS wrapper";

    py::class_<MIDAS::NormalCore>(m, "MIDAS")
            .def(py::init<int, int>())
            .def("add_edge", &MIDAS::NormalCore::operator());

    py::class_<MIDAS::RelationalCore>(m, "MIDASR")
            .def(py::init<int, int, float>())
            .def("add_edge", &MIDAS::RelationalCore::operator());

    py::class_<MIDAS::FilteringCore>(m, "MIDASF")
            .def(py::init<int, int, float, float>())
            .def("add_edge", &MIDAS::FilteringCore::operator());
}