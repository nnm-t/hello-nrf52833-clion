set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.19)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-as)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CPU_FLAGS "-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")

project(hello-nrf52833-clion C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(PROJ_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(NRF52_LINKER_SCRIPT ${PROJ_DIR}/generic_gcc_nrf52.ld)

set(SDK_ROOT $ENV{SDK_ROOT})
message("SDK root directory: ${SDK_ROOT}")

include_directories(
        ${PROJ_DIR}
        ${SDK_ROOT}/components
        ${SDK_ROOT}/components/boards
        ${SDK_ROOT}/components/drivers_nrf/nrf_soc_nosd
        ${SDK_ROOT}/components/libraries/atomic
        ${SDK_ROOT}/components/libraries/balloc
        ${SDK_ROOT}/components/libraries/bsp
        ${SDK_ROOT}/components/libraries/delay
        ${SDK_ROOT}/components/libraries/experimental_section_vars
        ${SDK_ROOT}/components/libraries/log
        ${SDK_ROOT}/components/libraries/log/src
        ${SDK_ROOT}/components/libraries/memobj
        ${SDK_ROOT}/components/libraries/ringbuf
        ${SDK_ROOT}/components/libraries/stderr
        ${SDK_ROOT}/components/libraries/timer
        ${SDK_ROOT}/components/libraries/util
        ${SDK_ROOT}/components/libraries/uart
        ${SDK_ROOT}/components/softdevice/mbr/headers
        ${SDK_ROOT}/components/softdevice/s140/headers
        ${SDK_ROOT}/components/toolchain/cmsis/include
        ${SDK_ROOT}/external/fprintf
        ${SDK_ROOT}/integration/nrfx
        ${SDK_ROOT}/modules/nrfx
        ${SDK_ROOT}/modules/nrfx/drivers
        ${SDK_ROOT}/modules/nrfx/drivers/include
        ${SDK_ROOT}/modules/nrfx/hal
        ${SDK_ROOT}/modules/nrfx/mdk
        ${SDK_ROOT}/modules/nrfx/soc
)

list(APPEND SOURCE_FILES
        ${PROJ_DIR}/main.c
        ${PROJ_DIR}/cppmain.cpp
)

list(APPEND LIB_FILES
        "${SDK_ROOT}/external/nrf_cc310/lib"
        "${SDK_ROOT}/external/micro-ecc/nrf52hf_armgcc/armgcc"
        "${SDK_ROOT}/external/nrf_oberon/lib/nrf52"
)

add_definitions(-DBLE_STACK_SUPPORT_REQD
        -DBOARD_PCA10100
        -DCONFIG_GPIO_AS_PINRESET
        -DFLOAT_ABI_HARD
        -DMBEDTLS_CONFIG_FILE=\"nrf_crypto_mbedtls_config.h\"
        -DNRF52833_XXAA
        -DNRF_CRYPTO_MAX_INSTANCE_COUNT=1
        -DNRF_SD_BLE_API_VERSION=6
        -DS140
        -DSOFTDEVICE_PRESENT
        -DSWI_DISABLE0
        -DuECC_ENABLE_VLI_API=0
        -DuECC_OPTIMIZATION_LEVEL=3
        -DuECC_SQUARE_FUNC=0
        -DuECC_SUPPORT_COMPRESSED_POINT=0
        -DuECC_VLI_NATIVE_LITTLE_ENDIAN=1
)

set(COMMON_FLAGS "-mthumb -mabi=aapcs -Wall -Werror -O3 -g3 -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin -fshort-enums ${CPU_FLAGS}")

set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS "-g3 mthumb -mabi=aapcs ${CPU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-mthumb -mabi=aapcs -std=gnu++14 -std=c11 -L\"${SDK_ROOT}/modules/nrfx/mdk\" -T${NRF52_LINKER_SCRIPT} ${CPU_FLAGS} -Wl,--gc-sections --specs=nano.specs -lc -lnosys -lm")

set(TARGET_NAME ${PROJECT_NAME})
add_executable(${TARGET_NAME}.out ${SOURCE_FILES} ${NRF52_LINKER_SCRIPT})

add_custom_command(TARGET ${TARGET_NAME}.out POST_BUILD
        POST_BUILD
        COMMAND arm-none-eabi-size ${TARGET_NAME}.out
        COMMAND arm-none-eabi-objcopy -O binary ${TARGET_NAME}.out "${TARGET_NAME}.bin"
        COMMAND arm-none-eabi-objcopy -O ihex ${TARGET_NAME}.out "${TARGET_NAME}.hex"
        COMMENT "post build steps for ${target_name}")