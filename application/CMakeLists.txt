cmake_minimum_required(VERSION 3.0.0)
project(TFG_VkRay VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CTest)
enable_testing()

set(MY_PROJECT_DEPENDENCIES Vulkan glfw3 GLM)   # Añadir dependencias necesarias

# Buscar e instalar las dependencias anteriores
# Si no existe, se descargará vcpkg en la carpeta de usuario.
if(NOT DEFINED ${CMAKE_TOOLCHAIN_FILE})
    if(NOT DEFINED ENV{VCPKG_ROOT})
        if(WIN32)
            set(VCPKG_ROOT $ENV{HOMEDRIVE}$ENV{HOMEPATH}/vcpkg)
        else()
            set(VCPKG_ROOT $ENV{HOME}/.vcpkg)
        endif()
    else()
        set(VCPKG_ROOT $ENV{VCPKG_ROOT})
    endif()

    if(NOT EXISTS ${VCPKG_ROOT})
        message("Cloning vcpkg in ${VCPKG_ROOT}")
        execute_process(COMMAND git clone https://github.com/Microsoft/vcpkg.git ${VCPKG_ROOT})
        # If a reproducible build is desired (and potentially old libraries are # ok), uncomment the
        # following line and pin the vcpkg repository to a specific githash.
        # execute_process(COMMAND git checkout 745a0aea597771a580d0b0f4886ea1e3a94dbca6 WORKING_DIRECTORY ${VCPKG_ROOT})
    else()
        # The following command has no effect if the vcpkg repository is in a detached head state.
        message("Auto-updating vcpkg in ${VCPKG_ROOT}")
        execute_process(COMMAND git pull WORKING_DIRECTORY ${VCPKG_ROOT})
    endif()

    if(NOT EXISTS ${VCPKG_ROOT}/README.md)
        message(FATAL_ERROR "***** FATAL ERROR: Could not clone vcpkg *****")
    endif()

    if(WIN32)
        set(BOOST_INCLUDEDIR ${VCPKG_ROOT}/installed/x86-windows/include) # NOTE la implementación inicial utilizaba boost. Quizás haga falta esto, lo dejaré por si acaso.
        set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg.exe)
        set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.bat)
    else()
        set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg)
        set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.sh)
    endif()

    if(NOT EXISTS ${VCPKG_EXEC})
        message("Bootstrapping vcpkg in ${VCPKG_ROOT}")
        execute_process(COMMAND ${VCPKG_BOOTSTRAP} WORKING_DIRECTORY ${VCPKG_ROOT})
    endif()

    if(NOT EXISTS ${VCPKG_EXEC})
        message(FATAL_ERROR "***** FATAL ERROR: Could not bootstrap vcpkg *****")
    endif()

    set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")

    message(STATUS "***** Checking project third party dependencies in ${VCPKG_ROOT} *****")
    execute_process(COMMAND ${VCPKG_EXEC} install ${MY_PROJECT_DEPENDENCIES} --triplet x64-windows WORKING_DIRECTORY ${VCPKG_ROOT})
else()
  message(STATUS "***** Checking project third party dependencies in ${VCPKG_ROOT} *****")
  execute_process(COMMAND ${VCPKG_ROOT}/vcpkg.exe install ${MY_PROJECT_DEPENDENCIES} --triplet x64-windows WORKING_DIRECTORY ${VCPKG_ROOT})
endif()


# Buscar librerías
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(TFG_VkRay src/main.cpp)

# Especificar ¿Ejecutable? ¿Proyecto? utiliza Vulkan. Cambiar vulkaninfo por lo correspondiente.
target_compile_definitions(TFG_VkRay PRIVATE VK_USE_PLATFORM_WIN32_KHR)
target_link_libraries(TFG_VkRay Vulkan::Vulkan PRIVATE glfw GLM::glm)

include(CPack)
