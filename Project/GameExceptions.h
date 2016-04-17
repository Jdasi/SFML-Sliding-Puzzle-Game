#pragma once

#include "GameException.h"

namespace Utility
{

DefinePuzzleGameException(MoveGenerationException);
DefinePuzzleGameException(FindBlankSpaceException);
DefinePuzzleGameException(ProfileStatConversionException);
DefinePuzzleGameException(BackgroundConversionException);
DefinePuzzleGameException(PuzzleEnumerationException);
DefinePuzzleGameException(ExecutablePathException);
DefinePuzzleGameException(FileEnumerationException);
DefinePuzzleGameException(ProfileAccessException);

} // Namespace Utility.
