/*************************************************
 *  
 *  EDID generator (1.3 with CEA 861)
 *
 *  (c) 2018 Antmicro <www.antmicro.com>
 *
 *  BSD-licensed.
 *
 *************************************************/

#include <stdint.h>

typedef struct __attribute__((__packed__)) {
        uint8_t magic[8]; // 00 FF FF FF FF FF FF 00
	uint16_t manufacturer_id;
	uint16_t product_id;
	uint32_t serial_number;
	uint16_t manufacture_date;
	uint8_t edid_version;
	uint8_t edid_revision;
	uint8_t video_input_type;
	uint8_t horizontal_size;
	uint8_t vertical_size;
	uint8_t display_gamma;
	uint8_t supported_features;
	uint8_t color_characteristics[10];
	uint16_t estabilished_supported_timings;
	uint8_t reserved_timing;
	uint8_t edid_standard_timings[16];
} edid_hdr;

typedef struct __attribute__((__packed__)) {
	uint8_t magic; // always '2'
	uint8_t revision;
	uint8_t pointer;
	uint8_t flags_and_count;
} edid_cea_861_extension_hdr;

uint8_t extension[128];
uint8_t descriptor[18];

typedef struct __attribute__((__packed__)) {
    uint16_t clock;
    uint8_t x_act_lsb;
    uint8_t x_blk_lsb;
    uint8_t x_msbs;
    uint8_t y_act_lsb;
    uint8_t y_blk_lsb;
    uint8_t y_msbs;
    uint8_t x_snc_off_lsb;
    uint8_t x_snc_pls_lsb;
    uint8_t y_snc_lsb;
    uint8_t xy_snc_msbs;
    uint8_t x_dsp_size;
    uint8_t y_dsp_size;
    uint8_t dsp_size_mbsb;
    uint8_t x_border;
    uint8_t y_border;
    uint8_t features;
} extended_descriptor;

uint8_t fixed_edid[128] = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 
 // desc2 - monitor name
 0x00, 0x00, 0x00, 0xFF, 0x00, 0x50, 0x32, 0x50, 0x43, 0x32, 0x34, 0x42, 0x34, 0x30, 0x36, 0x55, 0x4C, 0x0A, 
 // desc3
 0x00, 0x00, 0x00, 0xFC, 0x00, 0x44, 0x45, 0x4C, 0x4C, 0x20, 0x50, 0x32, 0x34, 0x31, 0x35, 0x51, 0x0A, 0x20, 
 // desc4
 0x00, 0x00, 0x00, 0xFD, 0x00, 0x1D, 0x4C, 0x1E, 0x8C, 0x1E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 

 0x00, 0x00,
};

void insert_crc(uint8_t *data) {
        uint8_t sum = 0;
        for (int i = 0; i < 127; i++) sum += data[i];
        data[127] = -sum;
}

int main() {
        edid_hdr *hdr = (edid_hdr*)fixed_edid;
	hdr->magic[0] = 0x00;
	hdr->magic[1] = 0xFF;
	hdr->magic[2] = 0xFF;
	hdr->magic[3] = 0xFF;
	hdr->magic[4] = 0xFF;
	hdr->magic[5] = 0xFF;
	hdr->magic[6] = 0xFF;
	hdr->magic[7] = 0x00;
        hdr->manufacturer_id = 0xAC10;
        hdr->product_id = 0xA0C0;
        hdr->serial_number = 0x3036554C;
        hdr->manufacture_date = 0x182D;
        hdr->edid_version = 0x01;
        hdr->edid_revision = 0x03;
        hdr->video_input_type = 0x80;
        hdr->horizontal_size = 0x35;
        hdr->vertical_size = 0x1E;
        hdr->display_gamma = 0x78;
        hdr->supported_features = 0xEA;
	hdr->color_characteristics[0] = 0xE2;
	hdr->color_characteristics[1] = 0x45;
	hdr->color_characteristics[2] = 0xA8;
	hdr->color_characteristics[3] = 0x55;
	hdr->color_characteristics[4] = 0x4D;
	hdr->color_characteristics[5] = 0xA3;
	hdr->color_characteristics[6] = 0x26;
	hdr->color_characteristics[7] = 0x0B;
	hdr->color_characteristics[8] = 0x50;
	hdr->color_characteristics[9] = 0x54;
	hdr->color_characteristics[10] = 0xA5;
        hdr->estabilished_supported_timings = 0x004B;
        hdr->reserved_timing = 0x71;
	hdr->edid_standard_timings[0] = 0x4F;
	hdr->edid_standard_timings[1] = 0x81;
        hdr->edid_standard_timings[2] = 0x80;
	hdr->edid_standard_timings[3] = 0xA9;
	hdr->edid_standard_timings[4] = 0xC0;
	hdr->edid_standard_timings[5] = 0xA9;
	hdr->edid_standard_timings[6] = 0x00;
	hdr->edid_standard_timings[7] = 0x40;
	hdr->edid_standard_timings[8] = 0xD1;
	hdr->edid_standard_timings[9] = 0xC0;
        hdr->edid_standard_timings[10] = 0xE1;
	hdr->edid_standard_timings[11] =  0x00;
	hdr->edid_standard_timings[12] = 0x01;
	hdr->edid_standard_timings[13] = 0x01;
	hdr->edid_standard_timings[14] = 0x01;
	hdr->edid_standard_timings[15] = 0x01;

	extended_descriptor descriptor;
	/* cvt 1600 900 60 
           Modeline "1600x900_60.00"  118.25  1600 1696 1856 2112  900 903 908 934 -hsync +vsync
        */
	descriptor.clock = 11825; // kHz
	descriptor.x_act_lsb = 1600 & 0xFF;

        descriptor.x_blk_lsb = 0x00;
        descriptor.x_msbs = 0x62;
        descriptor.y_act_lsb = 0x84;
        descriptor.y_blk_lsb = 0x22;
        descriptor.y_msbs = 0x30;
        descriptor.x_snc_off_lsb = 0x60;
        descriptor.x_snc_pls_lsb = 0xA0;
        descriptor.y_snc_lsb = 0x00;
        descriptor.xy_snc_msbs = 0xA0;
        descriptor.x_dsp_size = 0xEA;
        descriptor.y_dsp_size = 0x10;
        descriptor.dsp_size_mbsb = 0x00;
        descriptor.x_border = 0x00;
        descriptor.y_border = 0x1E;
        descriptor.features = 0x00;
        memcpy(fixed_edid + sizeof(edid_hdr), &descriptor, sizeof(descriptor));

        fixed_edid[126] = 1; // extension count;
        insert_crc(fixed_edid);

	for (int i = 0; i < 128; i++) printf("%02X ", fixed_edid[i]);
	printf("\n");

	memset(extension, 0, 128);
	edid_cea_861_extension_hdr *extension_hdr = (edid_cea_861_extension_hdr*)extension;

	extension_hdr->magic = 2;
	extension_hdr->revision = 3;
	extension_hdr->pointer = 4;
	extension_hdr->flags_and_count = 0x0;

        // maximum number of descriptors
	for (int i = 0; i < 6; i++) memcpy(extension + extension_hdr->pointer + i * sizeof(descriptor), &descriptor, sizeof(descriptor));
        insert_crc(extension);
	for (int i = 0; i < 128; i++) printf("%02X ", extension[i]);
	printf("\n");
}
