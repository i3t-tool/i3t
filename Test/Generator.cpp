#include "Generator.h"

std::default_random_engine& randomEngine()
{
  static std::default_random_engine u{};
  return u;
}
