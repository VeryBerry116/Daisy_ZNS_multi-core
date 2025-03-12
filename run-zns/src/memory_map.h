//////////////////////////////////////////////////////////////////////////////////
// memory_map.h for Cosmos+ OpenSSD
// Copyright (c) 2017 Hanyang University ENC Lab.
// Contributed by Yong Ho Song <yhsong@enc.hanyang.ac.kr>
//				  Jaewook Kwak <jwkwak@enc.hanyang.ac.kr>
//				  Sangjin Lee <sjlee@enc.hanyang.ac.kr>
//
// This file is part of Cosmos+ OpenSSD.
//
// Cosmos+ OpenSSD is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// Cosmos+ OpenSSD is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Cosmos+ OpenSSD; see the file COPYING.
// If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Company: ENC Lab. <http://enc.hanyang.ac.kr>
// Engineer: Jaewook Kwak <jwkwak@enc.hanyang.ac.kr>
//
// Project Name: Cosmos+ OpenSSD
// Design Name: Cosmos+ Firmware
// Module Name: Static Memory Allocator
// File Name: memory_map.h
//
// Version: v1.0.0
//
// Description:
//	 - allocate DRAM address space (0x0010_0000 ~ 0x3FFF_FFFF) to each module
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Revision History:
//
// * v1.0.0
//   - First draft
//////////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_MAP_H_
#define MEMORY_MAP_H_

#include "data_buffer.h"
#include "address_translation.h"
#include "request_allocation.h"
#include "request_schedule.h"
#include "request_transform.h"
#include "nvme_io_cmd_completion.h"
#include "request_ipc.h"

#define DRAM_START_ADDR						0x00100000

#define MEMORY_SEGMENTS_CORE0_START_ADDR	DRAM_START_ADDR
#define MEMORY_SEGMENTS_CORE0_END_ADDR		0x001FFFFF

#define MEMORY_SEGMENTS_CORE1_START_ADDR	0x00200000
#define MEMORY_SEGMENTS_CORE1_END_ADDR		0x002FFFFF

#define MEMORY_SEGMENTS_CORE2_START_ADDR	0x00300000
#define MEMORY_SEGMENTS_CORE2_END_ADDR		0x003FFFFF

#define NVME_MANAGEMENT_START_ADDR		    0x01000000
#define NVME_MANAGEMENT_END_ADDR		    0x01FFFFFF

#define SUBMISSION_QUEUE_CORE1_ADDR		    0x0C000000
#define SUBMISSION_QUEUE_CORE1_END_ADDR	    0x0CFFFFFF
#define COMPLETION_QUEUE_CORE1_ADDR		    0x10000000
#define COMPLETION_QUEUE_CORE1_END_ADDR	    0x10FFFFFF

#define SUBMISSION_QUEUE_CORE2_ADDR		    0x14000000
#define SUBMISSION_QUEUE_CORE2_END_ADDR	    0x14FFFFFF
#define COMPLETION_QUEUE_CORE2_ADDR		    0x18000000
#define COMPLETION_QUEUE_CORE2_END_ADDR	    0x18FFFFFF

#define QUEUE_SIZE                          0x100000

#define IDENTIFY_NS_BUFF_CORE1_ADDR	        0x1C000000
#define IDENTIFY_NS_BUFF_CORE1_END_ADDR     0x1CFFFFFF

#define IDENTIFY_NS_BUFF_CORE2_ADDR	        0x20000000
#define IDENTIFY_NS_BUFF_CORE2_END_ADDR     0x20FFFFFF

#define IPC_CMD_ADDR                        0x24000000
#define IPC_CMD_END_ADDR                    0x24FFFFFF

#define IPC_STORAGE_CORE1_ADDR              0x28000000
#define IPC_STORAGE_CORE2_ADDR              0x28000004

#define IPC_START_REQ_CORE1_ADDR            0x28000008
#define IPC_START_REQ_CORE2_ADDR            0x2800000C

