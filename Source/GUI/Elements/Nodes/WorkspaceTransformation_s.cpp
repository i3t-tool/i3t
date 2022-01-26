#include "WorkspaceTransformation_s.h"

WorkspaceTransformationFree::WorkspaceTransformationFree() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::Free>()){setDataItemsWidth();}

WorkspaceTransformationAxisAngle::WorkspaceTransformationAxisAngle() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::AxisAngle>()){setDataItemsWidth();}

WorkspaceTransformationLookAt::WorkspaceTransformationLookAt() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::LookAt>()){setDataItemsWidth();}

WorkspaceTransformationTranslation::WorkspaceTransformationTranslation() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::Translation>()){setDataItemsWidth();}
bool WorkspaceTransformationTranslation::drawDataSetValues(DIWNE::Diwne &diwne)
{
    auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Translation>>();
    return drawDataSetValues_builder(diwne,
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
WorkspaceTransformationEulerX::WorkspaceTransformationEulerX() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::EulerX>()){setDataItemsWidth();}
bool WorkspaceTransformationEulerX::drawDataSetValues(DIWNE::Diwne &diwne)
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::EulerX>>();
  return drawDataSetValues_builder(diwne,
                            { "angle x" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } }
                            );
}

WorkspaceTransformationEulerY::WorkspaceTransformationEulerY() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::EulerY>()){setDataItemsWidth();}
bool WorkspaceTransformationEulerY::drawDataSetValues(DIWNE::Diwne &diwne)
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::EulerY>>();
  return drawDataSetValues_builder(diwne,
                            { "angle y" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } }
                            );
}

WorkspaceTransformationEulerZ::WorkspaceTransformationEulerZ() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::EulerZ>()){setDataItemsWidth();}
bool WorkspaceTransformationEulerZ::drawDataSetValues(DIWNE::Diwne &diwne)
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::EulerZ>>();
  return drawDataSetValues_builder(diwne,
                            { "angle z" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } }
                            );
}

WorkspaceTransformationScale::WorkspaceTransformationScale() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::Scale>()){setDataItemsWidth();}
bool WorkspaceTransformationScale::drawDataSetValues(DIWNE::Diwne &diwne)
{
    auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Scale>>();
    return drawDataSetValues_builder(diwne,
                                {   "X", "Y", "Z" },
                                {   [nodebase](){return nodebase->getX();},
                                    [nodebase](){return nodebase->getY();},
                                    [nodebase](){return nodebase->getZ();} },
                                {   [nodebase](float v){return nodebase->setX(v);},
                                    [nodebase](float v){return nodebase->setY(v);},
                                    [nodebase](float v){return nodebase->setZ(v);} }
                            );
}


WorkspaceTransformationQuaternion::WorkspaceTransformationQuaternion() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::Quat>()){setDataItemsWidth();}
bool WorkspaceTransformationQuaternion::drawDataFull(DIWNE::Diwne &diwne)
{
    auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Scale>>();
    bool valueChanged = false, interaction_happen;
    glm::quat localData;

    interaction_happen = drawDataQuaternion(diwne, getId(), getNumberOfVisibleDecimal(), getDataItemsWidth(diwne), getFloatPopupMode(),
                                            m_nodebase->getData().getQuat(), m_nodebase->getDataMapRef(),
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

WorkspaceTransformationOrthoProjection::WorkspaceTransformationOrthoProjection() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::Ortho>()){setDataItemsWidth();}
bool WorkspaceTransformationOrthoProjection::drawDataSetValues(DIWNE::Diwne &diwne)
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Ortho>>();
  return drawDataSetValues_builder(diwne,
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

WorkspaceTransformationPerspective::WorkspaceTransformationPerspective() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::Perspective>()){setDataItemsWidth();}
bool WorkspaceTransformationPerspective::drawDataSetValues(DIWNE::Diwne &diwne)
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Perspective>>();
  return drawDataSetValues_builder(diwne,
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

WorkspaceTransformationFrustum::WorkspaceTransformationFrustum() : WorkspaceTransformation(Core::Builder::createTransform<ETransformType::Frustum>()){setDataItemsWidth();}
bool WorkspaceTransformationFrustum::drawDataSetValues(DIWNE::Diwne &diwne)
{
  auto nodebase = m_nodebase->as<Core::TransformImpl<ETransformType::Frustum>>();
  return drawDataSetValues_builder(diwne,
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


