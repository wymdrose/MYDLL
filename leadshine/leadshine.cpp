#include "../../lib/leadshine.h"

#include "../../lib/Leadshine/Dmc1380.h"
#pragma comment(lib, "Leadshine/Dmc1380.lib")

using namespace MotionCtrl;

leadshine::leadshine()
{

}

leadshine::~leadshine()
{

}

Dmc1380::Dmc1380()
{

}

Dmc1380::~Dmc1380()
{

}

short Dmc1380::Init()
{
	short num = d1000_board_init();

	if (num != 1)
	{
		qDebug() << "Init() Failed.";
	}
	else
	{

		for (int i = 0; i < axesPerCard; i++)
		{
			pulPerMM[i] = 1.0;
			accTime[i] = 0.2;
			decTime[i] = 0.2;
			minSpd[i] = 500;
			maxSpd[i] = 500;
		}
	}

	return 0;
}

void Dmc1380::Close()
{
	d1000_board_close();
}

uint Dmc1380::AbsMove(ushort axis, double pos, double speed)
{
	d1000_start_t_move(axis, (int)(pos * pulPerMM[axis]), minSpd[axis], speed, accTime[axis]);
	return 1;
}

uint Dmc1380::RelMove(ushort axis, double dist, double speed)
{
	d1000_start_ta_move(axis, (int)(dist * pulPerMM[axis]), minSpd[axis], speed, accTime[axis]);
	return 1;
}

uint Dmc1380::HomeMove(ushort axis, double speed)
{
	d1000_home_move(axis, minSpd[axis], speed, accTime[axis]);
	return 1;
}

uint Dmc1380::DecStop(ushort axis, double dec)
{
	d1000_decel_stop(axis);

	return 1;
}

double Dmc1380::GetPos(ushort axis)
{
	return (double)d1000_get_command_pos(axis) / pulPerMM[axis];
}

uint Dmc1380::SetPos(ushort axis, double pos)
{
	return (uint)(d1000_set_command_pos(axis, (int)(pos * pulPerMM[axis])));
}

ushort Dmc1380::CheckDone(ushort axis)
{
	return (ushort)d1000_check_done(axis);
}

ushort Dmc1380::AxisOnOff(ushort axis, ushort onoff)
{
	//	return (ushort)LTDMC.dmc_write_sevon_pin(cardID, axis, onoff);
	return 0;
}

double Dmc1380::GetEncPos(ushort axis)
{
	//	return (double)LTDMC.dmc_get_encoder(cardID, axis) / encPerMM[axis];
	return 0;
}

uint Dmc1380::SetEncPos(ushort axis, double pos)
{
	//	return (uint)(LTDMC.dmc_set_encoder(cardID, axis, (int)(pos * encPerMM[axis])));
	return 0;
}

uint Dmc1380::GetAxisStatus(ushort axis)
{
	uint dwStatus = d1000_get_axis_status(axis);
	uint ELP = ((dwStatus & 0x00000002) != 0) ? (uint)1 : 0;
	uint ELN = ((dwStatus & 0x00000004) != 0) ? (uint)1 : 0;
	uint ORG = ((dwStatus & 0x00000010) != 0) ? (uint)1 : 0;
	uint ALM = ((dwStatus & 0x00000001) != 0) ? (uint)1 : 0;
	//	uint SEVON = (uint)LTDMC.dmc_read_sevon_pin(cardID, axis);
	uint nStatus = ALM;
	nStatus = (nStatus << 1) + ORG;
	nStatus = (nStatus << 1) + ELN;
	nStatus = (nStatus << 1) + ELP;
	//	nStatus = (nStatus << 1) + SEVON;
	return nStatus;
}

ushort Dmc1380::GetInput(ushort bit)
{
	return (ushort)d1000_in_bit(bit);
}

ushort Dmc1380::GetOutput(ushort bit)
{
	return (ushort)d1000_get_outbit(bit);
}

void Dmc1380::SetOutput(ushort bit, ushort onoff)
{
	d1000_out_bit(bit, onoff);
}

void Dmc1380::AxisConfig(ushort axis, double ppm, double acc, double dec, double minspeed,
	ushort pulMode)
{
	pulPerMM[axis] = ppm;
	accTime[axis] = acc;
	decTime[axis] = dec;
	minSpd[axis] = minspeed;
	d1000_set_pls_outmode(axis, pulMode);
}