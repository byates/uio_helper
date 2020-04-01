/*
   uio_helper
   UIO helper functions - header file.

   Copyright (C) 2009, Hans J. Koch <hjk@linutronix.de>
   Copyright (C) 2009, Stephan Linz <linz@li-pro.net>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as
   published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifndef __UIO_HELPER_H__
#define __UIO_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* convensions */

#define UIO_MAX_NAME_SIZE 256
#define UIO_MAX_NUM       255

#define UIO_INVALID_SIZE -1
#define UIO_INVALID_ADDR (~0)

#define UIO_MMAP_NOT_DONE 0
#define UIO_MMAP_OK       1
#define UIO_MMAP_FAILED   2

/* This should be identical to the define in include/linux/uio_driver.h !!! */
#define MAX_UIO_MAPS 5

struct uio_map_t {
    unsigned long addr;
    int size;
    int mmap_result;
    void* internal_addr;
};

struct uio_dev_attr_t {
    char name[UIO_MAX_NAME_SIZE];
    char value[UIO_MAX_NAME_SIZE];
    struct uio_dev_attr_t* next;
};

struct uio_info_t {
    int uio_num;
    int map_count;
    struct uio_map_t maps[MAX_UIO_MAPS];
    unsigned long event_count;
    char name[UIO_MAX_NAME_SIZE];
    char version[UIO_MAX_NAME_SIZE];
    struct uio_dev_attr_t* dev_attrs;
    struct uio_info_t* next; /* for linked list */
};

// Return a device info structure by number (uio[n]).
// If filter_num is -1 then will return a linked list of all UIO devices.
// If filter_num is >=0 then will only return the matching UIO device.
struct uio_info_t* uio_find_devices(int filter_num);
// Return a device info structure by name (uiox.[name])
// NOTE: will return a linked list if more than one device has the same name.
struct uio_info_t* uio_find_devices_byname(const char* filter_name);
// Frees the info structure retrieved from uio_find_xxx.
// Also frees any attr_t structures previously allocated.
void uio_free_info(struct uio_info_t* info);
// Main function which reads all available info for a UIO device.
// After this call the info structure fields are all valid.
int uio_get_all_info(struct uio_info_t* info);
// MMAPs all of the avaiable memory regions for a device.
// NOTE: the device must previously be open()'d with fd being the resulting file descriptor.
void uio_mmap(struct uio_info_t* info, int fd);
// Un-maps all previously mmap'd memory regions.
void uio_munmap(struct uio_info_t* info);
// MMAPs a sepcific memory regions for a device.
// NOTE: the device must previously be open()'d with fd being the resulting file descriptor.
void* uio_single_mmap(struct uio_info_t* info, int map_num, int fd);
// Un-maps a specific mmap'd memory region.
void uio_single_munmap(struct uio_info_t* info, int map_num);

// Functions to read individual UIO information values from the sys filesystem.
// Normally these are not needed as the uio_get_all_info() will call all of these.
int uio_get_mem_size(struct uio_info_t* info, int map_num);
int uio_get_mem_addr(struct uio_info_t* info, int map_num);
int uio_get_event_count(struct uio_info_t* info);
int uio_get_name(struct uio_info_t* info);
int uio_get_version(struct uio_info_t* info);
int uio_get_device_attributes(struct uio_info_t* info);
// Frees the attr_t linked list retrieved from uio_get_device_attributes
void uio_free_dev_attrs(struct uio_info_t* info);

#ifdef __cplusplus
}
#endif

#endif /* __UIO_HELPER_H__ */
