#pragma once

#include <exception>

namespace Utility
{

class PuzzleGameException : public std::exception {};

#define DefinePuzzleGameException(Name) \
    class Name : public PuzzleGameException {}

} // Namespace Utility.
