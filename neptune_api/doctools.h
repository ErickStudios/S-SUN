/*++

SUNLib

File Name:

	doctools.h

Summary:

	documentation tools

--*/

#include "sal.h"

#ifndef _DOC_TOOLS_H
#define _DOC_TOOLS_H

//
// general doc
//

#define prototype
#define declaration
#define important

#define IN
#define OUT

#define for_compatibility

#define note(text)

#define multi_use
#define single_use

//
// functions tools
//
#define param(name)
#define retval(c)
#define for_debug

//
// extense docummentation
//

#define dparams(...)
#define dsummary(x)
#define dretval(x)

#endif // !_DOC_TOOLS_H