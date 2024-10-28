/**
 *  @ingroup    memory_ops
 *  @addtogroup memory_ops memory_operations
 *  @{
 *
 *  @package    memory_operations
 *  @brief      This module provides functionalities for managing memory operations, 
 *              including comparing, copying, and filling structures. 
 *              It abstracts the hardware details and provides a high-level API for memory operations.
 *
 *  @file       memory_ops.c
 *  @author     Rafael V. Volkmer (rafael.v.volkmer@gmail.com)
 * 
 *  @date       28.10.2024
 *
 *  @details    
 *              The Memory module handles various memory operations such as comparing, copying, and filling data structures.
 *              It provides a high-level API that abstracts the underlying hardware details, ensuring portability and ease of use.
 *              The module is designed to optimize memory operations, reduce latency, and ensure data integrity across different hardware platforms.
 *              
 *              Key functionalities include:
 *              - **MEM_compare**: Compares two memory blocks.
 *              - **MEM_copy**: Copies data from one memory block to another.
 *              - **MEM_fill**: Fills a memory block with a specified value.
 *              
 *              The module ensures that all memory operations adhere to the alignment and size requirements of the target architecture,
 *              enhancing performance and preventing potential memory access issues.
 *
 *  @note       
 *              - Ensure that memory operations are performed within the allocated memory boundaries to prevent corruption.
 *              - This module is thread-safe and can be used in multi-threaded applications without additional synchronization.
 *              - Always initialize the memory module before performing any operations.
 *
 *  @see        - memory_ops.h
 **/

/* =================================
 *       PRIVATE INCLUDE FILES     *
 * ================================*/

/* implemented: */
#include "memory_ops.h"

/* =================================
 *   PRIVATE FUNCTION DECLARATION  *
 * ================================*/

/**
 *  @fn      MEM_compareStructs
 *  @package memory_operations
 *
 *  @brief   Compares two structures byte by byte to check if they are equal - ASSEMBLY: ARM x86_64 (TI Arm Clang).
 *
 *  @details This function performs a byte-by-byte comparison of two structures to determine if they are identical.
 *           It checks each byte until a difference is found or until the entire structure has been compared.
 *
 *  @param   struct_a [in]  : Pointer to the first structure.
 *  @param   struct_b [in]  : Pointer to the second structure.
 *  @param   size     [in]  : Size of the structures to be compared.
 *
 *  @return  MEM_struct_compare_t - Returns the comparison status, which can be:
 *              * STRUCTS_ARE_EQUAL       : Structures are equal.
 *              * STRUCTS_ARENT_EQUAL     : Structures are not equal.
 *              * COMPARE_BAD_ADDRESS     : Error due to a null pointer.
 **/

MEM_struct_compare_t MEM_compareStructs(const void *struct_a, const void *struct_b, size_t size)
{
    MEM_struct_compare_t status_out = STRUCTS_ARE_EQUAL;

    if (struct_a == NULL || struct_b == NULL) 
    {
        status_out = COMPARE_BAD_ADDRESS;
        goto return_status;
    }

    asm volatile 
    (
        "cmp_loop:                          \n\t"
        "ldrb r2, [%1], #1                  \n\t"
        "ldrb r3, [%2], #1                  \n\t"
        "cmp r2, r3                         \n\t"
        "bne not_equal                      \n\t"
        "subs %0, %0, #1                    \n\t"
        "bne cmp_loop                       \n\t"
        "b equal                            \n\t"

        "not_equal:                         \n\t"
        "mov %3, %4                         \n\t"
        "b end                              \n\t"

        "equal:                             \n\t"
        "mov %3, %5                         \n\t"

        "end:                               \n\t"
        : "=r" (size), "=r" (struct_a), "=r" (struct_b), "=r" (status_out)
        : "I" (STRUCTS_ARENT_EQUAL), "I" (STRUCTS_ARE_EQUAL), "0" (size), "1" (struct_a), "2" (struct_b)
        : "r2", "r3", "cc", "memory"
    );

return_status:
    return status_out;
}

/**
 *  @fn      MEM_copyStruct
 *  @package memory_operations
 *
 *  @brief   Copies a structure to another byte by byte - ASSEMBLY: ARM x86_64 (TI Arm Clang).
 *
 *  @details This function copies the contents of one structure to another byte by byte. It is useful for duplicating
 *           structures or transferring data from one memory location to another.
 *
 *  @param   source  [in]  : Pointer to the source structure.
 *  @param   destine [out] : Pointer to the destination structure.
 *  @param   size    [in]  : Size of the structure to be copied.
 *
 *  @return  MEM_struct_copy_t - Returns the copy status, which can be:
 *              * STRUCT_COPIED         : Structure copied successfully.
 *              * STRUCT_NOT_COPIED     : Structure not copied correctly.
 *              * COPY_BAD_ADDRESS      : Error due to a null pointer.
 **/

MEM_struct_copy_t MEM_copyStruct(const void *source, void *destine, size_t size)
{
    MEM_struct_copy_t status_out = STRUCT_COPIED;

    if (source == NULL || destine == NULL) 
    {
        status_out = COPY_BAD_ADDRESS;
        goto return_status;
    }
    
    asm volatile 
    (
        "copy_loop:                         \n\t"
        "ldrb r2, [%1], #1                  \n\t"
        "strb r2, [%2], #1                  \n\t"
        "subs %0, %0, #1                    \n\t"
        "bne copy_loop                      \n\t"
        : "=r" (size), "=r" (source), "=r" (destine)
        : "0" (size), "1" (source), "2" (destine)
        : "r2", "cc", "memory"
    );
    
return_status:
    return status_out;
}

/**
 *  @fn      MEM_fillStruct
 *  @package memory_operations
 *
 *  @brief   Fills a structure with a specific value byte by byte - ASSEMBLY: ARM x86_64 (TI Arm Clang).
 *
 *  @details This function fills a structure with a specified value byte by byte. It is useful for initializing
 *           structures or resetting memory to a known state.
 *
 *  @param   struct_ptr [out] : Pointer to the structure to be filled.
 *  @param   size       [in]  : Size of the structure to be filled.
 *  @param   value      [in]  : Value to be used to fill the structure.
 *
 *  @return  MEM_struct_fill_t - Returns the fill status, which can be:
 *              * STRUCT_FILLED        : Structure filled successfully.
 *              * FILL_BAD_ADDRESS     : Error due to a null pointer.
 **/

MEM_struct_fill_t MEM_fillStruct(void *struct_ptr, size_t size, uint8_t value)
{
    MEM_struct_fill_t status_out = STRUCT_FILLED;

    if (struct_ptr == NULL) 
    {
        status_out = FILL_BAD_ADDRESS;
        goto return_status;
    }

    asm volatile 
    (
        "mov r2, %2                         \n\t"
        "fill_loop:                         \n\t"
        "strb r2, [%1], #1                  \n\t"
        "subs %0, %0, #1                    \n\t"
        "bne fill_loop                      \n\t"
        : "=r" (size), "=r" (struct_ptr)
        : "r" (value), "0" (size), "1" (struct_ptr)
        : "r2", "cc", "memory"
    );

return_status:
    return status_out;
}

/*** end of file ***/
