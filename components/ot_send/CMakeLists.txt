idf_component_register(SRCS "common.c" "send_udp.c" "send_ping.c" "init.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES "esp_event" "esp_netif" "esp_ot_cli_extension"
                    REQUIRES "openthread" "nvs_flash")
