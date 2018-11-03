#ifndef TRANFERACTIVATION_H
#define TRANFERACTIVATION_H

#include "activationfunction.h"
/**
 * @file transferactivation.h
 * @brief Contains class @ref TransferActivation
 * 
 * @author B. M. Manzi
 * @date 20/11/2017
 */
 /**
 * @brief Specialisation of @ref ActivationFunction for transfer function.
 *
 * This class defines an activation function of the trivial form: 
 * @f[ f(x) = x @f]
 */
class TransferActivation : public ActivationFunction
{
public:
    /**
    * @brief Constructor of the transfer activation function.
    */  
    TransferActivation()
    : ActivationFunction(){}
    /**
    * @brief Computes the function @f[ f(x) = x @f] .
    * 
    * @param x Point where to compute the activation function.
    * @return Value of the transfer function at \p x.
    */
    double equation(double x) const {return x;}
    /**
    * @brief Computes the derivative @f[ f'(x) = 1 @f] .
    * 
    * @param x Point where to compute the derivative of the activation function.
    * @return Value of the derivative of the transfer function at \p x.
    */
    double firstDerivative(double x) const {return 1;}
};

#endif // TRANFERACTIVATION_H