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

/// Cannot outlive the sequence.
struct TransformInfo
{
	bool isExternal = false;

	/// Sequence which contains the transform.
	Sequence* sequence = nullptr;
};

class SequenceTree
{
	Sequence* m_beginSequence;

public:
	/// Iterator for traversing sequence tree.
	///
	/// Goes from leaf to root, skipping empty sequences.
	class MatrixIterator
	{
		friend class SequenceTree;

	public:
		explicit MatrixIterator(Sequence* sequence);
		MatrixIterator(Sequence* sequence, Ptr<Node> node);

		MatrixIterator(const MatrixIterator& mt);

		/// \returns Non-owned pointer to the current sequence.
		///     Never null.
		Sequence* getSequence() const;

		const TransformInfo& transformInfo() const
		{
			return m_info;
		}

		/// \returns Non-owned pointer to the all matrices from start to the root,
		///     note that Ptr<Node> may points to operator with matrix output,
		///     not only to transformation.
		std::vector<Ptr<Node>> collect();

		std::pair<std::vector<Ptr<Node>>, std::vector<TransformInfo>> collectWithInfo();

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

		/// Sets m_currentMatrix to nullptr.
		void invalidate();

		SequenceTree* m_tree;
		/// Current matrix info.
		TransformInfo m_info;
		Ptr<Node> m_currentMatrix;
	};

public:
	explicit SequenceTree(Ptr<Node> sequence);

	/**
	 * \return Iterator which points to starting sequence and its last matrix.
	 */
	MatrixIterator begin();

	/**
	 * \return Points to the root sequence and matrix is nullptr,
	 *     so it is not possible to decrement or dereference it (as any other STL iterator).
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

struct TrackingResult
{
	std::size_t fullMatricesCount = 0;
	ID interpolatedTransformID = 0;
	glm::mat4 interpolatedMatrix{1.0f};
	std::map<ID, float> trackingProgress;
	std::map<ID, TransformInfo> transformInfo;

	void reset()
	{
		trackingProgress.clear();
		transformInfo.clear();
	}
};

enum class TrackingDirection;

class MatrixTracker final
{
public:
	MatrixTracker() = default;

	MatrixTracker(Sequence* beginSequence, TrackingDirection direction, std::vector<UPtr<IModelProxy>> model);

	void update();

	Ptr<Sequence> getSequence() const;
	ID getSequenceID() const;

	std::vector<Ptr<Model>> getModels() const;

	const glm::mat4& getInterpolatedMatrix() const
	{
		return m_state.interpolatedMatrix;
	}

	unsigned fullMatricesCount() const
	{
		return m_state.fullMatricesCount;
	}

	float getParam() const
	{
		return m_param;
	}

	bool setParam(float param);

	const std::map<Core::ID, float>& getTrackingProgress() const
	{
		return m_state.trackingProgress;
	}

	float getTrackingProgressForNode(ID transformID) const
	{
		if (!m_state.trackingProgress.contains(transformID))
		{
			return 0.0f;
		}

		return m_state.trackingProgress.at(transformID);
	}

	ID getInterpolatedTransformID() const
	{
		return m_state.interpolatedTransformID;
	}

	const TrackingResult& result() const
	{
		return m_state;
	}

private:
	/// \pre m_beginSequence is set
	void track();

	void handleEdgeCases(float& interpParam, const std::vector<Ptr<Node>>& matrices,
	                     const std::vector<TransformInfo>& info);

	/// Public interface is in the GraphManager.
	void stop();

	void setModelTransform();

	TrackingResult m_state;

	float m_param = 0.0f;

	std::vector<UPtr<IModelProxy>> m_models;

	TrackingDirection m_direction;

	/// On sequence destruction, tracker is destroyed too.
	Sequence* m_beginSequence = nullptr;
};
} // namespace Core
