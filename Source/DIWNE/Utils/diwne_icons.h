#pragma once

/**
 * \brief pin icon shape type
 * used in WorkspaceCorePin::content(), file WorkspaceElementWitCoreData.cpp::379
 * drawn by Diwne::DrawIcon, method Diwne::DrawIconXXXX, file Diwne.cpp
 */
enum IconType : unsigned int
{
	NoIcon,
	Circle,
	Rectangle,
	TriangleLeft,
	TriangleRight,
	TriangleDownLeft,
	TriangleDownRight,
	GrabDownLeft,
	GrabDownRight,
	Cross,
	Hyphen,
	Stop,         ///< Black Square For Stop (U+23F9)
	Pause,        ///< Double Vertical Bar (U+23F8)
	SkipBack,     ///< |< vertical bar followed by the left arrow
	SkipBack2,    ///< "<|" left arrow followed by the vertical bar
	SkipForward,  ///< ">|" right arrow followed by the vertical bar
	SkipForward2, ///< |> vertical bar followed by the right arrow
	Rewind,       ///< Black Left-Pointing Double Triangle (U+23EA)
	FastForward,  ///< Black Right-Pointing Double Triangle (U+23E9)
	AtFrom,       // todo, now a synonym to the FastForward
	AtTo,         // todo, now a synonym to the Rewind
};
