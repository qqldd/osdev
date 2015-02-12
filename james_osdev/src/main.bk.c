// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "common.h"
#include "monitor.h"
#include "stdio.h"
#include "string.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "kheap.h"
#include "fs.h"
#include "initrd.h"
#include "multiboot.h"
#include "task.h"

extern u32int placement_address;

u32int initial_esp;

int main(struct multiboot *mboot_ptr, u32int initial_stack)
{
    initial_esp = initial_stack;
	// Initialize all the ISRs and segmentation
	init_descriptor_tables();
		//So we can see that GDT and IDT are set at the begining

    // All our initialisation calls will go in here.
    monitor_clear();
    
	 // Initialise the PIT to 100Hz
    asm volatile("sti");
    init_timer(50);

    monitor_write("Hello, world!");
    
    monitor_write("\nTest monitor_write_hex (0x12ab56cd):");
    monitor_write_hex(0x12ab56cd);

    monitor_write("\nTest monitor_write_dec (12345):");
    monitor_write_dec(12345);
    monitor_write("\n");

    printf("\nTesting printf function\n");
    printf("print unsigned integer: %u\n", 418);
    printf("print unsigned hex value: %x\n", 0xabcd);

    printf("\nTesting memcpy\n");
    char src[] = "abcd";
    char dst[10];
    for(int i=0; i<9; i++){
        dst[i] = 'p' + i;
    }
    dst[9] = '\0';
    printf("The content in char* dst before memcpy:\n");
    printf("%s\n", dst);
    printf("copy size in byte: %u\n", sizeof(src)-1);
    memcpy((void*) &dst[0], (void*) src, sizeof(src)-1);
    printf("The content in char* dst:\n");
    printf("%s\n", dst);

    printf("\nTesting Interrupt\n");
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
    asm volatile ("sti");

    //printf("\nTesting IRQ and timer\n");
    //init_timer(50);
   
    printf("\nRead initrd module\n");
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*) mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*) (mboot_ptr->mods_addr+4);
    placement_address = initrd_end;

    printf("\nInitializing paging\n"); 
    initialise_paging();
    printf("\nTesting page fault (skipped)\n");
    //u32int* ptr = (u32int*) 0xA0000000;
    //u32int do_page_fault = *ptr;

    printf("\nTesting kernel heap\n");
    u32int b = kmalloc(8);
    u32int c = kmalloc(8);
    printf("b: %x\n", b);
    printf("c: %x\n", c);
    kfree((void*) c); kfree((void*) b);
    u32int d = kmalloc(12);
    printf("d: %x\n", d);

    printf("\nInitializing mutitask\n");
    initialise_tasking();

    printf("\nTesting VFS and initrd\n");
        
    fs_root = initialise_initrd(initrd_location);
    
    // list the contents of /
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
      monitor_write("Found file ");
      monitor_write(node->name);
      fs_node_t *fsnode = finddir_fs(fs_root, node->name);
    
      if ((fsnode->flags&0x7) == FS_DIRECTORY)
        monitor_write("\n\t(directory)\n");
      else
      {
        monitor_write("\n\t contents: \"");
        char buf[256];
        u32int sz = read_fs(fsnode, 0, 256, buf);
        int j;
        for (j = 0; j < sz; j++)
          monitor_put(buf[j]);
    
        monitor_write("\"\n");
      }
      i++;
    } 

    printf("\nTesting multitask\n");
    int ret = fork();
    printf("fork() returned: %x, and getpid() returned: %u\n", ret, getpid());
    printf("===================\n");
    

    return 0xDEADBABA;
}
