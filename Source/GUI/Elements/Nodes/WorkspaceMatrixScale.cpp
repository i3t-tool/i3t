#include "WorkspaceMatrixScale.h"

WorkspaceMatrixScale::WorkspaceMatrixScale()
    :   WorkspaceMatrix4x4(Core::Builder::createTransform<Core::Scale>())
{}

bool WorkspaceMatrixScale::drawDataSetValues(DIWNE::Diwne &diwne)
{
    Ptr<Core::Scale> nodebase = m_nodebase->as<Core::Scale>();
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();
    return drawDataSetValues_builder(diwne,
                                {   "scale" }, /* \todo labels from settings */
                                {   [nodebase](){return nodebase->getX();} },
                                {   [nodebase](float v){return nodebase->setX(v);}},
                                {   coreMap[0*4+0] /* \todo JH some better way how determine what element from DataMap should be used? */}
                            );
}
