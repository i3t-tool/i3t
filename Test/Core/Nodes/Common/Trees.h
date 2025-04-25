/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "Core/Nodes/GraphManager.h"

#include "../Utils.h"

/**
 *       cam1(P(d1, d2), V(d3)) / operator cam1(P(d1, d2), V(d3))
 *          \                                     |           \
 *	       (3)s1-\                           seq7 (S, T)   op7 (mat)
 *	       /  \   \
 *	    (2)s2  m5   \--M2M
 *	   /   \ \         |  \
 * (2)s3   s4 m4     M+M  M2Q
 * /   \    |\         \
 *m3 |->s6  | s5(1)    M2F
 *  op6  \  |    \       \
 *       m6 m1    m2     F2V
 */
struct GeneralCamTree
{
	Ptr<Sequence> seq1;
	Ptr<Sequence> seq2;
	Ptr<Sequence> seq3;
	Ptr<Sequence> seq4;
	Ptr<Sequence> seq5;
	Ptr<Sequence> seq6;
	Ptr<Sequence> seq7;

	Ptr<Node> m1;
	Ptr<Node> m2;
	Ptr<Node> m3;
	Ptr<Node> m4;
	Ptr<Node> m5;
	Ptr<Node> m6;

	Ptr<Node> op1; // M2M
	Ptr<Node> op2; // M2F
	Ptr<Node> op3; // F2V
	Ptr<Node> op4; // M+M
	Ptr<Node> op5; // M2Q
	Ptr<Node> op6;
	Ptr<Node> op7;

	Ptr<Camera> cam1;

	GeneralCamTree(bool emptyCam, bool operatorCam = false)
	{
		seq1 = GraphManager::createSequence();
		seq2 = GraphManager::createSequence();
		seq3 = GraphManager::createSequence();
		seq4 = GraphManager::createSequence();
		seq5 = GraphManager::createSequence();
		seq6 = GraphManager::createSequence();
		seq7 = GraphManager::createSequence();

		m1 = GraphManager::createModel();
		m2 = GraphManager::createModel();
		m3 = GraphManager::createModel();
		m4 = GraphManager::createModel();
		m5 = GraphManager::createModel();
		m6 = GraphManager::createModel();

		op1 = GraphManager::createNode<EOperatorType::MatrixToMatrix>();
		op2 = GraphManager::createNode<EOperatorType::MatrixToFloats>();
		op3 = GraphManager::createNode<EOperatorType::FloatsToVector3>();
		op4 = GraphManager::createNode<EOperatorType::MatrixAddMatrix>();
		op5 = GraphManager::createNode<EOperatorType::MatrixToQuat>();
		op6 = GraphManager::createNode<EOperatorType::MatrixToMatrix>();
		op7 = GraphManager::createNode<EOperatorType::MatrixToMatrix>();

		plug_expectOk(seq1, seq2, I3T_SEQ_OUT_MUL, I3T_SEQ_IN_MUL);
		plug_expectOk(seq2, seq3, I3T_SEQ_OUT_MUL, I3T_SEQ_IN_MUL);
		plug_expectOk(seq2, seq4, I3T_SEQ_OUT_MUL, I3T_SEQ_IN_MUL);
		plug_expectOk(seq3, seq6, I3T_SEQ_OUT_MUL, I3T_SEQ_IN_MUL);
		plug_expectOk(seq4, seq5, I3T_SEQ_OUT_MUL, I3T_SEQ_IN_MUL);
		plug_expectOk(seq4, m1, I3T_SEQ_OUT_MUL, 0);
		plug_expectOk(seq5, m2, I3T_SEQ_OUT_MUL, 0);
		plug_expectOk(seq3, m3, I3T_SEQ_OUT_MUL, 0);
		plug_expectOk(seq2, m4, I3T_SEQ_OUT_MUL, 0);
		plug_expectOk(seq1, m5, I3T_SEQ_OUT_MUL, 0);
		plug_expectOk(seq6, m6, I3T_SEQ_OUT_MUL, 0);

		plug_expectOk(seq1, op1, 1, 0);
		plug_expectOk(op1, op4, 0, 0);
		plug_expectOk(op1, op5, 0, 0);
		plug_expectOk(op4, op2, 0, 0);
		plug_expectOk(op2, op3, 0, 0);

		std::vector<Ptr<Transform>> matrices = {
		    // sequence 1
		    Builder::createTransform<ETransformType::EulerX>(),
		    Builder::createTransform<ETransformType::Scale>(),
		    Builder::createTransform<ETransformType::Translation>(),

		    // sequence 2
		    Builder::createTransform<ETransformType::Scale>(),
		    Builder::createTransform<ETransformType::Translation>(),

		    // sequence 3
		    Builder::createTransform<ETransformType::Scale>(),
		    Builder::createTransform<ETransformType::Translation>(),

		    // sequence 5
		    Builder::createTransform<ETransformType::Translation>(),

		    // cam 1 proj
		    Builder::createTransform<ETransformType::Perspective>(),
		    Builder::createTransform<ETransformType::Translation>(),

		    // cam 1 view
		    Builder::createTransform<ETransformType::LookAt>(),

		    // sequence 6
		    Builder::createTransform<ETransformType::Free>(),
		};

		// Add matrices to the sequences.
		seq1->pushMatrix(matrices[0]);
		seq1->pushMatrix(matrices[1]);
		seq1->pushMatrix(matrices[2]);

		seq2->pushMatrix(matrices[3]);
		seq2->pushMatrix(matrices[4]);

		seq3->pushMatrix(matrices[5]);
		seq3->pushMatrix(matrices[6]);

		matrices[7]->setValue(glm::mat4({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {5.5, 0, 0, 1}));
		seq5->pushMatrix(matrices[7]);

		cam1 = GraphManager::createCamera();
		if (!emptyCam)
		{
			cam1->getProj()->pushMatrix(matrices[8]);
			cam1->getProj()->pushMatrix(matrices[9]);
			cam1->getView()->pushMatrix(matrices[10]);
		}
		if (operatorCam)
		{
			plug_expectOk(seq7, cam1->getProj(), I3T_SEQ_OUT_MAT, I3T_SEQ_IN_MAT);
			plug_expectOk(op7, cam1->getView(), 0, I3T_SEQ_IN_MAT);
		}

		plug_expectOk(cam1, seq1, I3T_CAMERA_OUT_MUL, I3T_SEQ_IN_MUL);

		seq6->pushMatrix(matrices[11]);
		plug_expectOk(op6, seq6, 0, I3T_SEQ_IN_MAT);
	}
};


/**
 *                                         /----- Sequence 2 : | Scal | Tran |
 * (branch1) Sequence 1 : | RotX | Scal | Tran | ----|
 *                                         \----- Sequence 3 : | Scal | Tran |
 * (branch2)
 */
struct BranchedSequenceTree
{
	Ptr<Sequence> rootSequence;
	Ptr<Sequence> branch1Sequence;
	Ptr<Sequence> branch2Sequence;

