/**
 * @file	syscalls.c
 * @author	Wojciech Macek <wojciech.macek@gmail.com>
 * @date	05/04/2014
 *
 * @par LICENSE
 * Copyright (C) Wojciech Macek - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * Proprietary and confidential.
 */

#define SYSCALL_WEAK	__attribute__ ((weak, alias("_generic_syscall_handler")))

int SYSCALL_WEAK _close(void);
int SYSCALL_WEAK _fstat(void);
int SYSCALL_WEAK _isatty(void);
int SYSCALL_WEAK _lseek(void);
int SYSCALL_WEAK _read(void);
int SYSCALL_WEAK _write(void);

static int
_generic_syscall_handler(void)
{
	return 0;
}
