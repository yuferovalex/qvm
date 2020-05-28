#include "CsvDataReader.h"

#include <fstream>
#include <sstream>

CsvDataReader::CsvDataReader(Memory &memory, ParametersPathProvider &parametersPathProvider)
    : m_memory(memory), m_parametersPathProvider(parametersPathProvider)
{}

void CsvDataReader::readInput(const std::vector<ParameterMetadata> &input) {
    for (const auto &meta : input) {
        auto path = m_parametersPathProvider.param(meta.name, true);
        std::ifstream istream(path.string());
        readMatrix(istream, meta);
    }
}

void CsvDataReader::writeOutput(const std::vector<ParameterMetadata> &output) {
    for (const auto &meta : output) {
        auto path = m_parametersPathProvider.param(meta.name, false);
        std::ofstream ostream(path.string());
        writeMatrix(ostream, meta);
    }
}

void CsvDataReader::readMatrix(std::istream &istream, const ParameterMetadata &meta) {
    Command::Reference pointer = meta.address;
    for (size_t i = 0; i < meta.rows; ++i) {
        std::string line;
        std::string cell;
        std::getline(istream, line);
        std::stringstream lineStream(line);

        size_t columnsRead = 0;
        while (std::getline(lineStream, cell, (char) DEFAULT_DELIMITER)) {
            auto value = std::stod(cell);
            m_memory.setValue(pointer, Command::Value(value));
            columnsRead++;
            pointer++;
        }

        if (columnsRead != meta.columns) {
            throw std::runtime_error("wrong format or size of matrix");
        }
    }
}

void CsvDataReader::writeMatrix(std::ostream &ostream, const ParameterMetadata &meta) {
    Command::Reference pointer = meta.address;
    for (size_t i = 0; i < meta.rows; ++i) {
        for (size_t j = 0; j < meta.columns; ++j) {
            auto value = m_memory.value(pointer);
            if (value) {
                ostream << value.value().number << (char) DEFAULT_DELIMITER;
            } else {
                ostream << "no value" << (char) DEFAULT_DELIMITER;
            }
            pointer++;
        }
        ostream << ostream.widen('\n');
    }
    ostream.flush();
}
