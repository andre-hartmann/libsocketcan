#include "libsocketcan.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printInfo(const char *name)
{
	printf("====================\n");

	__u32 ms = 0;
	can_get_restart_ms(name, &ms);
	printf("can_get_restart_ms(name = %s, ms = %d)\n", name, (int)ms);

	struct can_bittiming bt = {0, 0, 0, 0, 0, 0, 0, 0};
	can_get_bittiming(name, &bt);
	printf("can_get_bittiming(name = %s, bt.bitrate = %d, "
		   "bt.sample_point = %d, bt.tq = %d, bt.prop_seg = %d, "
		   "bt.phase_seg1 = %d, bt.phase_seg2 = %d, "
		   "bt.sjw = %d, bt.brp = %d)\n",
		   name, (int)bt.bitrate,
		   (int)bt.sample_point, (int)bt.tq, (int)bt.prop_seg,
		   (int)bt.phase_seg1, (int)bt.phase_seg2,
		   (int)bt.sjw, (int)bt.brp);

	struct can_ctrlmode cm = {0, 0};
	can_get_ctrlmode(name, &cm);
	printf("can_get_ctrlmode(name = %s, cm.flags = %d, cm.mask = %d)\n",
		   name, (int)cm.flags, (int)cm.mask);

	int state = 0;
	can_get_state(name, &state);
	printf("can_get_state(name = %s, state = %d)\n", name, state);

	struct can_clock clock = {0};
	can_get_clock(name, &clock);
	printf("can_get_clock(name = %s, clock.freq = %d\n", name, (int)clock.freq);

	struct can_bittiming_const btc = {{0}, 0, 0, 0, 0, 0, 0, 0, 0};
	can_get_bittiming_const(name, &btc);
	printf("can_get_bittiming_const(name = %s, "
		   "btc.brp_min = %d, btc.brp_max = %d, btc.brp_inc = %d, "
		   "btc.tseg1_min = %d, btc.tseg1_max = %d, "
		   "btc.tseg2_min = %d, btc.tseg2_max = %d, "
		   "btc.sjw_max = %d)\n",
		   btc.name, (int)btc.brp_min, (int)btc.brp_max, (int)btc.brp_inc,
		   (int)btc.tseg1_min, (int)btc.tseg1_max, (int)btc.tseg2_min, (int)btc.tseg2_max,
		   (int)btc.sjw_max);

	struct can_berr_counter bc = {0, 0};
	can_get_berr_counter(name, &bc);
	printf("can_get_berr_counter(name = %s, bc.rxerr = %d, bc.txerr = %d)\n",
		   name, (int)bc.rxerr, (int)bc.txerr);

	memset(&bt, 0, sizeof(bt));
	can_get_data_bittiming(name, &bt);
	printf("can_get_data_bittiming(name = %s, bt.bitrate = %d, "
		   "bt.sample_point = %d, bt.tq = %d, bt.prop_seg = %d, "
		   "bt.phase_seg1 = %d, bt.phase_seg2 = %d, "
		   "bt.sjw = %d, bt.brp = %d)\n",
		   name, (int)bt.bitrate,
		   (int)bt.sample_point, (int)bt.tq, (int)bt.prop_seg,
		   (int)bt.phase_seg1, (int)bt.phase_seg2,
		   (int)bt.sjw, (int)bt.brp);

	memset(&btc, 0, sizeof(btc));
	can_get_data_bittiming_const(name, &btc);
	printf("can_get_data_bittiming_const(name = %s, "
		   "btc.brp_min = %d, btc.brp_max = %d, btc.brp_inc = %d, "
		   "btc.tseg1_min = %d, btc.tseg1_max = %d, "
		   "btc.tseg2_min = %d, btc.tseg2_max = %d, "
		   "btc.sjw_max = %d)\n",
		   btc.name, (int)btc.brp_min, (int)btc.brp_max, (int)btc.brp_inc,
		   (int)btc.tseg1_min, (int)btc.tseg1_max, (int)btc.tseg2_min, (int)btc.tseg2_max,
		   (int)btc.sjw_max);
}

void setBitrate(const char *name, int bitrate, int dataBitrate)
{
	printf("setBitrate(name = %s, bitrate = %d, dataBitrate = %d\n", name, bitrate, dataBitrate);
	
	int result = can_do_stop(name);
	printf("can_do_stop(name = %s) = %d\n", name, result);
	
	result = can_set_bitrate(name, (__u32)bitrate);
	printf("can_set_bitrate(name = %s, bitrate = %d) = %d\n", name, bitrate, result);
	
	result = can_set_data_bitrate(name, (__u32)dataBitrate);
	printf("can_set_data_bitrate(name = %s, dataBitrate = %d) = %d\n", name, dataBitrate, result);

	result = can_do_start(name);
	printf("can_do_start(name = %s) = %d\n", name, result);
}

static void verifySetBitrate(const char *name, __u32 bitrate)
{
	assert(can_set_bitrate(name, bitrate) == 0);
	struct can_bittiming bt;
	assert(can_get_bittiming(name, &bt) == 0);
	assert(bt.bitrate == bitrate);
}

static void verifySetDataBitrate(const char *name, __u32 dbitrate)
{
	assert(can_set_data_bitrate(name, dbitrate) == 0);
	struct can_bittiming bt;
	assert(can_get_data_bittiming(name, &bt) == 0);
	assert(bt.bitrate == dbitrate);
}

static void verifySetCanFdEnabled(const char *name, int enabled)
{
	struct can_ctrlmode cm = {
		.mask = CAN_CTRLMODE_FD,
		.flags = enabled ? CAN_CTRLMODE_FD : 0
	};
	assert(can_set_ctrlmode(name, &cm) == 0);
}

static void verifyIsCanFdEnabled(const char *name, int enabled)
{
	struct can_ctrlmode cm;
	assert(can_get_ctrlmode(name, &cm) == 0);
	int fd = (cm.flags & CAN_CTRLMODE_FD) ? 1 : 0;
	assert(fd == enabled);
}

static void verifyDataBitrate(const char *name, __u32 dbitrate)
{
	struct can_bittiming bt;
	assert(can_get_data_bittiming(name, &bt) == 0);
	assert(bt.bitrate == dbitrate);
}

int test(const char *name)
{
	assert(can_do_stop(name) == 0);

	verifySetCanFdEnabled(name, 0);

	verifySetBitrate(name, 125000);
	verifyIsCanFdEnabled(name, 0);

	verifySetDataBitrate(name, 500000);
	verifyIsCanFdEnabled(name, 1);

	verifySetBitrate(name, 250000);
	verifyIsCanFdEnabled(name, 1);
	verifyDataBitrate(name, 500000);

	assert(can_do_start(name) == 0);

	return 0;
}

int main(int argc, char *argv[])
{
	int bitrate = 250000;
	int dataBitrate = 2000000;

	if (argc == 2 && strcmp(argv[1], "--test") == 0) {
		return test("can0");
	}
	
	if (argc >= 2)
		bitrate = atoi(argv[1]);
	if (argc == 3)
		dataBitrate = atoi(argv[2]);
	
	setBitrate("can0", bitrate, dataBitrate);
	printInfo("can0");
	return 0;
}