	std::vector<Ptr<Node>> branch1ToRootMatrices;

	BranchedSequenceTree()
	{
		// Prepare graph.
		rootSequence = GraphManager::createSequence();
		branch1Sequence = GraphManager::createSequence();
		branch2Sequence = GraphManager::createSequence();

		std::vector<Ptr<Transform>> matrices = {
		    // sequence 1
		    Builder::createTransform<ETransformType::EulerX>(),
		    Builder::createTransform<ETransformType::Scale>(),
		    Builder::createTransform<ETransformType::Translation>(),

		    // sequence 2
		    Builder::createTransform<ETransformType::Scale>(),
		    Builder::createTransform<ETransformType::Translation>(),

		    // sequence 3
		    Builder::createTransform<ETransformType::Scale>(),
		    Builder::createTransform<ETransformType::Translation>(),
		};

		plug_expectOk(rootSequence, branch1Sequence, 0, 0);
		plug_expectOk(rootSequence, branch2Sequence, 0, 0);

		// Add matrices to the sequences.
		rootSequence->pushMatrix(matrices[0]);
		rootSequence->pushMatrix(matrices[1]);
		rootSequence->pushMatrix(matrices[2]);

		branch1Sequence->pushMatrix(matrices[3]);
		branch1Sequence->pushMatrix(matrices[4]);

		branch2Sequence->pushMatrix(matrices[5]);
		branch2Sequence->pushMatrix(matrices[6]);

		branch1ToRootMatrices = {
		    matrices[4], matrices[3], matrices[2], matrices[1], matrices[0],
		};
	}
};

/// X - translation on X axis
/// Y - translation on Y axis
/// Z - translation on Z axis
///
/// ```
///                      /- matrix1 (Z)                                     /- matrix2 (Z)
///                     /                                                  /
/// | sequence 1 | - | sequence 2 | - | sequence 3 | - | sequence 4 | - | sequence 5 |
/// | Y, X       |   | X          |   | X          |   |            |   | X          |
/// ```
struct TrickyTestTree
{
	Ptr<Sequence> sequence1;
	Ptr<Sequence> sequence2;
	Ptr<Sequence> sequence3;
	Ptr<Sequence> sequence4;
	Ptr<Sequence> sequence5;

