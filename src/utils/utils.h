#ifndef QVM_UTILS_H
#define QVM_UTILS_H

#include <boost/program_options/options_description.hpp>

#include "qvm/ParameterMetadata.h"

boost::program_options::options_description
buildParamOptions(const std::vector<ParameterMetadata> &params, const std::string &caption = std::string());

#endif //QVM_UTILS_H
