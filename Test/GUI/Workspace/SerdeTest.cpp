/**
 * \file
 * \brief
 * \author Martin Herich
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
// #include "gtest/gtest.h"

// #include <memory>

// #include "GUI/Elements/Nodes/WorkspaceSequence.h"
// #include "GUI/Elements/Nodes/WorkspaceTransformation_s.h"
// #include "State/NodeDeserializer.h"
// #include "State/SerializationVisitor.h"

// #include "Core/Nodes/Utils.h"

/*
class SerdeTest : public GraphManagerTestFixtureSuite {};

TEST_F(SerdeTest, CorrectOrderOfTransformsInTheSequence)
{
    DIWNE::SettingsDiwne settings;
    DIWNE::Diwne diwne(&settings);

    auto sequence = std::make_shared<WorkspaceSequence>(diwne);
    auto t1 = std::make_shared<WorkspaceTransformation_s<ETransformType::EulerX>>(diwne);
    auto t2 = std::make_shared<WorkspaceTransformation_s<ETransformType::Free>>(diwne);

    sequence->pushNode(t1);
    sequence->pushNode(t2);

    Memento serialized;
    SerializationVisitor visitor(serialized);
    visitor.dump({sequence});
}
 */
