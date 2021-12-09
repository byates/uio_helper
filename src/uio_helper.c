/*
   uio_find_devices_byname.c
   UIO helper function: ... ... ...

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
#include "uio_helper.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int __uio_line_from_file(char* filename, char* linebuf) {
    char* s;
    int i;
    memset(linebuf, 0, UIO_MAX_NAME_SIZE);
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    s = fgets(linebuf, UIO_MAX_NAME_SIZE, file);
    fclose(file);
    if (!s) {
        return -2;
    }
    for (i = 0; (*s) && (i < UIO_MAX_NAME_SIZE); i++) {
        if (*s == '\n') {
            *s = 0;
        }
        s++;
    }
    return 0;
}

static int __uio_num_from_filename(char* name) {
    enum scan_states { ss_u, ss_i, ss_o, ss_num, ss_err };
    enum scan_states state = ss_u;
    int i = 0, num = -1;
    char ch = name[0];
    while (ch && (state != ss_err)) {
        switch (ch) {
            case 'u':
                if (state == ss_u) {
                    state = ss_i;
                } else {
                    state = ss_err;
                }
                break;
            case 'i':
                if (state == ss_i) {
                    state = ss_o;
                } else {
                    state = ss_err;
                }
                break;
            case 'o':
                if (state == ss_o) {
                    state = ss_num;
                } else {
                    state = ss_err;
                }
                break;
            default:
                if ((ch >= '0') && (ch <= '9') && (state == ss_num)) {
                    if (num < 0) {
                        num = (ch - '0');
                    } else {
                        num = (num * 10) + (ch - '0');
                    }
                } else {
                    state = ss_err;
                }
        }
        i++;
        ch = name[i];
    }
    if (state == ss_err) {
        num = -1;
    }
    return num;
}

static struct uio_info_t* __uio_info_from_name(char* name, int filter_num) {
    struct uio_info_t* info;
    int num = __uio_num_from_filename(name);
    if (num < 0) {
        return NULL;
    }
    if ((filter_num >= 0) && (num != filter_num)) {
        return NULL;
    }
    info = malloc(sizeof(struct uio_info_t));
    if (!info) {
        return NULL;
    }
    memset(info, 0, sizeof(struct uio_info_t));
    info->uio_num = num;
    return info;
}

static struct uio_info_t* __uio_info_byname(char* name, const char* filter_name) {
    struct uio_info_t* info;
    char linebuf[UIO_MAX_NAME_SIZE];
    char filename[255];

    snprintf(filename, sizeof(filename), "/sys/class/uio/%s/name", name);
    if (__uio_line_from_file(filename, linebuf)) {
        return NULL;
    }
    if (strncmp(linebuf, filter_name, UIO_MAX_NAME_SIZE)) {
        return NULL;
    }
    info = malloc(sizeof(struct uio_info_t));
    if (!info) {
        return NULL;
    }
    memset(info, 0, sizeof(struct uio_info_t));
    info->uio_num = __uio_num_from_filename(name);
    return info;
}

struct uio_info_t* uio_find_devices(int filter_num) {
    struct dirent** namelist;
    struct uio_info_t *infolist = NULL, *infp, *last;
    int n;

    n = scandir("/sys/class/uio", &namelist, 0, alphasort);
    if (n < 0) {
        return NULL;
    }
    while (n--) {
        infp = __uio_info_from_name(namelist[n]->d_name, filter_num);
        free(namelist[n]);
        if (!infp) {
            continue;
        }
        if (!infolist) {
            infolist = infp;
        } else {
            last->next = infp;
        }
        last = infp;
    }
    free(namelist);
    return infolist;
}

struct uio_info_t* uio_find_devices_byname(const char* filter_name) {
    struct dirent** namelist;
    struct uio_info_t *infolist = NULL, *infp, *last;
    int n;

    n = scandir("/sys/class/uio", &namelist, 0, alphasort);
    if (n < 0) {
        return NULL;
    }
    while (n--) {
        infp = __uio_info_byname(namelist[n]->d_name, filter_name);
        free(namelist[n]);
        if (!infp)
            continue;
        if (!infolist) {
            infolist = infp;
        } else {
            last->next = infp;
        }
        last = infp;
    }
    free(namelist);
    return infolist;
}

int uio_get_all_info(struct uio_info_t* info) {
    int i;
    if (!info) {
        return -1;
    }
    if ((info->uio_num < 0) || (info->uio_num > UIO_MAX_NUM)) {
        return -1;
    }
    info->map_count = 0;
    for (i = 0; i < MAX_UIO_MAPS; i++) {
        uio_get_mem_size(info, i);
        uio_get_mem_addr(info, i);
        if (info->maps[i].size >= 0) {
            info->map_count++;
        }
    }
    uio_get_event_count(info);
    uio_get_name(info);
    uio_get_version(info);
    uio_get_device_attributes(info);
    return 0;
}

void uio_free_info(struct uio_info_t* info) {
    struct uio_info_t *p1, *p2;
    p1 = info;
    while (p1) {
        uio_free_dev_attrs(p1);
        p2 = p1->next;
        free(p1);
        p1 = p2;
    }
}

void uio_free_dev_attrs(struct uio_info_t* info) {
    struct uio_dev_attr_t *p1, *p2;
    p1 = info->dev_attrs;
    while (p1) {
        p2 = p1->next;
        free(p1);
        p1 = p2;
    }
    info->dev_attrs = NULL;
}

int uio_get_event_count(struct uio_info_t* info) {
    int ret;
    char filename[64];
    info->event_count = 0;
    snprintf(filename, sizeof(filename), "/sys/class/uio/uio%d/event", info->uio_num);
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    ret = fscanf(file, "%ld", &info->event_count);
    fclose(file);
    if (ret < 0) {
        return -2;
    }
    return 0;
}

static int __uio_dev_attr_filter(char* filename) {
    struct stat filestat;

    if (lstat(filename, &filestat)) {
        return 0;
    }
    if (S_ISREG(filestat.st_mode)) {
        return 1;
    }
    return 0;
}

int uio_get_device_attributes(struct uio_info_t* info) {
    struct dirent** namelist;
    struct uio_dev_attr_t *attr, *last;
    char fullname[300];
    int n;

    last = NULL;
    info->dev_attrs = NULL;
    snprintf(fullname, sizeof(fullname), "/sys/class/uio/uio%d/device", info->uio_num);
    n = scandir(fullname, &namelist, 0, alphasort);
    if (n < 0) {
        return -1;
    }

    while (n--) {
        snprintf(fullname, sizeof(fullname), "/sys/class/uio/uio%d/device/%s", info->uio_num,
                 namelist[n]->d_name);
        if (!__uio_dev_attr_filter(fullname)) {
            continue;
        }
        attr = malloc(sizeof(struct uio_dev_attr_t));
        if (!attr) {
            return -1;
        }
        strncpy(attr->name, namelist[n]->d_name, UIO_MAX_NAME_SIZE);
        free(namelist[n]);
        if (__uio_line_from_file(fullname, attr->value)) {
            free(attr);
            continue;
        }
        if (!info->dev_attrs) {
            info->dev_attrs = attr;
        } else {
            last->next = attr;
        }
        attr->next = NULL;
        last = attr;
    }
    free(namelist);

    return 0;
}

int uio_get_mem_addr(struct uio_info_t* info, int map_num) {
    int ret;
    char filename[64];
    if (map_num >= MAX_UIO_MAPS) {
        return -1;
    }
    info->maps[map_num].addr = UIO_INVALID_ADDR;
    snprintf(filename, sizeof(filename), "/sys/class/uio/uio%d/maps/map%d/addr", info->uio_num,
             map_num);
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    ret = fscanf(file, "0x%lx", &info->maps[map_num].addr);
    fclose(file);
    if (ret < 0) {
        return -2;
    }
    return 0;
}

int uio_get_mem_size(struct uio_info_t* info, int map_num) {
    int ret;
    char filename[64];
    if (map_num >= MAX_UIO_MAPS) {
        return -1;
    }
    info->maps[map_num].size = UIO_INVALID_SIZE;
    snprintf(filename, sizeof(filename), "/sys/class/uio/uio%d/maps/map%d/size", info->uio_num,
             map_num);
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    ret = fscanf(file, "0x%x", &info->maps[map_num].size);
    fclose(file);
    if (ret < 0) {
        return -2;
    }
    return 0;
}

int uio_get_name(struct uio_info_t* info) {
    char filename[64];
    snprintf(filename, sizeof(filename), "/sys/class/uio/uio%d/name", info->uio_num);

    return __uio_line_from_file(filename, info->name);
}

int uio_get_version(struct uio_info_t* info) {
    char filename[64];
    snprintf(filename, sizeof(filename), "/sys/class/uio/uio%d/version", info->uio_num);

    return __uio_line_from_file(filename, info->version);
}

void uio_single_munmap(struct uio_info_t* info, int map_num) {
    if (info->maps[map_num].mmap_result == UIO_MMAP_OK) {
        munmap(info->maps[map_num].internal_addr, info->maps[map_num].size);
        info->maps[map_num].mmap_result = UIO_MMAP_NOT_DONE;
    }
}

void uio_munmap(struct uio_info_t* info) {
    int i;
    for (i = 0; i < MAX_UIO_MAPS; i++) {
        uio_single_munmap(info, i);
    }
}

void* uio_single_mmap(struct uio_info_t* info, int map_num, int fd) {
    if (!fd) {
        return NULL;
    }
    info->maps[map_num].mmap_result = UIO_MMAP_NOT_DONE;
    if (info->maps[map_num].size <= 0) {
        return NULL;
    }
    info->maps[map_num].mmap_result = UIO_MMAP_FAILED;
    info->maps[map_num].internal_addr = mmap(NULL, info->maps[map_num].size, PROT_READ | PROT_WRITE,
                                             MAP_SHARED, fd, map_num * getpagesize());

    if (info->maps[map_num].internal_addr != MAP_FAILED) {
        info->maps[map_num].mmap_result = UIO_MMAP_OK;
        return info->maps[map_num].internal_addr;
    } else {
        printf("UIO_ERROR: mmap[%i] returned %i when size was %i.\n", map_num, errno,
               info->maps[map_num].size);
    }
    return NULL;
}

void uio_mmap(struct uio_info_t* info, int fd) {
    int map_num;
    if (!fd) {
        return;
    }
    for (map_num = 0; map_num < MAX_UIO_MAPS; map_num++) {
        uio_single_mmap(info, map_num, fd);
    }
}
