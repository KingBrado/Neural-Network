#ifndef INPUTREADER_H
#define INPUTREADER_H

#include <vector>
#include <string>
#include <utility>
#include <iostream>
/**
 * @file inputreader.h
 * @brief Contains enum @ref Mode and class @ref InputReader.
 * @author B. M. Manzi
 * @date 20/11/2017
 */
 /**
 * @brief Enumeration used to define batch or online mode of NN.
 */
enum Mode{BATCH, ONLINE};
/**
* @brief Class reading the parameters of the neural network from
* file "Input.txt" (hard coded).
*/
class InputReader
{
public:
    /**
    * @brief Constructor initialising all members using parameter file "Input.txt".
    */
    InputReader();
    /**
    * @brief Getter for the data file.
    *
    * @return The data file name, as a string.
    */
    const std::string& fileName() const {return m_fileName;}
    /**
    * @brief Getter for the number of entries in a single pattern (columns in file).
    *
    * @return The number of entries in a single pattern.
    */
    unsigned inColumns() const {return m_inColumns;}
    /**
    * @brief Getter for the number of entries for a single output (columns in file).
    *
    * @return The number of entries for a single output.
    */
    unsigned outColumns() const {return m_outColumns;}
    /**
    * @brief Getter for the number of hidden layers required.
    *
    * @return The number of hidden layers required.
    */
    unsigned nHiddenLayers() const {return m_nHiddenLayers;}
    /**
    * @brief Getter for the number of nodes in the hidden layers.
    *
    * @return std::vector of nodes per each layer.
    */
    const std::vector<unsigned>& nNodesPerLayer() const {return m_nNodesPerLayer;}
    /**
    * @brief Getter for the type of scaling required.
    *
    * @return "none", "normal" or "mean".
    */
    const std::string& scalingType() const {return m_scalingType;}
    /**
    * @brief Range used for initialising weights.
    *
    * @return std::pair of values as in [min, max].
    */
    const std::pair<double,double>& weightRange() const {return m_weightRange;}
    /**
    * @brief Range used for initialising thresholds.
    *
    * @return std::pair of values as in [min, max].
    */
    const std::pair<double,double>& thresholdsRange() const {return m_thresholdsRange;}
    /**
    * @brief Getter for the number of epochs of training.
    *
    * @return Number of epochs.
    */
    unsigned nEpochs() const {return m_nEpochs;}
    /**
    * @brief Getter for the learning rate of the Back propagation algorithm.
    *
    * @return Learning rate.
    */
    double learningRate() const {return m_learningRate;}
    /**
    * @brief Getter for the momentum of the Back propagation algorithm.
    *
    * @return Momentum.
    */
    double momentum() const {return m_momentum;}
    /**
    * @brief Getter for the traing mode. See @ref Mode.
    *
    * @return Learning mode (batch, online).
    */
    Mode mode() const {return m_mode;}
    /**
    * @brief Getter for number of patterns used for testing.
    *
    * @return Number of patterns used for testing.
    */
    unsigned nTestPatterns() const {return m_nTestPatterns;}
    /**
    * @brief Getter for fraction of training patterns used for k-fold cross-validation.
    *
    * @return Fraction of excluded training patterns.
    */
    unsigned k() const {return m_k;}
    /**
    * @brief Getter for the activation function used for hidden layers.
    *
    * @return std::string of the required activation function.
    */
    const std::string& hiddenFunction() const {return m_hiddenFunction;}
    /**
    * @brief Getter for the value used for the hidden layer activation function.
    *
    * @return Parameter used for the activation function.
    */
    double betaHidden() const {return m_betaHidden;}
    /**
    * @brief Getter for the activation function used for output.
    *
    * @return std::string of the required activation function.
    */
    const std::string& outFunction() const {return m_outFunction;}
    /**
    * @brief Getter for the value used for the output layer activation function.
    *
    * @return Parameter used for the activation function.
    */
    double betaOut() const {return m_betaOut;}
    /**
    * @brief Cost function used for evaluation of fitness. Only energy available.
    *
    * @return std::string of cost function type.
    */
    const std::string& costFunction() const {return m_costFunction;}
    /**
    * @brief Getter for the desired name of the output function.
    * 
    * @return std::string of output file name.
    */
    const std::string& outFileName() const {return m_outFileName;}
private:
    /**
    * @brief Holds name of data file.
    */
    std::string m_fileName;
    /**
    * @brief Holds number of entries in single input pattern.
    */
    unsigned m_inColumns;
    /**
    * @brief Holds number of entries in output.
    */
    unsigned m_outColumns;
    /**
    * @brief Hold number of required hidden layers.
    */
    unsigned m_nHiddenLayers;
    /**
    * @brief Holds number of nodes per layer. As many entries as @ref m_nHiddenLayers.
    */
    std::vector<unsigned> m_nNodesPerLayer;
    /**
    * @brief Holds "none", "normal" or "mean".
    */
    std::string m_scalingType;
    /**
    * @brief Interval of the form [min, max] for initialising weights in net.
    */
    std::pair<double,double> m_weightRange;
    /**
    * @brief Interval of the form [min, max] for initialising thresholds in net.
    */
    std::pair<double,double> m_thresholdsRange;
    /**
    * @brief Holds number of epochs for training.
    */
    unsigned m_nEpochs;
    /**
    * @brief Holds learning rate.
    */
    double m_learningRate;
    /**
    * @brief Holds momentum.
    */
    double m_momentum;
     /**
    * @brief Holds batch or online.
    */
    Mode m_mode;
     /**
    * @brief Holds number of patterns used for testing.
    */
    unsigned m_nTestPatterns;
     /**
    * @brief Holds fraction of patterns used for cross-validation.
    */
    unsigned m_k;
    /**
    * @brief Holds hidden layers activation function name.
    */
    std::string m_hiddenFunction;
    /**
    * @brief Holds parameter for hidden layers activation function.
    */
    double m_betaHidden;
    /**
    * @brief Holds output layer activation function name.
    */
    std::string m_outFunction;
    /**
    * @brief Holds parameter for output layer activation function.
    */
    double m_betaOut;
    /**
    * @brief Hold name for cost function.
    */
    std::string m_costFunction;
    /**
    * @brief Holds name of output file.
    */
    std::string m_outFileName;
    /**
    * @brief Helper function which actually does all of the work.
    */
    void readFile();
    /**
    * @brief Helper function which checks input consistency.
    */
    void errorcheck(std::iostream& inStream, const std::string& comment);
};

/**
* @brief operator overload to simplify output management.
*/
std::ostream& operator<<(std::ostream& os, const InputReader& ir);
#endif // INPUTREADER_H