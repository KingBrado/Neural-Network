#include "../include/bpneuralnetwork.h"
#include "../include/transferactivation.h"
#include "../include/logistic.h"
#include "../include/tanhfunction.h"
#include "../include/utility.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

BPNeuralNetwork::BPNeuralNetwork()
: m_ir()                                                 
, m_pm(m_ir.inColumns(),m_ir.outColumns())
, m_net(m_ir.nHiddenLayers())
, m_outputs(m_ir.outColumns())
{
    //Note that at this stage m_ir is fully initialised.
    //m_pm needs an assigned file, included in the parameter file
    //at entry #1
    m_pm.readFile(m_ir.fileName());
    //Defining how to scale the data
    m_pm.scale(m_ir.scalingType());
    std::srand(std::time(0));
    //After intialising rand() and m_pm, I start initialising the net
    //the activation functions and then I write all information to the
    //header of the output file.
    initialiseNet();
    initialiseHiddenFunction();
    initialiseOutFunction();
    printHeaderToFile();
}

BPNeuralNetwork::~BPNeuralNetwork()
{
    delete m_oFunction;
    m_oFunction = NULL;
    delete m_hFunction;
    m_hFunction = NULL;    
}

void BPNeuralNetwork::train(unsigned excluded)
{
    //The propagate-backPropagate functions are called once for every epoch and
    //for each pattern. The update function is called once per epoch and per pattern, if online
    //or just once per epoch if batch.
    for(unsigned t = 0; t < m_ir.nEpochs(); ++t)
    {   
        for(unsigned nPattern = 0; nPattern < m_pm.numberOfInputPatterns() - m_ir.nTestPatterns(); ++nPattern)
        {
            //Loop jumps over patterns which are excluded, for crossvalidation.
            if(nPattern % m_ir.k() == excluded)
            {
                continue;
            }
            propagate(nPattern);
            backPropagate(nPattern);
    
            if(m_ir.mode() == ONLINE)
            {
                update();    
                
            }
        }
        if(m_ir.mode() == BATCH)
        {
            update();
        }
    }
    //After training, printing weights to file.
    printWeightsToFile(excluded);
}

void BPNeuralNetwork::test()
{
    std::vector<std::vector<double> > results;
    std::vector<std::vector<double> > expectedResults;
    //For testing, I just propagate all test patterns and collect the outputs. Then I write to file.
    for(unsigned nPattern = m_pm.numberOfInputPatterns() - m_ir.nTestPatterns(); nPattern < m_pm.numberOfInputPatterns(); ++nPattern)
    {
        propagate(nPattern);
        std::vector<double> result(m_outputs.size());
        std::vector<double> expectedResult(m_pm.getOutput(nPattern).size());
        for(unsigned outIndex = 0; outIndex < m_outputs.size(); ++outIndex)
        {
            result[outIndex] = m_outputs[outIndex].output;
            expectedResult[outIndex] = m_pm.getOutput(nPattern)[outIndex];
            results.push_back(result);
            expectedResults.push_back(expectedResult);
        }
    }
    printTestResults(results, expectedResults);
}

void BPNeuralNetwork::crossvalidate(unsigned included)
{
    std::vector<std::vector<double> > results;
    std::vector<std::vector<double> > expectedResults;
    //Cross-validation is very similar to test, but acts on the included patterns, most likely the same as
    //the excluded during training. The results are the printed to output.
    for(unsigned nPattern = 0; nPattern < m_pm.numberOfInputPatterns() - m_ir.nTestPatterns(); ++nPattern)
    {
        if(nPattern % m_ir.k() == included)
        {
            propagate(nPattern);
            std::vector<double> result(m_outputs.size());
            std::vector<double> expectedResult(m_pm.getOutput(nPattern).size());
            for(unsigned outIndex = 0; outIndex < m_outputs.size(); ++outIndex)
            {
                result[outIndex] = m_outputs[outIndex].output;
                expectedResult[outIndex] = m_pm.getOutput(nPattern)[outIndex];
                results.push_back(result);
                expectedResults.push_back(expectedResult);
            }
        }
    }
    printCrossValidationResults(included, results, expectedResults);
}

