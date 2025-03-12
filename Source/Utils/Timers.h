/**
 * \file
 * \brief Defines the Timer, CPUTimer, and GPUTimer classes for performance measurement, as well as utility for buffer
 * release.
 * \author Matvei Korikov <korikmat@fel.cvut.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#pragma once

#include <memory>
#include <string>

#include "glad/glad.h"

#include "GLFW/glfw3.h"

/**
 * \typedef TimerPtr
 * \brief Alias for a shared pointer to a Timer object.
 */
class Timer;
using TimerPtr = std::shared_ptr<Timer>;

/**
 * \class Timer
 * \brief Abstract base class for measuring execution time and performance metrics.
 *
 * The Timer class provides a generic interface for timing operations.
 * Derived classes must implement methods for starting, stopping, and retrieving timing information.
 */
class Timer
{
public:
	/**
	 * \brief Constructor.
	 * \param name Name of the timer.
	 */
	Timer(std::string name) : name(std::move(name)) {}

	/**
	 * \brief Virtual destructor.
	 */
	virtual ~Timer() = default;

	/**
	 * \brief Starts the timer.
	 * \param sync Whether to synchronize with external operations before starting.
	 */
	virtual void start(bool sync = false) = 0;

	/**
	 * \brief Stops the timer.
	 * \param sync Whether to synchronize with external operations before stopping.
	 */
	virtual void stop(bool sync = false) = 0;

	/**
	 * \brief Retrieves the elapsed time for the most recent interval.
	 * \return The elapsed time in seconds.
	 */
	virtual double get() = 0;

	/**
	 * \brief Retrieves the average time over all measured intervals.
	 * \return The average time in seconds.
	 */
	virtual double getAverage() = 0;

	/**
	 * \brief Retrieves the number of recorded intervals.
	 * \return The interval count.
	 */
	[[nodiscard]] virtual unsigned int getCounter() const = 0;

	/**
	 * \brief Resets the timer, clearing all recorded data.
	 */
	virtual void reset() = 0;

	/**
	 * \brief Retrieves the name of the timer.
	 * \return A reference to the timer name.
	 */
	[[nodiscard]] const std::string& getName() const
	{
		return name;
	}

protected:
	std::string name;                 ///< Name of the timer.
	double timeStart{0};              ///< Start time of the current interval.
	double previousFrameTimeStart{0}; ///< Start time of the previous frame.
	double timeStop{0};               ///< Stop time of the current interval.
	double time{0};                   ///< Time elapsed for the current interval.
	double timeTotal{0};              ///< Total accumulated time.
	unsigned int counter{0};          ///< Number of recorded intervals.
	GLuint64 previousFrameQueryStart{0};
	GLuint queryStart{0}, queryStop{0}; ///< OpenGL query objects for timing.
	GLuint query{0};                    ///< OpenGL query object for timing.
};

/**
 * \class CPUTimer
 * \brief Concrete implementation of the Timer class for CPU-based timing.
 *
 * The CPUTimer class measures execution time on the CPU and provides methods
 * to retrieve elapsed and average times.
 */
class CPUTimer : public Timer
{
public:
	/**
	 * \brief Constructor.
	 * \param name Name of the timer.
	 */
	CPUTimer(std::string name) : Timer(std::move(name)) {}

	/**
	 * \brief Default destructor.
	 */
	~CPUTimer() override = default;

	/**
	 * \brief Starts the timer.
	 * \param sync Ignored in this implementation.
	 */
	void start(bool sync)
	{
		time = 0;
		if (sync)
			glFinish();
		previousFrameTimeStart = timeStart;
		timeStart = glfwGetTime();
	}

	/**
	 * \brief Stops the timer.
	 * \param sync Ignored in this implementation.
	 */
	void stop(bool sync)
	{
		if (sync)
			glFinish();
		timeStop = glfwGetTime();
	}

	/**
	 * \brief Retrieves the elapsed time for the most recent interval.
	 * \return The elapsed time in seconds.
	 */
	double get()
	{
		if (time == 0)
		{
			time = (timeStop < timeStart) ? timeStop - previousFrameTimeStart : timeStop - timeStart;
			timeTotal += time;
			counter++;
		}
		return time * 1000;
	}

	/**
	 * \brief Retrieves the average time over all measured intervals.
	 * \return The average time in seconds.
	 */
	double getAverage()
	{
		get();
		double const averageTime = timeTotal / counter;
		return (averageTime * 1000);
	}

	/**
	 * \brief Retrieves the number of recorded intervals.
	 * \return The interval count.
	 */
	[[nodiscard]] unsigned int getCounter() const
	{
		return counter;
	}

	/**
	 * \brief Resets the timer, clearing all recorded data.
	 */
	void reset()
	{
		time = 0;
		timeTotal = 0;
		counter = 0;
	}
};

/**
 * \class BufferRelease
 * \brief RAII utility to unbind a specific type of buffer upon construction and restore it upon destruction.
 *
 * This template class captures the currently bound buffer of a given target upon construction,
 * unbinds it, and then rebinds the previously bound buffer on destruction. Useful for ensuring
 * buffers are correctly unbound during certain operations (e.g., timing queries).
 *
 * \tparam target The OpenGL buffer target (e.g., GL_QUERY_BUFFER).
 * \tparam query The OpenGL parameter used to retrieve the current binding (e.g., GL_QUERY_BUFFER_BINDING).
 */
