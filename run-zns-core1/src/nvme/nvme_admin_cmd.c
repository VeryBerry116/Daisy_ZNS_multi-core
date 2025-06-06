//////////////////////////////////////////////////////////////////////////////////
// nvme_admin_cmd.c for Cosmos+ OpenSSD
// Copyright (c) 2016 Hanyang University ENC Lab.
// Contributed by Yong Ho Song <yhsong@enc.hanyang.ac.kr>
//				  Youngjin Jo <yjjo@enc.hanyang.ac.kr>
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
// Engineer: Sangjin Lee <sjlee@enc.hanyang.ac.kr>
//
// Project Name: Cosmos+ OpenSSD
// Design Name: Cosmos+ Firmware
// Module Name: NVMe Admin Command Handler
// File Name: nvme_admin_cmd.c
//
// Version: v1.0.0
//
// Description:
//   - handles NVMe admin command
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Revision History:
//
// * v1.0.0
//   - First draft
//////////////////////////////////////////////////////////////////////////////////


#include "xil_printf.h"
#include "debug.h"
#include "string.h"
#include "io_access.h"

#include "nvme.h"
#include "host_lld.h"
#include "nvme_identify.h"
#include "nvme_admin_cmd.h"

extern NVME_CONTEXT g_nvmeTask;

unsigned int set_num_of_queue(unsigned int dword11)
{
	ADMIN_SET_FEATURES_NUMBER_OF_QUEUES_DW11 requested;
	ADMIN_SET_FEATURES_NUMBER_OF_QUEUES_COMPLETE allocated;

	requested.dword = dword11;
	xil_printf("Number of IO Submission Queues Requested (NSQR, zero-based): 0x%04X\r\n", requested.NSQR);
	xil_printf("Number of IO Completion Queues Requested (NCQR, zero-based): 0x%04X\r\n", requested.NCQR);

	//IO submission queue allocating
	// kyj - temp
	/*if(requested.NSQR >= MAX_NUM_OF_IO_SQ)
		g_nvmeTask.numOfIOSubmissionQueuesAllocated = MAX_NUM_OF_IO_SQ;
	else
		g_nvmeTask.numOfIOSubmissionQueuesAllocated = requested.NSQR + 1;//zero-based -> non zero-based

	allocated.NSQA = g_nvmeTask.numOfIOSubmissionQueuesAllocated - 1;//non zero-based -> zero-based


	//IO completion queue allocating
	if(requested.NCQR >= MAX_NUM_OF_IO_CQ)
		g_nvmeTask.numOfIOCompletionQueuesAllocated = MAX_NUM_OF_IO_CQ;
	else
		g_nvmeTask.numOfIOCompletionQueuesAllocated = requested.NCQR + 1;//zero-based -> non zero-based

	allocated.NCQA = g_nvmeTask.numOfIOCompletionQueuesAllocated - 1;//non zero-based -> zero-based*/

	xil_printf("Number of IO Submission Queues Allocated (NSQA, zero-based): 0x%04X\r\n", allocated.NSQA);
	xil_printf("Number of IO Completion Queues Allocated (NCQA, zero-based): 0x%04X\r\n", allocated.NCQA);

	return allocated.dword;
}

unsigned int get_num_of_queue(unsigned int dword10)
{
	ADMIN_GET_FEATURES_NUMBER_OF_QUEUES_COMPLETE allocated;

	// kyj - temp
	/*allocated.NCQA = g_nvmeTask.numOfIOCompletionQueuesAllocated - 1;//non zero-based -> zero-based
	allocated.NSQA = g_nvmeTask.numOfIOSubmissionQueuesAllocated - 1;//non zero-based -> zero-based*/

	return allocated.dword;
}

