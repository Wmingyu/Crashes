#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

/*
 * Self-contained DRM UAPI bits.
 * UAPI defines:
 *   DRM_IOCTL_MODE_CREATE_DUMB = DRM_IOWR(0xB2, struct drm_mode_create_dumb)
 *   DRM_IOCTL_GEM_CHANGE_HANDLE = DRM_IOWR(0xD2, struct drm_gem_change_handle)
 * (base is 'd')
 */
#define DRM_IOCTL_BASE 'd'
#define DRM_IOWR(nr, type) _IOWR(DRM_IOCTL_BASE, nr, type)

struct drm_mode_create_dumb {
  uint32_t height;
  uint32_t width;
  uint32_t bpp;
  uint32_t flags;
  uint32_t handle; // returned
  uint32_t pitch;  // returned
  uint64_t size;   // returned
};

struct drm_gem_change_handle {
  uint32_t handle;
  uint32_t new_handle;
};

#define DRM_IOCTL_MODE_CREATE_DUMB     DRM_IOWR(0xB2, struct drm_mode_create_dumb)
#define DRM_IOCTL_GEM_CHANGE_HANDLE    DRM_IOWR(0xD2, struct drm_gem_change_handle)

int main(void) {
  const char *dev = "/dev/dri/card0";
  int fd = open(dev, O_RDWR | O_CLOEXEC);
  if (fd < 0) {
    fprintf(stderr, "open(%s) failed: %s\n", dev, strerror(errno));
    return 1;
  }

  // 1) Create a dumb buffer to obtain a valid GEM handle.
  struct drm_mode_create_dumb create;
  memset(&create, 0, sizeof(create));
  create.width = 4;
  create.height = 8;
  create.bpp = 32; // typical value; kernel returns a handle on success
  if (ioctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create) != 0) {
    fprintf(stderr, "DRM_IOCTL_MODE_CREATE_DUMB failed: %s\n", strerror(errno));
    close(fd);
    return 1;
  }
  printf("CREATE_DUMB ok: handle=%u pitch=%u size=%llu\n",
         create.handle, create.pitch, (unsigned long long)create.size);

  // 2) Trigger point: request a new_handle >= 0x80000000.
  struct drm_gem_change_handle ch;
  memset(&ch, 0, sizeof(ch));
  ch.handle = create.handle;
  ch.new_handle = 0x80000000u;

  errno = 0;
  int ret = ioctl(fd, DRM_IOCTL_GEM_CHANGE_HANDLE, &ch);
  int saved = errno;

  printf("GEM_CHANGE_HANDLE ret=%d errno=%d (%s)\n",
         ret, saved, strerror(saved));

  close(fd);
  return 0;
}
