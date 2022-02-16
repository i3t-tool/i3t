#include "WorkspaceTransformation_s.h"

WorkspaceTransformationFree::WorkspaceTransformationFree(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::Free>()){setDataItemsWidth();}

WorkspaceTransformationAxisAngle::WorkspaceTransformationAxisAngle(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::AxisAngle>()){setDataItemsWidth();}

WorkspaceTransformationLookAt::WorkspaceTransformationLookAt(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::LookAt>()){setDataItemsWidth();}

WorkspaceTransformationTranslation::WorkspaceTransformationTranslation(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::Translation>()){setDataItemsWidth();}
bool WorkspaceTransformationTranslation::drawDataSetValues()
{
    auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Translation>>();
    return drawDataSetValues_builder(
                                {   "x", /* \todo labels from settings? */
                                    "y",
                                    "z" },
                                {   [nodebase](){return nodebase->getX();},
                                    [nodebase](){return nodebase->getY();},
                                    [nodebase](){return nodebase->getZ();} },
                                {   [nodebase](float v){return nodebase->setX(v);},
                                    [nodebase](float v){return nodebase->setY(v);},
                                    [nodebase](float v){return nodebase->setZ(v);} }
                            );
}

/* \todo JH three rotations can be templated -> update addNodeToPosition() too */
WorkspaceTransformationEulerX::WorkspaceTransformationEulerX(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::EulerX>()){setDataItemsWidth();}
bool WorkspaceTransformationEulerX::drawDataSetValues()
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::EulerX>>();
  return drawDataSetValues_builder(
                            { "angle x" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } }
                            );
}

WorkspaceTransformationEulerY::WorkspaceTransformationEulerY(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::EulerY>()){setDataItemsWidth();}
bool WorkspaceTransformationEulerY::drawDataSetValues()
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::EulerY>>();
  return drawDataSetValues_builder(
                            { "angle y" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } }
                            );
}

WorkspaceTransformationEulerZ::WorkspaceTransformationEulerZ(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::EulerZ>()){setDataItemsWidth();}
bool WorkspaceTransformationEulerZ::drawDataSetValues()
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::EulerZ>>();
  return drawDataSetValues_builder(
                            { "angle z" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } }
                            );
}

WorkspaceTransformationScale::WorkspaceTransformationScale(DIWNE::Diwne &diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::Scale>()){setDataItemsWidth();}
bool WorkspaceTransformationScale::drawDataSetValues()
{
    bool inner_interaction_happen = false, value_changed = false;
	auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Scale>>();

	for (auto& [key, valueStore] : nodebase->getDefaultValues()) /* více typů dat */
	{
		switch (valueStore.opValueType)
		{
		case EValueType::Vec3:
		{
			auto localData = valueStore.getVec3();

            inner_interaction_happen |= drawDataSetValues_builder({fmt::format("{} X",key.c_str()), fmt::format("{} Y",key.c_str()), fmt::format("{} Z",key.c_str()) } /* \todo MH all/whole labels from core? */
                                                        ,   {&localData[0], &localData[1], &localData[2]}
                                                        ,   value_changed);
			if (value_changed)
			{
				nodebase->setDefaultValue(key, localData);
				setDataItemsWidth();
			}
		}
		}
	}
	return inner_interaction_happen;

	/*
    return drawDataSetValues_builder(
                                {   "X", "Y", "Z" },
                                {   [nodebase](){return nodebase->getX();},
                                    [nodebase](){return nodebase->getY();},
                                    [nodebase](){return nodebase->getZ();} },
                                {   [nodebase](float v){return nodebase->setX(v);},
                                    [nodebase](float v){return nodebase->setY(v);},
                                    [nodebase](float v){return nodebase->setZ(v);} }
                            );
                            */
}


