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
#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"

#include "Utils.h"

using namespace Core;

TEST(CameraNodeTest, CameraNodeCanBePluggedToScreenNode)
{
	auto cameraNode = GraphManager::createCamera();
	auto screenNode = Builder::createOperator<EOperatorType::Screen>();

	auto perspectiveProj = Builder::createTransform<ETransformType::Perspective>();
	auto lookAt = Builder::createTransform<ETransformType::LookAt>();

	cameraNode->getProj()->pushMatrix(perspectiveProj);
	cameraNode->getView()->pushMatrix(lookAt);

	EXPECT_TRUE(
	    Math::eq(cameraNode->getProj()->getData(I3T_SEQ_OUT_MAT).getMat4(), perspectiveProj->getData().getMat4()));
	EXPECT_TRUE(Math::eq(cameraNode->getView()->getData(I3T_SEQ_OUT_MAT).getMat4(), lookAt->getData().getMat4()));

	plug_expectOk(cameraNode, screenNode, 0, 0);

	auto expectedPV = perspectiveProj->getData().getMat4() * lookAt->getData().getMat4();
	EXPECT_EQ(expectedPV, cameraNode->getData(1).getMat4());
}

TEST(CameraNodeTest, CameraAndSequenceCannotBeConnected)
{
	auto camera = GraphManager::createCamera();
	auto sequence = GraphManager::createSequence();

	EXPECT_TRUE(camera->getOutput(I3T_CAMERA_OUT_MUL).isDisabled());

	const auto result = GraphManager::plug(camera, sequence, I3T_CAMERA_OUT_MUL, I3T_SEQ_IN_MUL);
	EXPECT_EQ(result, ENodePlugResult::Err_DisabledPin);
}

TEST(CameraNodeTest, AllowedCameraScreenProjectionLoop)
{
	auto camera = GraphManager::createCamera();
	auto screen = Builder::createOperator<EOperatorType::Screen>();
	auto perspective = Builder::createOperator<EOperatorType::MakePerspective>();

	plug_expectOk(camera, screen, 0, 0);
	plug_expectOk(screen, perspective, 1 /* 0 is screen type output */, 1);
	plug_expectOk(perspective, camera->getProj(), 0, I3T_SEQ_IN_MAT);
}

TEST(CameraNodeTest, UnallowedCameraMatrixMatrixLoop)
{
	auto camera = GraphManager::createCamera();
	auto matrix1 = Builder::createOperator<EOperatorType::MatrixToMatrix>();
	auto matrix2 = Builder::createOperator<EOperatorType::MatrixToMatrix>();

	plug_expectOk(camera, matrix1, 1, 0);
	plug_expectOk(matrix1, matrix2, 0, 0);

	auto result = GraphManager::plug(matrix2, camera->getProj(), 0, I3T_SEQ_IN_MAT);
	EXPECT_EQ(result, ENodePlugResult::Err_Loop);
}
