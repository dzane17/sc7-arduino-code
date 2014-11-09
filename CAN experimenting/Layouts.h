/*
 * Layouts.h
 * Definition for CAN layouts.
 */

#ifndef Layouts_h
#define Layouts_h

#include <cstdint>
#include "PacketIDs.h"
//#include "MCP2515_defs.h"

typedef struct
{
      unsigned long id;      // EID if ide set, SID otherwise
      uint8_t srr;                  // Standard Frame Remote Transmit Request
      uint8_t rtr;                  // Remote Transmission Request
      uint8_t ide;                  // Extended ID flag
      uint8_t dlc;                  // Number of data bytes
      union {
        // 8 bytes
        uint64_t value;
        // 4 bytes
        struct {
          uint32_t low;
          uint32_t high;
        };
        // 2 bytes
        struct {
          uint16_t s0;
          uint16_t s1;
          uint16_t s2;
          uint16_t s3;
        };
        // 1 byte
        uint8_t data[8];
      };
} Frame;

/*
 * Abstract base packet.
 */
class Layout {
public:
	/*
	 * Creates a Frame object to represent this layout.
	 */
	Frame generate_frame();

	uint16_t id;
protected:
	/*
	 * Fill out the header info for a frame.
	 */
	Frame set_header(Frame& f);
};

/*
 * Drive command packet.
 */
class DriveCmd : public Layout {
public:
	/*
	 * Initializes the DriveCmd with current c and velocity v.
	 * Used by the sender.
	 */
	DriveCmd(float c, float v) : current(c), velocity(v) { id = DRIVE_CMD_ID; }

	/*
	 * Initializes a DriveCmd from the given Frame. Used by the receiver.
	 */
	DriveCmd(Frame& frame) : velocity(frame.low), current(frame.high) { id = frame.id; }

	Frame generate_frame();

	float current;
	float velocity;
};

/*
 * Motor power command packet.
 */
class PowerCmd : Layout {
public:
	PowerCmd(float busCurrent);
	PowerCmd(Frame& frame);

	float busCurrent;
};

/*
 * Motor velocity packet.
 */
class MotorVelCmd : Layout {
public:
	MotorVelCmd(float carVelocity, float motorVelocity);
	MotorVelCmd(Frame& frame);

	float carVelocity;
	float motorVelocity;
};

class BusStateCmd : Layout {
public:
	BusStateCmd(float busCurrent, float busVoltage);

	float busCurrent;
	float busVoltage;
};

#endif