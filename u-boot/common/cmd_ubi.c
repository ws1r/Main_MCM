/*
 * Unsorted Block Image commands
 *
 *  Copyright (C) 2008 Samsung Electronics
 *  Kyungmin Park <kyungmin.park@samsung.com>
 *
 * Copyright 2008-2009 Stefan Roese <sr@denx.de>, DENX Software Engineering
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <command.h>
#include <exports.h>

#include <nand.h>
#include <onenand_uboot.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <ubi_uboot.h>
#include <asm/errno.h>
#include <jffs2/load_kernel.h>

#ifdef CONFIG_OEM_SDREAD
#include <def.h>
#include <concenwit_FAT.h>
#endif

#define DEV_TYPE_NONE		0
#define DEV_TYPE_NAND		1
#define DEV_TYPE_ONENAND	2
#define DEV_TYPE_NOR		3

/* Private own data */
static struct ubi_device *ubi;
static char buffer[80];
static int ubi_initialized;

struct selected_dev {
	char part_name[80];
	int selected;
	int nr;
	struct mtd_info *mtd_info;
};

static struct selected_dev ubi_dev;

static void ubi_dump_vol_info(const struct ubi_volume *vol)
{
	ubi_msg("volume information dump:");
	ubi_msg("vol_id          %d", vol->vol_id);
	ubi_msg("reserved_pebs   %d", vol->reserved_pebs);
	ubi_msg("alignment       %d", vol->alignment);
	ubi_msg("data_pad        %d", vol->data_pad);
	ubi_msg("vol_type        %d", vol->vol_type);
	ubi_msg("name_len        %d", vol->name_len);
	ubi_msg("usable_leb_size %d", vol->usable_leb_size);
	ubi_msg("used_ebs        %d", vol->used_ebs);
	ubi_msg("used_bytes      %lld", vol->used_bytes);
	ubi_msg("last_eb_bytes   %d", vol->last_eb_bytes);
	ubi_msg("corrupted       %d", vol->corrupted);
	ubi_msg("upd_marker      %d", vol->upd_marker);

	if (vol->name_len <= UBI_VOL_NAME_MAX &&
		strnlen(vol->name, vol->name_len + 1) == vol->name_len) {
		ubi_msg("name            %s", vol->name);
	} else {
		ubi_msg("the 1st 5 characters of the name: %c%c%c%c%c",
				vol->name[0], vol->name[1], vol->name[2],
				vol->name[3], vol->name[4]);
	}
	printf("\n");
}

static void display_volume_info(struct ubi_device *ubi)
{
	int i;

	for (i = 0; i < (ubi->vtbl_slots + 1); i++) {
		if (!ubi->volumes[i])
			continue;	/* Empty record */
		ubi_dump_vol_info(ubi->volumes[i]);
	}
}

static void display_ubi_info(struct ubi_device *ubi)
{
	ubi_msg("MTD device name:            \"%s\"", ubi->mtd->name);
	ubi_msg("MTD device size:            %llu MiB", ubi->flash_size >> 20);
	ubi_msg("physical eraseblock size:   %d bytes (%d KiB)",
			ubi->peb_size, ubi->peb_size >> 10);
	ubi_msg("logical eraseblock size:    %d bytes", ubi->leb_size);
	ubi_msg("number of good PEBs:        %d", ubi->good_peb_count);
	ubi_msg("number of bad PEBs:         %d", ubi->bad_peb_count);
	ubi_msg("smallest flash I/O unit:    %d", ubi->min_io_size);
	ubi_msg("VID header offset:          %d (aligned %d)",
			ubi->vid_hdr_offset, ubi->vid_hdr_aloffset);
	ubi_msg("data offset:                %d", ubi->leb_start);
	ubi_msg("max. allowed volumes:       %d", ubi->vtbl_slots);
	ubi_msg("wear-leveling threshold:    %d", CONFIG_MTD_UBI_WL_THRESHOLD);
	ubi_msg("number of internal volumes: %d", UBI_INT_VOL_COUNT);
	ubi_msg("number of user volumes:     %d",
			ubi->vol_count - UBI_INT_VOL_COUNT);
	ubi_msg("available PEBs:             %d", ubi->avail_pebs);
	ubi_msg("total number of reserved PEBs: %d", ubi->rsvd_pebs);
	ubi_msg("number of PEBs reserved for bad PEB handling: %d",
			ubi->beb_rsvd_pebs);
	ubi_msg("max/mean erase counter: %d/%d", ubi->max_ec, ubi->mean_ec);
}

