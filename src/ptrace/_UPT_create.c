/* libunwind - a platform-independent unwind library
   Copyright (C) 2003 Hewlett-Packard Co
        Contributed by David Mosberger-Tang <davidm@hpl.hp.com>

This file is part of libunwind.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

#include <string.h>

#include "_UPT_internal.h"

void *
_UPT_create (pid_t pid)
{
  struct UPT_info *ui = malloc (sizeof (struct UPT_info));

  if (!ui)
    return NULL;

  memset (ui, 0, sizeof (*ui));
  ui->pid = pid;
  ui->procfs_fd = -1;
  ui->edi.di_cache.format = -1;
  ui->edi.di_debug.format = -1;
#if UNW_TARGET_IA64
  ui->edi.ktab.format = -1;
#endif
  return ui;
}

void *
_UPT_create_ns (pid_t pid)
{
  char procfs_path[PATH_MAX];
  snprintf (procfs_path, sizeof (procfs_path), "/proc/%d", pid);
  int procfs_fd = open(procfs_path, O_RDONLY | O_DIRECTORY | O_PATH | O_CLOEXEC);
  if (0 > procfs_fd)
    return NULL;

  struct UPT_info *ui = _UPT_create (pid);
  ui->procfs_fd = procfs_fd;
  return ui;
}