void handle_set_features(NVME_ADMIN_COMMAND *nvmeAdminCmd, NVME_COMPLETION *nvmeCPL)
{
	ADMIN_SET_FEATURES_DW10 features;

	features.dword = nvmeAdminCmd->dword10;

	switch(features.FID)
	{
		case NUMBER_OF_QUEUES:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = set_num_of_queue(nvmeAdminCmd->dword11);
			break;
		}
		case INTERRUPT_COALESCING:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = 0x0;
			break;
		}
		case ARBITRATION:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = 0x0;
			break;
		}
		case ASYNCHRONOUS_EVENT_CONFIGURATION:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = 0x0;
			break;
		}
		case VOLATILE_WRITE_CACHE:
		{
			xil_printf("Set VWC: 0x%X\r\n", nvmeAdminCmd->dword11);
			// kyj - temp
			//g_nvmeTask.cacheEn = (nvmeAdminCmd->dword11 & 0x1);
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = 0x0;
			break;
		}
		case POWER_MANAGEMENT:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = 0x0;
			break;
		}
		case TIMESTAMP:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = 0x0;
			break;
		}
		case 0x80:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = 0x0;
			break;
		}
		default:
		{
			xil_printf("Not Support FID (Set): 0x%X\r\n", features.FID);
			ASSERT(0);
			break;
		}
	}
	if(__ADMIN_CMD_DONE_MESSAGE_PRINT)
    	xil_printf("Set Feature FID:0x%X\r\n", features.FID);
}

void handle_get_features(NVME_ADMIN_COMMAND *nvmeAdminCmd, NVME_COMPLETION *nvmeCPL)
{
	ADMIN_GET_FEATURES_DW10 features;
	NVME_COMPLETION cpl;

	features.dword = nvmeAdminCmd->dword10;

	switch(features.FID)
	{
		case NUMBER_OF_QUEUES:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = get_num_of_queue(nvmeAdminCmd->dword10);
			break;
		}
		case LBA_RANGE_TYPE:
		{
			//ASSERT(nvmeAdminCmd->NSID == 1);

			cpl.dword[0] = 0x0;
			cpl.statusField.SC = SC_INVALID_FIELD_IN_COMMAND;
			nvmeCPL->dword[0] = cpl.dword[0];
			nvmeCPL->specific = 0x0;
			break;
		}
		case TEMPERATURE_THRESHOLD:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = nvmeAdminCmd->dword11;
			break;
		}
		case VOLATILE_WRITE_CACHE:
		{
			
			//xil_printf("Get VWC: 0x%X\r\n", g_nvmeTask.cacheEn);
			nvmeCPL->dword[0] = 0x0;
			// kyj - temp
				//nvmeCPL->specific = g_nvmeTask.cacheEn;
			break;
		}
		case POWER_MANAGEMENT:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = 0x0;
			break;
		}
		case POWER_STATE_TRANSITION:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = 0x0;
			break;
		}
		case 0xD0:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = 0x0;
			break;
		}
		case 0x80:
		{
			nvmeCPL->dword[0] = 0x0;
			nvmeCPL->specific = 0x0;
			break;
		}
		default:
		{
			xil_printf("Not Support FID (Get): 0x%X\r\n", features.FID);
			ASSERT(0);
			break;
		}
	}
	if(__ADMIN_CMD_DONE_MESSAGE_PRINT)
    	xil_printf("Get Feature FID: 0x%X\r\n", features.FID);
}

void handle_create_io_sq(NVME_ADMIN_COMMAND *nvmeAdminCmd, NVME_COMPLETION *nvmeCPL)
{
	ADMIN_CREATE_IO_SQ_DW10 sqInfo10;
	ADMIN_CREATE_IO_SQ_DW11 sqInfo11;
	NVME_IO_SQ_STATUS *ioSqStatus;
	unsigned int ioSqIdx;

	sqInfo10.dword = nvmeAdminCmd->dword10;
	sqInfo11.dword = nvmeAdminCmd->dword11;

	xil_printf("Create IO SQ, DW11: 0x%08X, DW10: 0x%08X\r\n", sqInfo11.dword, sqInfo10.dword);

	ASSERT((nvmeAdminCmd->PRP1[0] & 0x3) == 0 && nvmeAdminCmd->PRP1[1] < 0x10000);
	ASSERT(0 < sqInfo10.QID && sqInfo10.QID <= 8 && sqInfo10.QSIZE < 0x100 && 0 < sqInfo11.CQID && sqInfo11.CQID <= 8);

	ioSqIdx = sqInfo10.QID - 1;
	// kyj - temp
	//ioSqStatus = g_nvmeTask.ioSqInfo + ioSqIdx;

	ioSqStatus->valid = 1;
	ioSqStatus->qSzie = sqInfo10.QSIZE;
	ioSqStatus->cqVector = sqInfo11.CQID;
	ioSqStatus->pcieBaseAddrL = nvmeAdminCmd->PRP1[0];
	ioSqStatus->pcieBaseAddrH = nvmeAdminCmd->PRP1[1];

	set_io_sq(ioSqIdx, ioSqStatus->valid, ioSqStatus->cqVector, ioSqStatus->qSzie, ioSqStatus->pcieBaseAddrL, ioSqStatus->pcieBaseAddrH);

	nvmeCPL->dword[0] = 0;
	nvmeCPL->specific = 0x0;

}

