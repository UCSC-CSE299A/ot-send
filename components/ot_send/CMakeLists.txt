idf_component_register(SRCS "connect.c" "send_udp.c" "init.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES "esp_event" "esp_netif" "esp_ot_cli_extension" "esp_driver_gpio" "espressif__led_strip"
                    REQUIRES "openthread" "nvs_flash")
