/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <map>
#include <set>

#include "glm/matrix.hpp"

#include "Model.h"

namespace Core
{
class Sequence;
class Transform;

class SequenceTree
{
	Sequence* m_beginSequence;

public:
	class MatrixIterator
	{
		friend class SequenceTree;
		SequenceTree* m_tree;
		Sequence* m_currentSequence;
		Ptr<Node> m_currentMatrix;

	public:
		explicit MatrixIterator(Sequence* sequence);
		MatrixIterator(Sequence* sequence, Ptr<Node> node);

		MatrixIterator(const MatrixIterator& mt);

		/// Move iterator to root sequence.
		MatrixIterator& operator++();

		/// Move iterator to root sequence.
		MatrixIterator operator++(int);

		MatrixIterator operator+(int);

		/// Move iterator towards to the leaf sequence.
		MatrixIterator& operator--();

		/// Move iterator towards to the leaf sequence.
		MatrixIterator operator--(int);

		/// Get current matrix, can be transformation or other node with matrix output.
		///
		/// \warning You have to extract data from the node by yourself.
		Ptr<Node> operator*() const;

		bool operator==(const MatrixIterator& rhs) const;
		bool operator!=(const MatrixIterator& rhs) const;

	private:
		/// Move to the next matrix (to the root).
		void advance();

		/// Move to the previous matrix (from the root).
		///
		/// \todo Not implemented correctly for sequences with matrix input plugged.
		void withdraw();
	};

public:
	explicit SequenceTree(Ptr<Node> sequence);

	/**
	 * \return Iterator which points to the sequence.
	 */
	MatrixIterator begin();

	/**
	 * \return Iterator which points to the sequence root.
	 */
	MatrixIterator end();
};

//----------------------------------------------------------------------------//

class IModelProxy
{
public:
	virtual ~IModelProxy() {}
	virtual void update(const glm::mat4& transform) = 0;
	virtual Ptr<Core::Model> getModel() = 0;
};

class MatrixTracker
{
public:
	MatrixTracker() = default;

	/**
	 * @param beginSequence
	 */
	explicit MatrixTracker(Sequence* beginSequence, UPtr<IModelProxy> model);

	void update();

	Ptr<Sequence> getSequence() const;
	ID getSequenceID() const;

	Ptr<Model> getModel() const
	{
		return m_model->getModel();
	}

	const glm::mat4& getInterpolatedMatrix()
	{
		return m_interpolatedMatrix;
	}

	unsigned fullMatricesCount() const
	{
		return m_fullMatricesCount;
	}

	float getParam() const
	{
		return m_param;
	}

	bool setParam(float param);

	const std::map<Core::ID, float>& getTrackingProgress() const
	{
		return m_trackingProgress;
	}

	float getTrackingProgressForNode(ID transformID) const
	{
		if (!m_trackingProgress.contains(transformID))
		{
			return 0.0f;
		}

		return m_trackingProgress.at(transformID);
	}

	ID getInterpolatedTransformID() const
	{
		return m_interpolatedTransformID;
	}

private:
	/// \pre m_beginSequence is set
	void track();

	/// Public interface is in the GraphManager.
	void stop();

	void setTransform();

	unsigned m_fullMatricesCount = 0;

	glm::mat4 m_interpolatedMatrix;
	float m_param = 0.0f;

	UPtr<IModelProxy> m_model;

	/// On sequence destruction, tracker is destroyed too.
	Sequence* m_beginSequence = nullptr;

	ID m_interpolatedTransformID = 0;
	std::map<ID, float> m_trackingProgress;
};
} // namespace Core
