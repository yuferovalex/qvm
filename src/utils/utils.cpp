#include "utils.h"

#include <boost/filesystem/path.hpp>

boost::program_options::options_description
buildParamOptions(const std::vector<ParamMeta> &params, const std::string &caption) {
    boost::program_options::options_description description(caption);
    for (const auto& param : params) {
        description.add_options()
            (
                    ("param:" + param.name).c_str(),
                    boost::program_options::value<boost::filesystem::path>(),
                    param.description.c_str()
            );
    }
    return description;
}
