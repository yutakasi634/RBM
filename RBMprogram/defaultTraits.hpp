#ifndef DEFAULTTRAITS
#define DEFAULTTRAITS

struct BBRBMTypeTraits{
  using potentialType = int;
  using connectionType = double;
  using biasType = double;
};

struct GBRBMTypeTraits{
  using potentialType = double;
  using connectionType = double;
  using biasType = double;
};
#endif
