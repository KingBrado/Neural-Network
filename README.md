<h1>Neural Network Example</h1>
<p>An example of neural network implemented in C++. It is a very simple software, yet implements all the basic concepts relevant to classical feed-forward neural networks with back propagation, such as </p>
<ul>
  <li>Training and testing part</li>
  <li>Cross-validation</li>
  <li>Learning Rate and Momentum</li>
  <li>Different types of input data normalisation</li>
  <li>Different types of transfer functions</li>
</ul>

<p>For compilation, a C++ compiler (e.g. <a href="https://www.gnu.org/software/gcc">g++</a>) is required, as well as <a href="https://cmake.org">CMake</a> version 2.8 or later</p>

<p>After downloading or cloning, <code>cd</code> to the folder and run</p>
<pre>
<code>
    mkdir build 
    cd build 
    cmake .. 
    make 
</code>
</pre>

<p>This will generate the executable <code>Neural-Network</code> which can be run from the command line.</p>

<p>The execution requires a file named <code>Input.txt</code>, which has a well defined format (see example in the <code>data</code> folder), requiring (in the same order):
  <ol>
    <li>Name of the data file, formatted with <code>m</code> columns corresponding to the input values, and <code>n</code> columns for the desired output</li>
    <li>Number of columns <code>m</code> </li>
    <li>Number of columns <code>n</code> </li>
    <li>Number of hidden layers (number of input layers is <code>m</code>, while output is <code>n</code>)</li>
    <li>Number of neurons in the hidden layers, provided as a set of spaced numbers, one for each of the hidden layers</li>
    <li>Type of data normalisation
      <ul>
        <li>Normal, for normalisation based on the maximum and minimum values in the input data set</li>
        <li>Mean, for normalisation based on average and standard deviation</li>
        <li>None, for no normalisation</li>
      </ul>
    </li>
    <li>
      Initial range of weights
    </li>
    <li>
      Initial range of thresholds
    </li>
    <li>Number of training epochs</li>
    <li>Learning Rate</li>
    <li>Momentum</li>
    <li>Learning mode, either batch (for weight updating based on several, user defined samples) or online (for update at every forward step)</li>
    <li>Number of patterns reserved for testing, starting from the end of the file.</li>
    <li><code>k</code> of cross-validation, where <code>1/k</code> is the fraction of training patterns taken away for cross-validation</li>
    <li>Type of activation function used in hidden layers
      <ul>
        <li>Transfer (<code>g(x) = x</code>)</li>
        <li>Logisitic (<code>g(x) =  1 / (1 - exp(-2bx))</code>)</li>
        <li>Tanh (<code>g(x) = tanh(bx)</code>)</li>
      </ul>
      For the latter two, the parameter <code>b</code> has to be provided
    </li>
    <li>Type of cost function (only <code>energy</code> implemented thus far)</li>
    <li>Name of the output file</li>
  </ol>
  </p>
