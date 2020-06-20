#include "mrt311_driver.h"



/***************************************
**���鹦�ܣ�NTC���� R - T ��
****************************************/
const float NTC_R_T_tab[NTC_TEMP_TAB][NTC_TEMP_R_NUM] = {
{	-30	,	1787.9797	}	,
{	-29	,	1679.6017	}	,
{	-28	,	1578.5061	}	,
{	-27	,	1484.1584	}	,
{	-26	,	1396.0662	}	,
{	-25	,	1313.7754	}	,
{	-24	,	1236.8685	}	,
{	-23	,	1164.9598	}	,
{	-22	,	1097.6941	}	,
{	-21	,	1034.7432	}	,
{	-20	,	975.8038	}	,
{	-19	,	920.5962	}	,
{	-18	,	868.8615	}	,
{	-17	,	820.3603	}	,
{	-16	,	774.871	}	,
{	-15	,	732.1889	}	,
{	-14	,	692.1238	}	,
{	-13	,	654.4999	}	,
{	-12	,	619.154	}	,
{	-11	,	585.9346	}	,
{	-10	,	554.7016	}	,
{	-9	,	525.3245	}	,
{	-8	,	497.6821	}	,
{	-7	,	471.6621	}	,
{	-6	,	447.1599	}	,
{	-5	,	424.0781	}	,
{	-4	,	402.3264	}	,
{	-3	,	381.8204	}	,
{	-2	,	362.4818	}	,
{	-1	,	344.2375	}	,
{	0	,	327.0195	}	,
{	1	,	310.764	}	,
{	2	,	295.4121	}	,
{	3	,	280.9084	}	,
{	4	,	267.2014	}	,
{	5	,	254.2428	}	,
{	6	,	241.9877	}	,
{	7	,	230.394	}	,
{	8	,	219.4224	}	,
{	9	,	209.0361	}	,
{	10	,	199.2007	}	,
{	11	,	189.8841	}	,
{	12	,	181.0559	}	,
{	13	,	172.6881	}	,
{	14	,	164.754	}	,
{	15	,	157.229	}	,
{	16	,	150.0898	}	,
{	17	,	143.3144	}	,
{	18	,	136.8825	}	,
{	19	,	130.7749	}	,
{	20	,	124.9734	}	,
{	21	,	119.4612	}	,
{	22	,	114.2223	}	,
{	23	,	109.2417	}	,
{	24	,	104.5053	}	,
{	25	,	100	}	,
{	26	,	95.7132	}	,
{	27	,	91.6333	}	,
{	28	,	87.7492	}	,
{	29	,	84.0505	}	,
{	30	,	80.5274	}	,
{	31	,	77.1707	}	,
{	32	,	73.9717	}	,
{	33	,	70.9222	}	,
{	34	,	68.0144	}	,
{	35	,	65.2411	}	,
{	36	,	62.5954	}	,
{	37	,	60.0707	}	,
{	38	,	57.661	}	,
{	39	,	55.3604	}	,
{	40	,	53.1635	}	,
{	41	,	51.0651	}	,
{	42	,	49.0602	}	,
{	43	,	47.1443	}	,
{	44	,	45.313	}	,
{	45	,	43.5621	}	,
{	46	,	41.8878	}	,
{	47	,	40.2862	}	,
{	48	,	38.7539	}	,
{	49	,	37.2876	}	,
{	50	,	35.8842	}	,
{	51	,	34.5405	}	,
{	52	,	33.2538	}	,
{	53	,	32.0214	}	,
{	54	,	30.8408	}	,
{	55	,	29.7096	}	,
{	56	,	28.6253	}	,
{	57	,	27.586	}	,
{	58	,	26.5895	}	,
{	59	,	25.6338	}	,
{	60	,	24.7171	}	,
{	61	,	23.8376	}	,
{	62	,	22.9937	}	,
{	63	,	22.1836	}	,
{	64	,	21.4061	}	,
{	65	,	20.6594	}	,
{	66	,	19.9424	}	,
{	67	,	19.2537	}	,
{	68	,	18.592	}	,
{	69	,	17.9562	}	,
{	70	,	17.3452	}	,
{	71	,	16.7578	}	,
{	72	,	16.193	}	,
{	73	,	15.6499	}	,
{	74	,	15.1276	}	,
{	75	,	14.6251	}	,
{	76	,	14.1417	}	,
{	77	,	13.6764	}	,
{	78	,	13.2286	}	,
{	79	,	12.7976	}	,
{	80	,	12.3825	}	,
{	81	,	11.9828	}	,
{	82	,	11.5978	}	,
{	83	,	11.227	}	,
{	84	,	10.8697	}	,
{	85	,	10.5254	}	,
{	86	,	10.1935	}	,
{	87	,	9.8736	}	,
{	88	,	9.5652	}	,
{	89	,	9.2678	}	,
{	90	,	8.9809	}	,
{	91	,	8.7042	}	,
{	92	,	8.4373	}	,
{	93	,	8.1797	}	,
{	94	,	7.9312	}	,
{	95	,	7.6912	}	,
{	96	,	7.4596	}	,
{	97	,	7.236	}	,
{	98	,	7.0201	}	,
{	99	,	6.8115	}	,
{	100	,	6.6101	}	,
{	101	,	6.4155	}	,
};


