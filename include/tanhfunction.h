#ifndef TANH_FUNCTION_H
#define TANH_FUNCTION_H

#include <cmath>
#include "activationfunction.h"
/**
 * @file tanhfunction.h
 * @brief Contains class @ref TanhFunction.
 * 
 * @author B. M. Manzi
 * @date 20/11/2017
 */
 /**
 * @brief Specialisation of @ref ActivationFunction for tanh function.
 *
 * This class defines an activation function of the type: 
 * @f[ f(x) = \tanh(\beta x) @f]
 */
class TanhFunction : public ActivationFunction
{
public:
    /**
    * @brief Constructor of the tanh activation function.
    *
    * @param beta Parameter of the function.
    */
    TanhFunction(double beta)
    : ActivationFunction()
    , m_beta(beta){}
    /**
    * @brief Computes the function @f[ f(x) = \tanh(\beta x) @f] .
    * 
    * @param x Point where to compute the activation function.
    * @return Value of the tanh function at \p x.
    */
    double equation(double x) const {return tanh(m_beta*x);}
    /**
    * @brief Computes the derivative @f[ f'(x) = \beta (1-(f(x))^2) @f] .
    * 
    * @param x Point where to compute the derivative of the activation function.
    * @return Value of the derivative of the tanh function at \p x.
    */
    double firstDerivative(double x) const {return m_beta * (1.0 - equation(x) * equation(x));}
private:
    /**
    * @brief Parameter of tanh function.
    */
    double m_beta;
};

#endif // TANH_FUNCTION_H