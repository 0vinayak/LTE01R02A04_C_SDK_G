#ifndef __GNSS_DRIVER_API_H__
#define __GNSS_DRIVER_API_H__

#define GNSS_MODEM_BIN_PATH_MAX (256)
typedef struct
{
    unsigned int base_addr;
    int bin_size;
    const char path[GNSS_MODEM_BIN_PATH_MAX];
} GnssModemBinInfo_T;

/** gnss firmware download and dump APIs **/

/** start power on(download)/power off **/
int gnss_driver_power_enable(int set_value);
/** set bin type **/
void gnss_driver_set_subsys(int set_value);
/** show bin type **/
int gnss_driver_show_subsys(char *buf, int len);
/** show gnss status **/
int gnss_driver_show_status(char *buf, int len);
/** init gnss_common_ctl **/
//int gnss_driver_common_ctl_probe(void);
int gnss_driver_common_ctl_probe(void);
/** remove gnss_common_ctl not used**/
int gnss_driver_common_ctl_remove(void);
/** start dump **/
int gnss_driver_dump_req(int set_value);

// int gnss_driver_set_bin_info(GnssModemBinInfo_T *pInfo);

/* get the gnss modem bin info */
// int gnss_drvier_get_bin_info(GnssModemBinInfo_T *pInfo);

#endif
