#pragma once

#include <bf2_classes/class_manager.hpp>
#include <bf2_classes/renderer.hpp>

namespace helpers::bf2_classes
{

::bf2_classes::CClassManager* get_class_manager_address();
::bf2_classes::CRenderer* get_renderer_address();

} // namespace helpers::bf2_classes
