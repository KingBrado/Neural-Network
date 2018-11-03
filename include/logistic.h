#ifndef LOGISTIC_H
#define LOGISTIC_H

#include <cmath>
#include "activationfunction.h"
/**
 * @file logistic.h
 * @brief Contains class @ref Logistic.
 * 
 * @author B. M. Manzi
 * @date 20/11/2017
 */
 /**
 * @brief Specialisation of @ref ActivationFunction for logistic function.
 *
 * This class defines an activation function of the type: 
 * @f[ f(x) = \frac{1}{1+\exp(-2\beta x)} @f]
 */
class Logistic : public ActivationFunction
{
public:
    /**
    * @brief Constructor of the logistic activation function.
    *
    * @param beta Parameter of the function.
    */    
    Logistic(double beta) 
    : ActivationFunction()
    , m_beta(beta){}
    /**
    * @brief Computes the function @f[ f(x) = \frac{1}{1+\exp(-2\beta x)} @f] .
    * 
    * @param x Point where to compute the activation function.
    * @return Value of the logistic function at \p x.
    */
    double equation(double x) const {return 1.0 / (1 + exp(-2.0 * m_beta * x));}
    /**
    * @brief Computes the derivative @f[ f'(x) = 2\beta f(x) (1-f(x)) @f] .
    * 
    * @param x Point where to compute the derivative of the activation function.
    * @return Value of the derivative of the logistic function at \p x.
    */
    double firstDerivative(double x) const {return 2.0 * m_beta * equation(x) * (1.0 - equation(x));}
private:
    /**
    * @brief Parameter of logistic function.
    */
    double m_beta;
};
#endif //LOGISTIC_H