#include <iostream>

#include "../include/bpneuralnetwork.h"
/**
 *  @mainpage Elementary Back Propagation Neural Network Example
 *  
 *  This code can be used as a beginner's reference on how to implment
 *  a Neural Network in C++. It is not production optimised, as, for instance
 *  no GPU parallelisation is used. However, several important concepts, i.e. 
 *  cross-validation, testing, normalisation and activation functions are 
 *  implemented and the code is quite short. 
 * 
 */
int main(int argc, char *argv[])
{
    BPNeuralNetwork bpnn;
    for(unsigned i = 0; i < bpnn.ir().k(); ++i)
    {
        bpnn.train(i);
        bpnn.test();
        bpnn.crossvalidate(i);
    }
    
    return 0;
}
