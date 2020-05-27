#include "Run_task.h"
#include "Data_Structure.h"
#include "include.h"


static void Run_Task(void)
{
	uint8_t msg = NO_MSG;
	
	OWL_BOARD_DEBUG("Run_Task\r\n");
	
	while(1)
	{
		msg = get_msg_Fifo();
		
		switch(msg)
		{
			case MSG_IOKEY1_SHORT:
				OWL_BOARD_DEBUG("MSG_IOKEY1_SHORT\r\n");
				break;
			case MSG_IOKEY2_SHORT:
				OWL_BOARD_DEBUG("MSG_IOKEY2_SHORT\r\n");
				break;
			case MSG_IOKEY3_SHORT:
				OWL_BOARD_DEBUG("MSG_IOKEY3_SHORT\r\n");
				break;
			case MSG_IOKEY4_SHORT:
				OWL_BOARD_DEBUG("MSG_IOKEY4_SHORT\r\n");
				break;
			case MSG_IOKEY1_HOLD:
				OWL_BOARD_DEBUG("MSG_IOKEY1_HOLD\r\n");
				break;
			case MSG_IOKEY2_HOLD:
				OWL_BOARD_DEBUG("MSG_IOKEY2_HOLD\r\n");
				break;
			case MSG_IOKEY3_HOLD:
				OWL_BOARD_DEBUG("MSG_IOKEY3_HOLD\r\n");
				break;
			case MSG_IOKEY4_HOLD:
				OWL_BOARD_DEBUG("MSG_IOKEY4_HOLD\r\n");
				break;
			case MSG_IOKEY1_LONG:
				OWL_BOARD_DEBUG("MSG_IOKEY1_LONG\r\n");
				break;
			case MSG_IOKEY2_LONG:
				OWL_BOARD_DEBUG("MSG_IOKEY2_LONG\r\n");
				break;
			case MSG_IOKEY3_LONG:
				OWL_BOARD_DEBUG("MSG_IOKEY3_LONG\r\n");
				break;
			case MSG_IOKEY4_LONG:
				OWL_BOARD_DEBUG("MSG_IOKEY4_LONG\r\n");
				break;
			case MSG_IOKEY1_LONG_UP:
				OWL_BOARD_DEBUG("MSG_IOKEY1_LONG_UP\r\n");
				break;
			case MSG_IOKEY2_LONG_UP:
				OWL_BOARD_DEBUG("MSG_IOKEY2_LONG_UP\r\n");
				break;
			case MSG_IOKEY3_LONG_UP:
				OWL_BOARD_DEBUG("MSG_IOKEY3_LONG_UP\r\n");
				break;
			case MSG_IOKEY4_LONG_UP:
				OWL_BOARD_DEBUG("MSG_IOKEY4_LONG_UP\r\n");
				break;
			case MSG_100MS:
				break;
			case MSG_200MS:
//				OWL_BOARD_DEBUG("MSG_200MS\r\n");
				break;
			case MSG_HALF_SECOND:
//				OWL_BOARD_DEBUG("MSG_HALF_SECOND\r\n");
				break;
			case MSG_800MS:
				break;
			default:
				break;
		}
	}
}


void Run_Start(void)
{
	printf("Run_Start\r\n");
	
	SeqQueue_Init();
	Clear_msg_Fifo();
	
	Run_Task();
}

