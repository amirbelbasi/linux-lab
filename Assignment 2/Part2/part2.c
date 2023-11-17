#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday{
	int day;
	int month;
	int year;
	struct list_head list;	
};

static LIST_HEAD(birthday_list);

struct birthday *make_new_node(int year, int month, int day){
	struct birthday *person = kmalloc(sizeof(struct birthday), GFP_KERNEL);
	person->day = day;
	person->month = month;
	person->year = year;
	return person;
}

int simple_init(void){
    printk(KERN_INFO "Loading Module\n");
    INIT_LIST_HEAD(&birthday_list);
    struct birthday *person;
    person = make_new_node(1, 1, 1950);
	list_add_tail(&person->list, &birthday_list);
    person = make_new_node(2, 2, 1960);
	list_add_tail(&person->list, &birthday_list);
    person = make_new_node(3, 3, 1970);
	list_add_tail(&person->list, &birthday_list);
    person = make_new_node(4, 4, 1980);
	list_add_tail(&person->list, &birthday_list);
    person = make_new_node(5, 5, 1990);
	list_add_tail(&person->list, &birthday_list);
    struct birthday *ptr;
	list_for_each_entry(ptr, &birthday_list, list) {
		printk(KERN_INFO "Item Added: %d.%d.%d\n", ptr->day, ptr->month, ptr->year);
	}
    return 0;
}

void simple_exit(void){
    struct birthday *tmp;
	struct list_head *ptr, *next;
	if (list_empty(&birthday_list)) return;
	list_for_each_safe(ptr, next, &birthday_list){
		tmp = list_entry(ptr, struct birthday, list);
		printk(KERN_INFO "Item Removed: %d.%d.%d\n", tmp->day, tmp->month, tmp->year);
		list_del(ptr);
		kfree(tmp);
	}
    printk(KERN_INFO "Removing Module\n");
}

module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple module");
MODULE_AUTHOR("SGG");