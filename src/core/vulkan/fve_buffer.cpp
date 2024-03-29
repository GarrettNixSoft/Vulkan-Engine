/*
 * Encapsulates a vulkan buffer
 *
 * Initially based off VulkanBuffer by Sascha Willems -
 * https://github.com/SaschaWillems/Vulkan/blob/master/base/VulkanBuffer.h
 */

#include "fve_buffer.hpp"
#include "../fve_globals.hpp"
#include "../../core/utils/fve_logger.hpp"

 // std
#include <cassert>
#include <cstring>
#include <iostream>

int BUFFER_ALLOCATIONS = 0;

namespace fve {

    /**
     * Returns the minimum instance size required to be compatible with devices minOffsetAlignment
     *
     * @param instanceSize The size of an instance
     * @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
     * minUniformBufferOffsetAlignment)
     *
     * @return VkResult of the buffer mapping call
     */
    VkDeviceSize FveBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
        if (minOffsetAlignment > 0) {
            return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
        }
        return instanceSize;
    }

    FveBuffer::FveBuffer(
        VmaAllocator allocator,
        FveDevice& device,
        VkDeviceSize instanceSize,
        uint32_t instanceCount,
        VkBufferUsageFlags usageFlags,
        VmaMemoryUsage vmaUsage,
        const char* debugFlag,
        VkDeviceSize minOffsetAlignment) :
        allocator{ allocator },
        device{ device },
        instanceSize{ instanceSize },
        instanceCount{ instanceCount },
        usageFlags{ usageFlags } {
        // align buffer properly
        alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
        // compute aligned size
        bufferSize = alignmentSize * instanceCount;
        // create the buffer
        device.allocateBuffer(buffer, bufferSize, usageFlags, vmaUsage, debugFlag);
    }

    FveBuffer::~FveBuffer() {
        // check the buffer info before it's destroyed
        VmaAllocationInfo allocInfo{};
        vmaGetAllocationInfo(allocator, buffer.allocation, &allocInfo);
        // destroy the buffer
        unmap();
        vmaDestroyBuffer(allocator, buffer.buffer, buffer.allocation);
        BUFFER_ALLOCATIONS--;
        FVE_CORE_TRACE("Destroyed buffer! Active allocations: {0}", BUFFER_ALLOCATIONS);
        // debug
        if (allocInfo.pUserData) FVE_CORE_TRACE("The buffer destroyed had user pointer data: {0}", (const char*)allocInfo.pUserData);
    }

    /**
     * Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
     *
     * @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete
     * buffer range.
     * @param offset (Optional) Byte offset from beginning
     *
     * @return VkResult of the buffer mapping call
     */
    VkResult FveBuffer::map(VkDeviceSize size, VkDeviceSize offset) {
        assert(buffer.buffer && "Called map on buffer before create");
        return vmaMapMemory(allocator, buffer.allocation, &mapped);
    }

    /**
     * Unmap a mapped memory range
     *
     * @note Does not return a result as vkUnmapMemory can't fail
     */
    void FveBuffer::unmap() {
        if (mapped) {
            vmaUnmapMemory(allocator, buffer.allocation);
            mapped = nullptr;
        }
    }

    /**
     * Copies the specified data to the mapped buffer. Default value writes whole buffer range
     *
     * @param data Pointer to the data to copy
     * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer
     * range.
     * @param offset (Optional) Byte offset from beginning of mapped region
     *
     */
    void FveBuffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset) {
        assert(mapped && "Cannot copy to unmapped buffer");

        if (size == VK_WHOLE_SIZE) {
            memcpy(mapped, data, bufferSize);
        }
        else {
            char* memOffset = (char*)mapped;
            memOffset += offset;
            memcpy(memOffset, data, size);
        }
    }

    /**
     * Flush a memory range of the buffer to make it visible to the device
     *
     * @note Only required for non-coherent memory
     *
     * @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
     * complete buffer range.
     * @param offset (Optional) Byte offset from beginning
     *
     * @return VkResult of the flush call
     */
    VkResult FveBuffer::flush(VkDeviceSize size, VkDeviceSize offset) {
        return vmaFlushAllocation(allocator, buffer.allocation, offset, size);
    }

    /**
     * Invalidate a memory range of the buffer to make it visible to the host
     *
     * @note Only required for non-coherent memory
     *
     * @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
     * the complete buffer range.
     * @param offset (Optional) Byte offset from beginning
     *
     * @return VkResult of the invalidate call
     */
    VkResult FveBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
        return vmaInvalidateAllocation(allocator, buffer.allocation, offset, size);
    }

    /**
     * Create a buffer info descriptor
     *
     * @param size (Optional) Size of the memory range of the descriptor
     * @param offset (Optional) Byte offset from beginning
     *
     * @return VkDescriptorBufferInfo of specified offset and range
     */
    VkDescriptorBufferInfo FveBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
        return VkDescriptorBufferInfo{
            buffer.buffer,
            offset,
            size,
        };
    }

    /**
     * Copies "instanceSize" bytes of data to the mapped buffer at an offset of index * alignmentSize
     *
     * @param data Pointer to the data to copy
     * @param index Used in offset calculation
     *
     */
    void FveBuffer::writeToIndex(void* data, int index) {
        writeToBuffer(data, instanceSize, index * alignmentSize);
    }

    /**
     *  Flush the memory range at index * alignmentSize of the buffer to make it visible to the device
     *
     * @param index Used in offset calculation
     *
     */
    VkResult FveBuffer::flushIndex(int index) { return flush(alignmentSize, index * alignmentSize); }

    /**
     * Create a buffer info descriptor
     *
     * @param index Specifies the region given by index * alignmentSize
     *
     * @return VkDescriptorBufferInfo for instance at index
     */
    VkDescriptorBufferInfo FveBuffer::descriptorInfoForIndex(int index) {
        return descriptorInfo(alignmentSize, index * alignmentSize);
    }

    /**
     * Invalidate a memory range of the buffer to make it visible to the host
     *
     * @note Only required for non-coherent memory
     *
     * @param index Specifies the region to invalidate: index * alignmentSize
     *
     * @return VkResult of the invalidate call
     */
    VkResult FveBuffer::invalidateIndex(int index) {
        return invalidate(alignmentSize, index * alignmentSize);
    }

}