void handle_delete_io_sq(NVME_ADMIN_COMMAND *nvmeAdminCmd, NVME_COMPLETION *nvmeCPL)
{
	ADMIN_DELETE_IO_SQ_DW10 sqInfo10;
	NVME_IO_SQ_STATUS *ioSqStatus;
	unsigned int ioSqIdx;

	sqInfo10.dword = nvmeAdminCmd->dword10;

	xil_printf("Delete IO SQ, DW10: 0x%08X\r\n", sqInfo10.dword);

	ioSqIdx = (unsigned int)sqInfo10.QID - 1;
	// kyj - temp
		//ioSqStatus = g_nvmeTask.ioSqInfo + ioSqIdx;

	ioSqStatus->valid = 0;
	ioSqStatus->cqVector = 0;
	ioSqStatus->qSzie = 0;
	ioSqStatus->pcieBaseAddrL = 0;
	ioSqStatus->pcieBaseAddrH = 0;

	set_io_sq(ioSqIdx, 0, 0, 0, 0, 0);

	nvmeCPL->dword[0] = 0;
	nvmeCPL->specific = 0x0;
}


void handle_create_io_cq(NVME_ADMIN_COMMAND *nvmeAdminCmd, NVME_COMPLETION *nvmeCPL)
{
	ADMIN_CREATE_IO_CQ_DW10 cqInfo10;
	ADMIN_CREATE_IO_CQ_DW11 cqInfo11;
	NVME_IO_CQ_STATUS *ioCqStatus;
	unsigned int ioCqIdx;

	cqInfo10.dword = nvmeAdminCmd->dword10;
	cqInfo11.dword = nvmeAdminCmd->dword11;

	xil_printf("Create IO CQ, DW11: 0x%08X, DW10: 0x%08X\r\n", cqInfo11.dword, cqInfo10.dword);

	ASSERT(((nvmeAdminCmd->PRP1[0] & 0x3) == 0) && (nvmeAdminCmd->PRP1[1] < 0x10000));
	ASSERT(cqInfo11.IV < 8 && cqInfo10.QSIZE < 0x100 && 0 < cqInfo10.QID && cqInfo10.QID <= 8);

	ioCqIdx = cqInfo10.QID - 1;
	// kyj - temp
		//ioCqStatus = g_nvmeTask.ioCqInfo + ioCqIdx;

	ioCqStatus->valid = 1;
	ioCqStatus->qSzie = cqInfo10.QSIZE;
	ioCqStatus->irqEn = cqInfo11.IEN;
	ioCqStatus->irqVector = cqInfo11.IV;
	ioCqStatus->pcieBaseAddrL = nvmeAdminCmd->PRP1[0];
	ioCqStatus->pcieBaseAddrH = nvmeAdminCmd->PRP1[1];

	set_io_cq(ioCqIdx, ioCqStatus->valid, ioCqStatus->irqEn, ioCqStatus->irqVector, ioCqStatus->qSzie, ioCqStatus->pcieBaseAddrL, ioCqStatus->pcieBaseAddrH);

	nvmeCPL->dword[0] = 0;
	nvmeCPL->specific = 0x0;
}

void handle_delete_io_cq(NVME_ADMIN_COMMAND *nvmeAdminCmd, NVME_COMPLETION *nvmeCPL)
{
	ADMIN_DELETE_IO_CQ_DW10 cqInfo10;
	NVME_IO_CQ_STATUS *ioCqStatus;
	unsigned int ioCqIdx;

	cqInfo10.dword = nvmeAdminCmd->dword10;

	xil_printf("Delete IO CQ, DW10: 0x%08X\r\n", cqInfo10.dword);

	ioCqIdx = (unsigned int)cqInfo10.QID - 1;
	// kyj - temp
		//ioCqStatus = g_nvmeTask.ioCqInfo + ioCqIdx;

	ioCqStatus->valid = 0;
	ioCqStatus->irqVector = 0;
	ioCqStatus->qSzie = 0;
	ioCqStatus->pcieBaseAddrL = 0;
	ioCqStatus->pcieBaseAddrH = 0;
	
	set_io_cq(ioCqIdx, 0, 0, 0, 0, 0, 0);

	nvmeCPL->dword[0] = 0;
	nvmeCPL->specific = 0x0;
}

