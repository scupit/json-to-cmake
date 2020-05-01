#ifndef CMAKECUSTOMFUNCTIONS_HPP
#define CMAKECUSTOMFUNCTIONS_HPP

namespace CmakeCustomFunctions {
	const char* const TOGGLEABLE_LIBRARY_CREATOR = 
"function( createLibraryWithTypeToggle libName libType libSources )"
  "\n\tif( libType STREQUAL STATIC )"
    "\n\t\tset( oppositeLibType SHARED )"
  "\n\telseif( libType STREQUAL SHARED )"
    "\n\t\tset( oppositeLibType STATIC )"
  "\n\tendif()"

  "\n\tif( NOT ${libName}_LIB_TYPE )"
    "\n\t\tset( ${libName}_LIB_TYPE ${libType} CACHE STRING "${libName} library type" FORCE )"
  "\n\tendif()"

  "\n\tset_property( CACHE ${libName}_LIB_TYPE PROPERTY STRINGS "${libType}" "${oppositeLibType}" )"

  "\n\tif( ${libName}_LIB_TYPE STREQUAL ${libType} )"
    "\n\t\tadd_library( ${libName} ${libType} ${libSources} )"
  "\n\telseif( ${libName}_LIB_TYPE STREQUAL ${oppositeLibType} )"
    "\n\t\tadd_library( ${libName} ${oppositeLibType} ${libSources} )"
  "\n\tendif()"
"endFunction()"
}

#endif