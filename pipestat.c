#define pr_fmt(fmt) "%s: " fmt, KBUILD_MODNAME

#include <linux/dcache.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm_types.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/namei.h>
#include <linux/pipe_fs_i.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");

static char *path = "";
module_param(path, charp, 0);

int init_module(void) {
  struct path path2;
  if (kern_path(path, LOOKUP_FOLLOW, &path2)) goto error;
  struct dentry *dentry = path2.dentry;
  if (!dentry) goto error2;
  struct inode *inode = dentry->d_inode;
  if (!inode) goto error2;
  if (!S_ISFIFO(inode->i_mode)) goto error2;
  struct pipe_inode_info *pipe = inode->i_pipe;
  if (!pipe) goto error2;
  pr_info("%s: ~ %ld B totally written, ~ %ld B currently buffered\n", path,
          pipe->head * PAGE_SIZE, (pipe->head - pipe->tail) * PAGE_SIZE);
  return -EBUSY;
error2:
  path_put(&path2);
error:
  return -EINVAL;
}