template <int target, int query>
class BufferRelease
{
public:
	/**
	 * \brief Constructor that unbinds the currently bound buffer if one is bound.
	 *
	 * Retrieves the currently bound buffer for the specified target via glGetIntegerv,
	 * stores it in \c buffer, and unbinds it if it is greater than 0.
	 */
	BufferRelease()
	{
		glGetIntegerv(query, &buffer);
		if (buffer > 0)
			glBindBuffer(target, 0);
	}

	/**
	 * \brief Destructor that rebinds the previously bound buffer, if it existed.
	 */
	~BufferRelease()
	{
		if (buffer > 0)
			glBindBuffer(target, buffer);
	}

private:
	GLint buffer; ///< Stores the previously bound buffer ID.
};

/**
 * \typedef QueryBufferRelease
 * \brief Convenience typedef for BufferRelease specialized for GL_QUERY_BUFFER.
 */
typedef BufferRelease<GL_QUERY_BUFFER, GL_QUERY_BUFFER_BINDING> QueryBufferRelease;

/**
 * \class GPUTimer
 * \brief Concrete implementation of the Timer class for GPU-based timing using OpenGL queries.
 *
 * The GPUTimer class uses OpenGL timestamp queries to measure the time spent on the GPU.
 * It creates query objects for start and stop timestamps and calculates elapsed time based on them.
 */
class GPUTimer : public Timer
{
public:
	/**
	 * \brief Constructor.
	 * \param name Name of the timer.
	 */
	GPUTimer(std::string name) : Timer(std::move(name)) {}

	/**
	 * \brief Destructor.
	 *
	 * Deletes the created query objects (if any).
	 */
	~GPUTimer() override
	{
		if (queryStart)
			glDeleteQueries(1, &queryStart);
		if (queryStop)
			glDeleteQueries(1, &queryStop);
	}

	/**
	 * \brief Starts the GPU timer.
	 * \param sync Whether to synchronize with external operations before starting.
	 *
	 * Creates query objects if not already created. Optionally calls glFinish() if \p sync is true.
	 * Places a timestamp query to mark the start of the measurement.
	 */
	void start(bool sync = false) override
	{
		if (queryStart == 0)
		{
			glCreateQueries(GL_TIMESTAMP, 1, &queryStart);
			glCreateQueries(GL_TIMESTAMP, 1, &queryStop);
		}
		time = 0;
		if (sync)
			glFinish();
		glGetQueryObjectui64v(queryStart, GL_QUERY_RESULT, &previousFrameQueryStart);
		glQueryCounter(queryStart, GL_TIMESTAMP);
	}

	/**
	 * \brief Stops the GPU timer.
	 * \param sync Whether to synchronize with external operations before stopping.
	 *
	 * Optionally calls glFinish() if \p sync is true.
	 * Places a timestamp query to mark the end of the measurement.
	 */
	void stop(bool sync = false) override
	{
		if (sync)
			glFinish();
		glQueryCounter(queryStop, GL_TIMESTAMP);
	}

	/**
	 * \brief Retrieves the elapsed GPU time for the most recent interval.
	 * \return The elapsed time in seconds.
	 *
	 * Reads back timestamp query results to compute elapsed GPU time.
	 * Accumulates this time into \c timeTotal and increments \c counter.
	 * The returned value is converted to seconds (1e-9 for nanoseconds).
	 */
	double get() override
	{
		if ((time == 0) && (queryStart > 0) && (queryStop > 0))
		{
			// Unbind query buffer if bound
			QueryBufferRelease queryBufferRelease;

			GLuint64 startTime = 0, stopTime = 0;
			glGetQueryObjectui64v(queryStart, GL_QUERY_RESULT, &startTime);
			glGetQueryObjectui64v(queryStop, GL_QUERY_RESULT, &stopTime);
			time = (stopTime < startTime) ? stopTime - previousFrameQueryStart : stopTime - startTime;
			timeTotal += time;
			counter++;
		}
		return time * 0.000001f;
	}

	/**
	 * \brief Retrieves the average GPU time over all measured intervals.
	 * \return The average time in seconds.
	 *
	 * Uses \c get() to ensure the last interval is accounted for, then returns
	 * timeTotal / counter (converted to seconds).
	 */
	double getAverage() override
	{
		get();
		double const averageTime = timeTotal / counter;
		return (averageTime * 0.000001f);
	}

	/**
	 * \brief Retrieves the number of recorded intervals.
	 * \return The interval count.
	 */
	unsigned int getCounter() const override
	{
		return counter;
	}

	/**
	 * \brief Resets the GPU timer, clearing all recorded data.
	 */
	void reset() override
	{
		timeTotal = 0;
		counter = 0;
	}

	/**
	 * \brief Returns the OpenGL query ID for start or stop query.
	 * \param start If true, returns \c queryStart; otherwise returns \c queryStop.
	 * \return The OpenGL query object ID.
	 */
	GLuint getId(bool start) const
	{
		return start ? queryStart : queryStop;
	}
};
