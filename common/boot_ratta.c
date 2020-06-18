#include <common.h>
#include <malloc.h>
#include <boot_rkimg.h>

#define BOOTLOADER_MESSAGE_OFFSET_IN_MISC	(16 * 1024)
#define BOOTLOADER_MESSAGE_BLK_OFFSET		(BOOTLOADER_MESSAGE_OFFSET_IN_MISC >> 9)
struct bootloader_message {
	char command[32];
	char status[32];
	char recovery[768];

	/*
	 * The 'recovery' field used to be 1024 bytes.  It has only ever
	 * been used to store the recovery command line, so 768 bytes
	 * should be plenty.  We carve off the last 256 bytes to store the
	 * stage string (for multistage packages) and possible future
	 * expansion.
	 */
	char stage[32];
	char slot_suffix[32];
	char reserved[192];
};

static void ratta_set_bootmode(const char *mode)
{
	char new[256];

	memset(new, 0, sizeof(new));
	snprintf(new, sizeof(new) - 1,
		 "ratta.bootmode=%s%s",
		 mode,
		 !strcmp(mode, "factory") ?
		 " androidboot.selinux=permissive" : "");
	env_update("bootargs", new);
}

int ratta_set_boot_mode(void)
{
	struct blk_desc *dev_desc;
	disk_partition_t part_info;
	struct bootloader_message *bmsg;
	struct bootloader_message msg;
	int size = DIV_ROUND_UP(sizeof(struct bootloader_message), RK_BLK_SIZE)
		   * RK_BLK_SIZE;
	int ret;

	dev_desc = rockchip_get_bootdev();
	if (!dev_desc) {
		printf("%s: dev_desc is NULL!\n", __func__);
		return -ENODEV;
	}

	ret = part_get_info_by_name(dev_desc, PART_MISC,
			&part_info);
	if (ret < 0) {
		printf("get part %s fail %d\n", PART_MISC, ret);
		return -EIO;
	}

	bmsg = memalign(ARCH_DMA_MINALIGN, size);
	ret = blk_dread(dev_desc,
			part_info.start + BOOTLOADER_MESSAGE_BLK_OFFSET,
			size >> 9, bmsg);
	if (ret != (size >> 9)) {
		free(bmsg);
		return -EIO;
	}

	if (!strcmp(bmsg->command, "boot-recovery")) {
		ratta_set_bootmode("recovery");
	} else if (!strcmp(bmsg->command, "boot-factory")) {
		ratta_set_bootmode("factory");
		memset(&msg, 0, sizeof(msg));
		ret = blk_dwrite(dev_desc,
				 part_info.start + BOOTLOADER_MESSAGE_BLK_OFFSET,
				 size >> 9, &msg);
		if (ret != (size >> 9))
			printf("clear factory boot message failed,%d\n", ret);
	} else {
		ratta_set_bootmode("normal");
	}

	return 0;
}
