#pragma once

#include <map>
#include <set>

#include "glm/matrix.hpp"

#include "Model.h"
#include "Sequence.h"

namespace Core
{
class SequenceTree
{
	Ptr<Sequence> m_beginSequence;

public:
	class MatrixIterator
	{
		friend class SequenceTree;
		SequenceTree* m_tree;
		Ptr<Sequence> m_currentSequence;
		Ptr<NodeBase> m_currentMatrix;

	public:
		explicit MatrixIterator(Ptr<Sequence>& sequence);
		MatrixIterator(Ptr<Sequence>& sequence, NodePtr node);

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

		Ptr<Transformation> operator*() const;

		bool operator==(const MatrixIterator& rhs) const;
		bool operator!=(const MatrixIterator& rhs) const;

	private:
		/// Move to the next matrix (to the root).
		void advance();

		/// Move to the previous matrix (from the root).
		void withdraw();
	};

public:
	explicit SequenceTree(Ptr<NodeBase> sequence);

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
	explicit MatrixTracker(Ptr<Sequence> beginSequence, UPtr<IModelProxy> model);

	void update();

	Ptr<Sequence> getSequence() const { return m_beginSequence; }
	Ptr<Model>    getModel() const { return m_model->getModel(); }

	const glm::mat4& getInterpolatedMatrix() { return m_interpolatedMatrix; }

	unsigned fullMatricesCount() const { return m_fullMatricesCount; }

	float getParam() const { return m_param; }

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

	ID getInterpolatedTransformID() const { return m_interpolatedTransformID; }

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
	Ptr<Sequence>     m_beginSequence = nullptr;

	ID m_interpolatedTransformID = 0;
	std::map<ID, float> m_trackingProgress;
};
}
