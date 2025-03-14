/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"

namespace Workspace
{
class TransformationBase : public CoreNode
{
public:
	WPtr<Sequence> m_parentSequence{};

	TransformationBase(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase);

	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<TransformationBase>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	void begin(DIWNE::DrawInfo& context) override;
	void topContent(DIWNE::DrawInfo& context) override;
	void centerContent(DIWNE::DrawInfo& context) override;
	void end(DIWNE::DrawInfo& context) override;

	virtual bool allowDrawing() override;

	void popupContent(DIWNE::DrawInfo& context) override;
	virtual void drawMenuLevelOfDetail() override = 0;

	std::vector<ImVec2> getInteractionPointsWithSequence();

	void onDestroy(bool logEvent) override;

	bool isInSequence();
	DIWNE::ID aboveSequence;

	Ptr<Core::Node> getNodebaseSequence();

	/**
	 * \brief helper function used for decision about showing the corrupted
	 * transform flag in topContent(). Overriden in WorkspaceTransformation_s.h
	 *
	 * \return Core::ETransformState:: Valid, Invalid, or Unknown
	 */
	virtual bool isMatrixValid() = 0; ///{return true;}; // todo = 0

	virtual void drawMenuSetDataMap();

	virtual bool drawDataFull();
	virtual bool inline drawDataSetValues()
	{
		return drawDataFull();
	};

	virtual int maxLengthOfData() override;

	void drawMenuStorevalues();

	virtual bool drawDataSetValues_InsideTablebuilder(std::vector<std::string> const& labels,
	                                                  std::vector<float*> const& local_data, bool& value_changed);
	virtual bool drawDataSetValuesTable_builder(std::string const cornerLabel,
	                                            std::vector<std::string> const& columnLabels,
	                                            std::vector<std::string> const& rowLabels,
	                                            std::vector<float*> const& local_data, bool& value_changed,
	                                            int& index_of_change);
};
} // namespace Workspace