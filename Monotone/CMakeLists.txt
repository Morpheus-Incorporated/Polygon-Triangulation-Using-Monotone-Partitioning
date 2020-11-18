# Create the Monotone interface library
add_library(Monotone INTERFACE)

# Set autodiff compilation features to be propagated to client code.
target_compile_features(Monotone INTERFACE cxx_std_17)

# target_include_directories(Monotone INTERFACE ${CMAKE_INSTALL_INCLUDEDIR})
# Add the include paths to the Reaktoro target
target_include_directories(Monotone
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Install Monotone interface library
install(TARGETS Monotone)

# EXPORT autodiffTargets

# Install autodiff header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/Monotone
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT headers
    PATTERN "CMakeLists.txt" EXCLUDE)