/***************************************
**���鹦�ܣ��������·�Χ��
****************************************/
const float IR_TemperatureRange_tab[IR_TEMP_RANGE_MXA] = {
	35,36,37,38,39,40,41,42,
};


/***************************************
**���鹦�ܣ��ɼ���ѹУ׼��Χ��
****************************************/
//const uint32_t IR_Voltage_Calibration_tab[IR_VC_RANGE_MXA] = {
//	3150,3220,3290,3340,3410,3490,3560,3610,3670,3740, 3810,3860
//};
const uint32_t IR_Voltage_Calibration_tab[IR_VC_RANGE_MXA] = {
	3220,3290,3340,3410,3490,3560,3634,3670,3770, 3830,3860,3930
};

/***************************************
**���鹦�ܣ��������¿̶ȱ�
****************************************/
const float IR_TemperatureScale_tab[IR_TEMP_SCALE_MXA] = {
	35	,
	35.1	,
	35.2	,
	35.3	,
	35.4	,
	35.5	,
	35.6	,
	35.7	,
	35.8	,
	35.9	,
	36	,
	36.1	,
	36.2	,
	36.3	,
	36.4	,
	36.5	,
	36.6	,
	36.7	,
	36.8	,
	36.9	,
	37	,
	37.1	,
	37.2	,
	37.3	,
	37.4	,
	37.5	,
	37.6	,
	37.7	,
	37.8	,
	37.9	,
	38	,
	38.1	,
	38.2	,
	38.3	,
	38.4	,
	38.5	,
	38.6	,
	38.7	,
	38.8	,
	38.9	,
	39	,
	39.1	,
	39.2	,
	39.3	,
	39.4	,
	39.5	,
	39.6	,
	39.7	,
	39.8	,
	39.9	,
	40	,
	40.1	,
	40.2	,
	40.3	,
	40.4	,
	40.5	,
	40.6	,
	40.7	,
	40.8	,
	40.9	,
	41	,
	41.1	,
	41.2	,
	41.3	,
	41.4	,
	41.5	,
	41.6	,
	41.7	,
	41.8	,
	41.9	,
	42	,
};