WorkspaceTransformationQuaternion::WorkspaceTransformationQuaternion(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::Quat>()){setDataItemsWidth();}
bool WorkspaceTransformationQuaternion::drawDataFull()
{
    auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Scale>>();
    bool valueChanged = false, interaction_happen;
    glm::quat localData;

    interaction_happen = drawDataQuaternion(diwne, getId(), getNumberOfVisibleDecimal(), getDataItemsWidth(), getFloatPopupMode(),
                                            m_nodebase->getData().getQuat(), {m_nodebase->as<Core::Transformation>()->getValueState({0,0}), m_nodebase->as<Core::Transformation>()->getValueState({1,0}), m_nodebase->as<Core::Transformation>()->getValueState({2,0}), m_nodebase->as<Core::Transformation>()->getValueState({3,0})},
                                            valueChanged, localData);

    if (valueChanged)
    {
        m_nodebase->setValue(localData);
        setDataItemsWidth();
    }
    return interaction_happen;
}
int WorkspaceTransformationQuaternion::maxLenghtOfData()
{
    return maxLenghtOfDataQuaternion( m_nodebase->getData().getQuat(), m_numberOfVisibleDecimal);
}

WorkspaceTransformationOrthoProjection::WorkspaceTransformationOrthoProjection(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::Ortho>()){setDataItemsWidth();}
bool WorkspaceTransformationOrthoProjection::drawDataSetValues()
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Ortho>>();
  return drawDataSetValues_builder(
                            {   "left",
                                "right",
                                "bottom",
                                "top",
                                "near",
                                "far"},
                            {   [nodebase](){return nodebase->getLeft();},
                                [nodebase](){return nodebase->getRight();},
                                [nodebase](){return nodebase->getBottom();},
                                [nodebase](){return nodebase->getTop();},
                                [nodebase](){return nodebase->getNear();},
                                [nodebase](){return nodebase->getFar();} },
                            {   [nodebase](float v){return nodebase->setLeft(v);},
                                [nodebase](float v){return nodebase->setRight(v);},
                                [nodebase](float v){return nodebase->setBottom(v);},
                                [nodebase](float v){return nodebase->setTop(v);},
                                [nodebase](float v){return nodebase->setNear(v);},
                                [nodebase](float v){return nodebase->setFar(v);}}
  );
}

WorkspaceTransformationPerspective::WorkspaceTransformationPerspective(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::Perspective>()){setDataItemsWidth();}
bool WorkspaceTransformationPerspective::drawDataSetValues()
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Perspective>>();
  return drawDataSetValues_builder(
                            {   "FOW",
                                "Aspect",
                                "ZNear",
                                "ZFar" },
                            {   [nodebase](){return nodebase->getFOV();},
                                [nodebase](){return nodebase->getAspect();},
                                [nodebase](){return nodebase->getZNear();},
                                [nodebase](){return nodebase->getZFar();} },
                            {   [nodebase](float v){return nodebase->setFOV(v);},
                                [nodebase](float v){return nodebase->setAspect(v);},
                                [nodebase](float v){return nodebase->setZNear(v);},
                                [nodebase](float v){return nodebase->setZFar(v);} }
  );
}

WorkspaceTransformationFrustum::WorkspaceTransformationFrustum(DIWNE::Diwne& diwne) : WorkspaceTransformation(diwne, Core::Builder::createTransform<ETransformType::Frustum>()){setDataItemsWidth();}
bool WorkspaceTransformationFrustum::drawDataSetValues()
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Frustum>>();
  return drawDataSetValues_builder(
                            {   "left",
                                "right",
                                "bottom",
                                "top",
                                "near",
                                "far"},
                            {   [nodebase](){return nodebase->getLeft();},
                                [nodebase](){return nodebase->getRight();},
                                [nodebase](){return nodebase->getBottom();},
                                [nodebase](){return nodebase->getTop();},
                                [nodebase](){return nodebase->getNear();},
                                [nodebase](){return nodebase->getFar();} },
                            {   [nodebase](float v){return nodebase->setLeft(v);},
                                [nodebase](float v){return nodebase->setRight(v);},
                                [nodebase](float v){return nodebase->setBottom(v);},
                                [nodebase](float v){return nodebase->setTop(v);},
                                [nodebase](float v){return nodebase->setNear(v);},
                                [nodebase](float v){return nodebase->setFar(v);}}
  );
}


