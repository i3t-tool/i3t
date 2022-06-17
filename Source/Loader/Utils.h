#include "yaml-cpp/yaml.h"

template <typename T>
void getValue(YAML::Node&& node, T& t)
{
	if (node) { t = node.as<T>(); }
}
