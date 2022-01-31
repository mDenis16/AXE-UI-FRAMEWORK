#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <Windows.h>
#include <iostream>
#include <cmath>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <stack_strings.hpp>
#include <config.h>

#include "framework_internal.h"


#include "elements/element.h"
#include "elements/window.h"
#include "elements/tab.h"
#include "elements/sub_tab.h"
#include "elements/child_window.h"
#include "elements/checkbox.h"
#include "elements/keybind.h"
#include "elements/key_selector.h"
#include "elements/colorpicker.h"
#include "elements/text_input.h"
#include "elements/small_text_input.h"
#include "elements/combobox.h"
#include "elements/combo_item.h"
#include "elements/multibox.h"
#include "elements/slider.h"
#include "elements/config_item.h"
#include "elements/config_box.h"
#include "elements/button.h"
#include "elements/panel.h"
#include "elements/panel_container.h"
#include "elements/element_extender.h"
#include "elements/bezier_editor.h"
#include "elements/inventory_changer.h"
#include "elements/inventory_item.h"


#define ICON_PLUS "a"
#define ICON_OK_SHIELD "b"
#define ICON_COLOR_PALLETE "c"
#define ICON_LEGITBOT "d"
#define VISUALS_ENEMIES "e"
#define VISUALS_TEAMMATES "f"
#define ICON_COPY "g"
#define ICON_PASTE "h"
#define ICON_STACK "i"
#define ICON_DOWNLOAD "j"
#define ICON_UPLOAD "k"
#define ICON_UNDO "l"
#define ICON_REDO "m"
#define ICON_WRENCH "n"
#define ICON_EQUALIZER "o"
#define ICON_BIN "p"
#define ICON_RAGEBOT "q"
#define ICON_ANTIAIM "r"
#define ICON_TAZER "s"
#define ICON_CLOUD "t"
#define ICON_WORLD "u"
#define ICON_VISUALS "v"
#define ICON_VISUALS_BLOCKED "w"
#define ICON_WARNING "x"
#define ICON_NOTIFICATION "y"
#define ICON_QUESTION "z"
#define ICON_INFO "A"
#define ICON_SETTINGS "B"
#define ICON_FIRE "C"
#define ICON_LOCAL "D"
#define ICON_MOVEMENT "E"
