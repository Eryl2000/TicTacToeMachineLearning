
#include "NeuralNet.h"

NeuralNet::NeuralNet() {}

// Constructor takes in the structure of the network as a matrix
NeuralNet::NeuralNet(const std::vector<unsigned int> &layerSizes)
    : m_layerSizes(layerSizes) {
  unsigned int numLayers = layerSizes.size() - 1;
  m_weights.reserve(numLayers);

  // Create vectors for weights. Each entry is a matrix for that layer
  for (unsigned int i = 0; i < numLayers; ++i) {
    m_weights.push_back(MatrixXd::Random(layerSizes[i] + 1, layerSizes[i + 1]));
  }
}

NeuralNet::NeuralNet(const NeuralNet &nn)
    : m_layerSizes(nn.m_layerSizes), m_weights(nn.m_weights) {}

void NeuralNet::operator=(const NeuralNet &nn) {
  m_layerSizes = nn.m_layerSizes;
  m_weights = nn.m_weights;
}

// Prints the current weights to the console
void NeuralNet::printWeights() const {
  std::cout << "Current weights:" << std::endl;
  for (unsigned int i = 0; i < m_weights.size(); ++i) {
    std::cout << "================================================"
              << std::endl;
    std::cout << m_weights[i] << std::endl;
  }
  std::cout << "================================================" << std::endl;
}

bool NeuralNet::saveToFile(std::string fileName) const {
  std::ofstream outputFile;
  outputFile.open(fileName.c_str());
  if (!outputFile.is_open()) {
    return false;
  }

  // Outuput number of layer
  outputFile << m_layerSizes.size() << "\n";

  // Ouput layerSizes
  for (unsigned int i = 0; i < m_layerSizes.size(); ++i) {
    outputFile << m_layerSizes[i] << " ";
  }
  outputFile << "\n";

  for (unsigned int lay = 0; lay < m_weights.size(); ++lay) {
    unsigned int rows = m_weights[lay].rows();
    unsigned int cols = m_weights[lay].cols();

    for (unsigned int col = 0; col < cols; ++col) {
      for (unsigned int row = 0; row < rows; ++row) {
        outputFile << m_weights[lay](row, col) << " ";
      }
    }
  }

  outputFile << "D\n";
  outputFile.close();
  return true;
}

bool NeuralNet::loadFromFile(std::string fileName) {
  std::ifstream inputFile;
  inputFile.open(fileName.c_str());
  if (!inputFile.is_open()) {
    return false;
  }

  unsigned int numLayers;
  inputFile >> numLayers;

  m_layerSizes.clear();
  for (unsigned int i = 0; i < numLayers; ++i) {
    unsigned int cur;
    inputFile >> cur;
    m_layerSizes.push_back(cur);
  }

  for (unsigned int lay = 0; lay < numLayers - 1; ++lay) {
    unsigned int rows = m_layerSizes[lay] + 1;
    unsigned int cols = m_layerSizes[lay + 1];
    MatrixXd cur(rows, cols);
    for (unsigned int col = 0; col < cols; ++col) {
      for (unsigned int row = 0; row < rows; ++row) {
        double temp;
        inputFile >> temp;
        cur(row, col) = temp;
      }
    }
    m_weights.push_back(cur);
  }

  char check;
  inputFile >> check;
  if (check != 'D') {
    std::cerr << "Error: Check at the end of input file was incorrect"
              << std::endl;
    return false;
  }

  inputFile.close();
  return true;
}

// Performs forward propagation using m_weights and 'input'
RowVectorXd NeuralNet::forward(const RowVectorXd &input) const {
  unsigned int numLayers = m_weights.size();

  // Stores the previous layer's output
  std::vector<RowVectorXd> layers;
  layers.reserve(numLayers + 1);
  layers.push_back(input);

  for (unsigned int lay = 0; lay < numLayers; ++lay) {
    unsigned int numCols = layers[lay].cols();
    layers[lay].conservativeResize(NoChange, numCols + 1);
    layers[lay](numCols) = 1.0;

    // Cur = f(layers * weights + bias)...where f(x) is nonlinearity funtion
    layers.push_back(
        applyNonlinearity(layers[lay] * m_weights[lay], Activations::sigmoid));
  }
  return layers[numLayers];
}

std::vector<MatrixXd> &NeuralNet::getWeights() { return m_weights; }

// Sets the internal weights
void NeuralNet::setWeights(const std::vector<MatrixXd> &weights) {
  if (weights.size() == 0 || weights.size() != m_weights.size()) {
    std::cerr << "Error: setWeights(): Weights have different sizes."
              << std::endl;
    exit(1);
  }
  for (unsigned int i = 0; i < m_weights.size(); ++i) {
    m_weights[i] = weights[i];
  }
}

inline RowVectorXd NeuralNet::applyNonlinearity(const RowVectorXd &input,
                                                Activations activation) const {
  double max, sum;
  switch (activation) {
    case Activations::sigmoid:  // 1.0/(1 + e^-x)
      return input.unaryExpr(&NeuralNet::sigmoid);
    case Activations::relu:  // max(0, x)
      return input.unaryExpr(&NeuralNet::relu);
    case Activations::softmax:
      max = input.maxCoeff();
      sum = (input.array() - max).exp().sum();
      return (input.array() - max - log(sum)).exp().matrix();
    default:
      return input;
  }
}

inline double NeuralNet::relu(double x) { return std::max(0.0, x); }

inline double NeuralNet::sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }
