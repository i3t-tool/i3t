#include "ViewportHighlightResolver.h"

#include <list>
#include <memory>
#include <unordered_set>

#include "Core/Nodes/Node.h"

#include "GUI/Elements/Nodes/WorkspaceCamera.h"
#include "GUI/Elements/Nodes/WorkspaceSequence.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"

#include "Viewport/Viewport.h"
#include "Viewport/entity/nodes/SceneModel.h"

ViewportHighlightResolver::ViewportHighlightResolver(WorkspaceDiwne* diwne)
{
	m_diwne = diwne;
}

void ViewportHighlightResolver::resolve()
{
	if (!m_runResolveThisFrame)
	{
		return;
	}

	// For each model, run bfs until a selected node is found.
	std::vector<Ptr<WorkspaceModel>> models = m_diwne->getAllModels();
	for (auto& model : models)
	{
		if (searchForSelectedNode(model))
		{
			model->m_influenceHighlight = true;
			auto viewportModel = model->m_viewportModel.lock();
			if (!model->m_selected)
			{
				viewportModel->m_highlight = true;
				viewportModel->m_highlightColor =
				    App::get().viewport()->getSettings().global().highlight_highlightColor;
			}
		}
		else
		{
			model->m_influenceHighlight = false;
			auto viewportModel = model->m_viewportModel.lock();
			if (!model->m_selected)
			{
				viewportModel->m_highlight = false;
			}
		}
	}

	m_runResolveThisFrame = false;
}

bool ViewportHighlightResolver::searchForSelectedNode(Ptr<WorkspaceModel>& rootGuiModel)
{
	if (rootGuiModel->getSelected())
	{
		// Model itself is selected so it should be highlighted anyway
		return true;
	}

	// Use BFS starting from the model node to look for any selected node.

	std::unordered_set<Core::ID> visited;
	std::list<Ptr<Core::Node>> queue;

	Ptr<Core::Node> rootModel = rootGuiModel->getNodebase();
	visited.insert(rootModel->getId());
	queue.push_back(rootModel);

	while (!queue.empty())
	{
		Ptr<Core::Node> node = queue.front();
		queue.pop_front();

		for (const auto& inputPin : node->getInputPins())
		{
			if (inputPin.isPluggedIn())
			{
				Ptr<Core::Node> neighbour = inputPin.getParentPin()->getOwner();
				Core::ID neighbourId = neighbour->getId();
				I3T_ASSERT(neighbour != nullptr, "Parent pin owner shouldn't be null!");
				auto it = visited.find(neighbourId);
				if (it == visited.end())
				{
					visited.insert(it, neighbourId);
					queue.push_back(neighbour);

					auto coreIdIt = m_diwne->m_coreIdMap.find(neighbourId);
					if (coreIdIt != m_diwne->m_coreIdMap.end())
					{
						WorkspaceNodeWithCoreData* guiNode = coreIdIt->second;
						I3T_ASSERT(guiNode != nullptr, "Core ID map gui node reference shouldn't be null!");
						if (guiNode->getSelected())
						{
							// Selected gui node found, end search
							return true;
						}
						else
						{
							// Check for nested selection
							// In Sequence
							if (auto guiSequence = dynamic_cast<WorkspaceSequence*>(guiNode))
							{
								if (anyInSequenceSelected(guiSequence))
									return true;
							}
							// In Camera
							if (auto guiCamera = dynamic_cast<WorkspaceCamera*>(guiNode))
							{
								if (anyInCameraSelected(guiCamera))
									return true;
							}
						}
					}
					else
					{
						throw std::runtime_error("Failed to find corresponding gui node for core node id: " +
						                         std::to_string(neighbourId) + "!");
					}
				}
			}
		}
	}
	return false;
}

void ViewportHighlightResolver::registerNodeCallbacks(Core::Node* node)
{
	node->addDeleteCallback([this](Core::Node* node) {
		resolveNeeded();
	});
	node->addPlugCallback([this](Core::Node* fromNode, Core::Node* toNode, size_t fromIndex, size_t toIndex) {
		resolveNeeded();
	});
	node->addUnplugCallback([this](Core::Node* fromNode, Core::Node* toNode, size_t fromIndex, size_t toIndex) {
		resolveNeeded();
	});
}

bool ViewportHighlightResolver::anyInSequenceSelected(WorkspaceSequence* sequence)
{
	if (sequence == nullptr)
		return false;

	if (sequence->getSelected())
		return true;

	if (sequence->getInputs().at(Core::I3T_SEQ_IN_MAT)->isConnected())
	{
		// Matrix data inputted from somewhere else
		return false;
	}
	else
	{
		for (const auto& transformation : sequence->getInnerWorkspaceNodes())
		{
			if (transformation->getSelected())
			{
				return true;
			}
		}
		return false;
	}
}

bool ViewportHighlightResolver::anyInCameraSelected(WorkspaceCamera* camera)
{
	if (camera == nullptr)
		return false;

	if (camera->getSelected())
		return true;

	return anyInSequenceSelected(camera->getProjection().get()) || anyInSequenceSelected(camera->getView().get());
}
