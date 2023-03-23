#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/cpu.h>


static ssize_t sample_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	int n = 0;
	char kb[16];

	if (*ppos != 0) {
		return 0;
	}
	// 这里我们把1234的输出给fix成4321的输出
	n = sprintf(kb, "%d\n", 4321);
	memcpy(ubuf, kb, n);
	*ppos += n;
	return n;
}

static unsigned long *(*_mod_find_symname)(struct module *mod, const char *name);
static struct mutex *_text_mutex;
struct module *mod;

static unsigned long sample_func_addr;
static unsigned long mod_func_addr;
static u32 store_orig_sample;			
static u32 store_jump_sample;			

static int __init re_init(void)
{
    //
    //0.准备工作
    //
	_text_mutex = (void *)kallsyms_lookup_name("text_mutex");
	if (!_text_mutex) {
		printk("text相关工作失败!");
		return -1;
	}

    //
    //1.获取原函数的地址
    //测试版本，测试jmp机制的可行，所以先替换模块内的函数
    //

    //获取mod_find_symname函数
    _mod_find_symname = (void *)kallsyms_lookup_name("mod_find_symname");
	if (!_mod_find_symname) {
		printk("mod_find_symname函数获取失败!");
		return -1;
	}

    //查找原函数的模块
    mod = find_module("sample");
	if (!mod) {
		printk("需要加载sample模块\n");
		return;
	}

    //查找模块中的被替换函数
    sample_func_addr = _mod_find_symname(mod, "sample_read");
	if (!sample_func_addr) {
		printk("一切还没有准备好!请先加载sample模块。\n");
		return -1;
	}

    //
    //2.获取目前函数的地址
    //
    mod_func_addr = (unsigned long)sample_read;
	memcpy((void *)&store_orig_sample, sample_func_addr, AARCH64_INSN_SIZE); 
	store_jump_sample = aarch64_insn_gen_branch_imm(sample_func_addr,mod_func_addr, AARCH64_INSN_BRANCH_NOLINK); 

    //
    //3.替换
    //
	get_online_cpus();
	mutex_lock(_text_mutex);
    aarch64_insn_patch_text_nosync(sample_func_addr, store_jump_sample);
    mutex_unlock(_text_mutex);
	put_online_cpus();

}

static void __exit re_exit(void)
{
	get_online_cpus();
	mutex_lock(_text_mutex);
    aarch64_insn_patch_text_nosync(sample_func_addr, store_orig_sample);
    mutex_unlock(_text_mutex);
	put_online_cpus();
}