#include "gtest/gtest.h"

#include <memory>

#include "GUI/Elements/Nodes/WorkspaceSequence.h"
#include "GUI/Elements/Nodes/WorkspaceTransformation_s.h"
#include "State/NodeDeserializer.h"
#include "State/SerializationVisitor.h"

#include "Core/Nodes/Utils.h"

/*
class SerdeTest : public GraphManagerTestFixture {};

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