static int ubi_info(int layout)
{
	if (layout)
		display_volume_info(ubi);
	else
		display_ubi_info(ubi);

	return 0;
}

static int verify_mkvol_req(const struct ubi_device *ubi,
			    const struct ubi_mkvol_req *req)
{
	int n, err = -EINVAL;

	if (req->bytes < 0 || req->alignment < 0 || req->vol_type < 0 ||
	    req->name_len < 0)
		goto bad;

	if ((req->vol_id < 0 || req->vol_id >= ubi->vtbl_slots) &&
	    req->vol_id != UBI_VOL_NUM_AUTO)
		goto bad;

	if (req->alignment == 0)
		goto bad;

	if (req->bytes == 0)
		goto bad;

	if (req->vol_type != UBI_DYNAMIC_VOLUME &&
	    req->vol_type != UBI_STATIC_VOLUME)
		goto bad;

	if (req->alignment > ubi->leb_size)
		goto bad;

	n = req->alignment % ubi->min_io_size;
	if (req->alignment != 1 && n)
		goto bad;

	if (req->name_len > UBI_VOL_NAME_MAX) {
		err = -ENAMETOOLONG;
		goto bad;
	}

	return 0;
bad:
	printf("bad volume creation request");
	return err;
}

static int ubi_create_vol(char *volume, int size, int dynamic)
{
	struct ubi_mkvol_req req;
	int err;

	if (dynamic)
		req.vol_type = UBI_DYNAMIC_VOLUME;
	else
		req.vol_type = UBI_STATIC_VOLUME;

	req.vol_id = UBI_VOL_NUM_AUTO;
	req.alignment = 1;
	req.bytes = size;

	strcpy(req.name, volume);
	req.name_len = strlen(volume);
	req.name[req.name_len] = '\0';
	req.padding1 = 0;
	/* It's duplicated at drivers/mtd/ubi/cdev.c */
	err = verify_mkvol_req(ubi, &req);
	if (err) {
		printf("verify_mkvol_req failed %d\n", err);
		return err;
	}
	printf("Creating %s volume %s of size %d\n",
		dynamic ? "dynamic" : "static", volume, size);
	/* Call real ubi create volume */
	return ubi_create_volume(ubi, &req);
}

static int ubi_remove_vol(char *volume)
{
	int i, err, reserved_pebs;
	int found = 0, vol_id = 0;
	struct ubi_volume *vol = NULL;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		vol = ubi->volumes[i];
		if (vol && !strcmp(vol->name, volume)) {
			printf("Volume %s found at valid %d\n", volume, i);
			vol_id = i;
			found = 1;
			break;
		}
	}
	if (!found) {
		printf("%s volume not found\n", volume);
		return -ENODEV;
	}
	printf("remove UBI volume %s (id %d)\n", vol->name, vol->vol_id);

	if (ubi->ro_mode) {
		printf("It's read-only mode\n");
		err = -EROFS;
		goto out_err;
	}

	err = ubi_change_vtbl_record(ubi, vol_id, NULL);
	if (err) {
		printf("Error changing Vol tabel record err=%x\n", err);
		goto out_err;
	}
	reserved_pebs = vol->reserved_pebs;
	for (i = 0; i < vol->reserved_pebs; i++) {
		err = ubi_eba_unmap_leb(ubi, vol, i);
		if (err)
			goto out_err;
	}

	kfree(vol->eba_tbl);
	ubi->volumes[vol_id]->eba_tbl = NULL;
	ubi->volumes[vol_id] = NULL;

	ubi->rsvd_pebs -= reserved_pebs;
	ubi->avail_pebs += reserved_pebs;
	i = ubi->beb_rsvd_level - ubi->beb_rsvd_pebs;
	if (i > 0) {
		i = ubi->avail_pebs >= i ? i : ubi->avail_pebs;
		ubi->avail_pebs -= i;
		ubi->rsvd_pebs += i;
		ubi->beb_rsvd_pebs += i;
		if (i > 0)
			ubi_msg("reserve more %d PEBs", i);
	}
	ubi->vol_count -= 1;

	return 0;
out_err:
	ubi_err("cannot remove volume %d, error %d", vol_id, err);
	return err;
}

