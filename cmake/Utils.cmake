function(read_filelist FILENAME RESULT_NAME)
  file(READ ${FILENAME} CONTENTS)
  # Convert file contents into a CMake list (where each element in the list
  # is one line of the file)
  string(REGEX REPLACE ";" "\\\\;" CONTENTS "${CONTENTS}")
  string(REGEX REPLACE "\n" ";" CONTENTS "${CONTENTS}")
  # Ignore all lines starting with # (comments)
  set(RESULT "")
  foreach(f ${CONTENTS})
    if(NOT "${f}" MATCHES "^#.*")
      list(APPEND RESULT "${f}")
    endif()
  endforeach()
  set(${RESULT_NAME} ${RESULT} PARENT_SCOPE)
endfunction(read_filelist)

# Deploy a file from the source directory into a subdirectory of the build
# directory. Relative paths for FILENAME are evaluated with respect to the
# current source directory, and relative paths for DESTDIR are evaluated with
# respect to the current build directory.
function(deploy_file FILENAME DESTDIR)
  get_filename_component(FILEDIR "${FILENAME}" DIRECTORY)
  file(COPY "${FILENAME}" DESTINATION "${DESTDIR}/${FILEDIR}")
endfunction(deploy_file)

# Automatically set source group based on folder
function(auto_source_group SOURCES)

  foreach(FILE ${SOURCES})
      get_filename_component(PARENT_DIR "${FILE}" PATH)

      # skip src or include and changes /'s to \\'s
      string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" GROUP "${PARENT_DIR}")
      string(REPLACE "/" "\\\\" GROUP "${GROUP}")
      string(REGEX REPLACE "^\\\\" "" GROUP "${GROUP}")

      source_group("${GROUP}" FILES "${FILE}")
  endforeach()
endfunction(auto_source_group)

# Force static runtime for MSVC
function(msvc_use_static_runtime)
  if(MSVC)
      set(variables
          CMAKE_CXX_FLAGS_DEBUG
          CMAKE_CXX_FLAGS_MINSIZEREL
          CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_RELWITHDEBINFO
      )
      foreach(variable ${variables})
          if(${variable} MATCHES "/MD")
              string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
              set(${variable} "${${variable}}"  PARENT_SCOPE)
          endif()
      endforeach()
  endif()
endfunction(msvc_use_static_runtime)

# Set output directory of target, ignoring debug or release
function(set_output_directory target dir)
	set_target_properties(${target} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${dir}
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${dir}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${dir}
		LIBRARY_OUTPUT_DIRECTORY ${dir}
		LIBRARY_OUTPUT_DIRECTORY_DEBUG ${dir}
		LIBRARY_OUTPUT_DIRECTORY_RELEASE ${dir}
	)
endfunction(set_output_directory)

# Set a default build type to release if none was specified
function(set_default_configuration_release)
    if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug;Release;MinSizeRel;RelWithDebInfo") # multiconfig generator?
        set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
	elseif(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	  message(STATUS "Setting build type to 'Release' as none was specified.")
	  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE )
	endif()
endfunction(set_default_configuration_release)

function(format_gencode_flags flags out)
  foreach(ver ${flags})
    set(${out} "${${out}}-gencode arch=compute_${ver},code=sm_${ver};")
  endforeach()
  set(${out} "${${out}}" PARENT_SCOPE)
endfunction(format_gencode_flags flags)
