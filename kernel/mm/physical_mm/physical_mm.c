/* Bitmap Based Allocation:
 * https://web.archive.org/web/20190316115948/http://www.brokenthorn.com/Resources/OSDev17.html
 */

#include <stdint.h>

#include <libk/stdio.h>

#include <mm/memory_map.h>
#include <mm/physical_mm.h>

extern uint32_t kernel_start;
extern uint32_t kernel_end;

void
physical_mm_init(void)
{
  free_memory_regions_t *free_memory_regions = memory_map_get_free_regions();
  for (int i = 0; i < free_memory_regions->n_regions; i++)
    printk("Kernel",
           "start: 0x%.08x | length: 0x%.08x",
           free_memory_regions->region_list[i]->addr_low,
           free_memory_regions->region_list[i]->len_low);

  printk("physical_mm", "Kernel starts at: 0x%x", &kernel_start);
  printk("physical_mm", "Kernel ends at: 0x%x", &kernel_end);
}