#ifdef CONFIG_OEM_SDREAD
static int ubi_volume_OEM_write(char *volume, void *buf, char *filename)
{
	int i = 0, err = -1;
	int rsvd_bytes = 0;
	int found = 0;
	struct ubi_volume *vol;
	size_t size = 0;
	unsigned long Dir = 0;

	DirFunRet FileDirFileRet = FindDirFile(filename);
	if( FileDirFileRet.FileSize == 0)
	{
		return 1;
	}
	else
	{
		Dir = FileDirFileRet.StartCluster;
		size = FileDirFileRet.FileSize;
	}

	for (i = 0; i < ubi->vtbl_slots; i++) {
		vol = ubi->volumes[i];
		if (vol && !strcmp(vol->name, volume)) {
			printf("Volume \"%s\" found at volume id %d.\n", volume, i);
			found = 1;
			break;
		}
	}
	if (!found) {
		printf("%s volume not found\n", volume);
		return 1;
	}
#ifdef CONFIG_CW210
	printf("UBIFS image length is 0x%x, Write UBIFS to %s, please wait.\n", size, volume);
#endif
	rsvd_bytes = vol->reserved_pebs * (ubi->leb_size - vol->data_pad);
	if (size < 0 || size > rsvd_bytes) {
		printf("rsvd_bytes=%d vol->reserved_pebs=%d ubi->leb_size=%d\n",
		     rsvd_bytes, vol->reserved_pebs, ubi->leb_size);
		printf("vol->data_pad=%d\n", vol->data_pad);
		printf("Size > volume size !!\n");
		return 1;
	}

	err = ubi_start_update(ubi, vol, size);
	if (err < 0) {
		printf("Cannot start volume update\n");
		return err;
	}

	err = OEM_ubi_more_update_data(ubi, vol, buf, size, Dir);
	if (err < 0) {
		printf("Couldnt or partially wrote data \n");
		return err;
	}

	if (err) {
		size = err;

		err = ubi_check_volume(ubi, vol->vol_id);
		if ( err < 0 )
			return err;

		if (err) {
			ubi_warn("volume %d on UBI device %d is corrupted",
					vol->vol_id, ubi->ubi_num);
			vol->corrupted = 1;
		}

		vol->checked = 1;
		ubi_gluebi_updated(vol);
	}
	printf("\nUBIFS write compilete!\n");
	return 0;
}
#endif

static int ubi_volume_write(char *volume, void *buf, size_t size)
{
	int i = 0, err = -1;
	int rsvd_bytes = 0;
	int found = 0;
	struct ubi_volume *vol;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		vol = ubi->volumes[i];
		if (vol && !strcmp(vol->name, volume)) {
			printf("Volume \"%s\" found at volume id %d\n", volume, i);
			found = 1;
			break;
		}
	}
	if (!found) {
		printf("%s volume not found\n", volume);
		return 1;
	}
#ifdef CONFIG_CW210
	printf("Write UBIFS to rootfs, please wait ...");
#endif
	rsvd_bytes = vol->reserved_pebs * (ubi->leb_size - vol->data_pad);
	if (size < 0 || size > rsvd_bytes) {
		printf("rsvd_bytes=%d vol->reserved_pebs=%d ubi->leb_size=%d\n",
		     rsvd_bytes, vol->reserved_pebs, ubi->leb_size);
		printf("vol->data_pad=%d\n", vol->data_pad);
		printf("Size > volume size !!\n");
		return 1;
	}

	err = ubi_start_update(ubi, vol, size);
	if (err < 0) {
		printf("Cannot start volume update\n");
		return err;
	}

	err = ubi_more_update_data(ubi, vol, buf, size);
	if (err < 0) {
		printf("Couldnt or partially wrote data \n");
		return err;
	}

	if (err) {
		size = err;

		err = ubi_check_volume(ubi, vol->vol_id);
		if ( err < 0 )
			return err;

		if (err) {
			ubi_warn("volume %d on UBI device %d is corrupted",
					vol->vol_id, ubi->ubi_num);
			vol->corrupted = 1;
		}

		vol->checked = 1;
		ubi_gluebi_updated(vol);
	}
	printf("OK.\nUBIFS write compilete!\n");
	return 0;
}

