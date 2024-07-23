#include "AutomataCelulargeneral.hpp"


int main()
{
  srand(time(NULL));
  std::cout << "Unidimensional" << std::endl;
  std::string filename1 = "init2.txt";
  FactoryCellACE110 factory2;
  Lattice1D_open lattice2(filename1.c_str(), factory2, "open", kDead);
  Execute(lattice2);

  PositionDim<1> position3(1, 10);
  FactoryCellACE30 factory3;
  std::cout << "CellACE30 periodic" << std::endl;
  Lattice1D_periodic lattice3(position3, factory3, "periodic", kDead);
  Execute(lattice3);


  std::cout << "Bidimensional" << std::endl;
  std::string filename = "init.txt";
  FactoryCellLife23_3 factory1;
  std::cout << "CellLife23_3 noborder" << std::endl;
  Lattice2D_noborder lattice1(filename.c_str(), factory1, "noborder");
  Execute(lattice1);

  PositionDim<2> position4(2, 10, 10);
  FactoryCellLife51_346 factory4;
  std::cout << "CellLife51_346 reflective" << std::endl;
  Lattice2D_reflective lattice4(position4, factory4, "reflective");
  Execute(lattice4);

  return 0;
}
