function(fetch_library libname url tag)
  include(FetchContent)

  FetchContent_Declare(
    ${libname}
    GIT_REPOSITORY ${url}
    GIT_TAG ${tag}
    GIT_SHALLOW TRUE
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  )

  FetchContent_MakeAvailable(${libname})
endfunction()
