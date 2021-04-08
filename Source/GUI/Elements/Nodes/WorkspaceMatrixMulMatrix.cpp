//#include "WorkspaceMatrixMulMatrix.h"
//
//WorkspaceMatrixMulMatrix::WorkspaceMatrixMulMatrix(ImTextureID headerBackground, WorkspaceMatrixMulMatrixArgs const& args)
//    : WorkspaceMatrix4x4(headerBackground, { .viewScale = args.viewScale, .headerLabel = args.headerLabel, .nodeLabel = args.nodeLabel, .nodebase = args.nodebase })
//{
//}
//
//WorkspaceMatrixMulMatrix::WorkspaceMatrixMulMatrix(ImTextureID headerBackground, std::string headerLabel, std::string nodeLabel)
//    : WorkspaceMatrix4x4(headerBackground, Builder::createNode<ENodeType::MatrixMulMatrix>(), headerLabel, nodeLabel)
//{}
//
//void WorkspaceMatrixMulMatrix::drawData(util::NodeBuilder& builder)
//{
//   drawDataFull(builder);
//}
//
//void WorkspaceMatrixMulMatrix::drawDataSetValues(util::NodeBuilder& builder)
//{
//
//}
//
//void WorkspaceMatrixMulMatrix::drawDataLabel(util::NodeBuilder& builder)
//{
//    builder.Middle();
//    ImGui::Text(this->m_label.c_str());
//    ImGui::Spring(0);
//}
