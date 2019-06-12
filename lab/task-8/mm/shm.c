#include <asm/segment.h>
#include <linux/kernel.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>

#define SHM_BLOCK_COUNT 20
#define SHM_NAME_SIZE 20
#define SHM_ILLEGAL_SIGN 1


struct shm_entry {
	int occupied;
	char name[SHM_NAME_SIZE];
	long addr;
} _shm_table[SHM_BLOCK_COUNT];


int _search_shm(char* name) {
	int i;
	
	for (i = 0; i < SHM_BLOCK_COUNT; i++) {
		if (!strcmp(name, _shm_table[i].name) && _shm_table[i].occupied) {
			return i;
		}
	}
	return -SHM_ILLEGAL_SIGN;
}


int sys_shmget(char* name) {
	int i, shm_id;
	char tmp_name[SHM_NAME_SIZE];
	
	for (i = 0; i < SHM_NAME_SIZE; i++) {
		tmp_name[i] = get_fs_byte(name + i);
		if (tmp_name[i] == '\0') {
			break;
		}
	}
	
	shm_id = _search_shm(tmp_name);
	if (shm_id != -SHM_ILLEGAL_SIGN) {
		return shm_id;
	}
	
	for (i = 0; i < SHM_BLOCK_COUNT; i++) {
		if (!_shm_table[i].occupied) {
			stpcpy(_shm_table[i].name, tmp_name);
			_shm_table[i].occupied = 1;
			_shm_table[i].addr = get_free_page();
			return i;
		}
	}
	
	printk("SHM capacity limited!\n");
	return -SHM_ILLEGAL_SIGN;
}


void* sys_shmat(int shm_id) {
	if (_shm_table[shm_id].occupied != 1) {
		printk("SHM not exists!\n");
		return -SHM_ILLEGAL_SIGN;
	}
	
	put_page(_shm_table[shm_id].addr, current->brk + current->start_code);
	return (void*)current->brk;
}
