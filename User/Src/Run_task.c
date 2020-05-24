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
				OWL_BOARD_DEBUG("MSG_IOKEY_SHORT\r\n");
				break;
			case MSG_IOKEY2_SHORT:
				OWL_BOARD_DEBUG("MSG_ADKEY_1_SHORT\r\n");
				break;
			case MSG_IOKEY3_SHORT:
				OWL_BOARD_DEBUG("MSG_ADKEY_2_SHORT\r\n");
				break;
			case MSG_IOKEY4_SHORT:
				OWL_BOARD_DEBUG("MSG_ADKEY_3_SHORT\r\n");
				break;
			case MSG_100MS:
				break;
			case MSG_200MS:
				OWL_BOARD_DEBUG("MSG_200MS\r\n");
				break;
			case MSG_HALF_SECOND:
				OWL_BOARD_DEBUG("MSG_HALF_SECOND\r\n");
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

