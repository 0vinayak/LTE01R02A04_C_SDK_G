#ifndef _USB_PLATFORM_NEW_H
#define _USB_PLATFORM_NEW_H

typedef struct
{
    bool (*init)(void);                        //init usb platform ops;
    void (*setclk)(void *usb_priv, bool on);   ///< set controller clock
    void (*setpower)(void *usb_priv, bool on); ///< set controller power
    void (*enable)(void *usb_priv);            ///< enable the usb controller
    void (*disable)(void *usb_priv);           ///< disable the usb controller
    void (*suspend)(void *usb_priv);           ///< suspend the usb controller
    void (*resume)(void *usb_priv);            ///< resume the usb controller
} usbPlatOps_t;

typedef struct
{
    bool clk_constrain;
    osiClockConstrainRegistry_t usb_clk;
    osiPmSource_t *pm_lock;
    usbPlatOps_t plat_ops;
} usbPlat_t;

usbPlat_t *usbPlatGet(void);
bool usbPlatInit(void);

#endif