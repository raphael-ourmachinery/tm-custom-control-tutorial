#include "ui_custom_controls_loader.h"

#include <foundation/api_registry.h>
#include <foundation/carray.inl>
#include <foundation/log.h>
#include <foundation/profiler.h>
#include <foundation/unicode.h>

#include <plugins/ui/draw2d.h>
#include <plugins/ui/ui.h>


struct tm_api_registry_api *tm_global_api_registry;

struct tm_allocator_api *tm_allocator_api;
struct tm_draw2d_api *tm_draw2d_api;
struct tm_font_api *tm_font_api;
struct tm_logger_api *tm_logger_api;
struct tm_profiler_api *tm_profiler_api;
struct tm_temp_allocator_api *tm_temp_allocator_api;
struct tm_ui_api *tm_ui_api;
struct tm_unicode_api *tm_unicode_api;

extern void load_custom_tab(struct tm_api_registry_api *reg, bool load);
extern void load_custom_controls(struct tm_api_registry_api *reg, bool load);

TM_DLL_EXPORT void tm_load_plugin(struct tm_api_registry_api *reg, bool load)
{
    tm_global_api_registry = reg;
    tm_allocator_api = reg->get(TM_ALLOCATOR_API_NAME);
    tm_draw2d_api = reg->get(TM_DRAW2D_API_NAME);
    tm_font_api = reg->get(TM_FONT_API_NAME);
    tm_logger_api = reg->get(TM_LOGGER_API_NAME);
    tm_profiler_api = reg->get(TM_PROFILER_API_NAME);
    tm_temp_allocator_api = reg->get(TM_TEMP_ALLOCATOR_API_NAME);
    tm_ui_api = reg->get(TM_UI_API_NAME);
    tm_unicode_api = reg->get(TM_UNICODE_API_NAME);

    load_custom_controls(reg, load);
    load_custom_tab(reg, load);
}
