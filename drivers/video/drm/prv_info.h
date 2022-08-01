/*
 *(C) Copyright htfyun
 * tanlq, Software Engineering, <tanluqiang@htfyun.com>.
 *
 */

/* tanlq add this for eink privata info
 * kernel里需要使用
 * 20191119: we may use at uboot/kernel and other place.
 *	redefine the struct just for simple use and save ram.
 */

#ifndef __PRIVDATA_INFO_H_
#define __PRIVDATA_INFO_H_

// 重要说明：下面的定义在 uboot/kernel/privdata 三个模块都用到，如果需要修改，请同时更新
// 这三个模块，并验证 开机LOGO是否正常。一般情况下，不建议修改已经定义的项(包括类型和顺序)，
// 如果需要，在结构体的后面增加新的项。
// 20191119: the offset of waveform at the reserved mem.
#define WAVE_FORM_OFFSET 		1024
#define PV_HEAD_SIZE 			(WAVE_FORM_OFFSET)

#define PVDATA_TAG		0x48544659

// 20191119:为了处理方便，这个结构体放在 pv 分区起始地址。预留空间是 PV_HEAD_SIZE。
struct pvdata_info {
	unsigned int 		tag;	// valid or not.
	int 				struct_size;	// the size of this struct,use as version.
	int 				pv_size; // the total size of privdata-area.

	int 				vcom; 	// if not set, =0.

	// waveform.
	int 				wf_size; // if not exit, =0.
	int 				wf_offset; // offset frome the start of privdata-area,must be the first section.

	// power-on logo,use at u-boot.may treat as file-data(fdata).
	int 				logo_size; // if not set, =0.
	int 				logo_offset; // offset frome the start of privdata-area.

	// the section to store key-value.
	int					key_offset;		// offset frome the start of privdata-area.
	int 				key_item_size;	// the size of key-value item.
	int 				key_item_cnt;	// the count of items.

	// the section to store key-data(key store at key-item).
	int 				fdata_offset;	// the offset to save long data(like files/logo)
	int					fdata_free_offset;
};


#endif
