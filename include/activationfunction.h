#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H
/**
 * @file activationfunction.h
 * @brief Contains class @ref ActivationFunction
 * @author B. M. Manzi
 * @date 20/11/2017
 */
 /**
 * @brief Abstract class for defining activation function to use
 * in back propagation.
 *
 * This abstract class defines an abstract activation function, with
 * 2 basic methods, equation and firstDerivative, necessary for the 
 * back propagation algorithm.
 */
class ActivationFunction
{
public:
    /**
    * @brief Constructor (no members to initialise)
    */
    ActivationFunction(){}
    /**
    * @brief Destructor
    */
    virtual ~ActivationFunction(){}
    /**
    * @brief Abstract member function defining the analytical 
    * form of the activation function.
    *
    * @param x Point in which the function is computed.
    * @return The value of the function at \p x.
    */
    virtual double equation(double x) const = 0;
    /**
    * @brief Abstract member function defining the analytical 
    * form of the derivative of the activation function.
    *
    * @param x Point in which the derivative is computed.
    * @return The value of the derivative at \p x.
    */
    virtual double firstDerivative(double x) const = 0;
protected:
};
#endif //ACTIVATION_FUNCTION_H