void handle_identify(NVME_ADMIN_COMMAND *nvmeAdminCmd, NVME_COMPLETION *nvmeCPL)
{
	ADMIN_IDENTIFY_COMMAND_DW10 identifyInfo;
	ADMIN_IDENTIFY_COMMAND_DW11 identifyInfo2;
	unsigned int pIdentifyData = ADMIN_CMD_DRAM_DATA_BUFFER;
	unsigned int prp[2];
	unsigned int prpLen;

	identifyInfo.dword = nvmeAdminCmd->dword10;
	identifyInfo2.dword = nvmeAdminCmd->dword11;

	if(identifyInfo.CNS == NVME_ID_CNS_CTRL)//CI: Controller Identify
	{
		if((nvmeAdminCmd->PRP1[0] & 0x3) != 0 || (nvmeAdminCmd->PRP2[0] & 0x3) != 0)
			xil_printf("CI: PRP1 = 0x%08X_%08X, PRP2 = %08X_%08X\r\n", nvmeAdminCmd->PRP1[1], nvmeAdminCmd->PRP1[0], nvmeAdminCmd->PRP2[1], nvmeAdminCmd->PRP2[0]);

		ASSERT((nvmeAdminCmd->PRP1[0] & 0x3) == 0 && (nvmeAdminCmd->PRP2[0] & 0x3) == 0);
		controller_identification(pIdentifyData);
	}
	else if(identifyInfo.CNS == NVME_ID_CNS_NS)//NI: Namespace Identify
	{
		if((nvmeAdminCmd->PRP1[0] & 0x3) != 0 || (nvmeAdminCmd->PRP2[0] & 0x3) != 0)
			xil_printf("NI: 0xPRP1 = %08X_%08X, PRP2 = %08X_%08X\r\n", nvmeAdminCmd->PRP1[1], nvmeAdminCmd->PRP1[0], nvmeAdminCmd->PRP2[1], nvmeAdminCmd->PRP2[0]);

		//ASSERT(nvmeAdminCmd->NSID == 1);
		ASSERT((nvmeAdminCmd->PRP1[0] & 0x3) == 0 && (nvmeAdminCmd->PRP2[0] & 0x3) == 0);
		namespace_identification(pIdentifyData);
	}
	else if(identifyInfo.CNS == NVME_ID_CNS_NS_ACTIVE_LIST)
	{
		active_namespace_identification(pIdentifyData);
	}
	else if(identifyInfo.CNS == NVME_ID_CNS_CS_NS && identifyInfo2.CSI == NVME_CSI_ZONED) //ZNS Namespace Identify
	{
		if((nvmeAdminCmd->PRP1[0] & 0x3) != 0 || (nvmeAdminCmd->PRP2[0] & 0x3) != 0)
			xil_printf("CI: PRP1 = 0x%08X_%08X, PRP2 = %08X_%08X\r\n", nvmeAdminCmd->PRP1[1], nvmeAdminCmd->PRP1[0], nvmeAdminCmd->PRP2[1], nvmeAdminCmd->PRP2[0]);

		ASSERT((nvmeAdminCmd->PRP1[0] & 0x3) == 0 && (nvmeAdminCmd->PRP2[0] & 0x3) == 0);
		zns_namespace_identification(pIdentifyData);
	}
	else if(identifyInfo.CNS == NVME_ID_CNS_CS_CTRL && identifyInfo2.CSI == NVME_CSI_NVM)
	{
		if((nvmeAdminCmd->PRP1[0] & 0x3) != 0 || (nvmeAdminCmd->PRP2[0] & 0x3) != 0)
			xil_printf("CI: PRP1 = 0x%08X_%08X, PRP2 = %08X_%08X\r\n", nvmeAdminCmd->PRP1[1], nvmeAdminCmd->PRP1[0], nvmeAdminCmd->PRP2[1], nvmeAdminCmd->PRP2[0]);

		ASSERT((nvmeAdminCmd->PRP1[0] & 0x3) == 0 && (nvmeAdminCmd->PRP2[0] & 0x3) == 0);

        memset((void *)pIdentifyData, 0, sizeof(ADMIN_IDENTIFY_CONTROLLER));
	}
	else if(identifyInfo.CNS == NVME_ID_CNS_CS_CTRL && identifyInfo2.CSI == NVME_CSI_ZONED) //ZNS Controller Identify
	{
		if((nvmeAdminCmd->PRP1[0] & 0x3) != 0 || (nvmeAdminCmd->PRP2[0] & 0x3) != 0)
			xil_printf("CI: PRP1 = 0x%08X_%08X, PRP2 = %08X_%08X\r\n", nvmeAdminCmd->PRP1[1], nvmeAdminCmd->PRP1[0], nvmeAdminCmd->PRP2[1], nvmeAdminCmd->PRP2[0]);

		ASSERT((nvmeAdminCmd->PRP1[0] & 0x3) == 0 && (nvmeAdminCmd->PRP2[0] & 0x3) == 0);
		zns_controller_identification(pIdentifyData);
	}
	else if(identifyInfo.CNS == NVME_ID_CNS_IO_COMMAND_SET)//IO COMMAND SET
	{
		if((nvmeAdminCmd->PRP1[0] & 0x3) != 0 || (nvmeAdminCmd->PRP2[0] & 0x3) != 0)
			xil_printf("CI: PRP1 = 0x%08X_%08X, PRP2 = %08X_%08X\r\n", nvmeAdminCmd->PRP1[1], nvmeAdminCmd->PRP1[0], nvmeAdminCmd->PRP2[1], nvmeAdminCmd->PRP2[0]);

		ASSERT((nvmeAdminCmd->PRP1[0] & 0x3) == 0 && (nvmeAdminCmd->PRP2[0] & 0x3) == 0);
		nvme_identify_cmd_set(pIdentifyData);
	}
	else
		ASSERT(0);
	
	prp[0] = nvmeAdminCmd->PRP1[0];
	prp[1] = nvmeAdminCmd->PRP1[1];

	prpLen = 0x1000 - (prp[0] & 0xFFF);
//	xil_printf("prpLen = %X, prp[1] = %X, prp[0] = %X\r\n",prpLen, prp[1], prp[0]);
	set_direct_tx_dma(pIdentifyData, prp[1], prp[0], prpLen);
	if(prpLen != 0x1000)
	{
		pIdentifyData = pIdentifyData + prpLen;
		prpLen = 0x1000 - prpLen;
		prp[0] = nvmeAdminCmd->PRP2[0];
		prp[1] = nvmeAdminCmd->PRP2[1];

//		ASSERT((prp[1] & 0xFFF) == 0);
//		xil_printf("prpLen = %X, prp[1] = %X, prp[0] = %X\r\n",prpLen, prp[1], prp[0]);
		set_direct_tx_dma(pIdentifyData, prp[1], prp[0], prpLen);
	}

	check_direct_tx_dma_done();
	nvmeCPL->dword[0] = 0;
	nvmeCPL->specific = 0x0;
}

