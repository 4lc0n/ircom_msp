#ifndef IRDA_HPP
#define IRDA_HPP

/*
    Definitions for the IrDA Protocol
*/


// definitions for IrLAP
#define IRLAP_BOF       (0xC0)
#define IRLAP_EOF       (0xC1)
#define IRLAP_CE        (0x7D)



typedef struct connection_parameter_t{
    uint8_t baud_rate;
    uint8_t turnaround_time;
    uint8_t data_size;
    uint8_t window_size;
    uint8_t add_BOFs;
    uint8_t turnaround_time_min;
    uint8_t link_disconnect_threshold;
}connection_parameter;

// connection parameters
// baud rate
#define BAUD_PI         (0x01)
#define BAUD_2400       (0x01)
#define BAUD_9600       (0x02)
#define BAUD_19200      (0x04)
#define BAUD_38400      (0x08)
#define BAUD_57600      (0x10)
#define BAUD_115200     (0x20)
#define BAUD_576000     (0x40)
#define BAUD_1152000    (0x80)

// max turnaround time
#define TURNAROUND_T_PI  (0x82)
#define TURNAROUND_T_500 (0x01)
#define TURNAROUND_T_250 (0x02) // only valid for 115200 bps or higher
#define TURNAROUND_T_100 (0x04) // only valid for 115200 bps or higher
#define TURNAROUND_T_50  (0x08) // only valid for 115200 bps or higher

// data size
#define DATA_SIZE_PI    (0x83)
#define DATA_SIZE_64    (0x01)    
#define DATA_SIZE_128   (0x02)
#define DATA_SIZE_256   (0x04)
#define DATA_SIZE_512   (0x08)
#define DATA_SIZE_1024  (0x10)
#define DATA_SIZE_2048  (0x20)

// window size (max. acked packages)
#define WINDOW_SIZE_PI  (0x84)
#define WINDOW_SIZE_1   (0x01)
#define WINDOW_SIZE_2   (0x02)
#define WINDOW_SIZE_3   (0x04)
#define WINDOW_SIZE_4   (0x08)
#define WINDOW_SIZE_5   (0x10)
#define WINDOW_SIZE_6   (0x20)
#define WINDOW_SIZE_7   (0x40)

// additional BOFs at 115200 bps
#define ADD_BOFS_PI     (0x85)
#define ADD_BOFS_48     (0x01)
#define ADD_BOFS_24     (0x02)
#define ADD_BOFS_12     (0x04)
#define ADD_BOFS_5      (0x08)
#define ADD_BOFS_3      (0x10)
#define ADD_BOFS_2      (0x20)
#define ADD_BOFS_1      (0x40)
#define ADD_BOFS_0      (0x80)

// divider for BOFs for selected baudrate
#define BOFS_DIV_2400   (48)
#define BOFS_DIV_9600   (12)
#define BOFS_DIV_19200  (6)
#define BOFS_DIV_38400  (3)
#define BOFS_DIV_57600  (2)
#define BOFS_DIV_115200 (1)


// min turnaround time
#define TURNAROUND_T_MIN_PI     (0x86)
#define TURNAROUND_T_MIN_10MS   (0x01)
#define TURNAROUND_T_MIN_5MS    (0x02)
#define TURNAROUND_T_MIN_1MS    (0x04)
#define TURNAROUND_T_MIN_0_5MS  (0x08)
#define TURNAROUND_T_MIN_0_1MS  (0x10)
#define TURNAROUND_T_MIN_50US   (0x20)
#define TURNAROUND_T_MIN_10US   (0x40)
#define TURNAROUND_T_MIN_0      (0x80)

// link disconnect threshold time
#define LINK_DISC_TH_T_PI       (0x08)
#define LINK_DISC_TH_T_3        (0x01)
#define LINK_DISC_TH_T_8        (0x02)
#define LINK_DISC_TH_T_12       (0x04)
#define LINK_DISC_TH_T_16       (0x08)
#define LINK_DISC_TH_T_20       (0x10)
#define LINK_DISC_TH_T_25       (0x20)
#define LINK_DISC_TH_T_30       (0x40)
#define LINK_DISC_TH_T_40       (0x80)

#endif