void BPNeuralNetwork::initialiseNet()
{
    //Initialisation of the network. This scheme of for cycles is repeated in other functions.
    //Basically I cycle through the layers, for each layer I cycle through the nodes, and,
    //for each node, I cycle through weights,deltaWeights and oldDeltaWeights,whenever necessary.
    for(unsigned layer = 0; layer < m_ir.nHiddenLayers(); ++layer)
    {
        //The number of node per layer is  defined in the parameter file.
        m_net[layer].resize(m_ir.nNodesPerLayer()[layer]);
        for(unsigned neuroIndex = 0; neuroIndex < m_ir.nNodesPerLayer()[layer]; ++neuroIndex)
        {
            //For the first layer, the number of weights corresponds to the number of input entries
            // for each pattern.
            if(layer == 0)
            {
                m_net[layer][neuroIndex].output = 0.0;
                m_net[layer][neuroIndex].derOutput = 0.0;
                m_net[layer][neuroIndex].weights.resize(m_ir.inColumns());
                m_net[layer][neuroIndex].deltaWeights.resize(m_ir.inColumns(),0.0);
                m_net[layer][neuroIndex].oldDeltaWeights.resize(m_ir.inColumns(),0.0);
                m_net[layer][neuroIndex].threshold = 0.0;
                m_net[layer][neuroIndex].deltaThreshold = 0.0;
                m_net[layer][neuroIndex].oldDeltaThreshold = 0.0;
                m_net[layer][neuroIndex].delta = 0.0;
            }
            //For other layers, the number of weights is just the number of nodes in the previous layer.
            else
            {
                m_net[layer][neuroIndex].output = 0.0;
                m_net[layer][neuroIndex].derOutput = 0.0;
                m_net[layer][neuroIndex].weights.resize(m_ir.nNodesPerLayer()[layer-1]);
                m_net[layer][neuroIndex].deltaWeights.resize(m_ir.nNodesPerLayer()[layer-1],0.0);
                m_net[layer][neuroIndex].oldDeltaWeights.resize(m_ir.nNodesPerLayer()[layer-1],0.0);
                m_net[layer][neuroIndex].threshold = 0.0;
                m_net[layer][neuroIndex].deltaThreshold = 0.0;
                m_net[layer][neuroIndex].oldDeltaThreshold = 0.0;
                m_net[layer][neuroIndex].delta = 0.0;
            }
            //Weights are initialised randomly to the range defined in the parameter file.
            for(unsigned wIndex = 0; wIndex < m_net[layer][neuroIndex].weights.size(); ++wIndex)
            {
                m_net[layer][neuroIndex].weights[wIndex] = static_cast<double>(std::rand()) / RAND_MAX * (m_ir.weightRange().second - m_ir.weightRange().first) + m_ir.weightRange().first;
	    }
            //Thresholds are initialised as weight if an interval is defined, or all set to the same value if [min max] with min = max.
            m_net[layer][neuroIndex].threshold = (m_ir.thresholdsRange().second == m_ir.thresholdsRange().first) ? m_ir.thresholdsRange().first 
                                                                                                                 : static_cast<double>(std::rand()) / RAND_MAX * (m_ir.thresholdsRange().second - m_ir.thresholdsRange().first) + m_ir.thresholdsRange().first;
	}
    }
    //The output layer is treated separately. There is only one layer, the cycle is for every output node,
    //and the number of weights  corresponds to the number of nodes in the last hidden layer.
    for(unsigned outIndex = 0; outIndex < m_outputs.size(); ++outIndex)
    {
        m_outputs[outIndex].output = 0.0;
        m_outputs[outIndex].derOutput = 0.0;
        m_outputs[outIndex].weights.resize(m_ir.nNodesPerLayer()[m_ir.nNodesPerLayer().size() - 1]);
        m_outputs[outIndex].deltaWeights.resize(m_ir.nNodesPerLayer()[m_ir.nNodesPerLayer().size() - 1],0.0);
        m_outputs[outIndex].oldDeltaWeights.resize(m_ir.nNodesPerLayer()[m_ir.nNodesPerLayer().size() - 1],0.0);
        m_outputs[outIndex].delta = 0.0;
        for(unsigned wIndex = 0; wIndex < m_outputs[outIndex].weights.size(); ++wIndex)
        {
            m_outputs[outIndex].weights[wIndex] = static_cast<double>(std::rand()) / RAND_MAX * (m_ir.weightRange().second - m_ir.weightRange().first) + m_ir.weightRange().first;
	}
        m_outputs[outIndex].threshold  = (m_ir.thresholdsRange().second == m_ir.thresholdsRange().first) ? m_ir.thresholdsRange().first 
                                                                          : static_cast<double>(std::rand()) / RAND_MAX * (m_ir.thresholdsRange().second - m_ir.thresholdsRange().first) + m_ir.thresholdsRange().first;
	m_outputs[outIndex].deltaThreshold = 0;
	m_outputs[outIndex].oldDeltaThreshold = 0;
    }
}

