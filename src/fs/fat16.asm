db 0xF0, 0xFF, 0xFF, 0xFF
times (3 * 512) - ($ - $$) db 0 ;FAT table of 3 sectors

times (256 * 32) db 0 ;root directory of 256 entries
times (128 * 1024) db 0 ;data area of 128KiB