	Ptr<Node> matrix1;
	Ptr<Node> matrix2;

	glm::vec3 translationX = {1, 0, 0};
	glm::vec3 translationY = {0, 1, 0};
	glm::vec3 translationZ = {0, 0, 1};

	TrickyTestTree()
	{
		sequence1 = GraphManager::createSequence();
		sequence1->pushMatrix(createTranslation(translationY));
		sequence1->pushMatrix(createTranslation(translationX));
		sequence2 = GraphManager::createSequence();
		sequence2->pushMatrix(createTranslation(translationX));
		sequence3 = GraphManager::createSequence();
		sequence3->pushMatrix(createTranslation(translationX));
		sequence4 = GraphManager::createSequence();
		sequence5 = GraphManager::createSequence();
		sequence5->pushMatrix(createTranslation(translationX));

		matrix1 = GraphManager::createNode<EOperatorType::MakeTranslation>();
		matrix1->setValue(glm::translate(translationZ));
		matrix2 = GraphManager::createNode<EOperatorType::MakeTranslation>();
		matrix2->setValue(glm::translate(translationZ));

		plug_expectOk(sequence1, sequence2, 0, 0);

		plug_expectOk(sequence2, sequence3, 0, 0);
		plug_expectOk(matrix1, sequence2, 0, I3T_SEQ_IN_MAT);

		plug_expectOk(sequence3, sequence4, 0, 0);

		plug_expectOk(sequence4, sequence5, 0, 0);

		plug_expectOk(matrix2, sequence5, 0, I3T_SEQ_IN_MAT);
	}

private:
	static Ptr<Transform> createTranslation(glm::vec3 translation)
	{
		auto transform = Builder::createTransform<ETransformType::Translation>();
		transform->setDefaultValue("translate", translation);

		return transform;
	}
};

/**
 *	       seq1
 *	       / | \
 *	   seq2 m5 M2M
 *	   / | \     |\
 *	s3 s4 m4    M+M M2Q
 *	 |  |\       |
 *	m3 m1 m2    M2F
 *                |
 *               F2V
 */
struct GeneralTestTree
{
	Ptr<Sequence> seq1;
	Ptr<Sequence> seq2;
	Ptr<Sequence> seq3;
	Ptr<Sequence> seq4;

	Ptr<Node> m1;
	Ptr<Node> m2;
	Ptr<Node> m3;
	Ptr<Node> m4;
	Ptr<Node> m5;

	Ptr<Node> op1;
	Ptr<Node> op2;
	Ptr<Node> op3;
	Ptr<Node> op4;
	Ptr<Node> op5;

	GeneralTestTree()
	{
		seq1 = GraphManager::createSequence();
		seq2 = GraphManager::createSequence();
		seq3 = GraphManager::createSequence();
		seq4 = GraphManager::createSequence();

		m1 = GraphManager::createModel();
		m2 = GraphManager::createModel();
		m3 = GraphManager::createModel();
		m4 = GraphManager::createModel();
		m5 = GraphManager::createModel();

		op1 = GraphManager::createNode<EOperatorType::MatrixToMatrix>();
		op2 = GraphManager::createNode<EOperatorType::MatrixToFloats>();
		op3 = GraphManager::createNode<EOperatorType::FloatsToVector3>();
		op4 = GraphManager::createNode<EOperatorType::MatrixAddMatrix>();
		op5 = GraphManager::createNode<EOperatorType::MatrixToQuat>();

		plug_expectOk(seq1, seq2, 0, 0);
		plug_expectOk(seq2, seq3, 0, 0);
		plug_expectOk(seq2, seq4, 0, 0);
		plug_expectOk(seq4, m1, 0, 0);
		plug_expectOk(seq4, m2, 0, 0);
		plug_expectOk(seq3, m3, 0, 0);
		plug_expectOk(seq2, m4, 0, 0);
		plug_expectOk(seq1, m5, 0, 0);

		plug_expectOk(seq1, op1, 1, 0);
		plug_expectOk(op1, op4, 0, 0);
		plug_expectOk(op1, op5, 0, 0);
		plug_expectOk(op4, op2, 0, 0);
		plug_expectOk(op2, op3, 0, 0);
	}
};