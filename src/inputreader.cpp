#include "../include/inputreader.h"

#include "../include/utility.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

InputReader::InputReader()
{
    readFile();
}

void InputReader::readFile()
{
    std::ifstream file("Input.txt");
    if(!file.is_open())
    {
        std::cerr << "Could not open Input.txt" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string line, commentLine;
    std::stringstream lineStream;
    //Reads header of the file. Discarded line.
    getline(file,line);
    
    //From now on, I read a comment line, and the relative parameter line.
    //The comment line is used for input failure feedback by errorcheck.
    
    //Pair of lines relative to the data input file.
    getline(file,commentLine);
    getline(file,line);
    m_fileName = line;
    //Happens only on Windows.
    if(m_fileName[m_fileName.length()-1] == '\r')
    {
        m_fileName=m_fileName.substr(0,m_fileName.length()-1);
    }
    //Pair of lines relative to the number of entries per pattern.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_inColumns;
    errorcheck(lineStream, commentLine);

    //Pair of lines relaitve to the number of entries per output.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_outColumns;
    errorcheck(lineStream, commentLine);
    
    //Pair of lines relative to the number of requested hidden layers.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_nHiddenLayers;
    errorcheck(lineStream, commentLine);
    
    //Pair of line relative to the requestes nodes per layer.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    m_nNodesPerLayer.resize(m_nHiddenLayers);
    for(unsigned i = 0; i < m_nHiddenLayers; ++i)
    {
       lineStream >> m_nNodesPerLayer[i];
    }
    errorcheck(lineStream, commentLine);
    
    //Pair of lines defining scaling type.
    getline(file,commentLine);
    getline(file,line);
    if(line[line.length() - 1] ==  '\r')
    {
        line = line.substr(0,line.length() - 1);
    }
    m_scalingType = line;
    Utility::tolower(m_scalingType);

    //Pair of lines relative to weights' range.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_weightRange.first >> m_weightRange.second;
    errorcheck(lineStream, commentLine);
    
    //Pair of lines relative to thresholds' range.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_thresholdsRange.first >> m_thresholdsRange.second;
    errorcheck(lineStream, commentLine);
    
    //Pair of lines relative to number of trainig epochs.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_nEpochs;
    errorcheck(lineStream, commentLine);
    
    //Pair of lines relative to the  learning rate.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_learningRate;
    errorcheck(lineStream, commentLine);

    //Pair of lines relative to momentum.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_momentum;
    errorcheck(lineStream, commentLine);
    
    //Pair of lines relative to learning mode.
    getline(file,commentLine);
    getline(file,line);
    Utility::tolower(line);
    if(line[line.length()-1] == '\r')
    {
        line = line.substr(0,line.length()-1);
    }
    if(line == "batch")
    {
        m_mode = BATCH;
    }
    else if(line == "online")
    {
        m_mode = ONLINE;
    }
    else
    {
        std::cerr << "Problem in line " << commentLine.substr(1) << std::endl;
    }   
        
    //Pair of lines relative to number of test patterns.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_nTestPatterns;
    errorcheck(lineStream, commentLine);
    
    //Pair of lines relative to cross-validation fraction.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_k;
    errorcheck(lineStream, commentLine);
    
    //Pair of lines for hidden layer activation function.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_hiddenFunction >> m_betaHidden;
    errorcheck(lineStream,commentLine);
    
    //Pair of lines for output layer activation function.
    getline(file,commentLine);
    getline(file,line);
    lineStream.str(line);
    lineStream >> m_outFunction >> m_betaOut;
    errorcheck(lineStream,commentLine);
    
    //Pair of lines relative to cost function.
    getline(file,commentLine);
    getline(file,line);
    Utility::tolower(line);
    if(line[line.length()-1] == '\r')
    {
        line = line.substr(0,line.length()-1);
    }
    m_costFunction = line;
    
    //Pair of lines relative to output file name.
    getline(file,commentLine);
    getline(file,line);
    Utility::tolower(line);    
    if(line[line.length()-1] == '\r')
    {
        line = line.substr(0,line.length()-1);
    }
    m_outFileName = line;
    file.close();
}

//Consistency check for input streams.
void InputReader::errorcheck(std::iostream& inStream, const std::string& comment)
{
    if(!inStream)
    {
        std::cerr << "Problem in line " << comment.substr(1) << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::ostream& operator<<(std::ostream& os, const InputReader& ir)
{
    os << "Reading file: " << ir.fileName() << std::endl;
    os << "Architecture of the Neural Network: " << ir.inColumns() << ":";
    for(unsigned layer = 0; layer < ir.nHiddenLayers(); ++layer)
    {
        os << ir.nNodesPerLayer()[layer] << ":";
    }
    os << ir.outColumns() << std::endl;
    os << "Type of data scaling: " << ir.scalingType() << std::endl;
    os << "Range used for weights: [" << ir.weightRange().first << ", " << ir.weightRange().second << "]" << std::endl;
    os << "Range used for thresholds: [" << ir.thresholdsRange().first << ", " << ir.thresholdsRange().second << "]" << std::endl;
    os << "Number of Epochs used: " << ir.nEpochs() << std::endl;
    os << "Learning Rate: " << ir.learningRate() << std::endl;
    os << "Momentum " << ir.momentum() << std::endl;
    os << "Training mode (BATCH = 0, ONLINE = 1): " << ir.mode() << std::endl;
    os << "Using " << ir.nTestPatterns() << " for tests" << std::endl;
    os << "Using k-fold cross-validation with k = " << ir.k() << std::endl;
    os << "Using " << ir.hiddenFunction() << " activation function for the hidden layers with b = " << ir.betaHidden();
    os << " and " << ir.outFunction() << " for the output layer with b = " << ir.betaOut() << std::endl;
    os << "Using " << ir.costFunction() << " as cost function" << std::endl;
    return os;
}