void BPNeuralNetwork::initialiseHiddenFunction()
{
    if(m_ir.hiddenFunction() == "transfer")
    {
        m_hFunction = new TransferActivation();
    }
    else if(m_ir.hiddenFunction() == "logistic")
    {
        m_hFunction = new Logistic(m_ir.betaHidden());
    }
    else if(m_ir.hiddenFunction() == "tanh")
    {
        m_hFunction = new TanhFunction(m_ir.betaHidden());
    }
}

void BPNeuralNetwork::initialiseOutFunction()
{
    if(m_ir.outFunction() == "transfer")
    {
        m_oFunction = new TransferActivation();
    }
    else if(m_ir.outFunction() == "logistic")
    {
        m_oFunction = new Logistic(m_ir.betaOut());
    }
    else if(m_ir.outFunction() == "tanh")
    {
        m_oFunction = new TanhFunction(m_ir.betaOut());
    }
}

void BPNeuralNetwork::propagate(unsigned nPattern)
{
    //Propagation computes for every node the sum of the weights multiplied by
    //the relative input, and uses the activation function on the result.
    //For hidden layers, the helper function computeOutput() is used for 
    //keeping the code cleaner.
    for(unsigned layer = 0; layer < m_net.size(); ++layer)
    {
        for(unsigned neuroIndex = 0; neuroIndex < m_net[layer].size(); ++neuroIndex)
        {
            computeOutput(layer, neuroIndex, nPattern);
        }
    }
    for(unsigned outIndex = 0; outIndex < m_outputs.size(); ++outIndex)
    {
        double sum = 0;
        for(unsigned wIndex = 0; wIndex < m_outputs[outIndex].weights.size(); ++wIndex)
        {
            sum += m_net[m_net.size() - 1][wIndex].output * m_outputs[outIndex].weights[wIndex];
        }
        sum += m_outputs[outIndex].threshold;
        m_outputs[outIndex].output = m_oFunction->equation(sum);
        m_outputs[outIndex].derOutput = m_oFunction->firstDerivative(sum);
    }
}

void BPNeuralNetwork::backPropagate(unsigned nPattern)
{
    //Backpropagate computes the deltas according to the standard multi layer perceptron model,
    //and the deltaWeights, which will be used during the update phase to compute the new weights.
    //This is done to be able to use this function in both batch and online mode.
    for(unsigned outIndex = 0; outIndex < m_outputs.size(); ++outIndex)
    {
        m_outputs[outIndex].delta = m_outputs[outIndex].derOutput * (m_pm.getOutput(nPattern)[outIndex] - m_outputs[outIndex].output);
        for(unsigned wIndex = 0; wIndex < m_outputs[outIndex].deltaWeights.size(); ++wIndex)
        {
            m_outputs[outIndex].deltaWeights[wIndex] += m_ir.learningRate() * m_outputs[outIndex].delta * m_net[m_net.size() - 1][wIndex].output; 
        }
        m_outputs[outIndex].deltaThreshold += m_ir.learningRate() * m_outputs[outIndex].delta;
    }   
    for(int layer = m_net.size() - 1; layer >= 0; --layer)
    {
        for(unsigned neuroIndex = 0; neuroIndex < m_net[layer].size(); ++neuroIndex)
        {
            if(layer == m_net.size() - 1)
            {
                m_net[layer][neuroIndex].delta = 0.0;
                for(unsigned outIndex = 0; outIndex < m_outputs.size(); ++outIndex)
                {
                    m_net[layer][neuroIndex].delta += m_outputs[outIndex].weights[neuroIndex] * m_outputs[outIndex].delta;
                }
            }
            else
            {
                m_net[layer][neuroIndex].delta = 0.0;
                for(unsigned nextIndex = 0; nextIndex < m_net[layer + 1].size(); ++nextIndex)
                {
                    m_net[layer][neuroIndex].delta += m_net[layer+1][nextIndex].weights[neuroIndex] * m_net[layer + 1][nextIndex].delta;
                }
            }
            m_net[layer][neuroIndex].delta *= m_net[layer][neuroIndex].derOutput;
            for(unsigned wIndex = 0; wIndex < m_net[layer][neuroIndex].deltaWeights.size(); ++wIndex)
            {
                if(layer != 0)
                {
                    m_net[layer][neuroIndex].deltaWeights[wIndex] += m_ir.learningRate() * m_net[layer][neuroIndex].delta * m_net[layer-1][wIndex].output;
                }
                else
                {
                    m_net[layer][neuroIndex].deltaWeights[wIndex] += m_ir.learningRate() * m_net[layer][neuroIndex].delta * m_pm.getInputPattern(nPattern)[wIndex];
                }
            }
            m_net[layer][neuroIndex].deltaThreshold += m_ir.learningRate() * m_net[layer][neuroIndex].delta;
        }
    }   
}

