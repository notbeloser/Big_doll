deps_config := \
	/home/notbeloser/esp/Big_doll/components/arduino/Kconfig \
	/home/notbeloser/esp/esp-idf/components/aws_iot/Kconfig \
	/home/notbeloser/esp/esp-idf/components/bt/Kconfig \
	/home/notbeloser/esp/esp-idf/components/esp32/Kconfig \
	/home/notbeloser/esp/esp-idf/components/ethernet/Kconfig \
	/home/notbeloser/esp/esp-idf/components/fatfs/Kconfig \
	/home/notbeloser/esp/esp-idf/components/freertos/Kconfig \
	/home/notbeloser/esp/esp-idf/components/log/Kconfig \
	/home/notbeloser/esp/esp-idf/components/lwip/Kconfig \
	/home/notbeloser/esp/esp-idf/components/mbedtls/Kconfig \
	/home/notbeloser/esp/esp-idf/components/openssl/Kconfig \
	/home/notbeloser/esp/esp-idf/components/spi_flash/Kconfig \
	/home/notbeloser/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/notbeloser/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/notbeloser/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/notbeloser/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
