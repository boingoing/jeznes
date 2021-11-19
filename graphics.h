// This file should be included underneath:
// #pragma bss-name(push, "BSS")

const char bg_palette[] = {
    0x0f,0x00,0x10,0x30,0x0f,0x00,0x26,0x31,0x0f,0x06,0x16,0x26,0x0f,0x09,0x19,0x29
};

const char sprite_palette[] = {
    0x0f,0x30,0x15,0x28,0x0f,0x12,0x22,0x32,0x0f,0x13,0x23,0x33,0x0f,0x14,0x24,0x34
};

const unsigned char playfield_screen[] = {
    0x01,0x70,0x01,0x20,0x41,0x43,0x01,0x1b,0x42,0x70,0x70,0x40,0x00,0x01,0x1b,0x50,
    0x70,0x70,0x40,0x00,0x01,0x1b,0x50,0x70,0x70,0x40,0x00,0x01,0x1b,0x50,0x70,0x70,
    0x40,0x00,0x01,0x1b,0x50,0x70,0x70,0x40,0x00,0x01,0x1b,0x50,0x70,0x70,0x40,0x00,
    0x01,0x1b,0x50,0x70,0x70,0x40,0x00,0x01,0x1b,0x50,0x70,0x70,0x40,0x00,0x01,0x1b,
    0x50,0x70,0x70,0x40,0x00,0x01,0x1b,0x50,0x70,0x70,0x40,0x00,0x01,0x1b,0x50,0x70,
    0x70,0x40,0x00,0x01,0x1b,0x50,0x70,0x70,0x40,0x00,0x01,0x1b,0x50,0x70,0x70,0x40,
    0x00,0x01,0x1b,0x50,0x70,0x70,0x40,0x00,0x01,0x1b,0x50,0x70,0x70,0x40,0x00,0x01,
    0x1b,0x50,0x70,0x70,0x40,0x00,0x01,0x1b,0x50,0x70,0x70,0x40,0x00,0x01,0x1b,0x50,
    0x70,0x70,0x40,0x00,0x01,0x1b,0x50,0x70,0x70,0x40,0x00,0x01,0x1b,0x50,0x70,0x70,
    0x40,0x00,0x01,0x1b,0x50,0x70,0x70,0x61,0x53,0x01,0x1b,0x62,0x70,0x70,0x40,0x60,
    0x01,0x1b,0x50,0x70,0x70,0x40,0x0f,0x08,0x19,0x08,0x0f,0x60,0x1e,0x1e,0x60,0x06,
    0x0f,0x08,0x04,0x15,0x36,0x60,0x1e,0x1e,0x60,0x16,0x06,0x12,0x15,0x08,0x60,0x01,
    0x03,0x50,0x70,0x70,0x40,0x0f,0x0c,0x19,0x08,0x16,0x60,0x1e,0x1e,0x60,0x17,0x04,
    0x15,0x0a,0x08,0x17,0x60,0x1e,0x1e,0x60,0x60,0x1e,0x01,0x07,0x50,0x70,0x70,0x40,
    0x60,0x01,0x1b,0x50,0x70,0x70,0x51,0x53,0x01,0x1b,0x52,0x70,0x01,0x40,0x6a,0x5a,
    0x01,0x05,0x9a,0x66,0x55,0x01,0x05,0x99,0x66,0x55,0x01,0x05,0x99,0x66,0x55,0x01,
    0x05,0x99,0x66,0x55,0x01,0x05,0x99,0xa6,0xa5,0x01,0x05,0xa9,0xae,0xaf,0x01,0x05,
    0xab,0x0a,0x05,0x01,0x05,0x05,0x01,0x00
};