void handle_get_log_page(NVME_ADMIN_COMMAND *nvmeAdminCmd, NVME_COMPLETION *nvmeCPL)
{
	/*ADMIN_GET_LOG_PAGE_DW10 getLogPageInfo;

	unsigned int prp1[2];
	unsigned int prp2[2];
	unsigned int prpLen;

	getLogPageInfo.dword = nvmeAdminCmd->dword10;

	prp1[0] = nvmeAdminCmd->PRP1[0];
	prp1[1] = nvmeAdminCmd->PRP1[1];
	prpLen = 0x1000 - (prp1[0] & 0xFFF);

	prp2[0] = nvmeAdminCmd->PRP2[0];
	prp2[1] = nvmeAdminCmd->PRP2[1];

	xil_printf("ADMIN GET LOG PAGE\n");

	//LID
	//Mandatory//1-Error information, 2-SMART/Health information, 3-Firmware Slot information
	//Optional//4-ChangedNamespaceList, 5-Command Effects Log
	xil_printf("LID: 0x%X, NUMD: 0x%X \r\n", getLogPageInfo.LID, getLogPageInfo.NUMD);

	xil_printf("PRP1[63:32] = 0x%X, PRP1[31:0] = 0x%X", prp1[1], prp1[0]);
	xil_printf("PRP2[63:32] = 0x%X, PRP2[31:0] = 0x%X", prp2[1], prp2[0]);*/

	nvmeCPL->dword[0] = 0;
    nvmeCPL->statusField.SCT = 1;
	nvmeCPL->specific = 0x9;//invalid log page
}

