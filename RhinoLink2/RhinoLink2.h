#pragma once

#include <string>
#include <msclr/marshal_cppstd.h>

using namespace msclr::interop;
using namespace RhinoCaller3;


__declspec(dllexport) void* launch_rhino(void* window_handle, void* ui_ptr) {
	return Caller3::launch_rhino(window_handle, ui_ptr);
}

__declspec(dllexport) void initialize_rhino() {
	Caller3::initialize_rhino();
}

__declspec(dllexport) std::string get_object_string() {
	return marshal_as<std::string>(Caller3::get_object_string());
}

__declspec(dllexport) void run_script(std::string script) {
	Caller3::run_script(marshal_as<System::String^>(script));
}