static int ubi_volume_read(char *volume, char *buf, size_t size)
{
	int err, lnum, off, len, tbuf_size, i = 0;
	size_t count_save = size;
	void *tbuf;
	unsigned long long tmp;
	struct ubi_volume *vol = NULL;
	loff_t offp = 0;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		vol = ubi->volumes[i];
		if (vol && !strcmp(vol->name, volume)) {
			printf("Volume %s found at volume id %d\n",
				volume, vol->vol_id);
			break;
		}
	}
	if (i == ubi->vtbl_slots) {
		printf("%s volume not found\n", volume);
		return -ENODEV;
	}

	printf("read %i bytes from volume %d to %x(buf address)\n",
	       (int) size, vol->vol_id, (unsigned)buf);

	if (vol->updating) {
		printf("updating");
		return -EBUSY;
	}
	if (vol->upd_marker) {
		printf("damaged volume, update marker is set");
		return -EBADF;
	}
	if (offp == vol->used_bytes)
		return 0;

	if (size == 0) {
		printf("Read [%lu] bytes\n", (unsigned long) vol->used_bytes);
		size = vol->used_bytes;
	}

	if (vol->corrupted)
		printf("read from corrupted volume %d", vol->vol_id);
	if (offp + size > vol->used_bytes)
		count_save = size = vol->used_bytes - offp;

	tbuf_size = vol->usable_leb_size;
	if (size < tbuf_size)
		tbuf_size = ALIGN(size, ubi->min_io_size);
	tbuf = malloc(tbuf_size);
	if (!tbuf) {
		printf("NO MEM\n");
		return -ENOMEM;
	}
	len = size > tbuf_size ? tbuf_size : size;

	tmp = offp;
	off = do_div(tmp, vol->usable_leb_size);
	lnum = tmp;
	do {
		if (off + len >= vol->usable_leb_size)
			len = vol->usable_leb_size - off;

		err = ubi_eba_read_leb(ubi, vol, lnum, tbuf, off, len, 0);
		if (err) {
			printf("read err %x\n", err);
			break;
		}
		off += len;
		if (off == vol->usable_leb_size) {
			lnum += 1;
			off -= vol->usable_leb_size;
		}

		size -= len;
		offp += len;

		memcpy(buf, tbuf, len);

		buf += len;
		len = size > tbuf_size ? tbuf_size : size;
	} while (size);

	free(tbuf);
	return err ? err : count_save - size;
}

static int ubi_dev_scan(struct mtd_info *info, char *ubidev,
		const char *vid_header_offset)
{
	struct mtd_device *dev;
	struct part_info *part;
	struct mtd_partition mtd_part;
	char ubi_mtd_param_buffer[80];
	u8 pnum;
	int err;

	if (find_dev_and_part(ubidev, &dev, &pnum, &part) != 0)
		return 1;

	sprintf(buffer, "mtd=%d", pnum);
	memset(&mtd_part, 0, sizeof(mtd_part));
	mtd_part.name = buffer;
	mtd_part.size = part->size;
	mtd_part.offset = part->offset;
	add_mtd_partitions(info, &mtd_part, 1);

	strcpy(ubi_mtd_param_buffer, buffer);
	if (vid_header_offset)
		sprintf(ubi_mtd_param_buffer, "mtd=%d,%s", pnum,
				vid_header_offset);
	err = ubi_mtd_param_parse(ubi_mtd_param_buffer, NULL);
	if (err) {
		del_mtd_partitions(info);
		return err;
	}

	err = ubi_init();
	if (err) {
		del_mtd_partitions(info);
		return err;
	}

	ubi_initialized = 1;

	return 0;
}

