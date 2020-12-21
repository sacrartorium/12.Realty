#pragma once

#include <dlib/clustering.h>
#include <dlib/svm_threaded.h>
#include <iostream>
#include <map>
#include <vector>

const size_t elementStringCount = 7;

using Cluster = uint64_t;
using Unit = dlib::matrix<double, elementStringCount, 1>;
using Kernel = dlib::radial_basis_kernel<Unit>;
using Trainer = dlib::one_vs_one_trainer<dlib::any_trainer<Unit>>;

Unit parse(std::string line) {
  std::stringstream sline(line);

  double param;
  char c;
  Unit unit;

  for (size_t i = 0; i < elementStringCount - 1; i++) {
    sline >> param;
    if (sline.fail()) {
      param = 0;
      sline.clear(std::ios_base::goodbit);
    }
    sline >> c;
    if (c != ';')
      throw std::logic_error("Error in format data!");

    unit(i) = param;
  };

  sline >> param;
  unit(elementStringCount - 1) = param;
  sline >> c;

  if (!sline.eof() && c == ';') {
    sline >> param;
    if (param == unit(elementStringCount - 1) || (1 >= param)) {
      unit(elementStringCount - 1) = 0;
    } else
      unit(elementStringCount - 1) = 1;
  }

  return unit;
}

double distance(Unit a, Unit b) {
  double dx(b(0) - a(0)), dy(b(1) - a(1));
  return dx * dx + dy * dy;
}

std::ostream &operator<<(std::ostream &stream, Unit unit) {
  std::ios_base::fmtflags flags(stream.flags());
  stream << std::fixed;
  for (size_t i = 0; i < elementStringCount - 1; i++) {
    stream << unit(i);
    stream << ";";
  }
  stream << unit(elementStringCount - 1) << std::endl;
  stream.setf(flags);
  return stream;
}
