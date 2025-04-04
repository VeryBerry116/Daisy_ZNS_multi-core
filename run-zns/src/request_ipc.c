//////////////////////////////////////////////////////////////////////////////////
// request_ipc.c for Cosmos+ OpenSSD
// Copyright (c) 2017 Hanyang University ENC Lab.
// Contributed by Yong Ho Song <yhsong@enc.hanyang.ac.kr>
//				  Jaewook Kwak <jwkwak@enc.hanyang.ac.kr>
//			      Sangjin Lee <sjlee@enc.hanyang.ac.kr>
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
// Module Name: Request Inter Process Communication
// File Name: request_ipc.c
//
// Version: v1.0.0
//
// Description:
//	 - transform request information
//   - check dependency between requests
//   - issue host DMA request to host DMA engine
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Revision History:
//
// * v1.0.0
//   - First draft
//////////////////////////////////////////////////////////////////////////////////


#include "xil_printf.h"
#include <assert.h>
#include "nvme/nvme.h"
#include "memory_map.h"

static P_IPC_QUEUE ipcQPtr;
static unsigned char ipcQueueIdx[NUM_NVME_M2][IPC_TYPE_NUM];

void InitIpcQueue()
{
	ipcQPtr = (P_IPC_QUEUE)IPC_CMD_ADDR; //revise address
	memset((void *)ipcQPtr, 0x00, sizeof(IPC_QUEUE));
}

void SubmitNvmeM2Req(unsigned int reqSlotTag)
{
    unsigned int chNo = 0, logicalSliceAddr = reqPoolPtr->reqPool[reqSlotTag].logicalSliceAddr;
    unsigned char reqHead;

    if(reqPoolPtr->reqPool[reqSlotTag].logicalSliceAddr >= (nvme_m2_storage[0] / NVME_BLOCKS_PER_SLICE))
    {
    	chNo = 1;
    	logicalSliceAddr = reqPoolPtr->reqPool[reqSlotTag].logicalSliceAddr - (nvme_m2_storage[0] / NVME_BLOCKS_PER_SLICE);
    }

    reqHead = ipcQueueIdx[chNo][IPC_TYPE_REQ];

    while(ipcQPtr->queue[chNo][IPC_TYPE_REQ][reqHead].queued == 1)
        ;

    ipcQPtr->queue[chNo][IPC_TYPE_REQ][reqHead].reqCode = reqPoolPtr->reqPool[reqSlotTag].reqCode;
    ipcQPtr->queue[chNo][IPC_TYPE_REQ][reqHead].startLba = logicalSliceAddr;
    ipcQPtr->queue[chNo][IPC_TYPE_REQ][reqHead].nlb = reqPoolPtr->reqPool[reqSlotTag].nvmeDmaInfo.numOfNvmeBlock;
    ipcQPtr->queue[chNo][IPC_TYPE_REQ][reqHead].bufAddr = (void *)GenerateDataBufAddr(reqSlotTag);
    ipcQPtr->queue[chNo][IPC_TYPE_REQ][reqHead].queued = 1;

    ipcQueueIdx[chNo][IPC_TYPE_REQ]++;
}

void ProcessNvmeM2Res(unsigned int chNo)
{
    unsigned char resTail = ipcQueueIdx[chNo][IPC_TYPE_RES];

    while(ipcQPtr->queue[chNo][IPC_TYPE_RES][resTail].queued == 1)
    {
        ipcQPtr->queue[chNo][IPC_TYPE_RES][resTail].queued = 0;
        GetFromNvmeM2ReqQ(chNo);        
        resTail = ++ipcQueueIdx[chNo][IPC_TYPE_RES];
    }
}

