#include "../include/patternsmanager.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cmath>
PatternsManager::PatternsManager(unsigned inputPatternSize, unsigned outputSize)
: m_inputPatternSize(inputPatternSize)
, m_outputSize(outputSize)
{

}

unsigned PatternsManager::numberOfInputPatterns() const
{
    return m_inputPatterns.size();
}

unsigned PatternsManager::numberOfOutputs() const
{
    return m_outputs.size();
}

const std::vector<double>& PatternsManager::getInputPattern(unsigned nIn) const
{
    return m_inputPatterns[nIn];
}

const std::vector<double>& PatternsManager::getOutput(unsigned nOut) const
{
    return m_outputs[nOut];
}

void PatternsManager::readFile(const std::string& fileName)
{
    std::string fileNameCopy = fileName;
    std::ifstream file(fileNameCopy.c_str());
    if(!file.is_open())
    {
        std::cerr << "Could not open " << fileNameCopy << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string line;
    do
    {
        getline(file,line);
    }
    while(line.length() <= 1 || line[0] == '#');
    std::vector<double> inputPattern(m_inputPatternSize);
    std::vector<double> output(m_outputSize);
    m_inMins.resize(m_inputPatternSize, 1e10);
    m_inMaxs.resize(m_inputPatternSize, -1e10);
    m_inMeans.resize(m_inputPatternSize, 0.0);
    m_outMins.resize(m_outputSize, 1e10);
    m_outMaxs.resize(m_outputSize, -1e10);
    m_outMeans.resize(m_outputSize, 0.0);
    do
    {
        std::stringstream lineStream(line);

        for(int in = 0; in < inputPattern.size(); ++in)
        {
            lineStream >> inputPattern[in];
            m_inMeans[in] += inputPattern[in];
            if(inputPattern[in] < m_inMins[in])
            {
                m_inMins[in] = inputPattern[in];
            }
            if(inputPattern[in] > m_inMaxs[in])
            {
                m_inMaxs[in] = inputPattern[in];
            }
        }
        for(int out = 0; out < output.size(); ++out)
        {
            lineStream >> output[out];
            m_outMeans[out] += output[out];
            if(output[out] < m_outMins[out])
            {
                m_outMins[out] = output[out];
            }
            if(output[out] > m_outMaxs[out])
            {
               m_outMaxs[out] = output[out];
            }
        }
        m_inputPatterns.push_back(inputPattern);
        m_outputs.push_back(output);
    }
    while(getline(file,line) && line.length() > 1);
    for(int in = 0; in < m_inMeans.size(); ++in)
    {
        m_inMeans[in] /= m_inputPatterns.size();
    }
    for(int out = 0; out < m_outMeans.size(); ++out)
    {
        m_outMeans[out] /= m_outputs.size();
    }
    
    m_inStdDevs.resize(m_inputPatternSize,0.0);
    for(int nPattern = 0; nPattern < m_inputPatterns.size(); ++nPattern)
    {
        for(int in = 0; in < m_inStdDevs.size(); ++in)
        {
            m_inStdDevs[in] += (m_inputPatterns[nPattern][in] - m_inMeans[in]) * (m_inputPatterns[nPattern][in] - m_inMeans[in]);
        }
    }
    for(int in = 0; in < m_inStdDevs.size(); ++in)
    {
        m_inStdDevs[in] /= m_inputPatterns.size();
        m_inStdDevs[in] = sqrt(m_inStdDevs[in]);
    }
    
    m_outStdDevs.resize(m_outputSize,0.0);
    for(int nPattern = 0; nPattern < m_outputs.size(); ++nPattern)
    {
        for(int out = 0; out < m_outStdDevs.size(); ++out)
        {
            m_outStdDevs[out] += (m_outputs[nPattern][out] - m_outMeans[out]) * (m_outputs[nPattern][out] - m_outMeans[out]);
        }
    }
    for(int out = 0; out < m_outStdDevs.size(); ++out)
    {
        m_outStdDevs[out] /= m_outputs.size();
        m_outStdDevs[out] = sqrt(m_outStdDevs[out]);
    }
    file.close();
}

void PatternsManager::scale(const std::string& scalingType)
{
    if(scalingType == "none")
    {
        return;
    }
    if(scalingType == "normal")
    {
        for(unsigned nPattern = 0; nPattern < m_inputPatterns.size(); ++nPattern)
        {
            for(unsigned nEntry = 0; nEntry < m_inputPatterns[nPattern].size(); ++nEntry)
            {
                m_inputPatterns[nPattern][nEntry] = (m_inputPatterns[nPattern][nEntry] - m_inMins[nEntry]) / (m_inMaxs[nEntry] - m_inMins[nEntry]);
            }
        }
        for(unsigned nPattern = 0; nPattern < m_outputs.size(); ++nPattern)
        {
            for(unsigned nEntry = 0; nEntry < m_outputs[nPattern].size(); ++nEntry)
            {
                m_outputs[nPattern][nEntry] = (m_outputs[nPattern][nEntry] - m_outMins[nEntry]) / (m_outMaxs[nEntry] - m_outMins[nEntry]);
            }
        }
    }
    if(scalingType == "mean")
    {
        for(unsigned nPattern = 0; nPattern < m_inputPatterns.size(); ++nPattern)
        {
            for(unsigned nEntry = 0; nEntry < m_inputPatterns[nPattern].size(); ++nEntry)
            {
                m_inputPatterns[nPattern][nEntry] = (m_inputPatterns[nPattern][nEntry] - m_inMeans[nEntry]) / m_inStdDevs[nEntry];
            }
        }
        for(unsigned nPattern = 0; nPattern < m_outputs.size(); ++nPattern)
        {
            for(unsigned nEntry = 0; nEntry < m_outputs[nPattern].size(); ++nEntry)
            {
                m_outputs[nPattern][nEntry] = (m_outputs[nPattern][nEntry] - m_outMeans[nEntry]) / m_outStdDevs[nEntry];
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const PatternsManager& pm)
{
    for(unsigned nPattern = 0; nPattern < pm.numberOfInputPatterns(); ++nPattern)
    {
        for(unsigned nEntry = 0; nEntry < pm.getInputPattern(nPattern).size(); ++nEntry)
        {
            os << pm.getInputPattern(nPattern)[nEntry] << " ";
        }
        for(unsigned nEntry = 0; nEntry < pm.getOutput(nPattern).size(); ++nEntry)
        {
            os << pm.getOutput(nPattern)[nEntry] << " ";
        }
        os << std::endl;
    }
	return os;
}