/***************************************
**���鹦�ܣ��������V-T��
**ÿһ�ж�Ӧһ���������¿̶�
**ÿһ�б�ʾһ�������¶ȿ̶�(NTC�¶�),��0��C��ʼ
****************************************/
const int16_t IR_V_T_tab[IR_TEMP_RANGE_MXA][IR_AMB_TEMP_RANGE_MXA] = {
/*35 (��C)*/	{	2752 	,	2684 	,	2612 	,	2549 	,	2491 	,	2420 	,	2346 	,	2285 	,	2220 	,	2148 	,	2070 	,	2007 	,	1929 	,	1852 	,	1769 	,	1688 	,	1607 	,	1531 	,	1458 	,	1381 	,	1305 	,	1219 	,	1138 	,	1055 	,	974 	,	892 	,	806 	,	719 	,	635 	,	547 	,	447 	,	349 	,	270 	,	184 	,	94 	,	0 	},
/*36 (��C)*/	{	2833 	,	2765 	,	2693 	,	2630 	,	2572 	,	2501 	,	2427 	,	2366 	,	2301 	,	2229 	,	2152 	,	2088 	,	2010 	,	1934 	,	1851 	,	1770 	,	1688 	,	1613 	,	1540 	,	1463 	,	1387 	,	1301 	,	1220 	,	1137 	,	1056 	,	974 	,	888 	,	801 	,	717 	,	629 	,	529 	,	431 	,	352 	,	266 	,	177 	,	82 	},
/*37 (��C)*/	{	2920 	,	2852 	,	2780 	,	2717 	,	2659 	,	2588 	,	2514 	,	2453 	,	2388 	,	2316 	,	2239 	,	2176 	,	2098 	,	2021 	,	1938 	,	1858 	,	1776 	,	1700 	,	1628 	,	1551 	,	1475 	,	1389 	,	1308 	,	1225 	,	1144 	,	1062 	,	977 	,	889 	,	805 	,	718 	,	618 	,	520 	,	441 	,	355 	,	265 	,	171 	},
/*38 (��C)*/	{	3005 	,	2938 	,	2866 	,	2803 	,	2745 	,	2674 	,	2600 	,	2539 	,	2474 	,	2402 	,	2325 	,	2262 	,	2184 	,	2108 	,	2024 	,	1944 	,	1863 	,	1787 	,	1714 	,	1637 	,	1562 	,	1476 	,	1395 	,	1312 	,	1231 	,	1149 	,	1064 	,	976 	,	892 	,	805 	,	705 	,	607 	,	528 	,	442 	,	353 	,	258 	},
/*39 (��C)*/	{	3101 	,	3034 	,	2961 	,	2899 	,	2841 	,	2770 	,	2696 	,	2635 	,	2570 	,	2498 	,	2421 	,	2358 	,	2280 	,	2204 	,	2121 	,	2041 	,	1959 	,	1884 	,	1811 	,	1734 	,	1658 	,	1572 	,	1491 	,	1408 	,	1328 	,	1246 	,	1161 	,	1073 	,	990 	,	902 	,	802 	,	704 	,	625 	,	539 	,	450 	,	356 	},
/*40 (��C)*/	{	3188 	,	3120 	,	3048 	,	2986 	,	2928 	,	2857 	,	2783 	,	2722 	,	2657 	,	2586 	,	2509 	,	2446 	,	2368 	,	2291 	,	2208 	,	2128 	,	2047 	,	1971 	,	1899 	,	1822 	,	1746 	,	1660 	,	1579 	,	1496 	,	1416 	,	1334 	,	1249 	,	1161 	,	1078 	,	990 	,	890 	,	793 	,	714 	,	628 	,	539 	,	444 	},
/*41 (��C)*/	{	3278 	,	3211 	,	3139 	,	3077 	,	3019 	,	2948 	,	2874 	,	2813 	,	2749 	,	2677 	,	2600 	,	2537 	,	2459 	,	2383 	,	2300 	,	2220 	,	2138 	,	2063 	,	1990 	,	1914 	,	1838 	,	1752 	,	1671 	,	1588 	,	1508 	,	1426 	,	1341 	,	1253 	,	1170 	,	1082 	,	983 	,	885 	,	806 	,	720 	,	631 	,	537 	},
/*42 (��C)*/	{	3377 	,	3310 	,	3238 	,	3175 	,	3118 	,	3046 	,	2973 	,	2912 	,	2848 	,	2776 	,	2699 	,	2636 	,	2558 	,	2482 	,	2399 	,	2319 	,	2238 	,	2162 	,	2090 	,	2013 	,	1938 	,	1852 	,	1771 	,	1688 	,	1608 	,	1526 	,	1441 	,	1354 	,	1270 	,	1183 	,	1083 	,	985 	,	907 	,	821 	,	732 	,	638 	},

};