#define IPC_START_RES_CORE1_ADDR            0x28000010
#define IPC_START_RES_CORE2_ADDR            0x28000014

#define IPC_BAR_CORE1_ADDR                  0x28000020
#define IPC_BAR_CORE2_ADDR                  0x28000028

#define IPC_DEBUG_IDX_ADDR                  0x28000030

#define IPC_BUFF_CORE1_ADDR                 0x28040000
#define IPC_BUFF_CORE2_ADDR                 0x28080000

#define IPC_DEBUG_ADDR                      0x28100000

#define FTL_MANAGEMENT_START_ADDR			0x30000000

// Uncached & Unbuffered
//for data buffer
#define DATA_BUFFER_BASE_ADDR 				0x30000000
#define SPARE_DATA_BUFFER_BASE_ADDR			(DATA_BUFFER_BASE_ADDR + AVAILABLE_DATA_BUFFER_ENTRY_COUNT * BYTES_PER_DATA_REGION_OF_SLICE)
#define RESERVED_DATA_BUFFER_BASE_ADDR 		(SPARE_DATA_BUFFER_BASE_ADDR + AVAILABLE_DATA_BUFFER_ENTRY_COUNT * BYTES_PER_SPARE_REGION_OF_SLICE)

//for nand request completion
#define COMPLETE_FLAG_TABLE_ADDR			0x37000000
#define STATUS_REPORT_TABLE_ADDR			(COMPLETE_FLAG_TABLE_ADDR + sizeof(COMPLETE_FLAG_TABLE))
#define ERROR_INFO_TABLE_ADDR				(STATUS_REPORT_TABLE_ADDR + sizeof(STATUS_REPORT_TABLE))
#define TEMPORARY_PAY_LOAD_ADDR				(ERROR_INFO_TABLE_ADDR+ sizeof(ERROR_INFO_TABLE))
// cached & buffered
// for buffers
#define DATA_BUFFER_MAP_ADDR		 		0x38000000
// for request pool
#define REQ_POOL_ADDR						(DATA_BUFFER_MAP_ADDR + sizeof(DATA_BUF_MAP))
// for request scheduler
#define DIE_STATE_TABLE_ADDR				(REQ_POOL_ADDR + sizeof(REQ_POOL))
#define RETRY_LIMIT_TABLE_ADDR				(DIE_STATE_TABLE_ADDR + sizeof(DIE_STATE_TABLE))
#define WAY_PRIORITY_TABLE_ADDR 			(RETRY_LIMIT_TABLE_ADDR + sizeof(RETRY_LIMIT_TABLE))
// for NVMe IO command completion when FUA is 1
#define NVME_IO_CMD_COMPLETE_TABLE_ADDR		(WAY_PRIORITY_TABLE_ADDR + sizeof(WAY_PRIORITY_TABLE))
// for bad block management
#define PHY_BLOCK_MAP_ADDR					(NVME_IO_CMD_COMPLETE_TABLE_ADDR + sizeof(NVME_IO_CMD_COMPLETE_TABLE))
#define BAD_BLOCK_TABLE_INFO_MAP_ADDR		(PHY_BLOCK_MAP_ADDR + sizeof(PHY_BLOCK_MAP))

#define FTL_MANAGEMENT_END_ADDR				(BAD_BLOCK_TABLE_INFO_MAP_ADDR + sizeof(BAD_BLOCK_TABLE_INFO_MAP) - 1)

#define RESERVED0_START_ADDR				(FTL_MANAGEMENT_END_ADDR + 1)
#define RESERVED0_END_ADDR					0x7FFFFFFF

#define DRAM_END_ADDR						0x7FFFFFFF

#define DUMMY_RD_WR_ADDR                    (DRAM_END_ADDR + 1 - 0x4000) // Reserved for NVMe IP.

#endif /* MEMORY_MAP_H_ */
