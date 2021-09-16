#pragma once

#include <string>
#include <vector>

#include "Core/Nodes/NodeVisitor.h"

class DumpVisitor : public Core::NodeVisitor
{
public:
	std::string dump(const Core::NodePtr& node);

private:
	void visit(const Core::NodePtr& node) override;

	std::string m_result;
};
