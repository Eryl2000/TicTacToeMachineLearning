#include <Eigen/Dense>
#include "Population.h"
#include "TicTacToe.h"

int main() {
  srand((unsigned int)time(NULL));

  // Where your player log files are stored
  std::string logFilePath = "data/";

  Population pop;
  pop.Init(TicTacToe::NUM_ACTIONS, std::cin, std::cout);
  double trainingTime = pop.Train<TicTacToe>(false);
  std::cout << "Time to train: " << trainingTime << " seconds" << std::endl;

  char input;
  std::cout << "Do you want to play against the best player? (y/n): ";
  std::cin >> input;
  if (input == 'y' || input == 'Y') {
    pop.PlayBest<TicTacToe>();
  }

  std::cout << "Do you want to save the best player to a file? (y/n): ";
  std::cin >> input;
  if (input == 'y' || input == 'Y') {
    std::string playerName;
    std::cout << "File name: ";
    std::cin >> playerName;
    if (pop.SaveBestPlayer(logFilePath + playerName)) {
      std::cout << "Player saved to: " << logFilePath << playerName
                << std::endl;
    } else {
      std::cout << "ERROR: Unable to save player to specified location."
                << std::endl;
    }
  }

  return 0;
}
