#include "Pin.h"

#include "Node.h"

namespace Core
{
static IdGenerator generator;

Pin::Pin(EValueType valueType, bool isInput, Ptr<Node> owner, int index)
    : m_valueType(valueType), m_isInput(isInput), m_master(owner.get()), m_index(index)
{
	m_id = generator.next();
}

[[nodiscard]] Ptr<Node> Pin::getOwner() const { return m_master->getPtr(); };

const DataStore& Pin::data() const
{
	if (m_isInput)
		return m_input->data();
	else
		return m_master->getData(m_index);
}

const DataStore& Pin::getStorage(unsigned int id)
{
	if (m_isInput)
	{
		// Debug::Assert(isPluggedIn(), "This input pin is not plugged to any output
		// pin!"); return m_input->m_master->getData(id); Get input data from parent
		// output pin.
		return m_input->data();
	}
	else
	{
		return m_master->getData(id);
	}
}

const char* Pin::getLabel() const
{
	auto* op = getOwner()->getOperation();
	const char* label = nullptr;

	if (m_isInput)
	{
		if (!op->defaultInputNames.empty())
		{
			label = op->defaultInputNames[m_index].c_str();
		}
	}
	else
	{
		if (!op->defaultOutputNames.empty())
		{
			label = op->defaultOutputNames[m_index].c_str();
		}
	}

	if (label == nullptr)
	{
		label = defaultIoNames[static_cast<size_t>(m_valueType)];
	}

	return label;
}

std::string Pin::getSig()
{
	return fmt::format("{} [{}, index: {}]", valueTypeToString(m_valueType), getOwner()->getSig(), m_index);
}
} // namespace Core
