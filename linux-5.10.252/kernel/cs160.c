// SPDX-License-Identifier: GPL-2.0-only

#include "asm/processor.h"
#include <linux/kernel.h>
#include <linux/gfp.h> // For __GFP_KERNEL
#include <linux/syscalls.h> // For SYSCALL_DEFINE macros
#include <asm/page.h> // For get_zeroed_page and free_page
#include <linux/errno.h> // For error constants like -ENOMEM
#include <asm/unistd.h> // You'll probably need stuff here
#include <linux/security.h> // In case you need anything here for permissions checking
#include <asm/paravirt.h> // For __pa and __va. This maps between physical and virtual addresses.
#include <asm/tlbflush.h> // You will want to flush the TLB after modifying page tables

// C89 does not have _Bool. Instaed, you can use `
#define true 1
#define false 0

// If you need more includes, you can add them here.

// End includes

/**
 * cs160_read_mmu - Walk the page table and return the entries for each level of the page table
 * @vaddr: The virtual address to read the MMU entry for
 * @pml4ep: Pointer to store the PML4 entry
 * @pdptep: Pointer to store the PDPTE entry
 * @pdep: Pointer to store the PDE entry
 * @ptep: Pointer to store the PTE entry
 *
 * To implement this syscall, you will calculate the address of each page table entry for the given virtual address.
 * Then, you will read the value of each entry and store it in the provided pointers.
 * You will need to use bit manipulation to extract the relevant bits from the virtual address and each page table entry.
 * Note that some bits of the page table entry are used to indicate certain things, such as whether or not the page is a large page
 * You will consult the x86 manual I provided to understand how to interpret the bits of the page table entries.
 *
 * Once you have computed the address of an entry, you will need to read its value. Note that the page tables hold physical addresses
 * but the kernel operates with virtual addresses. The __va and __pa macros can be used to convert between physical and virtual addresses.
 * You will need to use these macros to read the page table entries correctly.
 * 
 * If any errors occur, you must not crash the kernel. Instead, you should return a negative value corresponding to the appropriate error code.
 * For example, if the virtual address is not mapped, you might return -EFAULT.
 * Do not worry about permission checks for this syscall.
 * This means you MUST check if each address is mapped and points to a valid entry.
 *
 * Note that you will need to use the `copy_to_user` function to convert kernel-space pointers to user-space pointers.
 * 
 * You must not use any helper functions that walk the page tables for you. You must do all the calculations and walks yourself.
 * You can, however, use the helper function to read the cr3 register.
 */
SYSCALL_DEFINE5(cs160_read_mmu, unsigned long, vaddr, unsigned long *, pml4ep,
		unsigned long *, pdptep, unsigned long *, pdep, unsigned long *,
		ptep)
{
	// You should get rid of the __maybe_unused annotations once you start using these variables.
	// I only have them here for the starter code.
	unsigned long cr3;
	__maybe_unused unsigned long pml4e_address;
	__maybe_unused unsigned long pdpte_address;
	__maybe_unused unsigned long pde_address;
	__maybe_unused unsigned long pte_address;

	cr3 = read_cr3_pa();

	// Get rid of this printk once you start implementing the syscall. You can use it for debugging if you want, but it's not required.
	printk("Hello from cs160_read_mmu!. Your CR3 value is: %lu\n", cr3);

	// Step 1: Find the entry for the current process' pml4 entry.
	// You will need to construct an address from the bits in cr3 and the bits in the virtual address.
	// Then, read the value at the address and store it in the provided pointer.
	// Don't forget to use copy_to_user and check for errors when writing to user-space pointers.

	// Step 2: Find the entry for the virtual address' pdpt entry.
	// If it's a 1GB super page, 
	// You will construct a pointer to the pdpt entry from the pml4 entry and the bits in the virtual address.
	// Then, read the value at the address and store it in the provided pointer.

	// Step 2b. Check if the pdpt entry indicates that this is a 1GB super page. If it is, then you should not continue walking the page table.

	// Step 3: Find the entry for the virtual address' pd entry, or check if it's a large page.
	// You will construct a pointer to the pd entry from the pdpt entry and the bits in the virtual address.
	// Then, read the value at the address and store it in the provided pointer.

	// Step 3b. Check if the pd entry indicates that this is a 2MB large page. If it is, then you should not continue walking the page table.

	// Step 4: Find the entry for the virtual address' pt entry.
	// You will construct a pointer to the pt entry from the pd entry and the bits in the virtual address.
	// Then, read the value at the address and store it in the provided pointer.
	return 0;
}

/**
 * cs160_write_mmu - Update the page table entries for a given virtual address.
 * @vaddr: The virtual address to write to
 * @pml4e: The Page map level 4 entry value
 * @pdpte: The PDPT entry value
 * @pde: The PD entry value
 * @pte: The PT entry value
 *
 * This function writes values to the MMU entries for a given virtual address.
 * You will need to calculate the address of each page table entry for the given virtual address, just like in cs160_read_mmu.
 * It's probably worth putting those calculations in a helper function since you'll need to do them in both syscalls.
 * Then, you will write the provided values to each entry.
 * Do not start writing values until after you have calculated the address of each entry and checked that it's valid to write to.
 * Before returning, don't forget to flush the TLB!
 * Return: 0 on success, or an error code on failure.

 */
SYSCALL_DEFINE5(cs160_write_mmu, unsigned long, vaddr, unsigned long, pml4e,
		unsigned long, pdpte, unsigned long, pde, unsigned long, pte)
{

	printk("Hello from cs160_write_mmu!\n");

	return 0;
}

/**
 * allocPage - Allocate a page and return its physical address
 *
 * This function allocates a page and returns its physical address.
 *
 * Return: The physical address of the allocated page, or -ENOMEM if allocation fails.
 */
SYSCALL_DEFINE0(cs160_alloc_page)
{
	unsigned long *p = (unsigned long *)get_zeroed_page(GFP_KERNEL);
	// If p was null, get_zeroed_page failed to allocate a page, so we return -ENOMEM.
	// When a syscall returns a negative value, it indicates an error
	// The kernel will automatically convert this negative value to a positive errno value in userspace.
	return p == NULL ? -ENOMEM : (unsigned long)__pa(p);
}

/**
 * freePage - Free a page given its physical address
 * @physaddr: The physical address of the page to free
 *
 * This function converts the physical address to a virtual address and then
 * frees the page.
 */
SYSCALL_DEFINE1(cs160_free_page, unsigned long, physaddr)
{
	free_page((unsigned long)__va(physaddr));
	return 0;
}