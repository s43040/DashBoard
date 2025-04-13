idf_component_register(
    SRCS "MizzouRacingLogoBlackBackground.c" "MizzouRacingLogoTransparent.c" "TylerTheCreaterMugShotEdited.c" "can.c" "font_roboto_24_4bpp.c" "font_roboto_120_4bpp.c" "font_roboto_72_4bpp.c" "waveshare_rgb_lcd_port.c" "main.c" "lvgl_port.c" "Initializer.c"
    INCLUDE_DIRS ".")
idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_options(${lvgl_lib} PRIVATE -Wno-format)
include_directories(${CMAKE_SOURCE_DIR}/components/RadioLib/src)