void BPNeuralNetwork::update()
{
    //Update simply sums the deltaweights to the weights, adding a momentum term.
    for(unsigned layer = 0; layer < m_net.size(); ++layer)
    {
        for(unsigned neuroIndex = 0; neuroIndex < m_net[layer].size(); ++neuroIndex)
        {
            for(unsigned wIndex = 0; wIndex < m_net[layer][neuroIndex].weights.size(); ++wIndex)
            {
                m_net[layer][neuroIndex].weights[wIndex] = m_net[layer][neuroIndex].weights[wIndex] + m_net[layer][neuroIndex].deltaWeights[wIndex] + m_ir.momentum() * m_net[layer][neuroIndex].oldDeltaWeights[wIndex];
                m_net[layer][neuroIndex].oldDeltaWeights[wIndex] = m_net[layer][neuroIndex].deltaWeights[wIndex];
                m_net[layer][neuroIndex].deltaWeights[wIndex] = 0.0;
            }
            m_net[layer][neuroIndex].threshold = m_net[layer][neuroIndex].threshold + m_net[layer][neuroIndex].deltaThreshold + m_ir.momentum()* m_net[layer][neuroIndex].oldDeltaThreshold;
            m_net[layer][neuroIndex].oldDeltaThreshold = m_net[layer][neuroIndex].deltaThreshold;
            m_net[layer][neuroIndex].deltaThreshold = 0.0;
        }
    }
    for(unsigned outIndex = 0; outIndex < m_outputs.size(); ++outIndex)
    {
        for(unsigned wIndex = 0; wIndex < m_outputs[outIndex].weights.size(); ++wIndex)
        {
            m_outputs[outIndex].weights[wIndex] = m_outputs[outIndex].weights[wIndex] + m_outputs[outIndex].deltaWeights[wIndex] + m_ir.momentum() * m_outputs[outIndex].oldDeltaWeights[wIndex];
            m_outputs[outIndex].oldDeltaWeights[wIndex] = m_outputs[outIndex].deltaWeights[wIndex];
            m_outputs[outIndex].deltaWeights[wIndex] = 0.0;
        }
	m_outputs[outIndex].threshold = m_outputs[outIndex].threshold + m_outputs[outIndex].deltaThreshold + m_ir.momentum() * m_outputs[outIndex].oldDeltaThreshold;
	m_outputs[outIndex].oldDeltaThreshold = m_outputs[outIndex].deltaThreshold;
	m_outputs[outIndex].deltaThreshold = 0.0;
    }
}

void BPNeuralNetwork::computeOutput(unsigned layer, unsigned neuroIndex, unsigned nPattern)
{
    double sum = 0;
    if(layer == 0)
    {
        for(unsigned wIndex = 0; wIndex < m_net[layer][neuroIndex].weights.size(); ++wIndex)
        {
            sum  += m_pm.getInputPattern(nPattern)[wIndex] * m_net[layer][neuroIndex].weights[wIndex];
        }
     
        sum += m_net[layer][neuroIndex].threshold;
        m_net[layer][neuroIndex].output = m_hFunction->equation(sum);
        m_net[layer][neuroIndex].derOutput = m_hFunction->firstDerivative(sum);
    }
    else
    {
        for(unsigned wIndex = 0; wIndex < m_net[layer][neuroIndex].weights.size(); ++wIndex)
        {
            sum  += m_net[layer-1][wIndex].output * m_net[layer][neuroIndex].weights[wIndex];
        }
        sum += m_net[layer][neuroIndex].threshold;
        m_net[layer][neuroIndex].output = m_hFunction->equation(sum);
        m_net[layer][neuroIndex].derOutput = m_hFunction->firstDerivative(sum);
    }
}

void BPNeuralNetwork::printHeaderToFile()
{
    std::ofstream file(m_ir.outFileName().c_str());
    if(!file.is_open())
    {
        std::cerr << "Unable to open " << m_ir.outFileName() << std::endl;
        exit(EXIT_FAILURE);
    }
    file << m_ir;
    file << "Data Statistics: Min Max Mean StdDev" << std::endl;
    for(unsigned nData = 0; nData < m_pm.inMins().size(); ++nData)
    {
        file << "Input: " << m_pm.inMins()[nData] << "    " << m_pm.inMaxs()[nData] << "    " << m_pm.inMeans()[nData] << "    " << m_pm.inStdDevs()[nData] << std::endl;
    }
    for(unsigned nData = 0; nData < m_pm.outMins().size(); ++nData)
    {
        file << "Output: " << m_pm.outMins()[nData] << "    " << m_pm.outMaxs()[nData] << "    " << m_pm.outMeans()[nData] << "    " << m_pm.outStdDevs()[nData] << std::endl;
    }
    file.close();
}

