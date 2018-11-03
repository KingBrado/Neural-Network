#ifndef PATTERNSMANAGER_H
#define PATTERNSMANAGER_H

#include <string>
#include <fstream>
#include <vector>
/**
 * @file patternsmanager.h
 * @brief Contains class @ref PatternsManager.
 * 
 * @author B. M. Manzi
 * @date 20/11/2017
 */
 /**
 * @brief Class managing input data for training and testing.
 *
 * This class defines useful methods for simply access data
 * acquired from a data file and minimal statistica about these
 * data.
 */
class PatternsManager
{
public:
    /**
    * @brief Constructor
    * 
    * @param inputPatternSize Number of columns in the data file relative 
    * to number of entries in a single pattern.
    * @param outputSize Number of columns in the data file relative to 
    * number of entries in the output.
    */
    PatternsManager(unsigned inputPatternSize, unsigned outputSize);
    /**
    * @brief Reads the data file.
    * 
    * @param fileName Name of the file containing the data.
    */
    void readFile(const std::string& fileName);
    /**
    * @brief Getter for the total number of input patterns,
    *
    * @return Number of input patterns read.
    */
    unsigned numberOfInputPatterns() const;
     /**
    * @brief Getter for the total number of outputs. Should be same as @ref numberOfInputPatterns.
    *
    * @return Number of outputs read.
    */
    unsigned numberOfOutputs() const;
    /**
    * @brief Allows reading access to a single input pattern in the data.
    *
    * @param nIn Index of required pattern.
    * @return Value of pattern \p nIn.
    */
    const std::vector<double>& getInputPattern(unsigned nIn) const;
    /**
    * @brief Allows reading access to a single output in the data.
    *
    * @param nOut Index of required output.
    * @return Value of output \p nOut.
    */
    const std::vector<double>& getOutput(unsigned nOut) const;
    /**
    * @brief Returns minimum value for each of the entries in input patterns.
    *
    * @return std::vector of minima containing as many entries as entries in a single input pattern.
    */
    const std::vector<double>& inMins() const {return m_inMins;}
    /**
    * @brief Returns maximum value for each of the entries in input patterns.
    *
    * @return std::vector of maxima containing as many entries as entries in a single input pattern.
    */
    const std::vector<double>& inMaxs() const {return m_inMaxs;}
    /**
    * @brief Returns mean value for each of the entries in input patterns.
    *
    * @return std::vector of means containing as many entries as entries in a single input pattern.
    */
    const std::vector<double>& inMeans() const {return m_inMeans;}
    /**
    * @brief Returns standard deviation for each of the entries in input patterns.
    *
    * @return std::vector of standard deviations containing as many entries as entries in a single input pattern.
    */
    const std::vector<double>& inStdDevs() const {return m_inStdDevs;}
    /**
    * @brief Returns minimum value for each of the entries in outputs.
    *
    * @return std::vector of minima containing as many entries as entries in a single output.
    */
    const std::vector<double>& outMins() const {return m_outMins;}
    /**
    * @brief Returns maximum value for each of the entries in output.
    *
    * @return std::vector of maxima containing as many entries as entries in a single output.
    */
    const std::vector<double>& outMaxs() const {return m_outMaxs;}
     /**
    * @brief Returns mean value for each of the entries in output.
    *
    * @return std::vector of means containing as many entries as entries in a single output.
    */
    const std::vector<double>& outMeans() const {return m_outMeans;}
    /**
    * @brief Returns standard deviation for each of the entries in output.
    *
    * @return std::vector of standard deviations containing as many entries as entries in a single output.
    */
    const std::vector<double>& outStdDevs() const {return m_outStdDevs;}
    /**
    * @brief Scales the data according to requested scaling type.
    *
    * @param scalingType Either of "none", "normal","mean".
    */
    void scale(const std::string& scalingType);
private:
    /**
    * @brief Holder for number of entries in input pattern.
    */
    unsigned m_inputPatternSize;
    /**
    * @brief Holder for number of entries in output.
    */
    unsigned m_outputSize;
    /**
    * @brief Total number of input patterns read.
    */
    std::vector<std::vector<double> > m_inputPatterns;
    /**
    * @brief Total number of outputs read.
    */
    std::vector<std::vector<double> > m_outputs;
    /**
    * @brief Vector of minima for each of the entries in input pattern.
    */
    std::vector<double> m_inMins;
    /**
    * @brief Vector of maxima for each of the entries in input pattern.
    */
    std::vector<double> m_inMaxs;
    /**
    * @brief Vector of means for each of the entries in input pattern.
    */
    std::vector<double> m_inMeans;
    /**
    * @brief Vector of standard deviations for each of the entries in input pattern.
    */
    std::vector<double> m_inStdDevs;
    /**
    * @brief Vector of minima for each of the entries in output.
    */
    std::vector<double> m_outMins;
    /**
    * @brief Vector of maxima for each of the entries in output.
    */
    std::vector<double> m_outMaxs;
    /**
    * @brief Vector of means for each of the entries in output.
    */
    std::vector<double> m_outMeans;
    /**
    * @brief Vector of standard deviations for each of the entries in output.
    */
    std::vector<double> m_outStdDevs;
};

/**
* @brief Operator overload for @ref PatternsManager to simplify output management.
*/
std::ostream& operator<<(std::ostream& os, const PatternsManager& pm);
    
#endif // PATTERNSMANAGER_H
