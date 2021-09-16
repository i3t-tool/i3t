#include "DumpVisitor.h"

std::string DumpVisitor::dump(const Core::NodePtr& node)
{
	node->accept(*this);  // calls visit(node) and populates m_result variable.

	return m_result;
}

void DumpVisitor::visit(const Core::NodePtr& node)
{
	m_result = node->getSig();
}
