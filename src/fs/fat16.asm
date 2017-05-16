dw 0xF0FF, 0xFFFF
times (FAT_SIZE * 512) - ($ - $$) db 0 ;FAT table

times (ENTRIES * 32) db 0 ;root directory
times (CLUSTERS * 512) db 0 ;data area
