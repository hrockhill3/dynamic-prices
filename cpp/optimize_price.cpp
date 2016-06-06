#include <vector>
#include <utility>
#include <python.h>
#include <boost/python.hpp>
#include <boost/python/numeric.hpp>
#include <boost/python/tuple.hpp>
#include "PriceOptimizer.cpp"
using namespace boost::python;

std::vector<double> ndarray_to_vector(numeric::array& l) {
  std::vector<double> result;
  for (int i = 0; i < len(l); ++i) {
    double item = boost::python::extract<double>(l[i]);
    result.push_back(item);
  }
  return result;
}

void set_price_range(PriceOptimizer * self, numeric::array& l) {
  self->price_range = ndarray_to_vector(l);
}
void set_competitor_prices(PriceOptimizer * self, numeric::array& l) {
  self->competitor_prices = ndarray_to_vector(l);
}
void set_sales_model_coeff(PriceOptimizer * self, numeric::array& l) {
  self->sales_model_coeff = ndarray_to_vector(l);
}

void noop(PriceOptimizer * self) {}

tuple run(PriceOptimizer * self, int t, int n) {
  auto price_pair = self->run(t, n);
  return make_tuple(std::get<0>(price_pair), std::get<1>(price_pair));
}

BOOST_PYTHON_MODULE(optimize_price)
{
  boost::python::numeric::array::set_module_and_type("numpy", "ndarray");

  class_<PriceOptimizer>("PriceOptimizer", init<int, int>())
    .def("run", &run)
    .def("set_price_range", &set_price_range)
    .def("set_competitor_prices", &set_competitor_prices)
    .def("set_sales_model_coeff", &set_sales_model_coeff)
    .def_readwrite("L", &PriceOptimizer::L)
    .def_readwrite("Z", &PriceOptimizer::Z)
    .def_readwrite("M", &PriceOptimizer::M)
    .def_readwrite("delta", &PriceOptimizer::delta)
    .add_property("price_range", &noop, &set_price_range)
    .add_property("competitor_prices", &noop, &set_competitor_prices)
    .add_property("sales_model_coeff", &noop, &set_sales_model_coeff)
  ;
}