#include "WorkspaceTransformation_s.h"

WorkspaceTransformationFree::WorkspaceTransformationFree() : WorkspaceTransformation(Core::Builder::createTransform<Core::Free>()){setDataItemsWidth();}

WorkspaceTransformationAxisAngle::WorkspaceTransformationAxisAngle() : WorkspaceTransformation(Core::Builder::createTransform<Core::AxisAngleRot>()){setDataItemsWidth();}

WorkspaceTransformationLookAt::WorkspaceTransformationLookAt() : WorkspaceTransformation(Core::Builder::createTransform<Core::LookAt>()){setDataItemsWidth();}

WorkspaceTransformationTranslation::WorkspaceTransformationTranslation() : WorkspaceTransformation(Core::Builder::createTransform<Core::Translation>()){setDataItemsWidth();}
bool WorkspaceTransformationTranslation::drawDataSetValues(DIWNE::Diwne &diwne)
{
    Ptr<Core::Translation> nodebase = m_nodebase->as<Core::Translation>();
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
WorkspaceTransformationEulerX::WorkspaceTransformationEulerX() : WorkspaceTransformation(Core::Builder::createTransform<Core::EulerRotX>()){setDataItemsWidth();}
bool WorkspaceTransformationEulerX::drawDataSetValues(DIWNE::Diwne &diwne)
{
  Ptr<Core::EulerRotX> nodebase = m_nodebase->as<Core::EulerRotX>();
  return drawDataSetValues_builder(diwne,
                            { "angle x" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } }
                            );
}

WorkspaceTransformationEulerY::WorkspaceTransformationEulerY() : WorkspaceTransformation(Core::Builder::createTransform<Core::EulerRotY>()){setDataItemsWidth();}
bool WorkspaceTransformationEulerY::drawDataSetValues(DIWNE::Diwne &diwne)
{
  Ptr<Core::EulerRotY> nodebase = m_nodebase->as<Core::EulerRotY>();
  return drawDataSetValues_builder(diwne,
                            { "angle y" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } }
                            );
}

WorkspaceTransformationEulerZ::WorkspaceTransformationEulerZ() : WorkspaceTransformation(Core::Builder::createTransform<Core::EulerRotZ>()){setDataItemsWidth();}
bool WorkspaceTransformationEulerZ::drawDataSetValues(DIWNE::Diwne &diwne)
{
  Ptr<Core::EulerRotZ> nodebase = m_nodebase->as<Core::EulerRotZ>();
  return drawDataSetValues_builder(diwne,
                            { "angle z" },
                            { [nodebase]() {return nodebase->getAngle(); } },
                            { [nodebase](float v) {return nodebase->setValue(v); } }
                            );
}

WorkspaceTransformationScale::WorkspaceTransformationScale() : WorkspaceTransformation(Core::Builder::createTransform<Core::Scale>()){setDataItemsWidth();}
bool WorkspaceTransformationScale::drawDataSetValues(DIWNE::Diwne &diwne)
{
    Ptr<Core::Scale> nodebase = m_nodebase->as<Core::Scale>();
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
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


WorkspaceTransformationQuaternion::WorkspaceTransformationQuaternion() : WorkspaceTransformation(Core::Builder::createTransform<Core::QuatRot>()){setDataItemsWidth();}
bool WorkspaceTransformationQuaternion::drawDataFull(DIWNE::Diwne &diwne)
{
    Ptr<Core::Scale> nodebase = m_nodebase->as<Core::Scale>();
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
    bool valueChanged = false;
    glm::quat localData;

    drawDataQuaternion(diwne, getId(), getNumberOfVisibleDecimal(), getFloatPopupMode(), m_nodebase->getData().getQuat(), m_nodebase->getDataMapRef(), getDataItemsWidth(diwne),
                       valueChanged, localData);

    if (valueChanged)
    {
        m_nodebase->setValue(localData);
        setDataItemsWidth();
    }
}
int WorkspaceTransformationQuaternion::maxLenghtOfData()
{
    return maxLenghtOfDataQuaternion( m_nodebase->getData().getQuat(), m_numberOfVisibleDecimal);
}

WorkspaceTransformationOrthoProjection::WorkspaceTransformationOrthoProjection() : WorkspaceTransformation(Core::Builder::createTransform<Core::OrthoProj>()){setDataItemsWidth();}
bool WorkspaceTransformationOrthoProjection::drawDataSetValues(DIWNE::Diwne &diwne)
{
  Ptr<Core::OrthoProj> nodebase = m_nodebase->as<Core::OrthoProj>();
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

WorkspaceTransformationPerspective::WorkspaceTransformationPerspective() : WorkspaceTransformation(Core::Builder::createTransform<Core::PerspectiveProj>()){setDataItemsWidth();}
bool WorkspaceTransformationPerspective::drawDataSetValues(DIWNE::Diwne &diwne)
{
  Ptr<Core::PerspectiveProj> nodebase = m_nodebase->as<Core::PerspectiveProj>();
  return drawDataSetValues_builder(diwne,
                            {   "FOW",
                                "Aspect",
                                "ZNear",
                                "ZFar" },
                            {   [nodebase](){return nodebase->getFOW();},
                                [nodebase](){return nodebase->getAspect();},
                                [nodebase](){return nodebase->getZNear();},
                                [nodebase](){return nodebase->getZFar();} },
                            {   [nodebase](float v){return nodebase->setFOW(v);},
                                [nodebase](float v){return nodebase->setAspect(v);},
                                [nodebase](float v){return nodebase->setZNear(v);},
                                [nodebase](float v){return nodebase->setZFar(v);} }
  );
}

WorkspaceTransformationFrustum::WorkspaceTransformationFrustum() : WorkspaceTransformation(Core::Builder::createTransform<Core::Frustum>()){setDataItemsWidth();}
bool WorkspaceTransformationFrustum::drawDataSetValues(DIWNE::Diwne &diwne)
{
  Ptr<Core::Frustum> nodebase = m_nodebase->as<Core::Frustum>();
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