static int do_ubi(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	size_t size = 0;
	ulong addr = 0;
	int err = 0;

	if (argc < 2) {
		goto usage;
	}

	if (mtdparts_init() != 0) {
		printf("Error initializing mtdparts!\n");
		return 1;
	}

	if (strcmp(argv[1], "part") == 0) {
		char mtd_dev[16];
		struct mtd_device *dev;
		struct part_info *part;
		const char *vid_header_offset = NULL;
		u8 pnum;

		/* Print current partition */
		if (argc == 2) {
			if (!ubi_dev.selected) {
				printf("Error, no UBI device/partition selected!\n");
				return 1;
			}

			printf("Device %d: %s, partition %s\n",
			       ubi_dev.nr, ubi_dev.mtd_info->name, ubi_dev.part_name);
			return 0;
		}

		if (argc < 3) {
			cmd_usage(cmdtp);
			return 1;
		}

		/* todo: get dev number for NAND... */
		ubi_dev.nr = 0;

		/*
		 * Call ubi_exit() before re-initializing the UBI subsystem
		 */
		if (ubi_initialized) {
			ubi_exit();
			del_mtd_partitions(ubi_dev.mtd_info);
		}

		/*
		 * Search the mtd device number where this partition
		 * is located
		 */
		if (find_dev_and_part(argv[2], &dev, &pnum, &part)) {
			printf("Partition %s not found!\n", argv[2]);
			return 1;
		}
		sprintf(mtd_dev, "%s%d", MTD_DEV_TYPE(dev->id->type), dev->id->num);
		ubi_dev.mtd_info = get_mtd_device_nm(mtd_dev);
		if (IS_ERR(ubi_dev.mtd_info)) {
			printf("Partition %s not found on device %s!\n", argv[2], mtd_dev);
			return 1;
		}

		ubi_dev.selected = 1;

		if (argc > 3)
			vid_header_offset = argv[3];
		strcpy(ubi_dev.part_name, argv[2]);
		err = ubi_dev_scan(ubi_dev.mtd_info, ubi_dev.part_name,
				vid_header_offset);
		if (err) {
			printf("UBI init error %d\n", err);
			ubi_dev.selected = 0;
			return err;
		}

		ubi = ubi_devices[0];

		return 0;
	}

	if ((strcmp(argv[1], "part") != 0) && (!ubi_dev.selected)) {
		printf("Error, no UBI device/partition selected!\n");
		return 1;
	}

	if (strcmp(argv[1], "info") == 0) {
		int layout = 0;
		if (argc > 2 && !strncmp(argv[2], "l", 1))
			layout = 1;
		return ubi_info(layout);
	}

	if (strncmp(argv[1], "create", 6) == 0) {
		int dynamic = 1;	/* default: dynamic volume */

		/* Use maximum available size */
		size = 0;

		/* E.g., create volume size type */
		if (argc == 5) {
			if (strncmp(argv[4], "s", 1) == 0)
				dynamic = 0;
			else if (strncmp(argv[4], "d", 1) != 0) {
				printf("Incorrect type\n");
				return 1;
			}
			argc--;
		}
		/* E.g., create volume size */
		if (argc == 4) {
			size = simple_strtoul(argv[3], NULL, 16);
			argc--;
		}
		/* Use maximum available size */
		if (!size)
			size = ubi->avail_pebs * ubi->leb_size;
		/* E.g., create volume */
		if (argc == 3)
			return ubi_create_vol(argv[2], size, dynamic);
	}

	if (strncmp(argv[1], "remove", 6) == 0) {
		/* E.g., remove volume */
		if (argc == 3)
			return ubi_remove_vol(argv[2]);
	}

	if (strncmp(argv[1], "write", 5) == 0) {
		if (argc < 5) {
			printf("Please see usage\n");
			return 1;
		}

		addr = simple_strtoul(argv[2], NULL, 16);
		size = simple_strtoul(argv[4], NULL, 16);

		return ubi_volume_write(argv[3], (void *)addr, size);
	}

#ifdef CONFIG_CONCENWIT_FAT
	if (strncmp(argv[1], "OEMwrite", 8) == 0) {
		if (argc < 4) {
			printf("Please see usage\n");
			return 1;
		}
		addr = simple_strtoul(argv[2], NULL, 16);

		return ubi_volume_OEM_write(argv[3], (void *)addr, argv[4]);
	}
#endif

	if (strncmp(argv[1], "read", 4) == 0) {
		size = 0;

		/* E.g., read volume size */
		if (argc == 5) {
			size = simple_strtoul(argv[4], NULL, 16);
			argc--;
		}

		/* E.g., read volume */
		if (argc == 4) {
			addr = simple_strtoul(argv[2], NULL, 16);
			argc--;
		}

		if (argc == 3)
			return ubi_volume_read(argv[3], (char *)addr, size);
	}

usage:
	printf("Usage:\n%s\n", cmdtp->usage);
	return 1;
}

U_BOOT_CMD(ubi, 6, 1, do_ubi,
	"ubi commands",
	"part [part] [offset]\n"
		" - Show or set current partition (with optional VID"
		" header offset)\n"
	"ubi info [l[ayout]]"
		" - Display volume and ubi layout information\n"
	"ubi create[vol] volume [size] [type]"
		" - create volume name with size\n"
	"ubi write[vol] address volume size"
		" - Write volume from address with size\n"
#ifdef CONFIG_CONCENWIT_FAT
	"ubi OEMwrite vol filename"
		" - Write volume from address with size\n"
#endif
	"ubi read[vol] address volume [size]"
		" - Read volume to address with size\n"
	"ubi remove[vol] volume"
		" - Remove volume\n"
	"[Legends]\n"
	" volume: character name\n"
	" size: specified in bytes\n"
	" type: s[tatic] or d[ynamic] (default=dynamic)"
);