void handle_nvme_admin_cmd(NVME_COMMAND *nvmeCmd)
{
	NVME_ADMIN_COMMAND *nvmeAdminCmd;
	NVME_COMPLETION nvmeCPL;
	unsigned int opc;
	unsigned int needCpl;
	unsigned int needSlotRelease;

	nvmeAdminCmd = (NVME_ADMIN_COMMAND*)nvmeCmd->cmdDword;
	opc = (unsigned int)nvmeAdminCmd->OPC;

	needCpl = 1;
	needSlotRelease = 0;
	switch(opc)
	{
		case ADMIN_SET_FEATURES:
		{
			handle_set_features(nvmeAdminCmd, &nvmeCPL);
			break;
		}
		case ADMIN_CREATE_IO_CQ:
		{
			handle_create_io_cq(nvmeAdminCmd, &nvmeCPL);
			break;
		}
		case ADMIN_CREATE_IO_SQ:
		{
			handle_create_io_sq(nvmeAdminCmd, &nvmeCPL);
			break;
		}
		case ADMIN_IDENTIFY:
		{
			PRINT("ADMIN_IDENTIFY\r\n");
			handle_identify(nvmeAdminCmd, &nvmeCPL);
			break;
		}
		case ADMIN_GET_FEATURES:
		{
			handle_get_features(nvmeAdminCmd, &nvmeCPL);
			break;
		}
		case ADMIN_DELETE_IO_CQ:
		{
			handle_delete_io_cq(nvmeAdminCmd, &nvmeCPL);
			break;
		}
		case ADMIN_DELETE_IO_SQ:
		{
			handle_delete_io_sq(nvmeAdminCmd, &nvmeCPL);
			break;
		}
		case ADMIN_ASYNCHRONOUS_EVENT_REQUEST:
		{
			needCpl = 0;
			needSlotRelease = 1;
			nvmeCPL.dword[0] = 0;
			nvmeCPL.specific = 0x0;
			break;
		}
		case ADMIN_GET_LOG_PAGE:
		{
			handle_get_log_page(nvmeAdminCmd, &nvmeCPL);
			break;
		}
		case ADMIN_SECURITY_RECEIVE:
		{
			needCpl = 0;
			needSlotRelease = 0;
			nvmeCPL.dword[0] = 0;
			nvmeCPL.specific = 0x0;
			break;
		}
		case ADMIN_DOORBELL_BUFFER_CONFIG:
		{
			needCpl = 0;
			needSlotRelease = 0;
			nvmeCPL.dword[0] = 0;
			nvmeCPL.specific = 0x0;
			break;
		}
		case ADMIN_ABORT:
		{
			nvmeCPL.dword[0] = 0;
			nvmeCPL.specific = 0x0;
			break;
		}
		default:
		{
			xil_printf("Not Support Admin Command OPC: 0x%X\r\n", opc);
			nvmeCPL.statusFieldWord = 0;
			nvmeCPL.specific = 0x0;
			nvmeCPL.statusField.DNR = 1;
			nvmeCPL.statusField.SCT = 0;
			nvmeCPL.statusField.SC = 1;
			break;
		}
	}

	if(needCpl == 1)
		set_auto_nvme_cpl(nvmeCmd->cmdSlotTag, nvmeCPL.specific, nvmeCPL.statusFieldWord);
	else if(needSlotRelease == 1)
		set_nvme_slot_release(nvmeCmd->cmdSlotTag);
	else

	set_nvme_cpl(nvmeCmd->qID, nvmeAdminCmd->CID, nvmeCPL.specific, nvmeCPL.statusFieldWord);

	if(__ADMIN_CMD_DONE_MESSAGE_PRINT)
		xil_printf("Admin Command Done, OPC: 0x%02X\r\n", opc);
}

