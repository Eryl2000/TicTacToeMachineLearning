#ifndef PLAYER_H
#define PLAYER_H

#include <Eigen/Dense>
#include <vector>
using namespace Eigen;
#include "NeuralNet.h"

class Player {
 public:
  Player();
  Player(const Player &other);
  virtual ~Player();

  void operator=(const Player &right);
  bool operator<(const Player &right) const;

  static unsigned int count;
  unsigned int index;
  double fitness;

  virtual RowVectorXd getMove(const RowVectorXd &input) const = 0;

  static bool ComparePlayer(const Player *left, const Player *right);
  static void Swap(Player *left, Player *right);
};

// A player with a neural network brain
class NeuralPlayer : public Player {
 public:
  NeuralPlayer();
  NeuralPlayer(const std::vector<unsigned int> &layerSizes);
  NeuralPlayer(const NeuralPlayer &other);
  virtual ~NeuralPlayer();

  void operator=(const NeuralPlayer &right);
  NeuralNet neural;

  virtual RowVectorXd getMove(const RowVectorXd &input) const override;
};

// A player with a manual input brain
class ManualPlayer : public Player {
 public:
  ManualPlayer(std::istream &is, std::ostream &os, const int numActions);
  ManualPlayer(const ManualPlayer &other);
  virtual ~ManualPlayer();

  void operator=(const ManualPlayer &right);

  virtual RowVectorXd getMove(const RowVectorXd &input) const override;

 private:
  std::istream &m_is;
  std::ostream &m_os;
  const int m_numActions;
};

// A player with a random input brain
class RandomPlayer : public Player {
 public:
  RandomPlayer(const int _size);
  RandomPlayer(const RandomPlayer &other);
  virtual ~RandomPlayer();

  void operator=(const RandomPlayer &right);

  virtual RowVectorXd getMove(const RowVectorXd &input) const override;

 private:
  const int size;
};

// A player with a theoretically perfect brain
class PerfectPlayer : public Player {
 public:
  PerfectPlayer();
  PerfectPlayer(const PerfectPlayer &other);
  virtual ~PerfectPlayer();

  void operator=(const PerfectPlayer &right);

  virtual RowVectorXd getMove(const RowVectorXd &input) const override;
};

#endif
