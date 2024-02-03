// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

// ^That was automatically generated by Visual Studio

// I use Pascal case for almost everything, if you don't like it 1v1 me in the parking lot :smile:

// Framework defines
#define FRAMEWORK_VERSION 1.0.0
#define FRAMEWORK_MAJOR_VERSION 1
#define FRAMEWORK_MINOR_VERSION 0
#define FRAMEWORK_REWORK_VERSION 0

#define FRAMEWORK_CODENAME "OmegaWare"
#define FRAMEWORK_TARGET_GAME ""
#pragma warning(disable : 5056)
static_assert(FRAMEWORK_TARGET_GAME != "", "Target game not set."); // Make sure the target game title is set

#define FRAMEWORK_OTHER 1
#define FRAMEWORK_UNREAL 0
#define FRAMEWORK_UNITY 0

// Make sure a framework is selected and only one framework is selected
#if !FRAMEWORK_OTHER && !FRAMEWORK_UNREAL && !FRAMEWORK_UNITY
#error "No framework selected"
#else

#if FRAMEWORK_OTHER && (FRAMEWORK_UNREAL || FRAMEWORK_UNITY)
#error "Multiple frameworks selected"
#endif

#if FRAMEWORK_UNREAL && (FRAMEWORK_OTHER || FRAMEWORK_UNITY)
#error "Multiple frameworks selected"
#endif

#if FRAMEWORK_UNITY && (FRAMEWORK_OTHER || FRAMEWORK_UNREAL)
#error "Multiple frameworks selected"
#endif

#endif

// Make sure a rendering API is selected and only one rendering API is selected
#define FRAMEWORK_RENDER_D3D11 1
#define FRAMEWORK_RENDER_D3D12 0
static_assert(FRAMEWORK_RENDER_D3D11 != FRAMEWORK_RENDER_D3D12, "Cannot use no or both rendering API(s)"); // Don't allow both rendering API's to be used

// Set the rendering API to be used with kiero
#if FRAMEWORK_RENDER_D3D11
#define KIERO_INCLUDE_D3D11  1
#define KIERO_INCLUDE_D3D12  0
#endif

#if FRAMEWORK_RENDER_D3D12
#define KIERO_INCLUDE_D3D11  0
#define KIERO_INCLUDE_D3D12  1
#endif

#include "Kiero/kiero.h"

// A macro to get the current source location to be used with Utils::GetLocation
#include <source_location>
#define CurrentLoc std::source_location::current()

// Start the precompiled headers
#ifndef PCH_H
#define PCH_H

#if FRAMEWORK_UNREAL // If the framework set is Unreal include the SDK.h file that includes all the SDK headers made by an SDK generator
#include "SDK.h"

// Reminder remember to incude these files in the project
// BasicTypes_Package.cpp
// CoreUObject_Package.cpp
// Engine_Package.cpp
#endif

// Include standard libraries that are used in the project
#include <thread>
#include <chrono>
#include <memory>
#include <format>
#include <string>
#include <cmath>
#include <eh.h> // I dont remember what this was for, but I think it was for a scuffed try catch block to stop crashes on memory access violations

#define _USE_MATH_DEFINES // Define math constants for things like M_PI and M_SQRT2
#include <math.h>

#include "Utils/Utils.h" // Include the Utils.h file that contains various utility functions for the framework

#include "Interfaces/EasyHook.h"

#if FRAMEWORK_UNREAL // If the framework set is Unreal include the Unreal.h file that contains the Unreal interface class that is used to interact with the Unreal Engine
#include "Interfaces/Unreal.h"
#endif

#if FRAMEWORK_UNITY
#include "Interfaces/Mono.h"
#endif

// Include ImGui and the ImGui implementation for Win32
#define IMGUI_DEFINE_MATH_OPERATORS
#pragma warning(disable : 4244) // Disable the warning for the conversion from 'float' to 'int', possible loss of data
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"

// Include the ImGui implementation for the rendering API that is being used
#if FRAMEWORK_RENDER_D3D11
#include "ImGUI/imgui_impl_dx11.h"
#endif

#if FRAMEWORK_RENDER_D3D12
#include "ImGUI/imgui_impl_dx12.h"
#endif

#include "ImGUI/Styles.h" // Include the Styles.h file that contains the ImGui styles for the framework

#include "GUI/Custom.h" // Include the Custom.h file that contains the custom ImGui widgets for the framework
#include "GUI/GUI.h" // Include the GUI.h file that contains the GUI class that is used to create the framework's menu

#define DEG2RAD(deg) deg * M_PI / 180 // A macro to convert degrees to radians
#define RAD2DEG(rad) rad * 180.0 / M_PI; // A macro to convert radians to degrees

namespace Cheat
{
	static const std::string Framework = FRAMEWORK_CODENAME; // Set the framework name to the codename
	static const std::string Game = FRAMEWORK_TARGET_GAME; // Set the game name to the target game

	static const std::string Title = Framework + " (" + Game + ")"; // Set the title to the framework name and the game name

	static bool bShouldRun = true; // A boolean to check if the cheat should run or exit
	static DWORD dwThreadID = NULL; // A DWORD to store the thread ID of the cheat thread
	static HMODULE hModule = NULL; // A HMODULE to store the module handle of the cheat used for unloading the module

	static constexpr DWORD dwMenuKey = VK_INSERT; // A DWORD to store the key that opens and closes the menu
	static constexpr DWORD dwUnloadKey = VK_END; // A DWORD to store the key that unloads the cheat
	static constexpr DWORD dwConsoleKey = VK_HOME; // A DWORD to store the key that opens and closes the console

	inline std::unique_ptr<Console> console = std::make_unique<Console>(false, Title);  // A unique pointer to the console class that is used to create the console for the framework

	#if FRAMEWORK_UNREAL // If the framework set is Unreal create a unique pointer to the Unreal interface class
	inline std::unique_ptr<Unreal> unreal = std::make_unique<Unreal>();
	#endif

	#if FRAMEWORK_UNITY // If the framework set is Unity create a unique pointer to the Mono interface class
	inline Mono mono = Mono::Instance(); // I would use a unique pointer but the class is already setup as a singlton and I need to call the destructor to clean up the mono domain
	#endif
}

#include "Features/Feature.h" // Include the Feature.h file that contains the Feature class that is used to create the features for the framework

// https://stackoverflow.com/questions/13048301/pointer-to-array-of-base-class-populate-with-derived-class
inline std::vector<std::unique_ptr<Feature>> Features; // A vector of unique pointers to the Feature class that is used to store the features for the framework

#endif //PCH_H