#ifndef QVM_CSVDATAREADER_H
#define QVM_CSVDATAREADER_H

#include <vector>

#include <boost/filesystem/path.hpp>

#include "qvm/ParameterMetadata.h"
#include "memory/Memory.h"

class ParametersPathProvider;

class CsvDataReader {
public:
    enum { DEFAULT_DELIMITER = ';' };
    explicit CsvDataReader(Memory &memory, ParametersPathProvider &parametersPathProvider);
    void readInput(const std::vector<ParameterMetadata> &input);
    void writeOutput(const std::vector<ParameterMetadata> &output);

private:
    void readMatrix(std::istream &istream, const ParameterMetadata &meta);
    void writeMatrix(std::ostream &ostream, const ParameterMetadata &meta);

    Memory &m_memory;
    ParametersPathProvider &m_parametersPathProvider;
};


class ParametersPathProvider {
public:
    virtual ~ParametersPathProvider() = default;

    virtual boost::filesystem::path param(const std::string &name, bool requiredExists) const = 0;
};


#endif //QVM_CSVDATAREADER_H
