#include "yaml-cpp/yaml.h"

template <typename Node, typename T>
void getValue(Node&& node, T& t)
{
	static_assert(std::is_same_v<Node, YAML::Node>);

	if (node)
	{
		t = node.as<T>();
	}
}
