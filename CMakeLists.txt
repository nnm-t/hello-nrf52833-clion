set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.19)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(AS arm-none-eabi-as)
set(AR arm-none-eabi-ar)
set(OBJCOPY arm-none-eabi-objcopy)
set(OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(hello-nrf52833-clion C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(PROJ_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LINKER_SCRIPT ${PROJ_DIR}/generic_gcc_nrf52.ld)

set(SDK_ROOT $ENV{SDK_ROOT})
message("SDK root directory: ${SDK_ROOT}")

include_directories(
        ${PROJ_DIR}/Inc
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
        ${SDK_ROOT}/components/libraries/strerror
        ${SDK_ROOT}/components/libraries/timer
        ${SDK_ROOT}/components/libraries/util
        ${SDK_ROOT}/components/libraries/uart
        # ${SDK_ROOT}/components/softdevice/mbr/headers
        # ${SDK_ROOT}/components/softdevice/s140/headers
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

file(GLOB_RECURSE SOURCES
        # ${SDK_ROOT}/modules/nrfx/mdk/gcc_startup_nrf52833.S
        ${SDK_ROOT}/components/libraries/log/src/nrf_log_frontend.c
        ${SDK_ROOT}/components/libraries/log/src/nrf_log_str_formatter.c
        ${SDK_ROOT}/components/boards/boards.c
        ${SDK_ROOT}/components/libraries/util/app_error.c
        ${SDK_ROOT}/components/libraries/util/app_error_handler_gcc.c
        ${SDK_ROOT}/components/libraries/util/app_error_weak.c
        ${SDK_ROOT}/components/libraries/util/app_util_platform.c
        ${SDK_ROOT}/components/libraries/util/nrf_assert.c
        ${SDK_ROOT}/components/libraries/atomic/nrf_atomic.c
        ${SDK_ROOT}/components/libraries/balloc/nrf_balloc.c
        ${SDK_ROOT}/external/fprintf/nrf_fprintf.c
        ${SDK_ROOT}/external/fprintf/nrf_fprintf_format.c
        ${SDK_ROOT}/components/libraries/memobj/nrf_memobj.c
        ${SDK_ROOT}/components/libraries/ringbuf/nrf_ringbuf.c
        ${SDK_ROOT}/components/libraries/strerror/nrf_strerror.c
        ${SDK_ROOT}/modules/nrfx/soc/nrfx_atomic.c
        ${PROJ_DIR}/Src/*.*
        ${SDK_ROOT}/modules/nrfx/mdk/system_nrf52833.c
        )

add_definitions(
        -DBOARD_PCA10100
        -DBSP_DEFINES_ONLY
        -DCONFIG_GPIO_AS_PINRESET
        -DDEVELOP_IN_NRF52833
        -DFLOAT_ABI_HARD
        -DNRF52833_XXAA
        -DNRFX_COREDEP_DELAY_US_LOOP_CYCLES=3
        # -DMBEDTLS_CONFIG_FILE=\"nrf_crypto_mbedtls_config.h\"
        # -DNRF_CRYPTO_MAX_INSTANCE_COUNT=1
        # -DBLE_STACK_SUPPORT_REQD
        # -DNRF_SD_BLE_API_VERSION=6
        # -DS140
        # -DSOFTDEVICE_PRESENT
        # -DSWI_DISABLE0
        # -DuECC_ENABLE_VLI_API=0
        # -DuECC_OPTIMIZATION_LEVEL=3
        # -DuECC_SQUARE_FUNC=0
        # -DuECC_SUPPORT_COMPRESSED_POINT=0
        # -DuECC_VLI_NATIVE_LITTLE_ENDIAN=1
)

set(CPU_FLAGS "-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(COMMON_FLAGS "-mthumb -mabi=aapcs -Wall -Werror -O3 -g3 -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin -fshort-enums ${CPU_FLAGS}")

set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS "-g3 mthumb -mabi=aapcs ${CPU_FLAGS}")
# -Map=?
set(CMAKE_EXE_LINKER_FLAGS "-mthumb -mabi=aapcs -std=gnu++14 -std=gnu11 -L\"${SDK_ROOT}/modules/nrfx/mdk\" -T${LINKER_SCRIPT} ${CPU_FLAGS} -Wl,--gc-sections --specs=nano.specs -lc -lnosys -lm")

set(TARGET_NAME ${PROJECT_NAME})
add_executable(${TARGET_NAME}.out ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${TARGET_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${TARGET_NAME}.bin)

add_custom_command(TARGET ${TARGET_NAME}.out POST_BUILD
        COMMAND ${SIZE} ${TARGET_NAME}.out
        COMMAND ${OBJCOPY} -Oihex ${TARGET_NAME}.out ${HEX_FILE}
        COMMAND ${OBJCOPY} -Obinary ${TARGET_NAME}.out ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

set(NRFJPROG nrfjprog)
set(SDK_CONFIG_FILE ${PROJ_DIR}/Inc/sdk_config.h)
set(CMSIS_CONFIG_TOOL ${SDK_ROOT}/external_tools/cmsisconfig/CMSIS_Configration_Wizard.jar)

add_custom_target(Flash echo "Flashing: ${HEX_FILE}"
        COMMAND ${NRFJPROG} -f nrf52 --program ${HEX_FILE} --sectorerase
        COMMAND ${NRFJPROG} -f nrf52 --reset)
add_custom_target(Erase ${NRFJPROG} -f nrf52 --eraseall)
add_custom_target(SDK_Config java -jar ${CMSIS_CONFIG_TOOL} ${SDK_CONFIG_FILE})