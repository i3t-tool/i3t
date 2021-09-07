#include "WorkspaceMatrixTranslation.h"

WorkspaceMatrixTranslation::WorkspaceMatrixTranslation()
    :   WorkspaceMatrix4x4(Core::Builder::createTransform<Core::Translation>())
{}


bool WorkspaceMatrixTranslation::drawDataSetValues(DIWNE::Diwne &diwne)
{
    Ptr<Core::Translation> nodebase = m_nodebase->as<Core::Translation>();
    const Core::Transform::DataMap& coreMap = m_nodebase->getDataMapRef();

    return drawDataSetValues_builder(diwne,
                                {   "x", /* \todo labels from settings? */
                                    "y",
                                    "z" },
                                {   [nodebase](){return nodebase->getX();},
                                    [nodebase](){return nodebase->getY();},
                                    [nodebase](){return nodebase->getZ();} },
                                {   [nodebase](float v){return nodebase->setX(v);},
                                    [nodebase](float v){return nodebase->setY(v);},
                                    [nodebase](float v){return nodebase->setZ(v);} },
                                {   coreMap[3*4+0], /* \todo JH some better way how determine what element from DataMap should be used? */
                                    coreMap[3*4+1],
                                    coreMap[3*4+2] }
                            );
}