void BPNeuralNetwork::printWeightsToFile(unsigned excluded)
{
    std::ofstream file(m_ir.outFileName().c_str(),std::ofstream::app);
    if(!file.is_open())
    {
        std::cerr << "Unable to open " << m_ir.outFileName() << std::endl;
        exit(EXIT_FAILURE);
    }
    file << "The following are the weights obtained excluding every " << excluded << " +n" << m_ir.k() << std::endl; 
    for(unsigned layer = 0; layer < m_net.size(); ++layer)
    {
        file << "Layer " << layer << std::endl;
        for(unsigned neuroIndex = 0; neuroIndex < m_net[layer].size(); ++neuroIndex)
        {
            file << "Node " << neuroIndex << ": ";
            for(unsigned wIndex = 0; wIndex  < m_net[layer][neuroIndex].weights.size(); ++wIndex)
            {
                file << m_net[layer][neuroIndex].weights[wIndex]  << " ";
            }
            file << std::endl;
        }
    }
    file << "Output layer weights ";
    for(unsigned outIndex = 0; outIndex < m_outputs.size(); ++outIndex)
    {
        file << "Node " << outIndex << ": ";
        for(unsigned wIndex = 0; wIndex < m_outputs[outIndex].weights.size(); ++wIndex)
        {
            file << m_outputs[outIndex].weights[outIndex] << " ";
        }
        file << std::endl;
    }        
    file.close();
}

void BPNeuralNetwork::printCrossValidationResults(unsigned included, const std::vector<std::vector<double> >& results, const std::vector<std::vector<double> >& expectedResults)
{
    std::ofstream file(m_ir.outFileName().c_str(),std::ofstream::app);
    if(!file.is_open())
    {
        std::cerr << "Unable to open " << m_ir.outFileName() << std::endl;
        exit(EXIT_FAILURE);
    }
    file << "Results of cross validation using every " << included << " +n" << m_ir.k() << " pattern" << std::endl;
    std::vector<int> nWrongClass(results[0].size(),0);
    for(unsigned value = 0; value < results.size(); ++value)
    {
        for(unsigned nEntry = 0; nEntry < results[value].size(); ++nEntry)
        {
	  if(round(results[value][nEntry]) != expectedResults[value][nEntry])
	    {
	      nWrongClass[nEntry] += 1;
	    }
        }
    }
    file << "The error on these data is: ";
    for(unsigned nEntry = 0; nEntry < nWrongClass.size(); ++nEntry)
    {
      file << 100 * nWrongClass[nEntry]/results.size() << " ";
    }
    file << std::endl;
    file.close();
}

void BPNeuralNetwork::printTestResults(const std::vector<std::vector<double> >& results, const std::vector<std::vector<double> >& expectedResults)
{
  std::string resultsFileName = "Results.txt";
  std::ofstream resultsFile(resultsFileName.c_str());
    //This function prints the results to the output file.
    std::ofstream file(m_ir.outFileName().c_str(),std::ofstream::app);
    if(!file.is_open())
    {
        std::cerr << "Unable to open " << m_ir.outFileName() << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string fileName = m_ir.outFileName().substr(0,m_ir.outFileName().length()-4);
    file << "Results of tests:" << std::endl;
    std::vector<int> nWrongClass(results[0].size(),0);
    for(unsigned value = 0; value < results.size(); ++value)
    {
        for(unsigned nEntry = 0; nEntry < results[value].size(); ++nEntry)
        {
	  double binaryOutput = (results[value][nEntry] > 0.5 ) ? 1 : 0;
	  if(binaryOutput != expectedResults[value][nEntry])
	    {
    	      nWrongClass[nEntry] += 1;
	    }
	  resultsFile << results[value][nEntry] << "  " << expectedResults[value][nEntry] << std::endl;
        }
    }
    file << "The error on these data is: ";
    for(unsigned nEntry = 0; nEntry < nWrongClass.size(); ++nEntry)
    {
      file << 100 * nWrongClass[nEntry]/results.size() << " ";
    }
    file << std::endl;
    file.close();
    resultsFile.close();
}
