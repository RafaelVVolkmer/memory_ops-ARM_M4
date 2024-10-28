/**
 *  @ingroup    MemoryManagement
 *  @addtogroup MemoryManagement memory_operations
 *  @{
 *
 *  @package    memory_operations
 *  @brief      This module provides functionalities for managing memory operations, 
 *              including comparing, copying, and filling structures. 
 *              It abstracts the hardware details and provides a high-level API for memory operations.
 *
 *  @file       memory_ops.h
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

#ifndef MEMORY_OPS_H_
#define MEMORY_OPS_H_

/* =================================
 *       PUBLIC INCLUDE FILES     *
 * ================================*/

/* dependencies: */
#include <errno.h>

/* =================================
 *          PUBLIC DEFINES         *
 * ================================*/

/**
 * @def MEMORY_START
 * @brief Macro to define the start of memory.
 * @return The memory start value as uint32_t.
 **/
#define MEMORY_START (uint32_t)(0u)

/* =================================
 *      PUBLIC STATUS ENUMS     *
 * ================================*/

/**
 * @enum compareStructs
 * @brief Enumeration to define the possible states of structure comparison.
 * @package memory_operations
 *
 * @typedef MEM_struct_compare_t
 **/
typedef enum compareStructs
{
    STRUCTS_ARE_EQUAL       = (uint8_t)(1u), /**< Structures are equal */
    STRUCTS_ARENT_EQUAL     = (uint8_t)(0u), /**< Structures are not equal */
    STRUCTS_COMPARE_ERROR   = -(ENOSYS),     /**< Error in structure comparison */
    COMPARE_BAD_ADDRESS     = -(EFAULT)      /**< NULL pointer */
} MEM_struct_compare_t;

/**
 * @enum copyStructs
 * @brief Enumeration to define the possible states of structure copying.
 * @package memory_operations
 *
 * @typedef MEM_struct_copy_t
 **/
typedef enum copyStructs
{
    STRUCT_COPIED           = (uint8_t)(0u), /**< Structure copied successfully */
    STRUCT_NOT_COPIED       = (uint8_t)(1u), /**< Structure not copied correctly */
    STRUCT_COPY_ERROR       = -(ENOSYS),     /**< Error in structure copying */
    COPY_BAD_ADDRESS        = -(EFAULT)      /**< NULL pointer */
} MEM_struct_copy_t;

/**
 * @enum fillStruct
 * @brief Enumeration to define the possible states of structure filling.
 * @package memory_operations
 *
 * @typedef MEM_struct_fill_t
 **/
typedef enum fillStruct
{
    STRUCT_FILLED           = (uint8_t)(0u), /**< Structure filled successfully */
    STRUCT_NOT_FILLED       = (uint8_t)(1u), /**< Structure not filled correctly */
    STRUCT_FILL_ERROR       = -(ENOSYS),     /**< Error in structure filling */
    FILL_BAD_ADDRESS        = -(EFAULT)      /**< NULL pointer */
} MEM_struct_fill_t;

/* =================================
 *    PUBLIC FUNCTION PROTOTYPES   *
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
MEM_struct_compare_t MEM_compareStructs(const void *struct_a, const void *struct_b, size_t size);

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
MEM_struct_copy_t MEM_copyStruct(const void *source, void *destine, size_t size);

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
MEM_struct_fill_t MEM_fillStruct(void *struct_ptr, size_t size, uint8_t value);

#endif /* #ifndef MEMORY_OPS_H_ */
/**@}*/
/**@}*/
