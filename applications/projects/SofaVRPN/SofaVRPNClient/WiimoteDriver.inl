/*
 * WiimoteDriver.cpp
 *
 *  Created on: 8 sept. 2009
 *      Author: froy
 */

#ifndef SOFAVRPNCLIENT_WIIMOTEDRIVER_INL_
#define SOFAVRPNCLIENT_WIIMOTEDRIVER_INL_

#include <WiimoteDriver.h>

#include <sofa/core/ObjectFactory.h>

namespace sofavrpn
{

namespace client
{

// 	  Wiimote
	//    channel[0] = battery level (0-1)
	//    channel[1] = gravity X vector calculation (1 = Earth gravity)
	//    channel[2] = gravity Y vector calculation (1 = Earth gravity)
	//    channel[3] = gravity Z vector calculation (1 = Earth gravity)
	//    channel[4] = X of first sensor spot (0-1023, -1 if not seen)
	//    channel[5] = Y of first sensor spot (0-767, -1 if not seen)
	//    channel[6] = size of first sensor spot (0-15, -1 if not seen)
	//    channel[7] = X of second sensor spot (0-1023, -1 if not seen)
	//    channel[9] = Y of second sensor spot (0-767, -1 if not seen)
	//    channel[9] = size of second sensor spot (0-15, -1 if not seen)
	//    channel[10] = X of third sensor spot (0-1023, -1 if not seen)
	//    channel[11] = Y of third sensor spot (0-767, -1 if not seen)
	//    channel[12] = size of third sensor spot (0-15, -1 if not seen)
	//    channel[13] = X of fourth sensor spot (0-1023, -1 if not seen)
	//    channel[14] = Y of fourth sensor spot (0-767, -1 if not seen)
	//    channel[15] = size of fourth sensor spot (0-15, -1 if not seen)
	// and on the secondary controllers (skipping values to leave room for expansion)
	//    channel[16] = nunchuck gravity X vector
	//    channel[17] = nunchuck gravity Y vector
	//    channel[18] = nunchuck gravity Z vector
	//    channel[19] = nunchuck joystick angle
	//    channel[20] = nunchuck joystick magnitude
	//
	//    channel[32] = classic L button
	//    channel[33] = classic R button
	//    channel[34] = classic L joystick angle
	//    channel[35] = classic L joystick magnitude
	//    channel[36] = classic R joystick angle
	//    channel[37] = classic R joystick magnitude
	//
	//    channel[48] = guitar hero whammy bar
	//    channel[49] = guitar hero joystick angle
	//    channel[50] = guitar hero joystick magnitude

template<class Datatypes>
const unsigned int WiimoteDriver<Datatypes>::WIIMOTE_NUMBER_OF_IR_DOTS = 4;

template<class Datatypes>
WiimoteDriver<Datatypes>::WiimoteDriver()
: f_channels(initData(&f_channels, "channels", "Channels from VRPN Analog"))
, f_dots(initData(&f_dots, "dots", "IR dots in Wiimote camera view"))
{
    addInput(&f_channels);;

    addOutput(&f_dots);;

    setDirtyValue();

}

template<class Datatypes>
WiimoteDriver<Datatypes>::~WiimoteDriver()
{
	// TODO Auto-generated destructor stub
}

template<class Datatypes>
void WiimoteDriver<Datatypes>::update()
{
	cleanDirty();

	sofa::helper::ReadAccessor< Data<sofa::helper::vector<Real> > > channels = f_channels;
	sofa::helper::WriteAccessor< Data<VecCoord > > points = f_dots;

	points.clear();
	points.resize(4);

	if (channels.size() >= 16)
	{
		//std::cout << "Wiimote :" << this->getName() ;
		for (unsigned int i=0 ;i<WIIMOTE_NUMBER_OF_IR_DOTS ;i++)
		{
			for (unsigned int j=0 ;j<3 ;j++)
			{
				unsigned int index = i*3+j+4;
				points[i][j] = channels[index];
			}
		}
		//std::cout << std::endl;
	}
}

}

}

#endif //SOFAVRPNCLIENT_WIIMOTEDRIVER_INL_

