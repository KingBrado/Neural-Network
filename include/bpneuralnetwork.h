#ifndef BP_NEURALNETWORK_H
#define BP_NEURALNETWORK_H

#include <vector>
#include <utility>
#include "patternsmanager.h"
#include "inputreader.h"
#include "activationfunction.h"

/**
 * @file bpneuralnetwork.h
 * @brief Contains class @ref BPNeuralNetwork.
 * 
 * @author B. M. Manzi
 * @date 20/11/2017
 */
 /**
 * @brief Class implementing a Multilayer perceptron neural network
 * with backpropagation.
 * 
 * This class contains a neural network with arbitrary number of 
 * layers and nodes per layer, but also acts as a wrapper, since 
 * it includes input and output tasks. It is meant as a solution 
 * for the MESIIA exercise of Activity 1, and not as a optimal solution
 * to use for real applications.
 */
class BPNeuralNetwork
{
    /**
     * @brief Private Structure which holds information defining a "Neuron"
     * 
     * Each Neuron hold its output, obtained by applying the activation function
     * to the weighted sum, the derivative of the activation function computed
     * with the same argument, a vector of weights for each of the inputs,
     * the difference to apply to each weight after update, the previous value
     * of such weights, the eventual threshold (bias) and the delta computed
     * during backpropagation.
     */
    typedef struct
    { 
        double output; 
        double derOutput; 
        std::vector<double> weights; 
        std::vector<double> deltaWeights;
        std::vector<double> oldDeltaWeights;
        double threshold;
        double deltaThreshold;
        double oldDeltaThreshold;
        double delta;
    } Neuron;
public:
    /**
    * @brief Constructor reads the parameters file "Input.txt",
    * reads the data and initialises the net.
    */
    BPNeuralNetwork();
    /**
    * @brief Destructor cleaning the dynamic allocated memory.
    */
    ~BPNeuralNetwork();
    /**
    * @brief A call to this function will train the net on the 
    * selected data.
    * @param excluded Every \p excluded pattern is <b>not</b> used 
    * for training.
    */
    void train(unsigned excluded);
    /**
    * @brief A call to this function will test the trained network
    * on the test data.
    */
    void test();
    /**
    * @brief Test on \p included training data. Use for k-fold
    * cross-validation.
    *
    * @param included Data to include in the cross-validation. Most likely
    * the same as \p excluded in @ref train .
    */
    void crossvalidate(unsigned included);
    /**
    * @brief Allows access to the parameter file reader.
    *
    * @return Constant reference to the parameter @ref InputReader class.
    */
    const InputReader& ir() const {return m_ir;}
private:
    /**
    * @brief Object of @ref InputReader class containing all the parameters
    * defined in "Input.txt".
    */
    InputReader m_ir;
    /**
    * @brief Object of @ref PatternsManager class containig the training data.
    */
    PatternsManager m_pm;
    /**
    * @brief Pointer to @ref ActivationFunction used for the hidden layers.
    */
    ActivationFunction* m_hFunction;
    /**
    * @brief Pointer to @ref ActivationFunction used for the output layers.
    */
    ActivationFunction* m_oFunction;
     /**
    * @brief The actual net, defined as a vector containing layers, which are defined as
    * vectors of nodes, which are of type @ref Neuron.
    */
    std::vector<std::vector<Neuron> > m_net;
    /**
    * @brief Last layer (output) of the net, handled separetely from the rest of the net.
    */
    std::vector<Neuron> m_outputs;
    /**
    * @brief Initialisation function for the neural network.
    */
    void initialiseNet();
    /**
    * @brief Setup the activation function for the hidden layer, as defined in the 
    * @ref InputReader.
    */
    void initialiseHiddenFunction();
    /**
    * @brief Setup the activation function for the output layer, as defined in the 
    * @ref InputReader.
    */
    void initialiseOutFunction();
    /**
    * @brief Helper function which defines a single propagation epoch for a single
    * pattern.
    *
    * @param nPattern Index of the considered pattern.
    */
    void propagate(unsigned nPattern);
    /**
    * @brief Helper function which defines a single backpropagation epoch for a single
    * pattern.
    *
    * @param nPattern Index of the considered pattern.
    */
    void backPropagate(unsigned nPattern);
    /**
    * @brief Helper function which updates the weights of all nodes, using deltas 
    * computed during @ref backPropagate.
    */
    void update();
    /**
    * @brief Helper function which computes the output of intermediate layers, used during  
    * @ref propagate .
    * 
    * @param layer The layer containing the node for which to compute the output.
    * @param neuroIndex Index of the @ref Neuron in the \p layer. 
    * @param nPattern Index of the considered pattern.
    */
    void computeOutput(unsigned layer, unsigned neuroIndex, unsigned nPattern);
    /**
    * @brief Helper function printing the parameters used to the output file.
    */
    void printHeaderToFile();
    /**
    * @brief Helper function printing the weights after a complete training to output.
    * 
    * @param excluded See @ref train.
    */
    void printWeightsToFile(unsigned excluded);
    /** 
    * @brief Helper function printing the results of cross-validation to output.
    *
    * @param included See @ref crossvalidate .
    * @param results Results obtained from @ref crossvalidate .
    * @param expectedResults Expected outcome, given in the data.
    */
    void printCrossValidationResults(unsigned included, const std::vector<std::vector<double> >& results, const std::vector<std::vector<double> >& expectedResults);
    /** 
    * @brief Helper function printing the results of tests to output. Also prints 
    * data for scatter plot.
    *
    * @param results Results obtained from @ref crossvalidate().
    * @param expectedResults Expected outcome, given in the data.
    */
    void printTestResults(const std::vector<std::vector<double> >& results, const std::vector<std::vector<double> >& expectedResults);
};

#endif // BP_NEURALNETWORK_H
