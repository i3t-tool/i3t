/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>, Martin Herich
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "gtest/gtest.h"

#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Iterators.h"
#include "Trees.h"

using namespace Core;

TEST(TreeIteratorTest, PreorderIteratorLinear)
{
	TrickyTestTree s;

	NodeTree tree(s.sequence1.get());

	auto it = tree.begin();
	ASSERT_TRUE(it != tree.end());

	Node* node;

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.sequence1.get());
	++it;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.sequence2.get());
	++it;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.sequence3.get());
	++it;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.sequence4.get());
	++it;
	ASSERT_TRUE(it != tree.end());
	ASSERT_TRUE(it.valid());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.sequence5.get());
	++it;

	ASSERT_EQ(it, tree.end());
	ASSERT_TRUE(!it.valid());
	ASSERT_EQ(it.operator->(), nullptr);
}

TEST(TreeIteratorTest, PreorderIteratorBranched)
{
	GeneralTestTree s;

	NodeTree tree(s.seq1.get());

	auto it = tree.begin();
	ASSERT_TRUE(it != tree.end());

	Node* node;

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.seq1.get());
	it++;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.seq2.get());
	it++;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.seq3.get());
	++it;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.m3.get());
	++it;
	ASSERT_TRUE(it != tree.end());
	ASSERT_TRUE(it.valid());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.seq4.get());
	++it;

	// So on and so forth
	std::vector<Node*> expectedNodes = {{
	    s.m1.get(),
	    s.m2.get(),
	    s.m4.get(),
	    s.m5.get(),
	    s.op1.get(),
	    s.op4.get(),
	    s.op2.get(),
	    s.op3.get(),
	    s.op5.get(),
	}};

	for (int i = 0; it != tree.end(); ++it, ++i)
	{
		ASSERT_EQ(expectedNodes[i], it.ptr());
	}

	ASSERT_EQ(it, tree.end());
	ASSERT_TRUE(!it.valid());
	ASSERT_EQ(it.operator->(), nullptr);

	it++;

	ASSERT_EQ(it, tree.end());
	ASSERT_TRUE(!it.valid());
	ASSERT_EQ(it.operator->(), nullptr);

	++it;

	ASSERT_EQ(it, tree.end());
	ASSERT_TRUE(!it.valid());
	ASSERT_EQ(it.operator->(), nullptr);

	it++;

	ASSERT_EQ(it, tree.end());
	ASSERT_TRUE(!it.valid());
	ASSERT_EQ(it.operator->(), nullptr);


	int cnt = 0;
	for (const auto& e : tree)
		cnt++;
	ASSERT_EQ(cnt, 14);
}


TEST(TreeIteratorTest, PreorderIterator_ConnectionFilter)
{
	GeneralTestTree s;

	NodeTree tree(s.seq1.get(), static_cast<int>(EValueType::MatrixMul));

	auto it = tree.begin();
	ASSERT_TRUE(it != tree.end());

	Node* node;

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.seq1.get());
	it++;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.seq2.get());
	it++;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.seq3.get());
	++it;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.m3.get());
	++it;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.seq4.get());
	++it;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.m1.get());
	++it;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.m2.get());
	++it;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.m4.get());
	++it;
	ASSERT_TRUE(it != tree.end());

	node = it.ptr();
	ASSERT_EQ(it.ptr(), &*it);
	ASSERT_EQ(node, s.m5.get());
	++it;

	ASSERT_TRUE(it == tree.end());

	ASSERT_EQ(it, tree.end());
	ASSERT_TRUE(!it.valid());
	ASSERT_EQ(it.operator->(), nullptr);

	it++;

	ASSERT_EQ(it, tree.end());
	ASSERT_TRUE(!it.valid());
	ASSERT_EQ(it.operator->(), nullptr);

	int cnt = 0;
	for (const auto& e : tree)
		cnt++;
	ASSERT_EQ(cnt, 9);
}
